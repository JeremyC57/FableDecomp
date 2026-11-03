import sys,re,difflib
t=open(sys.argv[1]).read()
if '--- retail' not in t: sys.exit()
r=t.split('--- retail')[1].split('--- built')[0].splitlines()[1:]
b=t.split('--- built')[1].splitlines()[1:]
def nl(l):
    b=l[10:33].strip()
    if b.startswith('68') and len(b)==10: return 'push X'
    return re.sub(r'\[(\w+\*4) \+ X\]',r'[\1]',re.sub(r'\[0\]','[X]',re.sub(r'0x[0-9a-f]{5,8}','X',l[33:]))).strip()
norm=lambda L:[nl(l) for l in L if l.strip()]
R,B=norm(r),norm(b)
d=[l for l in difflib.unified_diff(R,B,lineterm='',n=1)]
print(sum(1 for l in d if l[:1] in '+-' and l[:3] not in ('+++','---')))
print('\n'.join(d))
