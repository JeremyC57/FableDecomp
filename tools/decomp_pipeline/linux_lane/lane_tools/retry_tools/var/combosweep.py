# combosweep.py <addr>...: LowerBound stand-in (lbsweep conversion) + def stand-ins (defsweep3 conversion) in one TU, probe-graded.
import sys,os,subprocess,importlib.util
W=os.environ["S"]+"/w22/retry10"
ARGS=sys.argv[1:]
def load(n):
    spec=importlib.util.spec_from_file_location(n,f"{W}/var/{n}.py"); m=importlib.util.module_from_spec(spec)
    sys.argv=[n]; spec.loader.exec_module(m); return m
lb=load("lbsweep"); d3=load("defsweep3")
for a in ARGS:
    p=f"{W}/src/{a}.cpp"
    if not os.path.exists(p): print(a,"no src"); continue
    src=open(p).read()
    r1=lb.convert(src) if hasattr(lb,"convert") else None
    s1=r1[0] if isinstance(r1,tuple) else r1
    if not s1: s1=src; w1="lb:none"
    else: w1="lb:ok"
    s2,w2=d3.convert(s1)
    if s2 is None: s2=s1
    vp=f"{W}/var/{a}_combo.cpp"; open(vp,"w").write(s2)
    r=subprocess.run(["python3",f"{W}/var/pv.py",a,vp],capture_output=True,text=True)
    lines=r.stdout.strip().splitlines()
    head=lines[0] if lines else r.stdout[-300:].replace("\n"," ")
    tail=lines[-1] if lines else ""
    print(a,"|",w1,w2,"|",head[:70],"|",tail,flush=True)
