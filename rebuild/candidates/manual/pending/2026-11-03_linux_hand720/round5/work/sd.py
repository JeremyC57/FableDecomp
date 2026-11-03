import sys,re,difflib
t=open(sys.argv[1]).read()
r=t.split('--- retail')[1].split('--- built')[0].splitlines()[1:]
b=t.split('--- built')[1].splitlines()[1:]
def nl(l):
    s=re.sub(r'0x[0-9a-f]{5,8}','X',l[33:]).strip()
    s=s.replace('[0]','[X]').replace('*4]','*4 + X]')
    s=re.sub(r'\[ebp - 0x[0-9a-f]+\]','[ebp-N]',s); s=re.sub(r'\[ebp - \d\]','[ebp-N]',s)
    return s
R=[nl(l) for l in r if l.strip()]; B=[nl(l) for l in b if l.strip()]
d=list(difflib.unified_diff(R,B,lineterm='',n=2))
print(sum(1 for l in d if l[:1] in '+-' and l[:3] not in ('+++','---')))
print('\n'.join(d))
