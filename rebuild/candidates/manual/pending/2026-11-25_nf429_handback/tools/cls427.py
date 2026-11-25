#!/usr/bin/env python3
"""classify NF427 worklist rows: member, self form, header model state -> evidence/classify.tsv"""
import csv,re,os,collections
R='/home/user/FableDecomp'; S=os.environ['S']
rows=list(csv.DictReader(open('evidence/worklist.tsv'),delimiter='\t'))
vth=set(open(S+'/nf425_vtheld.txt').read().split())
def lock(a):
    p=f"{S}/w22/nflocks/{a}/owner"
    return open(p).read().strip() if os.path.exists(p) else 'none'
out=[]
for r in rows:
    C=r['fam']; m=r['member']; a=r['target']
    h=f'{R}/rebuild/include/engine/{C}.h'
    hs='nohdr'; decl='-'
    if os.path.exists(h):
        t=open(h,errors='replace').read()
        vm=re.search(r'#ifdef FABLE_RETAIL_%s_VIRTUAL_MODEL(.*?)#else'%C.upper(),t,re.S)
        if vm:
            hs='vmodel'
            decl='y' if re.search(r'\b%s\s*\('%re.escape(m),vm.group(1)) else 'n'
        elif re.search(r'virtual',t): hs='poly'
        else: hs='flat'
    sl=r['self_leaf']
    if re.match(r'\?%s_\w+@@Y'%re.escape(C),sl) or re.match(r'\?\w+_%s@@Y'%m,sl): form='flat_fastcall'
    elif sl.startswith('?%s@%s@@'%(m,C)): form='member_same'
    elif sl.startswith('??'): form='special'
    elif '@@Y' in sl: form='free_other'
    elif sl=='' : form='noleaf'
    else: form='member_other'
    src=f"{R}/rebuild/src/compiled/{r['source']}" if r['source'] else ''
    body=''
    if src and os.path.exists(src):
        t=open(src,errors='replace').read()
        b=[l for l in t.splitlines() if l.strip() and not l.strip().startswith('//') and not l.startswith('#')]
        body=' '.join(b)[-160:]
    out.append([a,r['status'],r['tier'],C,m,form,hs,decl,lock(a),'V' if a in vth else '',sl,r['rtti'],r['source'],body])
w=csv.writer(open('evidence/classify.tsv','w'),delimiter='\t')
w.writerow(['addr','status','tier','class','member','self_form','hdr','decl','lock','vth','self_leaf','rtti','source','body_tail'])
w.writerows(out)
c=collections.Counter((x[4] if x[4] not in('Clone',) else 'Clone',x[5],x[6],x[7]) for x in out if x[4]!='Clone')
for k,v in c.most_common(60): print(v,k)
