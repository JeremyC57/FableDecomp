# print the local definition block of a class (from `class X` / `struct X` at column 0 to the matching `};`)
import re, sys
def block(text, cls):
    m = re.search(r'^[ \t]*(?:class|struct)\s+' + re.escape(cls) + r'\b[^;{]*\{', text, re.M)
    if not m: return None
    i = m.end(); d = 1
    while d and i < len(text):
        c = text[i]
        if c == '{': d += 1
        elif c == '}': d -= 1
        i += 1
    j = text.find(';', i)
    return text[m.start():j+1]
if __name__ == "__main__":
    cls = sys.argv[1]
    for f in sys.argv[2:]:
        b = block(open(f, encoding='utf-8', errors='replace').read(), cls)
        print("=== " + f); print(b)
