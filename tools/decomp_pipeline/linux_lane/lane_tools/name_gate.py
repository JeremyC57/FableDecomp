#!/usr/bin/env python3
"""name_gate.py <addr> <src.cpp> <full mangled leaf> [--test test.cpp] [--local-ok LOCAL_OK.tsv] [--descriptive DESCRIPTIVE.tsv] [--flags ...]

Naming gate for authored rows (owner 2026-11-25: "ensure that the rows being authored have correct naming and headers as they are
authored so that no more work is added to the naming queue"). Compiles the row like q.py, then measures every name the object uses
the way tools/decomp_pipeline/naming_status.py would after landing:
  * the row's own graded symbol (self, at <addr>);
  * every relocation's symbol and the retail address it lands on (reloc_identity.check: calls, globals, vftables).
For each (retail address, spelling) it adds the spelling to the spellings landed rows already bind there (rebuild/oracles/
reloc_bindings.tsv, plus the landed self leaf) and reports the status the tracker would give:
  OK     agree_pdb / agree_std after the change (a PDB name, or std / compiler-generated / proven library name), or the address was
         already open and this spelling is its PDB name (no new work);
  FAIL   standin (struct T, _<addr>, FUN_, DAT_, Unproven ...), a split this row would create or widen, a RELOC_IDENTITY / CONFLICT
         warning from reloc_identity, or a non-PDB spelling (agree_unverified) not listed in --descriptive;
  DESC   a non-PDB spelling listed in the descriptive file (address <TAB> spelling <TAB> evidence that no source names it).
  DEBT   a callee / data address that is already open with NO PDB spelling among its landed ones, and this row reuses a spelling
         landed rows already bind there (no new work). Where the PDB spelling is known (split_fixable), only that spelling is OK. Look for the PDB name anyway; if two sources give it, use it (then OK) and add the fix to NAME_FIXES.tsv.
         A stand-in spelling and the row's own self symbol never count as DEBT.
Header and code-rule checks (same object, same run):
  FAIL   SHARED_HEADER_AVAILABLE: the row or its test defines a class that has a shared header (rebuild/include or the overlay in
         FABLE_HDR_OVERLAY) instead of including it (verify_and_land.shared_header_redefinitions, the lander's strict-header rule);
  FAIL   LOCAL_CLASS: the row defines any other class / struct body (a local model). Generate the class's header into the overlay;
         a type retail really defined in that TU (evidence: the PDB's module or an anonymous namespace) goes in --local-ok
         (class <TAB> evidence) instead;
  FAIL   NOT_GENUINE (__asm / naked / _emit), BARE_FORCEINLINE, BAKED_STRING / BAKED_ADDRESS (typed-in retail addresses).
Exit status 0 always; the last line is `NAME_GATE PASS` or `NAME_GATE FAIL (<n>)`. Run it with FABLE_HDR_OVERLAY set to your hdr/.
"""
import csv, glob, os, sys
from pathlib import Path
csv.field_size_limit(10**8)
ROOT = "/home/user/FableDecomp"
sys.path.insert(0, ROOT + "/tools/decomp_pipeline")
argv = sys.argv[1:]; sys.argv = ["x"]
import naming_status as ns
import verify_and_land as V
import reloc_identity as RI

S = Path(os.environ["S"])
extra, desc_path, test_path, lok_path = [], "", "", ""
if "--test" in argv:
    i = argv.index("--test"); test_path = argv[i + 1]; del argv[i:i + 2]
if "--local-ok" in argv:
    i = argv.index("--local-ok"); lok_path = argv[i + 1]; del argv[i:i + 2]
if "--descriptive" in argv:
    i = argv.index("--descriptive"); desc_path = argv[i + 1]; del argv[i:i + 2]
if "--flags" in argv:
    i = argv.index("--flags"); extra = argv[i + 1:]; argv = argv[:i]
addr = argv[0].lower().replace("0x", "").zfill(8)
src = Path(argv[1]).read_text(encoding="utf-8")
leaf = argv[2] if len(argv) > 2 else None
retail = name = None
for f in list(glob.glob(str(S / "batch" / "*_oracle.tsv"))) + [ROOT + "/rebuild/oracles/auto-re-candidates.tsv"]:
    for r in csv.DictReader(open(f, encoding="utf-8-sig"), delimiter="\t"):
        if r["address"].lower().replace("0x", "") == addr:
            retail = bytes.fromhex(r["bytes"]); name = r["name"]; break
    if retail: break
if retail is None:
    print("no oracle for", addr); print("NAME_GATE FAIL (no oracle)"); sys.exit(0)
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
if not leaf or not leaf.startswith("?") or ("@@" not in leaf and not leaf.endswith("?A0x")):
    # obj_text falls back to the largest function for a short or plain leaf, so "self" could be the wrong function (advisor, 2026-11-25)
    print("NAME_GATE FAIL (give the row's FULL mangled leaf as the third argument, e.g. '?Name@Class@@QAEXXZ')"); sys.exit(0)
os.environ["LANDVERIFY_STRICT_LEAF"] = "1"   # the graded symbol must BE the leaf
work = S / "wine" / "ng" / addr; work.mkdir(parents=True, exist_ok=True)
e = V.env()
st, built, sec = V.parity_of(V.vc71(src), addr, leaf, retail, work, e, extra=tuple(extra))
print(addr, name, st)
if not st.startswith(("MATCH", "RELOCATION")):
    print("NAME_GATE FAIL (row does not match: %s)" % st); sys.exit(0)
