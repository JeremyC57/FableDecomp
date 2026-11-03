import sys, csv, bisect, collections, glob, re, struct, os
sys.path.insert(0,'tools')
import sync_manifest_from_catalog as M
S=os.environ["S"]; L=S+"/leafs"
# labels: every file, ordered by the date in its name (then name); raw parse so header-less / commented files count too
lab={}
for p in sorted(glob.glob('ghidra_out/labels_*.tsv'), key=lambda q:(M.file_date(q),q)):
    for line in open(p,encoding='utf-8-sig',errors='replace'):
        q=line.rstrip('\r\n').split('\t')
        if len(q)<2 or not re.fullmatch(r'(0x)?[0-9a-fA-F]{1,8}',q[0].strip()): continue
        n=q[1].strip()
        if not n or n in ('1','2','3'): continue
        lab[M.addr(q[0])]=(n, os.path.basename(p), q[2].strip() if len(q)>2 else '')
man={}
for r in csv.DictReader(open(M.MANIFEST,encoding='utf-8-sig'),delimiter='\t'): man[M.addr(r['address'])]=r
ps=open("rebuild/build_candidates.ps1",encoding="utf-8-sig").read()
landed={a.lower() for a in re.findall(r"Address = '([0-9a-fA-F]{8})'",ps)}
cat={}
for r in csv.DictReader(open(M.CATALOG,encoding='utf-8-sig'),delimiter='\t'): cat[r['address'].lower()]=(r['name'],int(r['length']))
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
TEXT_VA, TEXT_SZ, TEXT_OFF = 0x401000, 0xe2b8a8, 0x1000
RD_VA, RD_SZ, RD_OFF = 0x122d000, 0x14684c, 0xe2d000
text=exe[TEXT_OFF:TEXT_OFF+TEXT_SZ]; rdata=exe[RD_OFF:RD_OFF+RD_SZ]
def rd(va,n): return text[va-TEXT_VA:va-TEXT_VA+n]
miss=sorted(a for a in lab if TEXT_VA<=int(a,16)<TEXT_VA+TEXT_SZ and a not in man)
want={int(a,16) for a in miss}
# E8/E9 rel32 + 0F 8x jcc rel32 scan (byte-wise: over-counts nothing that matters for a yes/no, may over-count data)
calls=collections.defaultdict(list)
for i in range(len(text)-5):
    b=text[i]
    if b in (0xE8,0xE9):
        t=(TEXT_VA+i+5+struct.unpack_from('<i',text,i+1)[0])&0xffffffff
        if t in want: calls[t].append(('call' if b==0xE8 else 'jmp', TEXT_VA+i))
    elif b==0x0F and 0x80<=text[i+1]<=0x8F and i+6<=len(text):
        t=(TEXT_VA+i+6+struct.unpack_from('<i',text,i+2)[0])&0xffffffff
        if t in want: calls[t].append(('jcc', TEXT_VA+i))
# absolute dword references (vtables in .rdata, address tables)
DATA_OFF, DATA_SZ = 0xf74000, 0x44000
data=exe[DATA_OFF:DATA_OFF+DATA_SZ]
absref={t:(rdata.count(struct.pack('<I',t)), text.count(struct.pack('<I',t)), data.count(struct.pack('<I',t))) for t in want}
cstarts=sorted((int(a,16),a) for a in cat)
mstarts=sorted(int(a,16) for a in man)
PRO=[(b'\x55\x8b\xec','push ebp; mov ebp,esp'),(b'\x83\xec','sub esp,imm8'),(b'\x81\xec','sub esp,imm32'),(b'\x6a\xff','push -1 (EH frame)'),
     (b'\x64\xa1\x00\x00\x00\x00','mov eax,fs:[0]'),(b'\x8b\x44\x24','mov eax,[esp+n]'),(b'\x8b\x4c\x24','mov ecx,[esp+n]'),(b'\x8b\x54\x24','mov edx,[esp+n]'),
     (b'\x8b\x41','mov eax,[ecx+n]'),(b'\x8b\x01','mov eax,[ecx]'),(b'\x56','push esi'),(b'\x53','push ebx'),(b'\x57','push edi'),(b'\x51','push ecx'),(b'\x55','push ebp'),
     (b'\xd9','x87 load'),(b'\x33\xc0','xor eax,eax'),(b'\xb8','mov eax,imm32'),(b'\xb0','mov al,imm8'),(b'\x8b\xc1','mov eax,ecx'),(b'\xa1','mov eax,[abs]'),(b'\xe9','jmp rel32'),(b'\xc3','ret'),(b'\xc2','ret n')]
