# espmap.py <addr> <tag>: retail->ours esp-offset pairs from aligned instructions (offsets masked when aligning)
import sys,os,re,subprocess,difflib,collections
A=os.environ['S']+'/w22/agent860'; a,tag=sys.argv[1:3]
r=subprocess.run(['python3',A+'/bin/rdis.py',a],capture_output=True,text=True).stdout.splitlines()
o=subprocess.run(['bash',A+'/bin/o.sh',a,tag],capture_output=True,text=True).stdout.splitlines()
def ins(l): return l[30:].strip() if len(l)>30 else l
R=[ins(l) for l in r]; O=[ins(l) for l in o]
pat=re.compile(r'esp \+ (0x[0-9a-f]+|\d+)\]')
def norm(s):
    s=pat.sub('esp+N]',s); s=re.sub(r'0x[0-9a-f]{5,}','A',s); s=re.sub(r'^(call|j\w+) \S+',r'\1 X',s)
    s=re.sub(r'\b(e[abcd]x|e[sd]i|ebp|[abcd]l)\b','R',s); return s
sm=difflib.SequenceMatcher(None,[norm(x) for x in R],[norm(x) for x in O],autojunk=False)
m=collections.defaultdict(collections.Counter)
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op!='equal': continue
    for k in range(i2-i1):
        x=pat.search(R[i1+k]); y=pat.search(O[j1+k])
        if x and y: m[int(x.group(1),16)][int(y.group(1),16)-int(x.group(1),16)]+=1
for k in sorted(m): print(hex(k),' '.join(f'{d:+#x}:{c}' for d,c in m[k].most_common()))
