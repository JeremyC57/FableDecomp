#!/usr/bin/env python3
"""privmain.py <mcheck log> : for each `leafsym ?M@X@..@@QAEXXZ -> ..UAEXXZ` row (M = Main / Init, an entity class whose
non-virtual declaration now overrides the header's CEntityScriptBase slot), declare the override the way both PDBs spell
every entity override: private virtual (?Main@<entity>@..@@EAEXXZ, ?Init@<entity>@..@@EAEXXZ). Only when the row names
M nowhere but the declaration and the definition."""
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
R = os.environ["FABLE_ROOT"]
import glob
for l in open(sys.argv[1]):
    q = l.rstrip("\n").split("\t")
    if len(q) < 3 or q[1] != "RELOC_RENAMED": continue
    m = re.match(r"leafsym \?(Main|Init)@(\w+)@(?:[\w@]*?)@QAEXXZ -> \?\1@\2@[\w@]*@UAEXXZ$", q[2])
    if not m: continue
    M, X = m.group(1), m.group(2)
    f = glob.glob(R + "/rebuild/src/compiled/*/*/*_%s.cpp" % q[0])[0]
    t = open(f, encoding="utf-8").read()
    code = re.sub(r"//[^\n]*", "", t)
    if len(re.findall(r"\b" + M + r"\b", code)) != 2: print("skip", q[0], "uses", M); continue
    b = find_block(t, X)
    if not b: print("skip", q[0], "noblock", X); continue
    body = t[b[0]:b[1]]
    mm = re.search(r"^([ \t]*)void " + M + r"\(\);[^\n]*\n", body, re.M)
    if not mm: print("skip", q[0], "nodecl"); continue
    ind = mm.group(1)
    rep = "private:\n%svirtual void %s();   // PDB ?%s@<entity>@..@@EAEXXZ: every entity override is private (slot %d)\npublic:\n" % (ind, M, M, 1 if M == "Main" else 2)
    body = body[:mm.start()] + rep + body[mm.end():]
    t = t[:b[0]] + body + t[b[1]:]
    open(f, "w", encoding="utf-8", newline="\n").write(t)
    print("edit", os.path.relpath(f, R), M, X)