def prologue(bs, prev):
    for k,d in PRO:
        if bs.startswith(k): return d
    return ''
out=[]; prop=[]
for a in miss:
    va=int(a,16); n,f,ev=lab[a]
    i=bisect.bisect_right(cstarts,(va,'~'))-1
    verdict=''
    if i>=0:
        s,sa=cstarts[i]; nm,ln=cat[sa]
        if s<=va<s+ln:
            verdict=f"inside-{'landed' if sa in landed else 'unlanded'} 0x{sa.upper()} {nm} (+0x{va-s:X} of 0x{ln:X})"
    if not verdict:
        j=bisect.bisect_right(mstarts,va)-1; ps_=mstarts[j]; nx=mstarts[j+1] if j+1<len(mstarts) else None
        pa=f"{ps_:08x}"; pn=cat.get(pa,(man[pa]['module']+'::'+man[pa]['name'],0))
        endp=ps_+pn[1] if pn[1] else None
        verdict=f"unlisted-start (after 0x{pa.upper()} {pn[0]} len 0x{pn[1]:X} {'landed' if pa in landed else 'unlanded'}, ends 0x{endp:08X}; next row 0x{nx:08X})" if endp else f"unlisted-start (after manifest row 0x{pa.upper()} with no catalog extent; next row 0x{nx:08X})"
    bs=rd(va,24); prev=rd(va-1,1)
    refs=calls.get(va,[])
    ar=absref[va]
    rs=f"{sum(1 for r in refs if r[0]=='call')} call, {sum(1 for r in refs if r[0]=='jmp')} jmp, {sum(1 for r in refs if r[0]=='jcc')} jcc; abs dword {ar[0]} .rdata, {ar[1]} .text, {ar[2]} .data"
    ex=",".join(f"{k}@{x:08X}" for k,x in refs[:3])
    out.append([f"0x{a.upper()}",n,f,verdict,bs.hex(),f"prev byte {prev.hex()}; prologue: {prologue(bs,prev) or '?'}",rs+(f" ({ex})" if ex else "")])
allst=sorted(set(mstarts)|{s_ for s_,_ in cstarts}|want)
def extent(va):
    j=bisect.bisect_right(allst,va); nx=allst[j] if j<len(allst) else TEXT_VA+TEXT_SZ
    b=rd(va,nx-va); n=len(b)
    while n and b[n-1]==0xCC: n-=1
    return n, nx
import subprocess
EXE=f"{S}/dl/game/Fable The Lost Chapters/Fable.exe"
def ends_here(va):
    """linear-disassemble from the previous known start (manifest / catalog row or another label) to va: va must be an
    instruction boundary whose previous instruction ends the flow (ret / ret n / jmp / int3 padding)"""
    j=bisect.bisect_left(allst,va)-1; st=allst[j] if j>=0 else TEXT_VA
    if va-st>0x4000: st=va-0x40     # far from any start: a short window is enough to see the padding
    d=subprocess.run(["objdump","-d","-M","intel",f"--start-address=0x{st:x}",f"--stop-address=0x{va+1:x}",EXE],capture_output=True,text=True).stdout
    ins=[]
    for line in d.splitlines():
        m=re.match(r"\s+([0-9a-f]+):\t([0-9a-f ]+)\t?(.*)",line)
        if m and m.group(3): ins.append((int(m.group(1),16),m.group(3).split()[0] if m.group(3).split() else ""))
    addrs=[a for a,_ in ins]
    if va not in addrs: return ""
    k=addrs.index(va)
    if k==0: return "first instruction after a known start"
    op=ins[k-1][1]
    i=bisect.bisect_right(cstarts,(va-1,'~'))-1
    tag=""
    if i>=0:
        s_,sa=cstarts[i]
        if s_+cat[sa][1]==va: tag=f"previous row 0x{sa.upper()} ends here; "
    if op=="int3": return tag+"int3 padding before"
    if op in ("ret","jmp"): return tag+f"{op} before"
    return ""
