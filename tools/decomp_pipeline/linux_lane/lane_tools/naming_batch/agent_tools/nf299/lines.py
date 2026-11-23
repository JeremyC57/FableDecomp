# lines.py <mapfile>: per "binder old new" line, the code lines (comments stripped) naming old (or already naming new) in rows/<b>.cpp,
# else the repo source.
import re,sys,subprocess,os
A='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF299'
for ln in open(sys.argv[1]):
    a,o,n=ln.split(); f=A+'/rows/%s.cpp'%a
    if not os.path.exists(f): f=subprocess.run(['bash',A+'/tools/srcof.sh',a],capture_output=True,text=True).stdout.strip()
    print('== %s %s -> %s'%(a,o,n))
    for i,l in enumerate(open(f,encoding='utf-8').read().split('\n')):
        c=l.split('//')[0]
        if re.search(r'\b%s\b'%re.escape(o),c): print('  %d: %s'%(i+1,c.strip()[:140]))
        elif re.search(r'\b%s\b'%re.escape(n),c): print('  NEW %d: %s'%(i+1,c.strip()[:140]))
