#!/usr/bin/env python3
"""scan_externs.py <base.cpp> N1 N2 ... : insert N `extern int __sym_pad_k;` declarations after the last #include
(before the function), grade each with gr.sh, print N / status / masked diff-line count."""
import sys, os, re, subprocess
S=os.environ['S']; A=f'{S}/w22/research861'
base=sys.argv[1]; addr=os.path.basename(base)[:8]
src=open(base).read().split('\n')
incs=[i for i,l in enumerate(src) if l.startswith('#include')]
if incs: last_inc=max(incs)
else: last_inc=min(i for i,l in enumerate(src) if l.strip() and not l.startswith('//'))-1
mode = os.environ.get('PADMODE','extern')
for n in sys.argv[2:]:
    n=int(n)
    if mode=='extern': pad=[f'extern int __sym_pad_{k};' for k in range(n)]
    elif mode=='struct': pad=[f'struct __sym_pad_s{k} {{ int a; }};' for k in range(n)]
    elif mode=='func': pad=[f'void __sym_pad_f{k}(int, int);' for k in range(n)]
    elif mode=='prefn': pad=[f'extern int g__p{k}[8]; int __pre_f{k}(int a, int b, int c) {{ int t = a * b + g__p{k}[c]; g__p{k}[a] = t - b; for (int i = 0; i < c; ++i) t += g__p{k}[i] * b; return t ^ a; }}' for k in range(n)]
    elif mode=='postfn': pad=[]
    elif mode=='prefp': pad=[f'extern float gf__p{k}[8]; float __pre_g{k}(float a, float b, int c) {{ float t = a * b + gf__p{k}[c]; gf__p{k}[c] = t - b; return t * a - b / t; }}' for k in range(n)]
    out=src[:last_inc+1]+pad+src[last_inc+1:]
    if mode=='postfn': out=src+[f'extern int g__q{k}[8]; int __post_f{k}(int a, int b, int c) {{ int t = a * b + g__q{k}[c]; g__q{k}[a] = t - b; for (int i = 0; i < c; ++i) t += g__q{k}[i] * b; return t ^ a; }}' for k in range(n)]
    p=f'{A}/tmp/{addr}_pad{mode}{n}.cpp'; open(p,'w').write('\n'.join(out))
    r=subprocess.run([f'{A}/gr.sh',p],capture_output=True,text=True).stdout.split('\n')
    st=r[0].split()[2] if len(r[0].split())>2 else r[0]
    td=[l for l in r if l.startswith('textdiff')]
    print(f'{mode} N={n:5d}  {st}  {td[0] if td else ""}', flush=True)