for r in out:
    va=int(r[0],16); eh=ends_here(va); r[5]=(eh or "mid-flow (no instruction boundary after ret/jmp/int3)")+"; "+r[5]
    if not eh and r[3].startswith("unlisted-start (after manifest row"):
        j=bisect.bisect_right(mstarts,va)-1; pa=f"{mstarts[j]:08x}"; mr=man[pa]
        r[3]=(f"inside-{'landed' if pa in landed else 'unlanded'} 0x{pa.upper()} {mr['module']}::{mr['name']} "
              f"(manifest row with no catalog extent; the label is mid-function, +0x{va-mstarts[j]:X})")
    r.append(eh)
fields=open(M.MANIFEST,encoding='utf-8-sig').readline().rstrip('\r\n').split('\t')
fu=open(f"{L}/PROPOSED_ROWS_UNREFERENCED.tsv","w"); fu.write("\t".join(fields+["length","evidence"])+"\n")
with open(f"{L}/PROPOSED_ROWS.tsv","w") as fh:
    fh.write("\t".join(fields+["length","evidence"])+"\n")
    nprop=0
    for r in out:
        va=int(r[0],16); eh=r[-1]
        if not r[3].startswith("unlisted-start") or not eh: continue
        refs=calls.get(va,[]); ar=absref[va]
        reach=[]
        nc=sum(1 for x in refs if x[0] in ('call','jmp'))
        if nc: reach.append(f"{nc} call/jmp rel32 ("+", ".join(f"{k}@{x:08X}" for k,x in refs[:3])+")")
        if ar[0]: reach.append(f"{ar[0]} .rdata dword (vtable/table slot)")
        if ar[1] or ar[2]: reach.append(f"{ar[1]} .text + {ar[2]} .data absolute dword")
        tgt=fh
        if not reach: tgt=fu; reach=["nothing (no rel32 call/jmp, no absolute dword in .text/.rdata/.data; retail keeps such functions: 6,736 landed rows have no reference either)"]
        ln,nx=extent(va)
        mod,nm=M.split_scope(r[1]) if " " not in r[1] or "<" in r[1] else ("_global", r[1])
        row={k:"" for k in fields}
        row.update(address=f"{va:08x}",module=mod,name=nm,calling_convention="unknown",return_type="undefined",
                   parameter_count="0",prototype_complete="0",agent_verdict="accepted",agent_source="gapscan")
        ev=f"label {r[1]!r} ({r[2]}); {eh}; length is an upper bound; reached by "+"; ".join(reach)+f"; extent to next start 0x{nx:08X} less int3 padding"
        if " " in r[1] and "<" not in r[1]: ev+="; label is descriptive prose, needs a real name"
        tgt.write("\t".join([row[k] for k in fields]+[f"{ln}",ev])+"\n"); nprop+=(tgt is fh)
fu.close(); print("proposed",nprop)
for r in out: r.pop()
with open(f"{L}/MISSING_FROM_MANIFEST.tsv","w") as fh:
    fh.write("address\tlabel\tlabel_file\tverdict\tretail_bytes_24\tstart_shape\treferences\n")
    for r in out: fh.write("\t".join(r)+"\n")
print(len(out), collections.Counter(r[3].split()[0] for r in out))
print("unlisted-start with clean boundary:", sum(1 for r in out if r[3].startswith("unlisted") and not r[5].startswith("mid-flow")))