obj = work / (addr + ".obj")
res = V.obj_text(obj, leaf); selfsym = str(res[2]) if len(res) > 2 else ""
hard, warn, pairs = RI.check(ROOT, V.OBJDUMP, obj, leaf, addr, retail, built=built)

pdb = ns.pdb_names()
by = ns.table_spellings(open(ROOT + "/rebuild/oracles/reloc_bindings.tsv", encoding="utf-8", errors="replace").read())
ent = ns.landed_rows()
desc = {}
if desc_path and os.path.exists(desc_path):
    for l in open(desc_path, encoding="utf-8").read().splitlines():
        f = l.split("\t")
        if len(f) >= 3 and f[0] and not f[0].startswith("#"): desc[(f[0].lower().replace("0x", "").zfill(8), f[1])] = f[2]

def landed_self(t):
    r = ent.get(t)
    if not r: return ""
    try: return ns.resolve_leaf(ns.leaf_of(r["Source"]), {})[0]
    except Exception: return ""

def status(names):
    names = sorted(set(names)); pd = [s for s in names if s in pdb]
    if len(names) == 1:
        s = names[0]
        return "agree_pdb" if s in pdb else ("standin" if ns.STANDIN.search(s) else ("agree_std" if ns.is_std(s) else "agree_unverified"))
    return "split_fixable" if len(pd) == 1 else ("split_pdb_multi" if len(pd) > 1 else "split_unknown")

uses = {(addr, ns.norm(selfsym), "self")}
for off, kind, sym, t in pairs:
    uses.add(("%08x" % t, ns.norm(sym), "+%x" % off))
fails = 0
seen = set()
for t, s, where in sorted(uses):
    if (t, s) in seen: continue
    seen.add((t, s))
    have = {ns.norm(x) for x in by.get(t, {})}
    ls = landed_self(t)
    if ls and t != addr: have.add(ns.norm(ls))
    before = status(have) if have else "none"
    after = status(have | {s})
    if after in ("agree_pdb", "agree_std"): v = "OK"
    elif s in pdb and before not in ("agree_pdb", "agree_std"): v = "OK"        # already open; this is its PDB spelling
    elif (t, s) in desc and after == "agree_unverified" and not ns.STANDIN.search(s): v = "DESC"
    elif (s in have and before not in ("agree_pdb", "agree_std") and not any(x in pdb or ns.is_std(x) for x in have) and not ns.STANDIN.search(s)
          and t != addr): v = "DEBT"
    else: v = "FAIL"
    if v == "FAIL": fails += 1
    other = ",".join(sorted(have - {s}))[:160]
    print("%-4s %s %-6s %-16s %s%s" % (v, t, where, after, s, ("   [landed: %s]" % other) if other else ""))
for h in hard: print("FAIL", h); fails += 1
for w in warn:
    bad = w.startswith(("RELOC_CONFLICT", "RELOC_NAME", "RELOC_ALIAS", "RELOC_UNCHECKED"))
    print("FAIL" if bad else "WARN", w); fails += bad
# ---- headers and code rules
import re
sys.path.insert(0, ROOT + "/tools/decomp_pipeline/crawl")
import purity
from crawl import baked_symbols
import check_portable_spellings as CPS
tst = Path(test_path).read_text(encoding="utf-8") if test_path else ""
lok = set()
if lok_path and os.path.exists(lok_path):
    lok = {l.split("\t")[0] for l in open(lok_path, encoding="utf-8").read().splitlines() if l.strip() and not l.startswith("#")}
for which, text in (("row", src), ("test", tst)):
    if not text: continue
    for sn, rel in V.shared_header_redefinitions(text):
        print("FAIL SHARED_HEADER_AVAILABLE (%s) %s: #include \"%s\"" % (which, sn, rel)); fails += 1
code = V._strip_comments_and_strings(src)
shared = {sn for sn, _ in V.shared_header_redefinitions(src)}
for m in re.finditer(r"\b(?:struct|class|union)\s+([A-Za-z_]\w*)\s*(?::[^{;]*)?\{", code):
    sn = m.group(1)
    if sn in shared or sn in lok: continue
    print("FAIL LOCAL_CLASS %s: a local model; put the class in a header in the overlay (or --local-ok with evidence)" % sn); fails += 1
if not purity.is_genuine(src): print("FAIL NOT_GENUINE (%s)" % purity.classify(src)); fails += 1
for t_ in (src, tst):
    if t_ and CPS.bare_forceinline_lines(t_): print("FAIL BARE_FORCEINLINE (use FABLE_FORCEINLINE)"); fails += 1
bs = purity.baked_string_addresses(src) or baked_symbols.baked_symbol_names(src)
if bs: print("FAIL BAKED_STRING", bs); fails += 1
ba = purity.baked_data_addresses(src)
if ba: print("FAIL BAKED_ADDRESS", ["0x%08X" % v for v in ba]); fails += 1
print("NAME_GATE PASS" if not fails else "NAME_GATE FAIL (%d)" % fails)
