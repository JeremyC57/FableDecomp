import json,sys,os,re,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from common import *; from hdrparse import *
W=os.environ['S']+'/wave35a'
d=json.load(open(W+'/classify.json')); ent=ps1_entries()
c=collections.Counter()
for r in d:
    if r['kind']!='ent': continue
    h,base,body=hdr_class(r['O'],r['E'])
    M=r['M']; mm='~' if M.startswith('`') else M
    hs='nohdr' if not h else ('noclass' if body is None else ('decl' if re.search(r'\b%s\s*\('%re.escape(mm if mm!='~' else '~'+r['E']),body) else 'nodecl'))
    pc=''
    if body:
        m=re.search(r'([\w:]+)\s*(&|\*)\s*ParentClass\s*;',body); pc=m.group(2) if m else '?'
    al=[x for x,l in r['alloc'] if l]
    asrc=''
    if al:
        s=open(ROOT+'/rebuild/src/compiled/'+ent[al[0]]['Source']).read()
        n=nested(s,r['E'])
        asrc='A:'+('cls' if n else 'nocls')
        if n:
            m=re.search(r'([\w:]+)\s*(&|\*)\s*ParentClass\s*;',n[1]); asrc+=(m.group(2) if m else '?')
            asrc+= ' decl' if re.search(r'\b%s\s*\('%re.escape(mm if mm!='~' else '~'+r['E']),n[1]) else ' nodecl'
    r['survey']=(hs,pc,asrc)
    c[(M[:8],hs,pc,asrc)]+=1
for k,v in sorted(c.items(),key=lambda x:-x[1]): print(v,k)
json.dump(d,open(W+'/classify.json','w'),indent=0)
