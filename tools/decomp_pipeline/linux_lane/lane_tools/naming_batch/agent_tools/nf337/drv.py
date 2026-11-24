#!/usr/bin/env python3
"""drv.py (NF337): precheck / grade / objcheck driver for the Copy-thunk moves (run from the repo root, lane env sourced, S set).
  pre   <addr...>          snapshot landed source + test to work/pre/<a>/ (head.cpp, test.cpp), clang-precheck them and, when present,
                           work/gen/<a>.cpp (new.cpp) and work/gen/<a>_test.cpp (ntest.cpp) -> work/PRECHECK.tsv (appended)
  pretest <addr...>        the landed test only (before the lock)
  grade <run> <addr...>    vland --recheck-landed in work/<run>/: head (landed source + landed test), new (gen source + moved test; rows
                           with no gen source are graded only on the head side; NEW_OVERLAY=<tree> -> FABLE_HDR_OVERLAY on the new side only),
                           ctl (landed source + moved test) -> work/<run>/GRADE.tsv
  objcheck <head.obj> <new.obj> <map>   map = old=new,old=new (every DEF / UND rename), prints ALL_EQ / LIBCP / reason
Launch grade in the background with `timeout 900` on its own line."""
import os, re, sys, json, struct, subprocess, shutil, collections
R = "/home/user/FableDecomp"; S = os.environ["S"]; A = f"{S}/w22/agentNF337"; W = f"{A}/work"
LT = f"{R}/tools/decomp_pipeline/linux_lane/lane_tools"
AGENT = "NF337"
cat = open(f"{R}/rebuild/build_candidates.ps1", errors="replace").read()
ENT = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", cat, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1))
    if a: ENT.setdefault(a.group(1).lower(), dict(re.findall(r"(\w+) = '([^']*)'", m.group(1))))
def rd(p): return open(p, encoding="utf-8", errors="replace").read()
def lock_owner(a):
    p = f"{S}/w22/nflocks/{a}/owner"
    return open(p).read().strip() if os.path.exists(p) else "released"
VCINC = f"{S}/wine/vc71rtm/include"
def clang(path, defs=()):
    cmd = ["timeout", "60", "clang++", "-target", "i686-pc-windows-msvc", "-fms-extensions", "-fms-compatibility",
           "-fms-compatibility-version=13.10", "-Xclang", "-fno-wchar", "-fsyntax-only", "-w"] + \
          ([f"-I{os.environ['NF_TREE']}"] if os.environ.get("NF_TREE") else []) + [f"-I{R}/rebuild/include", "-isystem", VCINC] + [f"-D{d}" for d in defs] + [path]
    r = subprocess.run(cmd, capture_output=True, text=True, errors="replace", cwd=os.path.dirname(path))
    errs = [l for l in r.stderr.splitlines() if re.search(r'\b(error|fatal error)\b', l)]
    return ("PASS" if r.returncode == 0 else "FAIL"), errs[:4]
def rowdefs(a): return [f[2:] for f in ENT[a].get("CompilerFlags", "").split() if f.startswith("/D")]
def pre(addrs, tests_only=False):
    out = open(f"{W}/PRECHECK.tsv", "a")
    for a in addrs:
        kv = ENT[a]; d = f"{W}/pre/{a}"; os.makedirs(d, exist_ok=True); res = []
        jobs = [("test", f"{R}/rebuild/tests/{kv['TestSource']}", "test.cpp")] if kv.get("TestSource") else []
        if not tests_only:
            jobs = [("head", f"{R}/rebuild/src/compiled/{kv['Source']}", "head.cpp")] + jobs
            if os.path.exists(f"{W}/gen/{a}.cpp"): jobs.append(("new", f"{W}/gen/{a}.cpp", "new.cpp"))
            if os.path.exists(f"{W}/gen/{a}_test.cpp"): jobs.append(("ntest", f"{W}/gen/{a}_test.cpp", "ntest.cpp"))
        for k, src, dst in jobs:
            shutil.copy(src, f"{d}/{dst}")
            st, e = clang(f"{d}/{dst}", rowdefs(a) if k in ("head", "new") else ())
            res.append(f"{k}={st}"); out.write(f"{a}\t{k}\t{st}\t{' | '.join(e)[:300]}\n")
        print(a, " ".join(res), flush=True)
    out.close()
