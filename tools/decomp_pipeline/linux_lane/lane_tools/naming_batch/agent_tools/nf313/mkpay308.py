# mkpay305.py (NF313 copy of mkpay302.py) <outdir> <addr>...: recheck.py payload from the catalog, then each row's source_cpp (and leaf, when the row has a
# `// Leaf:` line) replaced by rows/<addr>.cpp, so vland --recheck-landed grades the NEW source (tests unchanged). Run from the repo root.
import sys,json,re,subprocess,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF313'
out=sys.argv[1]; addrs=sys.argv[2:]; os.makedirs(out,exist_ok=True)
print(subprocess.run(['python3','tools/decomp_pipeline/linux_lane/lane_tools/recheck.py',out]+addrs,capture_output=True,text=True).stdout.strip())
p=json.load(open(out+'/payload.json'))
for e in p['result']['authored']:
    f=A+'/rows/%s.cpp'%e['address']
    if os.path.exists(f):
        s=open(f,encoding='utf-8').read(); e['source_cpp']=s
        m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M)
        if m: e['leaf']=m.group(1)
        print(e['address'],'source from rows/, leaf',e['leaf'][:80])
    ft=A+'/rows/%s_test.cpp'%e['address']
    if os.path.exists(ft):
        e['test_cpp']=open(ft,encoding='utf-8').read(); print(e['address'],'test from rows/')   # NF313: changed test
json.dump(p,open(out+'/payload.json','w'))
