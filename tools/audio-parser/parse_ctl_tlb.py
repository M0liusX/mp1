import math

# Function to sort the list by first item of tuple
def sort_tuple(tup):
    tup.sort(key = lambda x: x[0])
    return tup

def read_rom(root_dir):
    with open(root_dir + "/baserom.us.z64", "rb") as f:
        return f.read()
def get_short(addr):
    return (rom[addr] << 8) + rom[addr + 1]
def get_long(addr):
    return (rom[addr] << 24) + (rom[addr + 1] << 16) + (rom[addr + 2] << 8) + rom[addr + 3]


def get_wave_ranges(ctl):
    banks = []
    instruments = set()
    waves = set()
    waveRanges = []

    for i in range(1, get_short(ctl + 2) + 1):
        banks.append(get_long(ctl + i * 4) + ctl)
    # Search for percussion
    # for bank in banks:
    #     print(get_long(bank + 8))

    for bank in banks:
        for i in range(0, get_short(bank)):
            instruments.add(get_long(bank + 0xC + i * 4) + ctl)

    for inst in instruments:
        for i in range(0, get_short(inst + 0xE)):
            sound = get_long(inst + 0x10 + i * 4) + ctl
            waves.add(get_long(sound + 0x8) + ctl)

    for wave in waves:
        base = get_long(wave)
        len = get_long(wave + 0x4)
        waveRanges.append((base, base + len, wave))
    waveRanges = sort_tuple(waveRanges)
    return waveRanges


def find_ctl(showPotential=False):
    potential = []
    likely = []

    for i in range(0, len(rom), 16):
        value = get_long(i);
        if value >= 0x42310000 and value <= 0x423100FF:
            potential.append(i)
            if showPotential:
                print("potential ctl: " + hex(i))

    for clt in potential:
        if clt + 2 >= len(rom): continue
        count = get_short(clt + 2)
        if count == 0: continue
        if (count * 4 + clt) >= len(rom): continue

        prev = 0
        found = True
        for i in range(1, count + 1):
            offset = get_long(i * 4 + clt)
            # print(offset)
            if offset <= prev:
                found = False
                break
            if (offset + clt >= len(rom)):
                found = False
                break
            prev = offset
        if found: likely.append(clt)

    for ctl in likely:
        print("Very likely ctl: " + hex(ctl))
    return likely


def find_tbl(ctl):
    waveRanges = get_wave_ranges(ctl)
    size = waveRanges[-1][1]
    
    # (count, offset)
    padpattern = []
    for wave in waveRanges:
        pad = math.ceil(wave[1]/8) * 8 - wave[1]
        if pad != 0: 
            padpattern.append((pad, wave[1]))

    if len(padpattern) < 5:
        print("unlikely to find")
        return
    
    potentialRange = math.floor(len(rom)/8) * 8 - size
    for base in range(0, potentialRange, 8):
        found = True
        for p in padpattern:
            for i in range(p[0]):
                if rom[base + p[1] + i] != 0:
                    found = False
                    break
            if not found: break
        if found:
            print("Found tbl: " + hex(base))
            return base
        

def get_ctl_range(addr):
    finalBankOffset = get_short(addr + 2) * 4 + addr
    finalBankAddress = get_long(finalBankOffset) + addr
    finalByte = get_short(finalBankAddress) * 4 + 0xC + finalBankAddress
    finalByte = math.ceil(finalByte/8) * 8
    print("ctl range: " + hex(addr) + " - " + hex(finalByte))


def sign_extend(value, bits):
    sign_bit = 1 << (bits - 1)
    return (value & (sign_bit - 1)) - (value & sign_bit)
def init_book(a, b, c):
    lst = [[ [0 for col in range(a)] for col in range(b)] for row in range(c)]
    return lst
def get_predictor_book(ctl, addr):
    wavType = rom[addr + 0x8]
    bookOffset = get_long(addr + 0x10)
    if wavType == 0:
        if bookOffset != 0:
            order = get_long(ctl + bookOffset)
            predictors = get_long(ctl + bookOffset + 0x4)
            table = init_book(order + 8, 8, predictors)
            for i in range(predictors):
                for j in range(order):
                    for k in range(8):
                        offset = 8 * order * i + 8 * j + k
                        table[i][k][j] = sign_extend(get_short(ctl + bookOffset + 0x8 + offset * 2), 16)

                for j in range(1, 8):
                    table[i][j][order] = table[i][j - 1][order - 1]
                table[i][0][order] = 1 << 11

                for j in range(1, 8):
                    for k in range(j):
                        table[i][k][j + order] = 0
                    for k in range(j, 8):
                        table[i][k][j + order] = table[i][k - j][order]
            return (order, predictors, table)


def get_tbl_range(ctl, tbl, showRanges=False):
    waveRanges = get_wave_ranges(ctl)
    if showRanges: 
        prev = 0
        for r in waveRanges:
            valid = r[0] >= prev
            print(hex(r[0] + tbl) + "-" + hex(r[1] + tbl) + " :" + "(range: + "+ hex(r[1] - r[0]) + "), " + "(wave: " + hex(r[2]) + ") " + ":" + str(valid))
            prev = r[0]
    print("tlb:  " + hex(tbl) + " - " + hex(waveRanges[-1][1] + tbl))


def inner_product(len, v1, v2):
    total = 0
    for i in range(len):
        total += v1[i] * v2[i]
    dout = total // (1 << 11)
    fiout = dout * (1 << 11)
    if total - fiout < 0: return dout - 1
    else: return dout


def vadpcm_dec(start, len, book):
    pos = 0
    outp = [0] * 16
    name = "sounds/sound" + str(start) + ".sw"
    with open(name, "wb") as aiff:
        while pos < len:
            ix = [0] * 16
            invec = [0] * (book[0] + 8)
            header = rom[start + pos]
            pos += 1
            scale = 1 << (header >> 4)
            optimalp = header & 0xf
            for i in range(0, 16, 2):
                c = rom[start + pos + (i >> 1)]
                ix[i] = c >> 4
                ix[i + 1] = c & 0xf
                ix[i] = sign_extend(ix[i], 4) * scale
                ix[i + 1] = sign_extend(ix[i + 1], 4) * scale
            for j in range(2):
                for i in range(8):
                    invec[i + book[0]] = ix[j * 8 + i]

                if j == 0:
                    for i in range(book[0]):
                        invec[i] = outp[16 - book[0] + i]
                else:
                    for i in range(book[0]):
                        invec[i] = outp[j * 8 - book[0] + i]

                for i in range(8):
                    result = inner_product(book[0] + 8, book[2][optimalp][i], invec)
                    if result > 32767: result = 32767
                    elif result < -32768: result = -32768
                    outp[i + j * 8] = result
            for value in outp:
                aiff.write((value & 0xffff).to_bytes(2, 'little'))
            pos += 8


rom = read_rom("./")
ctls = find_ctl()
tbls = [find_tbl(ctl) for ctl in ctls]
for i in range(len(ctls)):
    get_ctl_range(ctls[i])
    get_tbl_range(ctls[i], tbls[i])

# Decode wave ranges
# waves = get_wave_ranges(ctl)
# for wave in waves:
#     len = wave[1] - wave[0]
#     book = get_predictor_book(ctl, wave[2])
#     vadpcm_dec(tbl + wave[0], len, book)
