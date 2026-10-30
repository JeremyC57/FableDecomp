#!/usr/bin/env python3
"""floatset.py <file>...: CGSI float setters (wave 5 NEXT item 5). After sigfix gives the definition the PDB `float`
parameter, the row's `->m_X = value;` store target `long m_X;` becomes `float m_X;` so the store stays a raw dword copy."""
import sys, re
for f in sys.argv[1:]:
    t = open(f, encoding="utf-8").read()
    m = re.search(r"\bvoid CGameScriptInterface::\w+\(CScriptThing \w+, float (\w+)\)", t)
    if not m: print("skip", f, "no float def"); continue
    v = m.group(1); st = re.findall(r"->\s*(m_\w+)\s*=\s*" + v + r"\s*;", t)
    if len(st) != 1: print("skip", f, "stores", st); continue
    mem = st[0]
    t2, n = re.subn(r"(?m)^(\s*)long " + mem + r";(\s*)//([^\n]*)$", r"\1float " + mem + r";\2//\3 (float: the PDB setter's parameter type; retail copies the raw dword)", t)
    if n != 1: print("skip", f, "decl", n); continue
    open(f, "w", encoding="utf-8", newline="\n").write(t2); print("edit", f, mem)
