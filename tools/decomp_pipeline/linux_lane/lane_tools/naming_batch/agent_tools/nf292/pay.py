# pay.py <outdir> <addr>...: build a vland --recheck-landed payload with the repo's recheck.py (catalog entry, flags, leaf, landed
# test, pass pattern), then swap in rows/<addr>.cpp (and rows/<addr>_test.cpp when present) for each changed row.
# The oracle file holds retail bytes: keep <outdir> inside work/.
import sys,os,json,subprocess,re
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF292'
out=sys.argv[1]; addrs=sys.argv[2:]; os.makedirs(out,exist_ok=True)
r=subprocess.run(['python3','tools/decomp_pipeline/linux_lane/lane_tools/recheck.py',out]+addrs,cwd='/home/user/FableDecomp',capture_output=True,text=True)
print(r.stdout.strip(),r.stderr.strip()[-500:])
p=json.load(open(out+'/payload.json'))
for e in p['result']['authored']:
    a=e['address']; f=A+'/rows/%s.cpp'%a; t=A+'/rows/%s_test.cpp'%a
    if os.path.exists(f):
        e['source_cpp']=open(f,encoding='utf-8').read()
        m=re.search(r'^//\s*Leaf:\s*(\S+)',e['source_cpp'],re.M)
        if m: e['leaf']=m.group(1)
        print(a,'row source from rows/')
    if os.path.exists(t): e['test_cpp']=open(t,encoding='utf-8').read(); print(a,'test from rows/')
json.dump(p,open(out+'/payload.json','w'))
