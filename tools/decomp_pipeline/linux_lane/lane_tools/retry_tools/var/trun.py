import sys,os,subprocess
from pathlib import Path
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S=Path(os.environ["S"]); os.environ["FABLE_VC71"]=f"{S}/wine/vc71rtm"; os.environ["LANDVERIFY_WORK"]=f"{S}/wine/landverify_rtm"; os.environ["LINK"]="bufferoverflowu.lib"
e=V.env(); src=Path(sys.argv[1]); work=S/"wine"/"tcomp"; work.mkdir(parents=True,exist_ok=True)
tp=work/"t.cpp"; tp.write_text(V.vc71(src.read_text()),encoding="utf-8"); tobj=work/"t.obj"; exe=work/"t.exe"
tc=V.cl(["/nologo","/c","/Od","/W3",f"/Fo{tobj}",str(tp)],e); print(tc.stdout[-1500:])
lk=V.run_tool(V.VC/"bin"/"link.exe",["/nologo",f"/out:{exe}",str(tobj)],e); print(lk.stdout[-800:])
rn=subprocess.run(([V.WINE] if V.WINE else [])+[str(exe)],capture_output=True,text=True,errors="replace",env=e); print("rc",rn.returncode); print(rn.stdout[-2000:])
