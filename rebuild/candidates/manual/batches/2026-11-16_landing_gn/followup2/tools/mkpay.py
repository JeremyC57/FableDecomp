# mkpay.py <out.json> <addr>... : recheck payload from the landed rows (source, test, flags from // Flags:, leaf from // Leaf: or catalog name)
import json,re,sys
ps=open('rebuild/build_candidates.ps1',encoding='utf-8').read()
cat={l.split('\t')[0]:l.split('\t')[1] for l in open('rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8') if '\t' in l}
out=[]
for a in sys.argv[2:]:
    i=ps.index("Address = '%s'"%a); blk=ps[i:ps.index('}',i)]
    g=lambda k:(re.search(r"%s = '([^']*)'"%k,blk) or [None,None])[1]
    s=open('rebuild/src/compiled/'+g('Source'),encoding='utf-8').read(); t=open('rebuild/tests/'+g('TestSource'),encoding='utf-8').read()
    fl=re.search(r'// Flags:\s*(.*)',s); lf=re.search(r'// Leaf:\s*(\S+)',s)
    nm=cat[a].split('::')[-1]
    out.append(dict(address=a,module=g('Module'),name=nm,source_cpp=s,test_cpp=t,pass_pattern=g('PassPattern'),
                    flags=[x for x in fl.group(1).split() if x!='none'] if fl else [],leaf=lf.group(1) if lf else nm))
json.dump({'result':{'authored':out}},open(sys.argv[1],'w'),indent=1); print(len(out))
