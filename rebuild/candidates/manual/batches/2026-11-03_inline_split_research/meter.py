#!/usr/bin/env python3
"""meter.py src.cpp CALLEE_SUBSTR MARK_SUBSTR [T values...] [-- extra cl flags]
For each function in the TU (other than callee), print the per-site pattern
(i = inline expansion, seen as a call to MARK; C = call to CALLEE) at each /d2inlT value.
T='-' means default (74)."""
import sys,subprocess,re,os
S=os.environ['S']
args=sys.argv[1:]
extra=[]
if '--' in args: i=args.index('--'); extra=args[i+1:]; args=args[:i]
src,callee,mark=args[:3]; Ts=args[3:] or ['-']
def pattern(asm):
    out={}; cur=None
    for ln in asm.splitlines():
        m=re.match(r'^[0-9a-f]+ <(.*)>:',ln)
        if m: cur=m.group(1); out[cur]=''; continue
        m=re.search(r'(DISP32|DIR32|REL32)\s+(\S+)',ln)
        if m and cur:
            if callee in m.group(2): out[cur]+='C'
            elif mark in m.group(2): out[cur]+='i'
    return out
rows={}
for T in Ts:
    fl=([] if T=='-' else [f'/d2inlT{T}'])+extra
    r=subprocess.run([f'{S}/inlinesplit/cc.sh',src]+fl,capture_output=True,text=True)
    asmf=src[:-4]+'.asm'
    if 'COMPILE FAIL' in r.stdout: print('FAIL',T,r.stdout); continue
    for f,p in pattern(open(asmf).read()).items():
        if callee in f or mark in f: continue
        rows.setdefault(f,[]).append(p or '.')
w=max(len(f) for f in rows)
print(' '*min(w,40)+'  '+'  '.join(f'T={t}' for t in Ts))
for f,ps in rows.items(): print(f[:40].ljust(min(w,40))+'  '+'  '.join(ps))
