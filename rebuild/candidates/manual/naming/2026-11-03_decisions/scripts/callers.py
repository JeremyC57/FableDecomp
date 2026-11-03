# usage: callers.py <targets-file (hex per line)>  -> prints target, callsite, kind, containing landed row, source
import sys, struct, re, os, bisect
EXE = os.environ["S"] + "/dl/game/Fable The Lost Chapters/Fable.exe"
d = open(EXE, "rb").read()
pe = struct.unpack_from("<I", d, 0x3c)[0]; nsec = struct.unpack_from("<H", d, pe + 6)[0]
optsz = struct.unpack_from("<H", d, pe + 20)[0]; base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
text = None
for i in range(nsec):
    o = pe + 24 + optsz + 40 * i; name = d[o:o + 8].rstrip(b"\0").decode()
    vsz, va, rsz, raw = struct.unpack_from("<IIII", d, o + 8)
    if name == ".text": text = (va, d[raw:raw + rsz])
targets = {int(l.split()[0], 16) for l in open(sys.argv[1]) if l.strip()}
va, blob = text; hits = []
for i in range(len(blob) - 5):
    if blob[i] in (0xE8, 0xE9):
        rel = struct.unpack_from("<i", blob, i + 1)[0]; src = base + va + i; t = (src + 5 + rel) & 0xffffffff
        if t in targets: hits.append((t, src, "call" if blob[i] == 0xE8 else "jmp"))
ps = open("rebuild/build_candidates.ps1", encoding="utf-8-sig").read(); ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[int(kv["Address"], 16)] = kv
cat = []
for l in open("rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); cat.append((int(q[0], 16), int(q[2]), q[1]))
cat.sort(); starts = [c[0] for c in cat]
for t, s, k in sorted(hits):
    j = bisect.bisect_right(starts, s) - 1; row = cat[j] if j >= 0 and cat[j][0] <= s < cat[j][0] + cat[j][1] else None
    if row:
        e = ent.get(row[0]); print("%08x\t%08x\t%s\t%08x\t%s\t%s" % (t, s, k, row[0], "LANDED" if e else "unlanded", e["Source"] if e else row[2]))
    else: print("%08x\t%08x\t%s\t-\tnorow\t-" % (t, s, k))
