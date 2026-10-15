import sys, struct
# (saved from wave-25 agent 62) callers of a function = the fastest way to name a row whose manifest label is propagated
# usage: xref.py <hexva> [...]  -- finds E8/E9 rel32 call/jmp sites and absolute dword refs to each VA
import os; EXE = os.environ["S"] + "/dl/game/Fable The Lost Chapters/Fable.exe"
d = open(EXE, "rb").read()
pe = struct.unpack_from("<I", d, 0x3c)[0]
nsec = struct.unpack_from("<H", d, pe + 6)[0]
optsz = struct.unpack_from("<H", d, pe + 20)[0]
base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
secs = []
for i in range(nsec):
    o = pe + 24 + optsz + 40 * i
    name = d[o:o + 8].rstrip(b"\0").decode()
    vsz, va, rsz, raw = struct.unpack_from("<IIII", d, o + 8)
    secs.append((name, va, vsz, raw, rsz))
for t in sys.argv[1:]:
    tv = int(t, 16)
    out = []
    for name, va, vsz, raw, rsz in secs:
        blob = d[raw:raw + rsz]
        if name == ".text":
            for i in range(len(blob) - 5):
                if blob[i] in (0xE8, 0xE9):
                    rel = struct.unpack_from("<i", blob, i + 1)[0]
                    src = base + va + i
                    if src + 5 + rel == tv:
                        out.append("%s %08x" % ("call" if blob[i] == 0xE8 else "jmp", src))
        pat = struct.pack("<I", tv)
        j = blob.find(pat)
        while j >= 0:
            out.append("abs %s %08x" % (name, base + va + j))
            j = blob.find(pat, j + 1)
    print(t, " | ".join(out[:30]), "(%d)" % len(out))
