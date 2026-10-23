// 006dc070 Traits::TransferBinaryIn
// Flags: /O2 /Oy /W3
// Leaf: TransferBinaryIn
// Layout: CDataInputStream shared header; zone fields PDB12911-12913, map pair key/value reads from retail.
#include "engine/CDataInputStream.h"
namespace N006dc070 {

extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
struct Stream: CDataInputStream {
 void Overflow(unsigned char*,int);
 __forceinline void Read(void* out,int size) {
  __int64 next=(__int64)(StreamPos+size);
  if(next<0 || next>0x7fffffff) return;
  if(SourceChunkBytesRemaining>=size) {memcpy(out,CurrentSourcePtr,size);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;SourceChunkBytesRemaining-=size;StreamPos+=size;}
  else Overflow((unsigned char*)out,size);
 }
 __forceinline unsigned int ReadWord() {unsigned int v;Read(&v,4);return v;}
 __forceinline void ReadScalar(int* out){int v;Read(&v,4);*out=v;}
};

struct Pair {int Key;float Value;};
struct InsertResult {void* Position;bool Added;InsertResult(void* p,bool added):Position(p),Added(added){}InsertResult(const InsertResult& x):Position(x.Position),Added(x.Added){}};
struct Map {void* Head;unsigned int Count;unsigned int Extra; InsertResult Insert(const Pair&);};
template<class T> struct Traits {void TransferBinaryIn(Stream&,T*);};
template<class T> void Traits<T>::TransferBinaryIn(Stream& in,T* out) {
 unsigned int count=in.ReadWord();
 while(count!=0) {
  int key;in.ReadScalar(&key);
  int bits;in.ReadScalar(&bits);
  Pair p;p.Key=key;memcpy(&p.Value,&bits,4);
  out->Insert(p);
  --count;
 }
}
template void Traits<Map>::TransferBinaryIn(Stream&,Map*);

}