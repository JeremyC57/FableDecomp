# assert_names_check.py <base-include> <new-include>: for every header in both trees, compare the effective typedef name of each
# FABLE_STATIC_ASSERT (line number, or the pinned number of FABLE_STATIC_ASSERT_AT) matched by its expression text and occurrence.
import os,re,sys,collections
base,new=sys.argv[1:3]
pat=re.compile(r'FABLE_STATIC_ASSERT(?:_AT\(\s*(\d+)\s*,|\()\s*(.*)\);\s*$')
def names(p):
    d=collections.defaultdict(list)
    for i,l in enumerate(open(p,encoding='utf-8',errors='replace').read().split('\n'),1):
        m=pat.search(l)
        if m and not l.lstrip().startswith('#define') and not l.lstrip().startswith('//'):
            d[' '.join(m.group(2).split())].append(int(m.group(1)) if m.group(1) else i)
    return d
bad=0
for root,_,fs in os.walk(new):
    for f in fs:
        if not f.endswith('.h'): continue
        rel=os.path.relpath(os.path.join(root,f),new); bp=os.path.join(base,rel)
        if not os.path.exists(bp): continue
        a,b=names(bp),names(os.path.join(root,f))
        for k,v in b.items():
            if k in a and a[k]!=v[:len(a[k])] and len(v)==len(a[k]):
                print(rel,k[:70],'base',a[k],'now',v); bad+=1
print('mismatches',bad)
