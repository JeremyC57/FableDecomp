# superchunk.py <addr>...: replace the invented C3DMeshFileChunk base with CChunkedFileSuperChunk (RTTI direct base; protected ctor 0x00AA8360)
import sys, re, glob, os, shutil
S = os.environ["S"]; O = S + "/wave34/orig/"
for a in sys.argv[1:]:
    p = glob.glob(f"rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp")[0]; s0 = s = open(p, encoding="utf-8").read()
    s, n1 = re.subn(r"class CChunkedFileSuperChunk \{ public:\n    virtual ~CChunkedFileSuperChunk\(\);\n\};\nstruct C3DMeshFileChunk : CChunkedFileSuperChunk \{\n    C3DMeshFileChunk\(const char\* (\w+)\);(.*)\n(    unsigned char _base_body.*\n)\};",
                    r"class CChunkedFileSuperChunk { public:\n    virtual ~CChunkedFileSuperChunk();\nprotected:\n    CChunkedFileSuperChunk(const char* \1);\2 (Ego_r / FableWin protected IAE; RTTI: the direct base of the root chunks)\npublic:\n\3};", s)
    n2 = 0
    if not n1:
        s, n2 = re.subn(r"^struct C3DMeshFileChunk \{", "class CChunkedFileSuperChunk { public:      // RTTI direct base of the mesh/anim chunks (was the invented C3DMeshFileChunk)", s, flags=re.M)
        s = re.sub(r"^(\s*)C3DMeshFileChunk\(const char\* (\w+)\);(.*)$", r"\1protected:\n\1CChunkedFileSuperChunk(const char* \2);\3 (Ego_r / FableWin protected IAE)\n\1public:", s, flags=re.M)
    s = s.replace("C3DMeshFileChunk", "CChunkedFileSuperChunk")
    if s != s0:
        if not os.path.exists(O + p): os.makedirs(os.path.dirname(O + p), exist_ok=True); shutil.copy(p, O + p)
        open(p, "w", encoding="utf-8").write(s)
    print(a, n1, n2)
