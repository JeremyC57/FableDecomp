#!/usr/bin/env python3
"""sigfix.py [--dry] <file>...: the row defines CGameScriptInterface::<leaf> with a signature the shared header (PDB
spellings) does not declare (C2511 / C2556 after the header migration). Rewrite the definition's return type, parameter
types and const to the header's single declaration of that name, keeping the row's parameter names. mcheck is the gate:
a type that changes the code (by-value vs reference, float vs long) shows up as DIFFER and the row is reverted."""
import sys, re, os
R = os.environ["FABLE_ROOT"]
H = open(R + "/rebuild/include/engine/CGameScriptInterface.h").read()
H = H[H.index("class CGameScriptInterface : public CGameScriptInterfaceBase {"):]; H = H[:H.index("};")]
def split(s):
    out, d, cur = [], 0, ""
    for c in s:
        if c in "<(": d += 1
        elif c in ">)": d -= 1
        if c == "," and d == 0: out.append(cur.strip()); cur = ""
        else: cur += c
    if cur.strip(): out.append(cur.strip())
    return out
def fix(t, name):
    decls = re.findall(r"virtual\s+([^;()]*?)\s*\b" + name + r"\s*\(([^;]*)\)\s*(const)?\s*;", H)
    if len(decls) != 1: return None, f"{len(decls)} header decls"
    hret, hpar, hconst = decls[0]
    rx = re.compile(r"^([^\n;{}()]*?)\bCGameScriptInterface::" + name + r"\s*\(([^)]*)\)([ \t]*)(const)?(\s*(?:\{|\n|//))", re.M)
    ms = list(rx.finditer(t))
    if len(ms) != 1: return None, f"{len(ms)} definitions"
    m = ms[0]
    rp = [p for p in split(m.group(2)) if p != "void"]; hp = [p for p in split(hpar) if p != "void"]
    if len(rp) != len(hp): return None, "arity"
    np = []
    for r, h in zip(rp, hp):
        mm = re.search(r"([A-Za-z_]\w*)\s*$", r)
        nm = mm.group(1) if mm and not re.fullmatch(r"(long|int|float|bool|char|short|double|unsigned|signed|const|\w+[&*])", r.split()[-1]) and len(r.split()) > 1 else ""
        if nm and r.rstrip().endswith(("&", "*")): nm = ""
        np.append((h + " " + nm).strip())
    head = m.group(1)
    pre = re.match(r"^(\s*(?:inline\s+|__declspec\([^)]*\)\s*)*)", head).group(1)
    new = f"{pre}{hret} CGameScriptInterface::{name}({', '.join(np)}){' const' if hconst else ''}{m.group(5)}"
    return t[:m.start()] + new + t[m.end():], "ok"
if __name__ == "__main__":
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        name = re.search(r"CGameScriptInterface_(\w+?)_[0-9a-f]{8}\.cpp$", f)
        if not name: print("skip", f, "name"); continue
        t = open(f, encoding="utf-8").read()
        r, why = fix(t, name.group(1))
        if r is None: print("skip", f, why); continue
        if "--dry" not in sys.argv: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
