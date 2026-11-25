#!/usr/bin/env python3
"""vtsig.py: retail vftable 0x012B8AE0 slots 0..29: argument bytes popped by each slot function (ret imm16 up to the first int3 padding;
a tail `jmp` to another function counts as that callee's frame, shown as tail) against the SDK 2.10 signature in the header's slot order."""
import struct
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
d=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; so=struct.unpack_from('<H',d,pe+20)[0]
secs=[(struct.unpack_from('<I',d,pe+24+so+i*40+12)[0],struct.unpack_from('<I',d,pe+24+so+i*40+8)[0],struct.unpack_from('<I',d,pe+24+so+i*40+20)[0]) for i in range(n)]
def off(va):
    r=va-0x400000
    for a,s,raw in secs:
        if a<=r<a+s: return raw+r-a
def rd(va): return struct.unpack_from('<I',d,off(va))[0]
md=Cs(CS_ARCH_X86,CS_MODE_32)
def body(va):
    o=off(va); rets=set(); tail=[]; first=None
    for i in md.disasm(d[o:o+2000],va):
        if i.mnemonic=='int3': break
        if first is None: first=f'{i.mnemonic} {i.op_str}'
        if i.mnemonic=='ret': rets.add(int(i.op_str,16) if i.op_str else 0)
        if i.mnemonic=='jmp' and i.op_str.startswith('0x') and not (va<=int(i.op_str,16)<va+2000 and int(i.op_str,16)>va): tail.append(i.op_str)
    return rets,tail,first
sig=[('`deleting dtor`(unsigned)',4),('Create(const CCodaSource*)',4),('SetBuffer(const CCodaSource*)',4),('Release()',0),('IsSoftware() const',0),
 ('Is3D() const',0),('IsCreated() const',0),('PlayBuffer(const CCodaSource*)',4),('PlayQueue(CCodaSource*,__int64)',12),('Update(CCodaSource*,__int64)',12),
 ('Resume(__int64)',8),('Stop(__int64)',8),('ReleaseEnvelopes(__int64)',8),('GetPlayCursor() const',0),('SetPlayCursor(unsigned)',4),
 ('EAXSet(const GUID*,int,void*,int)',16),('EAXGet(const GUID*,int,void*,int)',16),('InitPlay()',0),('SetPlayBuffer(const CSalBuffer&,bool)',8),
 ('GetPlatformObjectSize()',0),('GetPlatformBufferLoopStart()',0),('GetPlatformBufferEnd()',0),('InitParams(const CCodaSource*,const CODASOURCE&)',8),
 ('ConvertFormat(CSalSwBuffer&,const void*,unsigned,unsigned)',16),('Mix(CSalSwBuffer&,unsigned,bool)',12),('MixReverbInput(CSalReverb*,CSalSwBuffer&,unsigned)',12),
 ('MixChannel(CSalSwBuffer&,unsigned,unsigned)',12),('StartRunOut(__int64)',8)]
print('slot\tretail\tsdk_header_slot\tsdk_arg_bytes\tretail_ret\ttail_jmp\tretail_first_insn\tverdict')
for k in range(30):
    a=rd(0x012B8AE0+4*k); r,t,f=body(a)
    if k<len(sig):
        s,nb=sig[k]
        if r: v='consistent' if r=={nb} else 'ARG_MISMATCH'
        else: v='tail-call only (args set by the callee)'
    else: s,nb,v='(beyond the 28 header slots)','-','-'
    print(f'{k}\t{a:08x}\t{s}\t{nb}\t{sorted(r)}\t{",".join(t) or "-"}\t{f}\t{v}')
