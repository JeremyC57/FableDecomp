#!/usr/bin/env python3
"""Wave 8 family TIMER: rows that define a local `class CTimer { <one long word> [methods] };` move onto engine/CTimer.h
(Ego_r.pdb CTimer {long TimerIndex}; methods ??0 ??1 ??4(long) GetTime const ==, !=, <, >, <=, >= GetIndex Pause Unpause).
The word is respelled TimerIndex; inline bodies the local model gave (ctor / dtor / GetTime / the invented SetTime / Set
setter, which is the retail operator=(long), Ego_r 0x00CCE720 ??4CTimer@@QAEAAV0@J@Z) are defined after the include with
the same inline / __forceinline keyword; call sites `x.SetTime(v);` become `x = v;`.
usage: mig_ctimer.py [--dry] <file>..."""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from nsutil import mask, top_insert_pos
SETTERS = ("SetTime", "Set", "operator=")
def block(t, m):
    i = t.index("{", m.start()); d = 0; j = i
    while True:
        if t[j] == "{": d += 1
        elif t[j] == "}":
            d -= 1
            if d == 0: break
        j += 1
    k = j + 1
    while k < len(t) and t[k] in " \t": k += 1
    if k >= len(t) or t[k] != ";": return None
    k += 1
    if k < len(t) and t[k] == "\n": k += 1
    return m.start(), i, j, k
def members(body):
    """split a masked class body into member texts (ends at ; at depth 0 or at a closing } of a body at depth 0)"""
    out = []; d = 0; s = 0; i = 0
    while i < len(body):
        c = body[i]
        if c in "({[": d += 1
        elif c in ")}]":
            d -= 1
            if d == 0 and c == "}":
                out.append((s, i + 1)); s = i + 1
                if body[s:].lstrip().startswith(";"): s = body.index(";", s) + 1
        elif c == ";" and d == 0:
            out.append((s, i + 1)); s = i + 1
        i += 1
    if body[s:].strip(): out.append((s, len(body)))
    return out
def postfix_start(m, i):
    """start of the postfix expression ending just before i (masked text)"""
    j = i
    while j > 0:
        c = m[j - 1]
        if c.isalnum() or c in "_.": j -= 1; continue
        if c == ">" and j >= 2 and m[j - 2] == "-": j -= 2; continue
        if c in ")]":
            op = "(" if c == ")" else "["; d = 0; k = j - 1
            while k >= 0:
                if m[k] == c: d += 1
                elif m[k] == op:
                    d -= 1
                    if d == 0: break
                k -= 1
            j = k; continue
        break
    return j
