// 006dc070 CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn (catalog; bsim .92; Ego_r: CHeroStatIncreaseData = { map<EHeroExperienceStatCategory,float> StatIncreases } at +0)
// Flags: /O2 /Oy /W3
// Leaf: TransferBinaryIn
// Layout: CDataInputStream shared header; zone fields PDB12911-12913, map pair key/value reads from retail.
#include "engine/CDataInputStream.h"

extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
class CMemoryDataInputStream: public CDataInputStream { public:
 void ReadWithSrcChunkOverflow(unsigned char*,long);
 __forceinline void Read(void* out,int size) {
  __int64 next=(__int64)(StreamPos+size);
  if(next<0 || next>0x7fffffff) return;
  if(SourceChunkBytesRemaining>=size) {memcpy(out,CurrentSourcePtr,size);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;SourceChunkBytesRemaining-=size;StreamPos+=size;}
  else ReadWithSrcChunkOverflow((unsigned char*)out,size);
 }
 __forceinline unsigned int ReadWord() {unsigned int v;Read(&v,4);return v;}
 __forceinline void ReadScalar(int* out){int v;Read(&v,4);*out=v;}
};

namespace N006dc070 {
// Model of map<EHeroExperienceStatCategory,float> (value_type, insert result, tree header).
struct Pair {int Key;float Value;};
struct InsertResult {void* Position;bool Added;InsertResult(void* p,bool added):Position(p),Added(added){}InsertResult(const InsertResult& x):Position(x.Position),Added(x.Added){}};
struct Map {void* Head;unsigned int Count;unsigned int Extra; InsertResult insert(const Pair&);};
}
using namespace N006dc070;
class CHeroStatIncreaseData { public:Map StatIncreases;};
template<class T> struct CPersistTraits {void TransferBinaryIn(CMemoryDataInputStream&,T*);};
template<class T> void CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStream& in,T* out) {
 unsigned int count=in.ReadWord();
 while(count!=0) {
  int key;in.ReadScalar(&key);
  int bits;in.ReadScalar(&bits);
  Pair p;p.Key=key;memcpy(&p.Value,&bits,4);
  out->StatIncreases.insert(p);
  --count;
 }
}
template void CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn(CMemoryDataInputStream&,CHeroStatIncreaseData*);
