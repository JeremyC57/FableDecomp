# mkpend.py <outdir> <addr>... : payload + oracle (from Fable.exe) for pass 12 verified_pending rows (sources as returned, or
# overridden by <outdir>/src/<addr>.cpp when present). Flags from $S/codex12land/rows.tsv; module/name from the Leaf.
import sys, os, json, subprocess, re, struct
S = os.environ["S"]; out = sys.argv[1]; os.makedirs(out, exist_ok=True)
V = "rebuild/candidates/manual/pending/2026-11-02_codex_pass12/verified_pending"
rows = {l.split("\t")[0]: l.rstrip("\n").split("\t") for l in open(S + "/codex12land/rows.tsv")}
exe = open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe", "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]; nsec = struct.unpack_from("<H", exe, pe + 6)[0]; opt = struct.unpack_from("<H", exe, pe + 20)[0]
secs = [struct.unpack_from("<IIII", exe, pe + 24 + opt + 40 * i + 8) for i in range(nsec)]
def rd(va, n):
    for vs, sva, rs, ro in secs:
        if sva <= va - 0x400000 < sva + rs: off = va - 0x400000 - sva + ro; return exe[off:off + n]
auth = []; orc = ["address\tname\tlength\tbytes"]
for a in sys.argv[2:]:
    r = rows[a]; n = int(r[2]); flags = [] if r[3] == "-" else r[3].split(); leaf = r[4]
    sp = f"{out}/src/{a}.cpp"; src = open(sp if os.path.exists(sp) else f"{V}/{a}/{a}.cpp").read()
    tp = f"{out}/src/{a}_test.cpp"; tst = open(tp if os.path.exists(tp) else f"{V}/{a}/{a}_test.cpp").read()
    m = re.match(r"\?+(?:\$)?(\w+)@((?:\w+@)*)@", leaf)
    parts = [p for p in m.group(2).split("@") if p] if m else []
    name = m.group(1) if m else leaf; module = "::".join(reversed(parts)) if parts else "_global"
    if leaf.startswith("??1"): name = "~" + (parts[0] if parts else name)
    e = dict(address=a, module=module, name=name, source_cpp=src, test_cpp=tst, pass_pattern=f"{a}_TEST PASS", leaf=leaf)
    if flags: e["flags"] = flags
    auth.append(e); orc.append(f"{a}\t{module}::{name}\t{n}\t{rd(int(a, 16), n).hex()}")
json.dump({"result": {"authored": auth}}, open(out + "/payload.json", "w"))
open(out + "/oracle.tsv", "w").write("\n".join(orc) + "\n"); print("rows", len(auth))
