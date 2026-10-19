import struct,ctypes,csv,re,json,sys
from pathlib import Path
D=Path('work/codex-lane/w22/astra235_whole')
class SV(ctypes.Structure):_fields_=[('n',ctypes.c_size_t),('p',ctypes.c_char_p)]
lib=ctypes.CDLL('/lib/x86_64-linux-gnu/libLLVM-17.so.1');dem=getattr(lib,'_ZN4llvm17microsoftDemangleESt17basic_string_viewIcSt11char_traitsIcEEPmPiNS_15MSDemangleFlagsE');dem.argtypes=[SV,ctypes.POINTER(ctypes.c_size_t),ctypes.POINTER(ctypes.c_int),ctypes.c_int];dem.restype=ctypes.c_void_p
free=ctypes.CDLL(None).free;free.argtypes=[ctypes.c_void_p]
def und(s):
 b=s.encode();n=ctypes.c_size_t();st=ctypes.c_int();p=dem(SV(len(b),b),ctypes.byref(n),ctypes.byref(st),0)
 if not p:return s
 out=ctypes.string_at(p).decode();free(p);return out
def coff(path):
 b=Path(path).read_bytes();_,ns,_,ps,ny,_,_=struct.unpack_from('<HHIIIHH',b,0);strings=b[ps+ny*18:];syms={};i=0
 while i<ny:
  off=ps+i*18;n=b[off:off+8]
  if n[:4]==b'\0'*4:n=strings[struct.unpack_from('<I',n,4)[0]:].split(b'\0')[0]
  else:n=n.split(b'\0')[0]
  val,sec,typ,storage,aux=struct.unpack_from('<IhHBB',b,off+8);syms[i]=(n.decode(),val,sec);i+=1+aux
 secs=[]
 for i in range(ns):
  o=20+i*40;size,raw,reloc,_,nr=struct.unpack_from('<IIIIH',b,o+16);rels={}
  for j in range(nr):v,idx,kind=struct.unpack_from('<IIH',b,reloc+j*10);rels[v]=syms[idx][0]
  secs.append((size,rels))
 return syms,secs
