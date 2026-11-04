# collect.py <famdir> <log> : record per-row grades with the sha1 of the graded source+test (from the payload in the log's rk dir)
import sys,re,json,os,hashlib,glob,time
W='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/wave35b'
fam=sys.argv[1]
pay={}
for p in glob.glob(fam+'/rk_part_*/payload.json'):
    for e in json.load(open(p))['result']['authored']:
        pay[e['address']]=hashlib.sha1((e['source_cpp']+'\0'+e['test_cpp']).encode()).hexdigest()
st={}; warn={}
for lg in sys.argv[2:]:
    for l in open(lg):
        m=re.match(r'^([0-9a-f]{8})\s+(\S+)\s+(\S+)?',l)
        if not m: continue
        a,g,b=m.groups()
        if g=='WARN':
            w=l.split('WARN',1)[1].strip()
            if not w.startswith('SHARED_HEADER_AVAILABLE'): warn.setdefault(a,[]).append(w)
            continue
        st[a]=(g,b)
out=open(W+'/results.tsv','a')
for a,(g,b) in st.items():
    ok=bool(re.match(r'^(RELOCATION_)?MATCH(\[[^\]]*\])?$',g)) and b=='PASS' and not warn.get(a)
    out.write('\t'.join([a,'PASS' if ok else 'FAIL',g+' '+str(b),'; '.join(warn.get(a,[])),os.path.basename(fam.rstrip('/')),pay.get(a,'?'),os.path.abspath(sys.argv[2]),time.strftime('%H:%M:%S')])+'\n')
print(len(st),'recorded')
