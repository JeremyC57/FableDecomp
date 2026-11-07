import sys,re,difflib
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def norm(l):
    p=l.split(None,2); s=p[2] if len(p)>2 else l
    s=re.sub(r" \+ 0x[0-9a-f]{6,}\]","]",s); s=re.sub(r"\[0x[0-9a-f]{6,}\]|\[0\]","[G]",s)
    s=re.sub(r"^(call|j\w+) 0x[0-9a-f]+",r"\1 T",s); s=re.sub(r"(push|mov \w+, )0x[0-9a-f]{6,}$",r"\1G",s)
    return s
R=[norm(x) for x in r]; B=[norm(x) for x in b]
d=[l for l in difflib.unified_diff(R,B,lineterm="",n=1)]
n=sum(1 for l in d if l[:1] in "+-" and l[:3] not in ("---","+++"))
if len(sys.argv)>2: print("\n".join(d))
print("MD",n)
