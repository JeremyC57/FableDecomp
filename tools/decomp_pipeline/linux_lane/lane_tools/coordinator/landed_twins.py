# landed_twins.py (repo root, S exported): list unlanded catalog functions whose retail bytes equal a landed row's bytes
# (exact, or after masking image-range absolute operands and call/jmp rel32). Output: addr, module, name, length, EXACT|MASKED,
# twin address, twin catalog name. Quick-win source for chunk cutting (chunk 894). Run with python3 -I.
# Find unlanded catalog functions whose retail bytes equal a landed row's bytes (exact, or equal after masking
# 4-byte absolute image addresses and call/jmp rel32 targets).
import csv,glob,os,struct,sys,re
csv.field_size_limit(1<<30)
S=os.environ['S']
exe=open(f'{S}/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',exe,0x3c)[0];n=struct.unpack_from('<H',exe,pe+6)[0];o=struct.unpack_from('<H',exe,pe+20)[0]
secs=[struct.unpack_from('<IIII',exe,pe+24+o+40*i+8) for i in range(n)]
def rd(va,k):
    for vs,sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+k]
    return b''
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
md=Cs(CS_ARCH_X86,CS_MODE_32); md.detail=False
def mask(b,base):
    out=bytearray(b)
    for ins in md.disasm(b,base):
        rel=ins.address-base; L=ins.size
        if ins.mnemonic in('call','jmp') and L==5 and b[rel]in(0xe8,0xe9): out[rel+1:rel+5]=b'\0'*4; continue
        # mask any 4-byte little-endian value in image range
        for k in range(rel,rel+L-3):
            v=struct.unpack_from('<I',b,k)[0]
            if 0x00400000<=v<0x01600000: out[k:k+4]=b'\0'*4
    return bytes(out)
landed={}
for r in csv.DictReader(open('rebuild/oracles/auto-re-candidates.tsv'),delimiter='\t'):
    landed[r['address'].lower().zfill(8)]=(r['name'],bytes.fromhex(r['bytes']))
def is_landed(a): return a in landed or bool(glob.glob(f'rebuild/src/compiled/{a[:2]}/{a[2:4]}/*_{a}.cpp'))
man=[r for r in csv.DictReader(open('rebuild/manifest/functions.tsv'),delimiter='\t')]
addrs=sorted(int(r['address'],16) for r in man)
nxt={addrs[i]:addrs[i+1] for i in range(len(addrs)-1)}
byname={r['address'].lower():r for r in man}
exact={};masked={}
for a,(nm,b) in landed.items():
    if len(b)<12: continue
    exact.setdefault(b,[]).append(a)
    try: masked.setdefault(mask(b,int(a,16)),[]).append(a)
    except Exception: pass
hits=[]
for r in man:
    a=r['address'].lower()
    if is_landed(a): continue
    va=int(a,16); k=nxt.get(va,va+4096)-va
    if k<=0 or k>4096: continue
    b=rd(va,k).rstrip(b'\xcc')
    if len(b)<12: continue
    # try the full span and prefixes ending at a ret
    cands=[b]
    for L in sorted({len(x) for x in exact} ):
        pass
    t=None
    if b in exact: t=('EXACT',exact[b][0])
    else:
        m=mask(b,va)
        if m in masked: t=('MASKED',masked[m][0])
    if t: hits.append((a,r['module'],r['name'],len(b),t[0],t[1],landed[t[1]][0]))
for h in hits: print('\t'.join(map(str,h)))
print(len(hits),file=sys.stderr)
