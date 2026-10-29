// 006dc070 CPersistTraits<CHeroStatIncreaseData>::TransferBinaryIn (catalog; bsim .92; Ego_r: CHeroStatIncreaseData = { map<EHeroExperienceStatCategory,float> StatIncreases } at +0)
// Flags: /O2 /Oy /W3
// Leaf: TransferBinaryIn
// Layout: CDataInputStream shared header; zone fields PDB12911-12913, map pair key/value reads from retail.
#define FABLE_CDATAINPUTSTREAM_FRIEND class CMemoryDataInputStream   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"

extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
class CMemoryDataInputStream: public CDataInputStream { public:
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


#include <cstdio>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
#include <cstring>
extern "C++" {
struct TreeNode {TreeNode* Left;TreeNode* Right;Pair Item;TreeNode(const Pair& p):Left(0),Right(0),Item(p){}};
static int slowCalls,liveNodes,inserts,duplicates;static void* lastPosition;static bool lastAdded;
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* p,long size){++slowCalls;memcpy(p,CurrentSourcePtr,size);CurrentSourcePtr=(unsigned char*)CurrentSourcePtr+size;SourceChunkBytesRemaining=0;StreamPos+=size;}
InsertResult Map::insert(const Pair& p){TreeNode** link=(TreeNode**)&Head;while(*link){if(p.Key==(*link)->Item.Key){++duplicates;lastPosition=*link;lastAdded=false;return InsertResult(*link,false);}link=p.Key<(*link)->Item.Key?&(*link)->Left:&(*link)->Right;}*link=new TreeNode(p);++liveNodes;++Count;++inserts;lastPosition=*link;lastAdded=true;return InsertResult(*link,true);}
static TreeNode* find(Map& m,int key){TreeNode* p=(TreeNode*)m.Head;while(p&&p->Item.Key!=key)p=key<p->Item.Key?p->Left:p->Right;return p;}
static void clear(TreeNode* p){if(!p)return;clear(p->Left);clear(p->Right);delete p;--liveNodes;}
}
int main(){unsigned int words[]={4,0xfffffffd,0x3f800000,0,0x40000000,7,0x40800000,0xfffffffd,0x41000000};Map m;std::memset(&m,0,sizeof m);CPersistTraits<CHeroStatIncreaseData> traits;for(int mode=0;mode!=2;++mode){CMemoryDataInputStream in;std::memset(&in,0,sizeof in);in.CurrentSourcePtr=words;in.SourceChunkBytesRemaining=mode?0:sizeof words;traits.TransferBinaryIn(in,(CHeroStatIncreaseData*)&m);if(in.StreamPos!=sizeof words||m.Count!=3||find(m,-3)->Item.Value!=1.0f||find(m,0)->Item.Value!=2.0f||find(m,7)->Item.Value!=4.0f||lastPosition!=find(m,-3)||lastAdded)return 1;if(!mode&&slowCalls)return 2;}if(slowCalls!=9||inserts!=3||duplicates!=5)return 3;Pair p;p.Key=11;p.Value=8.0f;InsertResult added=m.insert(p);if(!added.Added||added.Position!=find(m,11))return 4;p.Value=16.0f;InsertResult repeated=m.insert(p);if(repeated.Added||repeated.Position!=added.Position||find(m,11)->Item.Value!=8.0f)return 5;unsigned int zero=0;CMemoryDataInputStream empty;std::memset(&empty,0,sizeof empty);empty.CurrentSourcePtr=&zero;empty.SourceChunkBytesRemaining=4;traits.TransferBinaryIn(empty,(CHeroStatIncreaseData*)&m);if(m.Count!=4||empty.StreamPos!=4)return 6;clear((TreeNode*)m.Head);if(liveNodes)return 7;std::printf("006dc070_TEST PASS\n");return 0;}