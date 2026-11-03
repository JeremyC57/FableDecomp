# Map every EH funclet (unwind action / catch handler) in Fable.exe to its FuncInfo, __ehhandler thunk and parent push site.
import struct,sys,os,re,bisect,json
S=os.environ["S"]; exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]; ns=struct.unpack_from("<H",exe,pe+6)[0]; os_=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(ns):
    o=pe+24+os_+40*i; name=exe[o:o+8].rstrip(b"\0").decode(); vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); ch=struct.unpack_from("<I",exe,o+36)[0]; secs.append((name,va,vs,ro,rs,ch))
def rd(va,n):
    rva=va-0x400000
    for nm,sva,svs,ro,rs,ch in secs:
        if sva<=rva<sva+rs: return exe[ro+rva-sva:ro+rva-sva+n]
    return None
def secof(va):
    rva=va-0x400000
    for nm,sva,svs,ro,rs,ch in secs:
        if sva<=rva<sva+max(svs,rs): return nm
code=[s for s in secs if s[5]&0x20]
fis={}
for nm,sva,svs,ro,rs,ch in secs:
    blob=exe[ro:ro+rs]
    for m in re.finditer(re.escape(struct.pack("<I",0x19930520)),blob):
        if m.start()%4: continue
        fi=0x400000+sva+m.start()
        try:
            magic,maxs,pun,ntry,ptry=struct.unpack_from("<IiIiI",blob,m.start())
            if not (0<=maxs<5000 and 0<=ntry<500): continue
            acts=[]
            for k in range(maxs):
                to,act=struct.unpack_from("<iI",rd(pun+8*k,8)); acts.append((k,to,act))
            catches=[]
            for t in range(ntry):
                lo,hi,chi,nc,pha=struct.unpack_from("<iiiiI",rd(ptry+20*t,20))
                for c in range(nc):
                    adj,pty,disp,h=struct.unpack_from("<IIiI",rd(pha+16*c,16)); catches.append((t,c,h))
            fis[fi]=dict(acts=acts,catches=catches,maxs=maxs,ntry=ntry)
        except Exception as ex: pass
print("FuncInfos",len(fis),file=sys.stderr)
# ehhandler thunks: mov eax, FI ; jmp
eh={}
for nm,sva,svs,ro,rs,ch in code:
    blob=exe[ro:ro+rs]
    for m in re.finditer(rb"\xb8(....)\xe9",blob,re.S):
        v=struct.unpack("<I",m.group(1))[0]
        if v in fis: eh.setdefault(v,[]).append(0x400000+sva+m.start())
print("thunks",sum(len(v) for v in eh.values()),file=sys.stderr)
ehset={a:fi for fi,l in eh.items() for a in l}
# push sites: 68 <eh>
push={}
for nm,sva,svs,ro,rs,ch in code:
    blob=exe[ro:ro+rs]
    for m in re.finditer(rb"\x68(....)",blob,re.S):
        v=struct.unpack("<I",m.group(1))[0]
        if v in ehset: push.setdefault(v,[]).append(0x400000+sva+m.start())
print("push sites",sum(len(v) for v in push.values()),file=sys.stderr)
out={}
for fi,d in fis.items():
    for e in eh.get(fi,[]):
        for p in push.get(e,[]):
            pass
    rec=dict(fi=fi,thunks=eh.get(fi,[]),push=[p for e in eh.get(fi,[]) for p in push.get(e,[])],acts=d["acts"],catches=d["catches"])
    out["%08x"%fi]=rec
json.dump(out,open(sys.argv[1],"w"))
