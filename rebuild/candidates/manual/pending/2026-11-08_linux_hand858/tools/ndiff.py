# ndiff.py <addr> <tag> <leaf>: retail vs ours with ebp offsets / call targets / relocs normalised
import sys,os,re,subprocess,difflib
S=os.environ['S']; A=S+'/w22/agent858'
a,tag,leaf=sys.argv[1:4]
r=subprocess.run(['python3',A+'/bin/rdis.py',a],capture_output=True,text=True).stdout.splitlines()
o=subprocess.run(['python3',A+'/bin/odis.py',a,tag,leaf],capture_output=True,text=True).stdout.splitlines()
def norm(l):
    p=l.split(None,2)
    if len(p)<3: return None
    t=p[2]
    t=re.sub(r'\[ebp [+-] (0x)?[0-9a-f]+\]','[ebp+X]',t); t=re.sub(r'\[ebp\]','[ebp+X]',t); t=re.sub(r'\[esp [+-] (0x)?[0-9a-f]+\]','[esp+X]',t)
    t=re.sub(r'call 0x[0-9a-f]+','call F',t); t=re.sub(r'(j\w+) 0x[0-9a-f]+',r'\1 L',t)
    t=re.sub(r'\[(0x[0-9a-f]+|0)\]','[G]',t); t=re.sub(r'0x1[0-9a-f]{6}\b','G',t)
    return t
R=[norm(x) for x in r]; O=[norm(x) for x in o]
R=[x for x in R if x]; O=[x for x in O if x]
sm=difflib.SequenceMatcher(None,R,O,autojunk=False)
n=0
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal': print(f'  == {i2-i1} (r{i1} o{j1})'); continue
    for x in R[i1:i2]: print('  - '+x); n+=1
    for x in O[j1:j2]: print('  + '+x); n+=1
print('NORM_MD',n,file=sys.stderr)
