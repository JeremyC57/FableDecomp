# mkpay.py <dir> <addr>...: recheck.py payload + oracle for the rows (run from the repo root), then each row's source_cpp replaced by
# rows/<addr>.cpp (the changed row; rows/<addr>_test.cpp too when present). The oracle holds retail bytes: keep <dir> under work/, never in a hand-back file.
import json,subprocess,sys
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF306/'
d=sys.argv[1]; ad=sys.argv[2:]
import os,re; os.makedirs(d,exist_ok=True)   # NF306: recheck.py needs the directory
subprocess.run(['python3','tools/decomp_pipeline/linux_lane/lane_tools/recheck.py',d]+ad,check=True,cwd='/home/user/FableDecomp')
p=json.load(open(d+'/payload.json'))
for e in p['result']['authored']:
    e['source_cpp']=open(A+'rows/%s.cpp'%e['address'],encoding='utf-8').read()
    m=re.search(r'^//\s*Leaf:\s*(\S+)',e['source_cpp'],re.M)
    if m: e['leaf']=m.group(1)   # NF306: a changed Leaf line (0x0077CCA0) follows the row
    print('patched',e['address'],e.get('leaf'),e.get('flags'))
    import os
    if os.path.exists(A+'rows/%s_test.cpp'%e['address']):   # NF306: a changed test (stub respelled where the test links the row)
        e['test_cpp']=open(A+'rows/%s_test.cpp'%e['address'],encoding='utf-8').read(); print('patched test',e['address'])
json.dump(p,open(d+'/payload.json','w'))
