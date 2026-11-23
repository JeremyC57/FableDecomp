#!/usr/bin/env python3
# delaudit2.py: for every TABLE_CHANGES delete / replace line (old symbol X at target T), the landed binders of X from the ENTRIES
# `spellings` column (the tracker's per-spelling binder list: "sym (b1,b2,...)"), each classified MOVED (in rows.txt), SELF (the target's
# own row), or LEFT (still binds X after this hand-back -> the line must be a rewitness, not a delete). Also: exe callers of T (xsites /
# absrows) whose row landed mid-run (work/landed_mid.txt), with a source grep for the old member name. NF309.
import re,os,subprocess
A=os.path.dirname(os.path.abspath(__file__))+'/..'
moved=set(l.strip().lower().replace('0x','') for l in open(A+'/rows.txt') if l.strip())
mid=set(l.split()[0].lower().replace('0x','') for l in open(A+'/work/landed_mid.txt') if l.strip())
E={}
for l in open(A+'/ENTRIES.tsv').read().splitlines()[1:]:
    c=l.split('\t'); E.setdefault(c[0].replace('0x','').lower(),[]).append(c[11])
bad=0
for l in open(A+'/TABLE_CHANGES.tsv').read().splitlines()[1:]:
    f=l.split('\t'); act,sym,tgt,wit=f[:4]
    if act not in ('delete','replace'): continue
    X=sym.split(' -> ')[0].strip(); binders=None
    for sp in E.get(tgt,[]):
        for part in sp.split(' || '):
            m=re.match(r'\s*(\S+) \(([^)]*)\)',part)
            if m and m.group(1)==X: binders=[b for b in m.group(2).split(',') if b]
    if binders is None: print('%s %s @%s: X not in ENTRIES spellings (stale-line entry; see evidence/u*_head_binding / u8_stale_lines)'%(act,X,tgt)); 
    else:
        cl=[]
        for b in binders:
            v='SELF' if b in ('self',tgt) else ('MOVED' if b in moved else 'LEFT')
            if v=='LEFT': bad+=1
            cl.append('%s:%s'%(b,v))
        print('%s %s @%s witness=%s binders: %s'%(act,X,tgt,wit,' '.join(cl)))
    xs=subprocess.run(['python3',A+'/tools/xsites.py',tgt],capture_output=True,text=True).stdout.splitlines()+\
       [ '\t'.join(['abs']+s.split('\t')[1:]) for s in subprocess.run(['python3',A+'/tools/absrows.py',tgt],capture_output=True,text=True).stdout.splitlines()]
    for s in xs:
        c=s.split('\t'); row=c[-1]; r=row[:8]
        if r in mid: print('    MID-RUN LANDED caller %s %s'%(c[-2] if len(c)>2 else '',row[:70]))
print('LEFT binders total:',bad)
