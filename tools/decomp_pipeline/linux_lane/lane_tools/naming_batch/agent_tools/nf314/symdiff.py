# symdiff.py <addr>...: external (scl 2) symbols only in HEAD obj / only in new obj, per row (work/obj/H_<a>.obj, N_<a>.obj)
import sys,subprocess
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF314'
def syms(p): return set(l.split()[-1] for l in subprocess.run(['objdump','-t',p],capture_output=True,text=True).stdout.splitlines() if '(scl   2)' in l)
for a in sys.argv[1:]:
    h=syms(A+'/work/obj/H_%s.obj'%a); n=syms(A+'/work/obj/N_%s.obj'%a)
    print(a,'H-only',sorted(h-n),'N-only',sorted(n-h))
