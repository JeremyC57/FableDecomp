# pd.py <addr> <probe.cpp> <leaf>: diff the probe build against retail (from g.sh listing of the parked src), ignoring call targets
import sys,re,subprocess,os,difflib
S=os.environ["S"]; W=f"{S}/w22/retry10"
addr,src,leaf=sys.argv[1:4]
ret=subprocess.run(["bash",f"{W}/g.sh",addr],capture_output=True,text=True).stdout
r=re.findall(r"--- retail.*?(?=--- built)",ret,re.S)[0].splitlines()[1:]
pb=subprocess.run(["python3",f"{W}/probe.py",src,leaf],capture_output=True,text=True).stdout
b=pb.splitlines()[1:]
if not pb.startswith("---"): print(pb[-1500:]); sys.exit(1)
f=lambda L:[" ".join(l.split()[2:]) for l in L if l.strip()]
rr=f(r); bb=f(b)
print(f"retail {len(rr)} built {len(bb)} insns; {pb.splitlines()[0]}")
n=0
for l in difflib.unified_diff(rr,bb,lineterm="",n=0):
    if not l.startswith(" ") and "call 0x" not in l and not l.startswith(("@@","---","+++")): print(l); n+=1
print("diff lines:",n)
