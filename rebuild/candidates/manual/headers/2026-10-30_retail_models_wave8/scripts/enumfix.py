#!/usr/bin/env python3
"""enumfix.py [--dry] <file>...: compile each row (base flags) and, for every C2664 "cannot convert parameter N from 'int'
to 'E'" (E an enum), wrap argument N of the call on that line in `(E)`. Bytes are unchanged: an int literal converted to
an enum is the same constant. Repeats up to 4 rounds per file. Prints what it changed and any other first error."""
import sys, os, re
ROOT = os.environ["FABLE_ROOT"]; sys.path.insert(0, ROOT + "/tools/decomp_pipeline")
import verify_and_land as V
S = os.environ["S"]; w = S + "/mig8/work/enumfix"; os.makedirs(w, exist_ok=True)
def compile_(src):
    lines = src.splitlines(); keep = [i for i, l in enumerate(lines) if "static_assert" not in l]
    text = V.vc71(src); p = w + "/t.cpp"; open(p, "w").write(text)
    cp = V.cl(["/nologo", "/c"] + V.BASE_FLAGS + ["/Fo" + w + "/t.obj", p], V.env())
    return cp.returncode, (cp.stdout or ""), keep
def split_args(s, start):
    """s[start] is just after '('; return list of (a, b) spans of top-level args and the index of the closing ')'."""
    d = 0; i = start; spans = []; a = start; q = None
    while i < len(s):
        c = s[i]
        if q:
            if c == "\\": i += 2; continue
            if c == q: q = None
        elif c in "\"'": q = c
        elif c in "([{": d += 1
        elif c in ")]}":
            if d == 0: spans.append((a, i)); return spans, i
            d -= 1
        elif c == "," and d == 0: spans.append((a, i)); a = i + 1
        i += 1
    return None, None
def fix_one(src):
    rc, out, keep = compile_(src)
    if rc == 0: return src, [], None
    lines = src.splitlines(keepends=True); changed = []; other = None
    for m in re.finditer(r"t\.cpp\((\d+)\) : error C2664: '([^']*)' : cannot convert parameter (\d+) from '(?:const )?(?:int|long|unsigned long|bool)' to '(\w+)'", out):
        ln = keep[int(m.group(1)) - 1]; fn = m.group(2).split("::")[-1]; n = int(m.group(3)); E = m.group(4)
        L = lines[ln]
        for cm in re.finditer(r"\b" + re.escape(fn) + r"\s*\(", L):
            spans, end = split_args(L, cm.end())
            if not spans or len(spans) < n: continue
            a, b = spans[n - 1]; arg = L[a:b]
            if arg.strip().startswith("(" + E + ")"): continue
            lead = len(arg) - len(arg.lstrip())
            L = L[:a + lead] + "(" + E + ")" + L[a + lead:]
            changed.append(f"L{ln+1} {fn} arg{n} -> ({E})"); break
        lines[ln] = L
    if not changed:
        e = re.search(r"error C\d+: [^\n]{0,200}", out); other = e.group(0) if e else out[-300:]
    return "".join(lines), changed, other
if __name__ == "__main__":
    dry = "--dry" in sys.argv
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        src = open(f, encoding="utf-8").read(); allc = []; other = None
        for _ in range(4):
            src2, ch, other = fix_one(src)
            if not ch: break
            allc += ch; src = src2
        if allc and not dry: open(f, "w", encoding="utf-8", newline="\n").write(src)
        print(f, "fixed" if allc else "nofix", "; ".join(allc), ("| " + other) if other else "")