def vland(d, rows, vt=880, overlay=None):
    os.makedirs(d, exist_ok=True)
    json.dump({"result": {"authored": rows}}, open(f"{d}/payload.json", "w"))
    want = {r["address"] for r in rows}; cat_ = {}
    for l in open(f"{R}/rebuild/oracles/auto-re-candidates.tsv"):
        a = l[:8].lower()
        if a in want: cat_[a] = l.rstrip("\n")
    open(f"{d}/oracle.tsv", "w").write("address\tname\tlength\tbytes\n" + "".join("\t".join(cat_[r["address"]].split("\t")[:4]) + "\n" for r in rows))
    wd = f"{os.path.dirname(d)}/wd"     # ONE work path for head / new / ctl (moved after each run)
    env = dict(os.environ, LANDVERIFY_WORK=wd)
    env.pop("FABLE_HDR_OVERLAY", None)
    if overlay: env["FABLE_HDR_OVERLAY"] = overlay      # the new side only: the include tree with this run's add-only opt-ins
    with open(f"{d}/vland.log", "w") as lf:
        subprocess.run(["timeout", str(vt), "python3", "-u", f"{LT}/vland.py", f"{d}/payload.json", f"{d}/oracle.tsv", "--recheck-landed"],
                       stdout=lf, stderr=subprocess.STDOUT, env=env, cwd=R)
    if os.path.exists(wd): os.rename(wd, f"{d}/wd")
    st = {}
    for l in open(f"{d}/vland.log", errors="replace"):
        m = re.match(r"([0-9a-f]{8})\s+(\S+)\s+(\S+)\s", l)
        if m and m.group(2) != "WARN": st.setdefault(m.group(1), (m.group(2), m.group(3)))
    return st
def leaf(src):
    m = re.search(r'^// Leaf:\s*(\S+)', src, re.M); return m.group(1) if m else None
def payload(a, srcpath, testpath):
    kv = ENT[a]; s = rd(srcpath)
    p = dict(address=a, module=kv.get("Module", ""), name="Copy", source_cpp=s, test_cpp=rd(testpath) if testpath else "",
             pass_pattern=kv.get("PassPattern", f"{a}_TEST PASS"))
    lf = leaf(s)
    if lf: p["leaf"] = lf
    ex = [f for f in kv.get("CompilerFlags", "").split() if f not in ("/O2", "/Oy", "/W3")]
    if ex: p["flags"] = ex
    return p
def grade(run, addrs):
    bad = [a for a in addrs if lock_owner(a) != AGENT]
    if bad: print("NOT LOCKED", bad); sys.exit(2)
    for a in addrs:
        kv = ENT[a]; d = f"{W}/pre/{a}"
        if rd(f"{d}/head.cpp") != rd(f"{R}/rebuild/src/compiled/{kv['Source']}"): print("STALE", a); sys.exit(2)
        if kv.get("TestSource") and rd(f"{d}/test.cpp") != rd(f"{R}/rebuild/tests/{kv['TestSource']}"): print("STALE TEST", a); sys.exit(2)
        if os.path.exists(f"{W}/gen/{a}.cpp") and (rd(f"{d}/new.cpp") != rd(f"{W}/gen/{a}.cpp") or (os.path.exists(f"{W}/gen/{a}_test.cpp") and rd(f"{d}/ntest.cpp") != rd(f"{W}/gen/{a}_test.cpp"))):
            print("STALE GEN", a); sys.exit(2)
    rdir = f"{W}/{run}"; assert not os.path.exists(rdir), "run dir exists"
    os.makedirs(rdir)
    newrows = [a for a in addrs if os.path.exists(f"{W}/pre/{a}/new.cpp")]
    sh = vland(f"{rdir}/head", [payload(a, f"{W}/pre/{a}/head.cpp", f"{W}/pre/{a}/test.cpp" if ENT[a].get("TestSource") else None) for a in addrs])
    def nt(a): return f"{W}/pre/{a}/ntest.cpp" if os.path.exists(f"{W}/pre/{a}/ntest.cpp") else (f"{W}/pre/{a}/test.cpp" if ENT[a].get("TestSource") else None)
    sn = vland(f"{rdir}/new", [payload(a, f"{W}/pre/{a}/new.cpp", nt(a)) for a in newrows], overlay=os.environ.get("NEW_OVERLAY")) if newrows else {}
    ctlrows = [a for a in newrows if os.path.exists(f"{W}/pre/{a}/ntest.cpp")]   # rows whose test moved (an unchanged landed test has no control)
    sc = vland(f"{rdir}/ctl", [payload(a, f"{W}/pre/{a}/head.cpp", f"{W}/pre/{a}/ntest.cpp") for a in ctlrows]) if ctlrows else {}
    with open(f"{rdir}/GRADE.tsv", "w") as o:
        o.write("address\thead\tnew\tctl\n")
        for a in addrs:
            o.write(f"{a}\t{' '.join(sh.get(a, ('NOT_GRADED', '-')))}\t{' '.join(sn.get(a, ('-', '-')))}\t{' '.join(sc.get(a, ('-', '-')))}\n")
    print(rd(f"{rdir}/GRADE.tsv"))
