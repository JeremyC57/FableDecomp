import sys,os
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
src=sys.argv[1]; out=src[:-4]+".asm"
e=V.env()
cp=V.cl(["/nologo","/c"]+V.BASE_FLAGS+["/FAs","/Fa"+out,"/Fo"+src[:-4]+".obj",src],e)
print(cp.stdout[-1500:],cp.stderr[-500:])
