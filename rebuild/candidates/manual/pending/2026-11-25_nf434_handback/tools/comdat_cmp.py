# COMDAT-keyed object comparison (NF388). Every section of both objects is keyed by its COMDAT symbol (the first non-section
# symbol defined in it; a non-COMDAT section keys as `<name>#<n-th of that name>`), never by position. For each key: bytes equal?
# relocations equal as (offset, type, target name)? Keys present on one side only are listed. --map old=new renames a head key
# (and head relocation targets) before comparing (a respelt leaf).
# usage: python3 comdat_cmp.py <head.obj> <new.obj> [--map old=new ...]
import sys, os, re, struct, importlib.util
ANON = lambda s: re.sub(r'\?A0x[0-9a-f]{8}@', '?A0x@', s)   # anonymous-namespace hashes depend on the build path
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sp = importlib.util.spec_from_file_location("ri", "/home/user/FableDecomp/tools/decomp_pipeline/reloc_identity.py")
RI = importlib.util.module_from_spec(sp); sp.loader.exec_module(RI)
def relocs_full(path, secs):
    d = open(path, "rb").read(); out = []
    nsec = struct.unpack_from("<H", d, 2)[0]; opt = struct.unpack_from("<H", d, 16)[0]
    for i in range(nsec):
        h = 20 + opt + 40 * i; prel = struct.unpack_from("<I", d, h + 24)[0]; nrel = struct.unpack_from("<H", d, h + 32)[0]
        out.append([struct.unpack_from("<IIH", d, prel + 10 * k) for k in range(nrel)])
    return out
def keyed(path, mp):
    secs, syms = RI.coff_read(path); rel = relocs_full(path, secs)
    secnames = {s["name"] for s in secs}
    owner = {}
    for idx in sorted(syms):
        sy = syms[idx]
        if sy["sec"] > 0 and sy["name"] not in secnames and not sy["name"].startswith("$") and sy["sec"] not in owner:
            owner[sy["sec"]] = sy["name"]
    out = {}; seen = {}
    for i, s in enumerate(secs, 1):
        k = owner.get(i)
        if k is None:
            seen[s["name"]] = seen.get(s["name"], 0) + 1; k = f"{s['name']}#{seen[s['name']]}"
        k = ANON(mp.get(k, k))
        r = tuple((o, t, ANON(mp.get(syms[x]["name"], syms[x]["name"]))) for o, x, t in rel[i - 1])
        out[k] = (s["name"], s["data"] if not s["chars"] & 0x80 else b"\0" * s["size"], r)
    return out
args = sys.argv[1:]; mp = {}
for i, a in enumerate(args):
    if a == "--map": o, n = args[i + 1].split("=", 1); mp[o] = n
h, n = keyed(args[0], mp), keyed(args[1], {})
bad = 0
for k in sorted(set(h) | set(n)):
    if k not in h: print(f"ONLY_NEW  {n[k][0]:10} {k}  {len(n[k][1])}B"); bad += 1; continue
    if k not in n: print(f"ONLY_HEAD {h[k][0]:10} {k}  {len(h[k][1])}B"); bad += 1; continue
    be, re_ = h[k][1] == n[k][1], h[k][2] == n[k][2]
    print(f"{'EQ  ' if be and re_ else 'DIFF'}      {h[k][0]:10} {k}  bytes={'eq' if be else 'DIFF'} relocs={'eq' if re_ else 'DIFF'}  {len(h[k][1])}B/{len(n[k][1])}B")
    if not re_:
        for x, y in zip(h[k][2], n[k][2]):
            if x != y: print(f"            head {x} | new {y}")
    if not (be and re_): bad += 1
print("ALL_EQ" if not bad else f"{bad} key(s) differ")