# ------------------------------------------------------------------------------------------------ objcheck (explicit rename map)
def coff(obj):
    d = open(obj, "rb").read(); nsec, = struct.unpack_from("<H", d, 2); symp, nsym = struct.unpack_from("<II", d, 8); opt_ = struct.unpack_from("<H", d, 16)[0]
    strt = symp + nsym * 18
    def nm(e):
        if e[:4] == b"\0\0\0\0":
            o, = struct.unpack_from("<I", e, 4); return d[strt + o:d.index(b"\0", strt + o)].decode("latin-1")
        return e[:8].rstrip(b"\0").decode("latin-1")
    syms = {}; dfn, und_ = set(), set(); i = 0
    while i < nsym:
        e = d[symp + i * 18:symp + i * 18 + 18]; name = nm(e); syms[i] = name
        val, sec, typ, cl, aux = struct.unpack_from("<IhHBB", e, 8)
        if cl == 2 and sec > 0: dfn.add(name)
        if cl == 2 and sec == 0 and val == 0: und_.add(name)
        i += 1 + aux
    secs = []
    for k in range(nsec):
        h = d[20 + opt_ + 40 * k:20 + opt_ + 40 * (k + 1)]; name = h[:8].rstrip(b"\0").decode("latin-1")
        size, ptr, prel, _, nrel = struct.unpack_from("<IIIIH", h, 16); ch = struct.unpack_from("<I", h, 36)[0]
        if name.startswith(".debug"): continue
        raw = d[ptr:ptr + size] if ptr else b""
        rel = [(struct.unpack_from("<I", d, prel + 10 * j)[0], syms.get(struct.unpack_from("<I", d, prel + 10 * j + 4)[0], "?"),
                struct.unpack_from("<H", d, prel + 10 * j + 8)[0]) for j in range(nrel)]
        secs.append((name, raw, rel, ch))
    return secs, dfn, und_
def objcheck(oh, on, mp):
    sh, dh, uh = coff(oh); sn, dn, un = coff(on); m = lambda s: mp.get(s, s)
    if {m(x) for x in dh} != dn: return "DEF differ: +" + ",".join(sorted(dn - {m(x) for x in dh})) + " -" + ",".join(sorted({m(x) for x in dh} - dn))
    if {m(x) for x in uh} != un: return "UND differ: +" + ",".join(sorted(un - {m(x) for x in uh})) + " -" + ",".join(sorted({m(x) for x in uh} - un))
    if any(x.startswith(("??_7", "??_R")) for x in dn): return "vftable / RTTI symbol defined"
    if len(sh) != len(sn): return f"section count {len(sh)} v {len(sn)}"
    libcp = False
    for (a, ra, la, ca), (b, rb, lb, cb) in zip(sh, sn):
        if a != b or ca != cb: return f"section {a} v {b} (flags {ca:x} v {cb:x})"
        if a == ".drectve":
            if ra == rb: continue
            if sorted(rb.decode("latin-1").split()) == sorted(ra.decode("latin-1").split() + ['/DEFAULTLIB:"libcp"']): libcp = True; continue
            return ".drectve differs"
        if ra != rb: return f"section {a} bytes differ"
        if [(o, m(s), t) for o, s, t in la] != lb: return f"section {a} relocations differ"
    return "LIBCP" if libcp else "ALL_EQ"
if __name__ == "__main__":
    c = sys.argv[1]
    if c == "pre": pre([x.lower() for x in sys.argv[2:]])
    elif c == "pretest": pre([x.lower() for x in sys.argv[2:]], True)
    elif c == "grade": grade(sys.argv[2], [x.lower() for x in sys.argv[3:]])
    elif c == "objcheck":
        mp = dict(kv.split("=", 1) for kv in sys.argv[4].split(",")) if len(sys.argv) > 4 and sys.argv[4] else {}
        print(objcheck(sys.argv[2], sys.argv[3], mp))
