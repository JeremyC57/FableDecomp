"""objcmp.py A.obj B.obj: per-section compare of two COFF objects keyed by (section name, COMDAT symbol); data + relocation symbol names; prints .drectve, DEF and UND set diffs."""
import sys,struct
def load(p):
    d=open(p,'rb').read(); ns=struct.unpack_from('<H',d,2)[0]; so,nsym=struct.unpack_from('<II',d,8); oh=struct.unpack_from('<H',d,16)[0]; st=so+nsym*18
    def nm(e): return (e[:8].split(b'\0')[0] if e[:4]!=b'\0\0\0\0' else d[st+struct.unpack_from('<I',e,4)[0]:].split(b'\0')[0]).decode('latin1')
    syms={}; i=0; secsym={}; und=set(); dfn=set()
    while i<nsym:
        e=d[so+i*18:so+i*18+18]; n=nm(e); sec=struct.unpack_from('<h',e,12)[0]; cls=e[16]; syms[i]=n
        if cls==2 and sec==0: und.add(n)
        elif cls==2 and sec>0: dfn.add(n); secsym.setdefault(sec,n)
        i+=1+e[17]
    secs={}
    for k in range(ns):
        h=d[20+oh+k*40:20+oh+k*40+40]; name=h[:8].split(b'\0')[0].decode(); sz,ptr,rp,_,nr=struct.unpack_from('<IIIIH',h,16); ch=struct.unpack_from('<I',h,36)[0]
        data=d[ptr:ptr+sz] if ptr else b''
        rel=[(struct.unpack_from('<I',d,rp+j*10)[0],syms.get(struct.unpack_from('<I',d,rp+j*10+4)[0]),struct.unpack_from('<H',d,rp+j*10+8)[0]) for j in range(nr)]
        secs[(name,secsym.get(k+1,''))]=(data,rel,ch)
    return secs,und,dfn
A=load(sys.argv[1]); B=load(sys.argv[2])
for k in sorted(set(A[0])|set(B[0])):
    a=A[0].get(k); b=B[0].get(k)
    if a!=b:
        if k[0]=='.drectve': print('DRECTVE A:',a and a[0]); print('DRECTVE B:',b and b[0])
        else: print('DIFF',k, 'A' if a else '-', 'B' if b else '-')
print('UND only A',sorted(A[1]-B[1])); print('UND only B',sorted(B[1]-A[1]))
print('DEF only A',sorted(A[2]-B[2])); print('DEF only B',sorted(B[2]-A[2]))
