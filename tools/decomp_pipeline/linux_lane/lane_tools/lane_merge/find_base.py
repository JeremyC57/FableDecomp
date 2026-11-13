"""find_base.py <header relpath> <overlay file> <out>: write the version of rebuild/include/<header> from this branch's history that the
overlay was made from: the newest version all of whose lines survive in the overlay (else the one losing the fewest). Prints what it chose."""
import subprocess,sys,difflib
try:
    h,ov,out=sys.argv[1:4]; p='rebuild/include/'+h
    o=open(ov,errors='replace').read().split('\n')
    cs=subprocess.run(['git','log','--format=%h','-n','80','--',p],capture_output=True,text=True).stdout.split()
    best=None
    for c in cs:
        t=subprocess.run(['git','show',f'{c}:{p}'],capture_output=True,text=True,errors='replace').stdout
        b=t.split('\n'); lost=0
        for op,a1,a2,b1,b2 in difflib.SequenceMatcher(None,b,o,autojunk=False).get_opcodes():
            if op in('delete','replace'): lost+=a2-a1
        if best is None or lost<best[0]: best=(lost,c,t)
        if lost==0: break
    if best is None:
        open(out,'w').write(open(p,errors='replace').read()); print('base=repo(no history)')
    else:
        open(out,'w').write(best[2]); print(f'base={best[1]} lost={best[0]}')
except Exception as e:
    print('find_base failed:',type(e).__name__,str(e)[:120])
