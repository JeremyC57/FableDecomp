# micro.py <file.cpp> [flags...]: compile a micro TU with VC7.1 (BASE_FLAGS + flags) and disassemble all functions
import sys,os,subprocess
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src=sys.argv[1]; fl=sys.argv[2:]
obj=src[:-4]+'.obj'
e=V.env()
cp=V.cl(["/nologo","/c"]+V.BASE_FLAGS+fl+[f"/Fo{obj}",src],e)
if cp.returncode: print(cp.stdout[-2000:]); sys.exit(1)
print(subprocess.run([V.OBJDUMP,"-d","-M","intel","--no-show-raw-insn",obj],capture_output=True,text=True).stdout)
