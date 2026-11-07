# sub.py <src.cpp> <leaf> <addr-hex> <len-hex> [flags...]: compile src (overlay agent857/hdr), diff leaf against retail exe bytes
import sys,os,re,struct,difflib
sys.path.insert(0,"/home/user/FableDecomp/tools/decomp_pipeline")
S=os.environ['S']; A=S+'/w22/agent857'; os.environ["FABLE_HDR_OVERLAY"]=A+"/hdr"
import verify_and_land as V
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
src,leaf,addr,ln=sys.argv[1:5]; fl=sys.argv[5:]
addr=int(addr,16); ln=int(ln,16)
b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; ns=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
for i in range(ns):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from('<IIII',b,o+8)
    if 0x400000+va<=addr<0x400000+va+rs: retail=b[addr-0x400000-va+ro:addr-0x400000-va+ro+ln]
w=Path(A)/'w'/'sub'; w.mkdir(parents=True,exist_ok=True)
st,built,sec=V.parity_of(V.vc71(open(src).read()),'sub',leaf,retail,w,V.env(),extra=tuple(fl))
print(st)
if built:
    rel=V.obj_relocs(w/'sub.obj',sec)
    bp=bytearray(built)
    for o in rel:
        if o+4<=len(bp): bp[o:o+4]=(0x00ABCDEF).to_bytes(4,'little')
    cs=Cs(CS_ARCH_X86,CS_MODE_32)
    def dis(x): return [i.mnemonic+' '+i.op_str for i in cs.disasm(x,addr)]
    n=lambda s: re.sub(r'^(call|j[a-z]+) \S+',r'\1 X',re.sub(r'0x[0-9a-f]{5,}','ADDR',s))
    r=dis(retail); o=dis(bytes(bp))
    d=[l for l in difflib.unified_diff([n(x) for x in r],[n(x) for x in o],lineterm='',n=1)]
    print(len([l for l in d if l[:1] in '+-' and l[:3] not in('---','+++')]),'diff lines'); print('\n'.join(d[:int(os.environ.get('NL','60'))]))
