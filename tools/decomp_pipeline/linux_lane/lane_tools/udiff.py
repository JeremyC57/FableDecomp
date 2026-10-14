import sys,re,difflib
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
f=lambda L:[l.split(None,2)[2] if len(l.split(None,2))>2 else l for l in L]
for l in difflib.unified_diff(f(r),f(b),lineterm="",n=1): print(l)
