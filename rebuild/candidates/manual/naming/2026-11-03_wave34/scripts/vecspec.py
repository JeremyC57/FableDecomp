# vecspec.py <tag> <addr:Name>... : replace a local `struct Name {...};` stand-in for std::vector<long> with an explicit
# specialisation std::vector<long, std::allocator<long> > (same members) and respell every use; saves pristine copies via orig/.
import sys, glob, os, shutil, re
S = os.environ["S"]; O = S + "/wave34/orig/"
for arg in sys.argv[2:]:
    a, name = arg.split(":")
    p = glob.glob(f"rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp")[0]
    t = open(p).read()
    m = re.search(r"^struct " + name + r" \{[^\n]*\n", t, re.M)
    i = m.start(); j = t.index("\n};", m.end()) + 3
    body = t[m.end():j]
    head = ("namespace std {\ntemplate <class _Tp> class allocator;\ntemplate <class _Tp, class _Alloc = allocator<_Tp> > class vector;\n"
            "template <> class vector<long, allocator<long> > { public:   // STLport vector<long> (12 bytes)\n")
    t = t[:i] + head + body + "\n}" + t[j:]
    t = re.sub(r"\b" + name + r"\b", "std::vector<long>", t)
    if not os.path.exists(O + p): os.makedirs(os.path.dirname(O + p), exist_ok=True); shutil.copy(p, O + p)
    open(p, "w").write(t); print("ok", p)
