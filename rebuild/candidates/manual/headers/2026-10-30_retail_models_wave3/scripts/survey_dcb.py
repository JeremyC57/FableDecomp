# survey_dcb.py <cluster ids...>: per file in those CDefClassBase clusters, list the other local def-chain classes (normalised)
import sys, re, collections
sys.path.insert(0, __file__.rsplit('/',1)[0]); from common import find_block, norm
S="/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad"
keys=set(sys.argv[1:])
files=[l.split("\t")[1].strip() for l in open(S+"/mig3/work/dcb_clusters.tsv") if l.split("\t")[0] in keys]
C=["CIVCountedPointeeBase","CResource","CDefPointeeBase","CSubDefClassBase","CParentDefClassBase","CDefBase","CBaseClass"]
shapes=collections.Counter(); ex={}
for f in files:
    t=open(f,encoding="utf-8").read()
    sig=[]
    for c in C:
        b=find_block(t,c)
        if b: sig.append(c+"{"+norm(t[b[0]:b[1]])[len(c)+6:][:150]+"}")
    sig.append("PACK" if "#pragma pack(push, 1)" in t else "nopack")
    sig.append("DEFRET" if "FABLE_RETAIL_DEF_CLASS_BASE" in t else "")
    sig.append("INCL:"+",".join(sorted(set(re.findall(r'#include "engine/(\w+)\.h"',t)))))
    k=" | ".join(sig); shapes[k]+=1; ex.setdefault(k,f)
for k,v in shapes.most_common(): print(v, ex[k].split('/')[-1], "\n    ", k[:900])
