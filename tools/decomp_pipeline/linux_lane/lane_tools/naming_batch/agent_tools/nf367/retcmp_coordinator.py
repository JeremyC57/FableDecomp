"""retcmp.py <exe> <obj> SYM=ADDR...: compare an object's COMDAT/function body with retail at ADDR: bytes with relocation fields masked,
then each relocation's target (REL32: call/jmp destination; DIR32: absolute) against the symbol's reloc_bindings address (or a SYM=ADDR given)."""
import sys,struct
exe=open(sys.argv[1],'rb').read(); obj=sys.argv[2]; pairs=dict(a.split('=') for a in sys.argv[3:])
src=open(__import__('os').path.join(__import__('os').path.dirname(__file__),'objcmp.py')).read().split('A=load')[0]; exec(src)
secs=load(obj)[0]
pe=struct.unpack_from('<I',exe,0x3c)[0]; ns=struct.unpack_from('<H',exe,pe+6)[0]; op=struct.unpack_from('<H',exe,pe+20)[0]
SS=[struct.unpack_from('<IIII',exe,pe+24+op+40*i+8) for i in range(ns)]
def rd(va,n):
    r=va-0x400000
    for vs,sva,rs,ro in SS:
        if sva<=r<sva+rs: return exe[ro+r-sva:ro+r-sva+n]
tab={}
for l in open('rebuild/oracles/reloc_bindings.tsv',errors='replace'):
    p=l.rstrip('\n').split('\t')
    try: tab.setdefault(p[0],set()).add(int(p[1],16))
    except (IndexError,ValueError): pass
for s,a in pairs.items(): tab[s]={int(a,16)}
for sym,addr in pairs.items():
    k=[k for k in secs if k[1]==sym and k[0]=='.text']
    if not k: print(sym,'NOT IN OBJ'); continue
    data,rel,ch=secs[k[0]]; va=int(addr,16); ret=bytearray(rd(va,len(data))); mine=bytearray(data); bad=[]
    for off,rs,typ in rel:
        r=struct.unpack_from('<I',ret,off)[0]
        tgt=(va+off+4+struct.unpack('<i',struct.pack('<I',r))[0])&0xffffffff if typ==20 else r
        if tgt not in tab.get(rs,set()): bad.append((off,rs,hex(tgt),[hex(x) for x in tab.get(rs,[])]))
        ret[off:off+4]=b'\0'*4; mine[off:off+4]=b'\0'*4
    print(sym[:60],addr,'len',len(data),'bytes', 'EQ' if ret==mine else 'DIFF','relocs', 'EQ' if not bad else bad)
