# Convert the STLport namespace spelling _STL -> std (retail mangles @std@@). usage: conv.py files...
import re, sys
R = re.compile(r'(?<![A-Za-z0-9_])_STL(?![A-Za-z0-9_])')
n = 0
for f in sys.argv[1:]:
    t = open(f, encoding="utf-8", newline="").read()
    u = R.sub("std", t)
    if u != t:
        open(f, "w", encoding="utf-8", newline="").write(u); n += 1; print(f)
print("changed", n, file=sys.stderr)
