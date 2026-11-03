# vecspec2.py <elem> <addr:Name>... : as vecspec.py for std::vector<elem, std::allocator<elem> > (no default argument in the
# forward declaration, so a later full vector template in the same file still compiles); uses respelled in full.
import sys, glob, os, shutil, re
S = os.environ["S"]; O = S + "/wave34/orig/"
E = sys.argv[1]; full = f"std::vector<{E}, std::allocator<{E}> >"
for arg in sys.argv[2:]:
    a, name = arg.split(":")
    p = glob.glob(f"rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp")[0]
    t = open(p).read()
    m = re.search(r"^struct " + name + r" \{[^\n]*\n", t, re.M)
    i = m.start(); j = t.index("\n};", m.end()) + 3
    body = t[m.end():j]
    body = re.sub(r"(~?)\b" + name + r"\(", r"\1vector(", body)   # its own ctor / dtor
    head = ("namespace std {\ntemplate <class _Tp> class allocator;\ntemplate <class _Tp, class _Alloc> class vector;\n"
            f"template <> class vector<{E}, allocator<{E}> > {{ public:   // STLport vector<{E}> (12 bytes)\n")
    t = t[:i] + head + body + "\n}" + t[j:]
    t = re.sub(r"\b" + name + r"\b", full, t)
    if not os.path.exists(O + p): os.makedirs(os.path.dirname(O + p), exist_ok=True); shutil.copy(p, O + p)
    open(p, "w").write(t); print("ok", p)
