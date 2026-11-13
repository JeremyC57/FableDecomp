"""nesting_check.py <old-commit>: for every header changed since <old-commit>, every line that survives must still sit under the conditions
it sat under before (the old stack of enclosing #if conditions must be a subsequence of the new one; new enclosing conditions are allowed).
Catches merges that balance by accident (a stray #endif closing the wrong block). Prints the violations."""
import subprocess,sys,re,difflib,os
old=sys.argv[1]
NAMES=lambda c: frozenset(re.findall(r'[A-Z_][A-Z0-9_]{3,}',c))
def stacks(text):
    """Per line: the tuple of enclosing branches, each as the set of macro names that decide it (an #else carries the names of its whole chain)."""
    st=[]; out=[]
    for l in text.split('\n'):
        s=re.sub(r'//.*','',l).strip(); s=re.sub(r'\s+',' ',s)
        if re.match(r'# ?if',s): st.append([NAMES(s),NAMES(s)]); out.append(None)
        elif re.match(r'# ?elif',s):
            if st: st[-1]=[NAMES(s),st[-1][1]|NAMES(s)]
            out.append(None)
        elif re.match(r'# ?else',s):
            if st: st[-1]=[st[-1][1],st[-1][1]]
            out.append(None)
        elif re.match(r'# ?endif',s):
            if st: st.pop()
            out.append(None)
        else: out.append(tuple(x[0] for x in st))
    return out
def subseq(a,b):
    it=iter(b); return all(any(x==y for y in it) for x in a)
files=subprocess.run(['git','diff','--name-only',old,'--','rebuild/include'],capture_output=True,text=True).stdout.split(); bad=0
for f in files:
    if not os.path.exists(f): continue
    p=subprocess.run(['git','show',f'{old}:{f}'],capture_output=True,text=True,errors='replace')
    if p.returncode: continue
    a=p.stdout.split('\n'); b=open(f,errors='replace').read().split('\n'); sa=stacks(p.stdout); sb=stacks('\n'.join(b)); n=0
    for op,a1,a2,b1,b2 in difflib.SequenceMatcher(None,a,b,autojunk=False).get_opcodes():
        if op!='equal': continue
        for k in range(a2-a1):
            x=sa[a1+k]; y=sb[b1+k]
            if x is None or y is None or not a[a1+k].strip(): continue
            it=iter(y)
            if not all(any(cx<=cy for cy in it) for cx in x):
                if n<2: print(f'{f.replace("rebuild/include/","")}:{b1+k+1}: was under {[sorted(c)[:3] for c in x][-2:]} now under {[sorted(c)[:3] for c in y][-2:]} | {b[b1+k].strip()[:60]}')
                n+=1
    if n: bad+=1; print(f'  -> {n} surviving lines of {f.replace("rebuild/include/","")} lost an enclosing condition')
print('headers checked',len(files),'| with violations',bad)
