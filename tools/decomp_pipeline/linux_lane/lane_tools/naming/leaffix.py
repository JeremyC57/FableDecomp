#!/usr/bin/env python3
"""leaffix.py files...: set the class-key letters inside mangled `// Leaf:` lines to retail's key
(KEYS.tsv). A `U`/`V` type reference `Name@Q1@Q2@@` (or `?$Tmpl@args@Q@@`) is resolved to its full
qualified name Q2::Q1::Name; only exact KEYS entries are rewritten (names under invented namespaces,
backreferenced qualifiers and std:: are left as they are). The grade verifies each leaf still names a
defined symbol."""
import re, sys, os
from pathlib import Path
HERE = Path(__file__).resolve().parent
keys = os.environ.get("KEYCONV_KEYS", str(HERE.parents[4] / "ghidra_out" / "pdb_class_keys.tsv"))
types = {}; tmpls = {}
for line in open(keys, encoding="utf-8").read().splitlines()[1:]:
    p = line.split("\t")
    if p[2] not in ("U", "V") or p[0].startswith("std::") or "`" in p[0]: continue
    (tmpls if p[1] == "template" else types)[p[0]] = p[2]

def skip_name(s, i):
    """skip one mangled name fragment starting at i (identifier@, ?$template@args@, or a digit backref);
    returns (text-or-None, end)."""
    if i < len(s) and s[i].isdigit(): return None, i + 1
    if s.startswith("?$", i):
        j = s.find("@", i + 2)
        if j < 0: return None, len(s)
        name = s[i + 2:j]
        # template args: walk until the matching terminating '@' -- approximate by depth on '?$' / '@@'
        k = j + 1; depth = 1
        while k < len(s) and depth:
            if s.startswith("?$", k):
                depth += 1; k = s.find("@", k + 2) + 1; continue
            if s.startswith("@", k):
                depth -= 1; k += 1; continue
            k += 1
        return "<" + name, k
    j = s.find("@", i)
    if j < 0: return None, len(s)
    return s[i:j], j + 1

def fix(leaf):
    out = list(leaf); n = 0
    for m in re.finditer(r"(?<=[A-Z@$0-9])[UV](?=[A-Za-z_?])", leaf):
        i = m.end(); parts = []
        frag, i = skip_name(leaf, i)
        if frag is None: continue
        parts.append(frag)
        ok = True
        while i < len(leaf) and leaf[i] != "@":
            q, i = skip_name(leaf, i)
            if q is None or q.startswith("<"): ok = False; break
            parts.append(q)
        if not ok or i >= len(leaf): continue
        head = parts[0]
        qual = "::".join(reversed(parts[1:]))
        if head.startswith("<"):
            full = (qual + "::" if qual else "") + head[1:]; want = tmpls.get(full)
        else:
            full = (qual + "::" if qual else "") + head; want = types.get(full)
        if want and want != leaf[m.start()]:
            out[m.start()] = want; n += 1
    return "".join(out), n

if __name__ == "__main__":
    tot = nf = 0
    for f in sys.argv[1:]:
        t = open(f, encoding="utf-8", errors="surrogateescape", newline="").read()
        c = [0]
        def rl(m):
            if not m.group(2).startswith("?"): return m.group(0)
            new, n = fix(m.group(2)); c[0] += n
            return m.group(1) + new
        t2 = re.sub(r"(^//\s*Leaf:\s*)(\S+)", rl, t, flags=re.M)
        if c[0]:
            open(f, "w", encoding="utf-8", errors="surrogateescape", newline="").write(t2); tot += c[0]; nf += 1
            print(f"{f}\t{c[0]}")
    print(f"leaffix: {tot} key letter(s) in {nf} file(s)", file=sys.stderr)
