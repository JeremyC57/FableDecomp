#!/usr/bin/env python3
"""mkpay.py <out.json> <addr>... : one payload from final/<addr>.cpp + final/<addr>_test.cpp (rows.tsv gives module/name; Leaf/Flags from source)"""
import sys,json,re,csv
D='$S/w22/agentP6F'
rows={r['address']:r for r in csv.DictReader(open(D+'/rows.tsv'),delimiter='\t')}
out=[]
for a in sys.argv[2:]:
    s=open(f'{D}/final/{a}.cpp').read(); t=open(f'{D}/final/{a}_test.cpp').read()
    leaf=re.search(r'// Leaf: (.*)',s).group(1).strip()
    fl=re.search(r'^// Flags: (.*)$',s,re.M); flags=re.findall(r'/[A-Za-z0-9:-]+',fl.group(1)) if fl else []
    pp=re.findall(r'"([0-9a-f]{8}_TEST PASS)',t)
    out.append(dict(address=a,module=rows[a]['module'],name=rows[a]['name'],leaf=leaf,source_cpp=s,test_cpp=t,pass_pattern=pp[-1] if pp else a+'_TEST PASS',flags=flags))
json.dump({'result':{'authored':out}},open(sys.argv[1],'w'),indent=1)
print([(o['address'],o['flags'],o['pass_pattern']) for o in out])
