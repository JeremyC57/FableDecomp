# protctor.py <addr>...: make the CChunkedFileChunk / CChunkedFileSuperChunk (const char*) ctor declarations protected (Ego_r / FableWin IAE)
import sys, re, glob, os, shutil
S = os.environ["S"]; O = S + "/wave34/orig/"
rx = re.compile(r"^(\s*)(CChunkedFileChunk|CChunkedFileSuperChunk)\(const char\* (\w+)\);(.*)$", re.M)
for a in sys.argv[1:]:
    p = glob.glob(f"rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp")[0]; s0 = open(p, encoding="utf-8").read()
    s, n = rx.subn(lambda m: f"{m.group(1)}protected:\n{m.group(1)}{m.group(2)}(const char* {m.group(3)});{m.group(4)} (Ego_r / FableWin: protected ??0...@@IAE@PBD@Z)\n{m.group(1)}public:", s0)
    if n:
        if not os.path.exists(O + p): os.makedirs(os.path.dirname(O + p), exist_ok=True); shutil.copy(p, O + p)
        open(p, "w", encoding="utf-8").write(s)
    print(a, n)
