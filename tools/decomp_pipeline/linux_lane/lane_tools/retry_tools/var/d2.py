import sys,re,difflib
t=open(sys.argv[1]).read()
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
f=lambda L:[" ".join(l.split()[2:]) for l in L if l.strip()]
for l in difflib.unified_diff(f(r),f(b),lineterm="",n=0):
    if not l.startswith(" ") and "call 0x" not in l and not l.startswith("@@") and not l.startswith("---") and not l.startswith("+++"): print(l)