def fix(t):
    if '#include "engine/CTimer.h"' in t: return None, "already"
    mt = mask(t)
    ms = list(re.finditer(r"^[ \t]*(?:class|struct)\s+CTimer\s*\{", mt, re.M))
    if len(ms) != 1: return None, "blocks %d" % len(ms)
    b = block(mt, ms[0])
    if not b: return None, "noblock"
    s0, bo, bc, e0 = b
    ins = top_insert_pos(t, s0)
    if ins is None: return None, "nested"
    body = t[bo + 1:bc]; mbody = mt[bo + 1:bc]
    word = None; wtype = None; defs = []; setter = None; getter_used = False; ctor_decl = dtor_decl = False
    for a, z in members(mbody):
        raw = body[a:z]; mm = mbody[a:z]
        x = re.sub(r"\b(public|private|protected)\s*:", " ", mm).strip()
        rawx = re.sub(r"\b(public|private|protected)\s*:", " ", raw).strip()
        if not x or x == ";": continue
        m = re.fullmatch(r"(long|unsigned long|int|unsigned|unsigned int|DWORD)\s+(\w+)\s*;", x)
        if m:
            if word: return None, "two words"
            wtype, word = m.group(1), m.group(2); continue
        if re.fullmatch(r"CTimer\s*\(\s*\)\s*;", x): ctor_decl = True; continue
        if re.fullmatch(r"~CTimer\s*\(\s*\)\s*;", x): dtor_decl = True; continue
        fi = "__forceinline" if x.startswith("__forceinline") else "inline"
        x2 = re.sub(r"^(__forceinline|inline)\s+", "", x); r2 = re.sub(r"^(__forceinline|inline)\s+", "", rawx)
        m = re.fullmatch(r"CTimer\s*\(\s*\)\s*(:\s*\w+\s*\(.*\))?\s*\{.*\}", x2, re.S)
        if m:
            k = r2.index("("); defs.append((fi, "CTimer::CTimer" + r2[k:])); continue
        m = re.fullmatch(r"~CTimer\s*\(\s*\)\s*\{.*\}", x2, re.S)
        if m:
            k = r2.index("("); defs.append((fi, "CTimer::~CTimer" + r2[k:])); continue
        m = re.fullmatch(r"(void|long|CTimer\s*&)\s+(\w+|operator\s*=)\s*\(\s*(?:const\s+)?(long|int)\s+(\w+)\s*\)\s*\{(.*)\}", x2, re.S)
        if m and re.sub(r"\s", "", m.group(2)) in SETTERS:
            nm = re.sub(r"\s", "", m.group(2))
            if setter: return None, "two setters"
            ob = r2.index("{"); bod = r2[ob + 1:r2.rindex("}")]
            if m.group(1) == "void":
                if re.search(r"\breturn\b", bod): return None, "setter return"
                bod = bod.rstrip() + " return *this; "
            elif m.group(1) == "long": return None, "setter long"
            setter = nm
            defs.append((fi, "CTimer& CTimer::operator=(%s %s) {%s}" % (m.group(3), m.group(4), bod))); continue
        m = re.fullmatch(r"long\s+GetTime\s*\(\s*\)\s*(const\s*)?\{(.*)\}", x2, re.S)
        if m:
            ob = r2.index("{"); defs.append((fi, "long CTimer::GetTime() const " + r2[ob:])); continue
        return None, "member " + x[:60]
    if not word: return None, "no word"
    if wtype not in ("long",): return None, "word type " + wtype
    # respell word and setter outside the block
    outside = mt[:s0] + " " * (e0 - s0) + mt[e0:]
    others = re.findall(r"(?:class|struct)\s+\w+[^;{]*\{", outside)
    reps = []
    if word != "TimerIndex":
        uses = [m for m in re.finditer(r"(\.|->)\s*" + word + r"\b", outside)]
        if uses:
            if re.search(r"\b(long|int|unsigned|DWORD|void\s*\*)\s+" + word + r"\s*[;\[]", outside): return None, "word name clash"
            for m in uses: reps.append((m.end() - len(word), m.end(), "TimerIndex"))
        if re.search(r"\bCTimer::" + word + r"\b", outside): return None, "qualified word"
    if setter and setter != "operator=":
        for m in re.finditer(r"(\.|->)\s*" + setter + r"\s*\(", outside):
            if m.group(1) == "->": return None, "setter via ->"
            if re.search(r"\b(void|bool|long)\s+" + setter + r"\s*\(", outside): return None, "setter name clash"
            p = m.start(); q0 = m.end() - 1; d = 0; q = q0
            while True:
                if outside[q] == "(": d += 1
                elif outside[q] == ")":
                    d -= 1
                    if d == 0: break
                q += 1
            ps = postfix_start(outside, p)
            lhs = t[ps:p]; arg = t[q0 + 1:q]
            pre = outside[:ps].rstrip(); post = outside[q + 1:].lstrip()
            if not (pre[-1:] in (";", "{", "}", ")") and post.startswith(";")): return None, "setter not a statement"
            if pre.endswith(")") and not re.search(r"\b(if|else|while|for)\s*\([^;]*\)$", pre[-120:]): return None, "setter after )"
            if not lhs.strip(): return None, "setter lhs"
            reps.append((ps, q + 1, "%s = %s" % (lhs, arg.strip())))
    if setter == "operator=" or setter is None:
        if re.search(r"\.\s*(SetTime|Set)\s*\(", outside) and not re.search(r"\b(void|bool|long)\s+(SetTime|Set)\s*\(", outside):
            return None, "setter call without def"
    # moved bodies: respell the word
    dl = []
    for fi, d in defs:
        if word != "TimerIndex": d = re.sub(r"\b" + word + r"\b", "TimerIndex", d)
        dl.append("%s %s" % (fi, d))
    # check nothing in the bodies uses names local to an enclosing namespace: only allow top-level blocks
    if ins != s0 and dl: return None, "ns with bodies"
    new = []
    if "FABLE_RETAIL_NSCRIPT" not in t[:ins]: new.append("#define FABLE_RETAIL_NSCRIPT")
    new.append("// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail")
    new.append("//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.")
    new.append('#include "engine/CTimer.h"')
    new += dl
    newtxt = "\n".join(new) + "\n"
    out = t
    edits = sorted(reps + [(s0, e0, "" if ins != s0 else newtxt)], reverse=True)
    for a, z, r in edits: out = out[:a] + r + out[z:]
    if ins != s0: out = out[:ins] + newtxt + out[ins:]
    return out, "ok setter=%s defs=%d" % (setter, len(dl))
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); r, why = fix(t)
        if r is None: print("skip", f, why); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(r)
        print("edit", f, why)
