# cvcheck.py <head_file> <tag>: compile the ConvertVector body of a head file as an out-of-line function and grade it
# against retail 0x0052E580 (80 bytes); also report the noinline-ness. Run from the repo root with the lane env.
import sys,os,re,struct
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
from pathlib import Path
import verify_and_land as V
S=os.environ["S"]; A=Path(S)/"w22/agent862"
h=open(sys.argv[1]).read(); tag=sys.argv[2]
h=h.replace("inline C2DVector __fastcall NUISystem::CUIScreenUnitConverter::ConvertVector","C2DVector __fastcall NUISystem::CUIScreenUnitConverter::ConvertVector")
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8)
    if va<=0x52e580-0x400000<va+rs: off=0x52e580-0x400000-va+ro; retail=exe[off:off+80]
work=A/"work"/("cv_"+tag); work.mkdir(parents=True,exist_ok=True)
os.environ["FABLE_HDR_OVERLAY"]=str(A/"hdr")
st,built,sec=V.parity_of(V.vc71(h),"0052e580","?ConvertVector@CUIScreenUnitConverter@NUISystem@@SI?AVC2DVector@@V3@@Z",retail,work,V.env(),extra=("/Gr",))
print(tag, st, len(built) if built else None)
