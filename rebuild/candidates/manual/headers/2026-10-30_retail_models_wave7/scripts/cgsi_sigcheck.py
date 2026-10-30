#!/usr/bin/env python3
"""Wave 4: for each row mig_cgsi.py would edit, compare every named slot the row CALLS (local block) with the shared
header's declaration at that slot: return type, parameter types (names dropped), const. Prints one line per row:
OK | DIFF <slot name: local -> header>... (mcheck stays the byte gate; this is the spelling audit the brief asks for).
usage: cgsi_sigcheck.py <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block, norm
R = os.environ["FABLE_ROOT"]
H = open(R + "/rebuild/include/engine/CGameScriptInterface.h").read()
H = H[H.index("class CGameScriptInterface : public CGameScriptInterfaceBase {"):]
H = H[:H.index("};")]
KW = {"int","long","char","short","float","double","bool","unsigned","signed","void","const"}
def canon(s):
    s = re.sub(r"\s+", " ", s).strip()
    s = re.sub(r"\bstd::", "", s)
    s = re.sub(r"vector<(\w+)\s*,\s*allocator<\1\s*>\s*>", r"vector<\1>", s)
    s = re.sub(r"^(\w+(?:<[^&*]*>)?) const\b", r"const \1", s)
    return re.sub(r"\s*([&*])", r"\1", s).strip()
def rtype(r): return canon(r)
def ptype(p):
    p = re.sub(r"=.*$", "", p).strip()
    p = re.sub(r"\s+", " ", p)
    p = re.sub(r"\s*([&*])\s*", r"\1 ", p).strip()
    toks = p.split(" ")
    if len(toks) > 1 and re.fullmatch(r"\w+", toks[-1]) and toks[-1] not in KW: toks = toks[:-1]
    s = " ".join(toks).replace("& ", "&").replace("* ", "*").strip()
    return canon(s)
def split_params(ps):
    out, d, cur = [], 0, ""
    for c in ps:
        if c in "<(": d += 1
        if c in ">)": d -= 1
        if c == "," and d == 0: out.append(cur); cur = ""
        else: cur += c
    if cur.strip(): out.append(cur)
    r = [ptype(x) for x in out]
    return [] if r == ["void"] else r
DECL = re.compile(r"virtual\s+([^;{}]*?)(~\w+|\b\w+)\s*\(([^;{}]*)\)\s*(const)?\s*(?:=\s*0)?\s*;")
HS = {}
for m in re.finditer(r"virtual\s+([^;{}]*?)(~?\w+)\s*\(([^;{}]*)\)\s*(const)?\s*;\s*//\s*(?:slot\s*)?(\d+) 0x", H):
    HS[int(m.group(5))] = (m.group(2), rtype(m.group(1)), split_params(m.group(3)), bool(m.group(4)))
def called(t, name): return re.search(r"(?:->|\.)" + re.escape(name) + r"\s*\(", t) is not None
for f in sys.argv[1:]:
    t = open(f, encoding="utf-8").read()
    b = find_block(t, "CGameScriptInterface")
    if not b: print(f, "NOBLOCK"); continue
    rest = t[:b[0]] + t[b[1]:]
    idx = 0; diffs = []
    for d in DECL.finditer(re.sub(r"//[^\n]*|/\*.*?\*/", "", t[b[0]:b[1]], flags=re.S)):
        name = d.group(2)
        if name.startswith("~"): idx += 1; continue
        if re.fullmatch(r"[vsg]\d+|VFunc\d+|vf\d+|Slot\d+|UnknownSlot\d+", name) or not called(rest, name): idx += 1; continue
        h = HS.get(idx)
        loc = (name, rtype(d.group(1)), split_params(d.group(3)), bool(d.group(4)))
        if h is None: diffs.append(f"{idx}:{name}: no header slot")
        elif (loc[1], loc[2], loc[3]) != (h[1], h[2], h[3]):
            diffs.append(f"{idx}:{name}->{h[0]}: {loc[1]}({','.join(loc[2])}){' const' if loc[3] else ''} -> {h[1]}({','.join(h[2])}){' const' if h[3] else ''}")
        idx += 1
    print(f, "OK" if not diffs else "DIFF " + " | ".join(diffs))
