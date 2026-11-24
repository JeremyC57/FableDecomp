# pk.py <obj> : compare named COMDAT copies of the row object with retail (relocation fields masked on both sides)
import sys, struct, re, subprocess
sys.path.insert(0,'/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
EXE='/opt/fable_lane/dl/game/Fable The Lost Chapters/Fable.exe'; d=open(EXE,'rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; ns=struct.unpack_from('<H',d,pe+6)[0]; so=struct.unpack_from('<H',d,pe+20)[0]
secs=[struct.unpack_from('<IIII',d,pe+24+so+40*i+8) for i in range(ns)]
def rd(va,n):
    rva=va-0x400000
    for vs,vaddr,rs,ro in secs:
        if vaddr<=rva<vaddr+rs: return d[ro+rva-vaddr:ro+rva-vaddr+n]
T=[('push_back@?$vector@U?$_List_iterator',0x007115C0),('_M_insert_overflow@?$vector@U?$_List_iterator',0x00711080),
   ('push_back@?$vector@V?$CIVCountedPointer@VC3DAnimationTransitionState',0x00711C50),('_M_insert_overflow@?$vector@V?$CIVCountedPointer',0x00711A50),
   ('??1?$vector@V?$CIVCountedPointer@VC3DAnimationTransitionState',0x00711760),('??$stable_sort@',0x00711C90),
   ('?GetProgressFromAlpha@CAnimateThing',0x0070BE40),('?SetStartState@C3DAnimationTransitionState',0x00710560),
   ('?SetEndState@C3DAnimationTransitionState',0x007105A0),('?GetMeshBank@CAnimateThing',0x0070C0D0),('?GetMeshAnim@CAnimSet',0x0070C900),
   ('_$E2',0x01229630),('??1?$vector@U?$_List_iterator',None)]
out=subprocess.run([V.OBJDUMP,'-t',sys.argv[1]],capture_output=True,text=True).stdout
names=sorted(set(re.findall(r'\(sec\s+[1-9]\d*\).*\(scl\s+[23]\).*0x0+ (\S+)$',out,re.M)))
for pat,va in T:
    hits=[n for n in names if pat in n]
    if not hits: print('ABSENT', hex(va) if va else '-', pat); continue
    for n in hits:
        try: b,sec,sym=V.obj_text(sys.argv[1],n)
        except Exception as e: print('ERR',n[:60],e); continue
        if va is None: print('NOADDR', len(b), n[:110]); continue
        r=bytearray(rd(va,len(b))); o=bytearray(b)
        for off in V.obj_relocs(sys.argv[1],sec):
            for k in range(4):
                if off+k<len(o): o[off+k]=0; r[off+k]=0
        print('EQ ' if r==o else 'NEQ', hex(va), len(b), n[:110])
