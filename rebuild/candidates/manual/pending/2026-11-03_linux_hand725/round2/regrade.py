import json,subprocess,os
E='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agent725b'
d=json.load(open(E+'/payload.json'))
for r in d['result']['authored']:
    a=r['address']; p=f'{E}/src/{a}.cpp'
    fl=r.get('flags') or []
    out=subprocess.run(['bash',E+'/var.sh',a,p,'rg',r['leaf'],'-']+fl,capture_output=True,text=True,env=dict(os.environ,AGENTDIR=E)).stdout.strip().split('\n')[-1]
    print(out)
