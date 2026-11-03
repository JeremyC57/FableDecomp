import sys,re,difflib
t=open(sys.argv[1]).read()
if '--- retail' not in t: sys.exit()
r=t.split('--- retail')[1].split('--- built')[0].splitlines()[1:]
b=t.split('--- built')[1].splitlines()[1:]
def nl(l):
    bb=l[10:33].strip()
    if bb.startswith('68') and len(bb)==10: return 'push X'
    return re.sub(r'\[(\w+\*4) \+ X\]',r'[\1]',re.sub(r'\[0\]','[X]',re.sub(r'0x[0-9a-f]{5,8}','X',l[33:]))).strip()
R=[nl(l) for l in r if l.strip()]; B=[nl(l) for l in b if l.strip()]
n=int(sys.argv[2]) if len(sys.argv)>2 else 1
d=[l for l in difflib.unified_diff(R,B,lineterm='',n=n)]
print(sum(1 for l in d if l[:1] in '+-' and l[:3] not in ('+++','---')))
print('\n'.join(d))
