# dump.py <worker> <n> <list>: save g.sh output (with the src's Leaf/Flags) to lst/<addr>.txt
import sys,os,re,subprocess
S="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"; W=f"{S}/w22/retry10"
wk,nw,lst=int(sys.argv[1]),int(sys.argv[2]),sys.argv[3]
leafs={l.split("\t")[0]:l.split("\t")[2] for l in open(f"{W}/rows.tsv")}
env=dict(os.environ); env["FABLE_VC71"]=f"{S}/wine/vc71rtm"; env["LANDVERIFY_WORK"]=f"{S}/wine/landverify_rtm_w{wk}"; env["LINK"]="bufferoverflowu.lib"
for i,a in enumerate([l.strip() for l in open(lst) if l.strip()]):
    if i%nw!=wk: continue
    src=f"{W}/src/{a}.cpp"; txt=open(src,errors="replace").read()
    m=re.search(r"^// Leaf:\s*(\S+)",txt,re.M); leaf=m.group(1) if m else leafs.get(a,"")
    fm=re.search(r"^// Flags:\s*(.*)$",txt,re.M); fl=fm.group(1).split() if fm else []
    r=subprocess.run(["python3",f"{S}/tools/q.py",a,src,leaf]+(["--flags"]+fl if fl else []),capture_output=True,text=True,env=env,cwd="/home/user/FableDecomp").stdout
    open(f"{W}/s7/lst/{a}.txt","w").write(r)
