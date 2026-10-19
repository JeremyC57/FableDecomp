from pathlib import Path
import struct,json
import pefile
p=Path('work/codex-lane/w22/coordinator_retry154');b=next((p/'dry_work').rglob('0099aa90.obj')).read_bytes()
u=lambda b,o=0:struct.unpack_from('<I',b,o)[0]
n=struct.unpack_from('<H',b,2)[0];sp=u(b,8);ns=u(b,12);st=sp+18*ns
secs=[]
for i in range(n):
 o=20+40*i;size,raw,rel=struct.unpack_from('<III',b,o+16);nr=struct.unpack_from('<H',b,o+32)[0];secs.append((b[raw:raw+size],rel,nr))
syms={};ids={};i=0
while i<ns:
 o=sp+18*i;name=b[o:o+8]
 if name[:4]==b'\0'*4:
  start=st+u(name,4);name=b[start:b.index(b'\0',start)]
 else:name=name.rstrip(b'\0')
 name=name.decode();value,section=struct.unpack_from('<Ih',b,o+8);syms[name]=(section,value);ids[i]=name;i+=1+b[o+17]
def obj(name):
 s,v=syms[name];data,r,n=secs[s-1];rels={}
 for i in range(n):
  off,idx,typ=struct.unpack_from('<IIH',b,r+10*i);rels[off-v]=ids[idx]
 return data[v:],rels
pe=pefile.PE(str(p.parents[1]/'dl/game/Fable The Lost Chapters/Fable.exe'));base=pe.OPTIONAL_HEADER.ImageBase
native=lambda a,n:pe.get_data(a-base,n)
ti,rel=obj('__TI2?AVCBBBFileException@@');assert ti[:16]==bytes(16) and list(rel)==[12]
nti=struct.unpack('<4I',native(0x1369380,16));assert nti[:3]==(0,0,0)
cta,rel=obj(rel[12]);assert u(cta)==2 and list(rel)==[4,8]
ncta=struct.unpack('<3I',native(nti[3],12));assert ncta[0]==2
out=[]
for i,size in enumerate((4,1)):
 ct,cr=obj(rel[4+i*4]);fields=struct.unpack('<7I',ct[:28]);nf=struct.unpack('<7I',native(ncta[i+1],28));assert fields[:1]+fields[2:]==nf[:1]+nf[2:]==(0,0,0xffffffff,0,size,0)
 td,tr=obj(cr[4]);name=td[8:].split(b'\0')[0];nn=native(nf[1]+8,80).split(b'\0')[0];assert name==nn
 out.append(dict(name=name.decode(),size=size,adjustment=[0,-1,0],copy=0))
print(json.dumps(dict(result='PASS',throw_attributes=0,unwind=0,forward=0,catchable_types=out),indent=2))
