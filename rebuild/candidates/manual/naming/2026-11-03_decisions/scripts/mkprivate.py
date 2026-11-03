# mkprivate.py <src>...: put the in-class `template <class _KT> ... _M_find(` declaration of _Rb_tree under `private:`
# (STLport 4.6 _tree.h line 460), restoring the access label that was in effect after it.
import re, sys
for p in sys.argv[1:]:
    L = open(p, encoding="utf-8").read().split("\n"); out = []; acc = None; key = None; done = 0
    for i, line in enumerate(L):
        mc = re.match(r"^\s*(?:template\s*<[^;]*>\s*)?(class|struct)\s+(\w+)\b[^;]*$", line)
        if mc: key = mc.group(1); acc = "private" if key == "class" else "public"
        ma = re.match(r"^\s*(public|protected|private)\s*:", line)
        if ma: acc = ma.group(1)
        if re.match(r"^\s*template <class _KT> .*\b_M_find\(const _KT&", line) and acc != "private":
            ind = re.match(r"^(\s*)", line).group(1)
            out += ["private:", line, f"{acc}:"]; done += 1; continue
        out.append(line)
    if done: open(p, "w", encoding="utf-8").write("\n".join(out)); print("private", done, p)
    else: print("unchanged", p)
