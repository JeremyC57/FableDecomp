import sys,subprocess
D=sys.argv[1]; base=open(sys.argv[2]).read(); tag=sys.argv[-1]
pairs=sys.argv[3:-1]
for k in range(0,len(pairs),2):
    old,new=pairs[k],pairs[k+1]
    assert old in base, 'old not found: '+old[:60]
    base=base.replace(old,new,1)
open(f'{D}/work/b/pr/b_{tag}.cpp','w').write(base)
subprocess.run(f'{D}/work/b/mk.sh {D}/work/b/pr/v_{tag}.cpp {D}/work/b/pr/b_{tag}.cpp',shell=True)
r=subprocess.run(f'bash {D}/work/p.sh {D} 00659800 {D}/work/b/pr/v_{tag}.cpp CalculateSecuritySweepTasks {D}/work/b/pr/o_{tag}.txt',shell=True,capture_output=True,text=True)
t=open(f'{D}/work/b/pr/o_{tag}.txt').read()
r2=subprocess.run(['python3',f'{D}/work/b/pr/dl.py',f'{D}/work/b/pr/o_{tag}.txt'],capture_output=True,text=True)
import re
fr=re.search(r'sub esp, (0x[0-9a-f]+)',t.split('--- built')[1]) if '--- built' in t else None
print(tag, t.splitlines()[0].split()[2] if t else '?', 'frame', fr.group(1) if fr else '?', 'difflines', r2.stdout.splitlines()[0] if r2.stdout else '?')
