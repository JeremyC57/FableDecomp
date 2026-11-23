#!/usr/bin/env python3
# delaudit.py: for every TABLE_CHANGES delete / replace line (old symbol X, target T), list every exe reference to T: E8/E9 call or jmp
# sites (xsites.py) and 4-byte absolute references (absrows.py: vtable slots, deleter stores). For each landed referencing row R:
#   MOVED     R is in rows.txt (respelled in this hand-back; its new object no longer references X: proof_u*.txt symdiff)
#   OBJ_X     R's HEAD object (work/obj/H_R.obj) references X            -> the delete would break R (must be a rewitness)
#   OBJ_NOX   R's HEAD object does not reference X
#   SRC_NAME  no HEAD object; R's source names the old member name        -> needs reading
#   SRC_NONE  no HEAD object; R's source does not name the old member name
# NF309.
import subprocess,os,re,sys
A=os.path.dirname(os.path.abspath(__file__))+'/..'
R='/home/user/FableDecomp/rebuild/src/compiled'
moved=set(l.strip().lower().replace('0x','') for l in open(A+'/rows.txt') if l.strip())
def run(t,addr): return subprocess.run(['python3',A+'/tools/'+t,addr],capture_output=True,text=True).stdout.splitlines()
def objsyms(r):
    p=A+'/work/obj/H_%s.obj'%r
    if not os.path.exists(p): return None
    out=subprocess.run(['objdump','-t',p],capture_output=True,text=True).stdout.splitlines()
    return set(l.split()[-1] for l in out if '(scl   2)' in l)
def srcof(r):
    import glob
    g=glob.glob(R+'/%s/%s/*_%s.cpp'%(r[:2],r[2:4],r))
    return open(g[0],errors='replace').read() if g else None
def member(x):
    if x.startswith('??0'): return x[3:].split('@')[0]
    return x.lstrip('?@').split('@')[0]
for l in open(A+'/TABLE_CHANGES.tsv').read().splitlines()[1:]:
    f=l.split('\t'); act,sym,tgt,wit=f[:4]
    if act not in ('delete','replace'): continue
    X=sym.split(' -> ')[0].strip()
    print('== %s %s target=%s witness=%s'%(act,X,tgt,wit))
    refs=[]
    for s in run('xsites.py',tgt):
        c=s.split('\t'); refs.append((c[1],c[2],c[3]))
    for s in run('absrows.py',tgt):
        c=s.split('\t'); refs.append(('abs',c[1],c[2]))
    if not refs: print('   (no exe reference)')
    for kind,site,row in refs:
        if row=='-': print('   %s %s DATA (no row; vtable / data)'%(kind,site)); continue
        r=row[:8]; land=row[8]=='L'
        if not land: print('   %s %s %s CATALOG_ONLY'%(kind,site,row[:70])); continue
        if r==tgt: v='SELF'
        elif r in moved: v='MOVED'
        else:
            s=objsyms(r)
            if s is not None: v='OBJ_X' if X in s else 'OBJ_NOX'
            else:
                t=srcof(r); m=member(X)
                v='NOSRC' if t is None else ('SRC_NAME' if re.search(r'\b'+re.escape(m)+r'\b',t) else 'SRC_NONE')
        print('   %s %s %s %s'%(kind,site,row[:70],v))
