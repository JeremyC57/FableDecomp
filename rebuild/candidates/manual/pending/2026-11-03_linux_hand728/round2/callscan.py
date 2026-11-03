import struct,sys
exe=open("/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; so=struct.unpack_from("<H",exe,pe+20)[0]
o=pe+24+so; va=struct.unpack_from("<I",exe,o+12)[0]; raw=struct.unpack_from("<I",exe,o+20)[0]; rs=struct.unpack_from("<I",exe,o+16)[0]
tgt=int(sys.argv[1],16); follow=[int(x,16) for x in sys.argv[2].split(',')] if len(sys.argv)>2 else []
calls=[]
for i in range(raw,raw+rs-5):
    if exe[i]==0xe8:
        t=0x400000+va+(i-raw)+5+struct.unpack_from("<i",exe,i+1)[0]
        calls.append((0x400000+va+i-raw,t))
for k,(a,t) in enumerate(calls):
    if t==tgt:
        nxt=[c for c in calls[k+1:k+8] if c[0]-a<0x80]
        if not follow or any(c[1] in follow for c in nxt): print(hex(a), [hex(c[1]) for c in nxt])
