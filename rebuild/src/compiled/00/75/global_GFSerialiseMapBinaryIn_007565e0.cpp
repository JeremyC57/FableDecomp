// 007565e0 GFSerialiseMapBinaryIn
// Flags: /O2 /Oy /W3 /Gr
// Leaf: GFSerialiseMapBinaryIn
// Layout: shared stream layout, virtual slot6 returns CCharString, tree header Parent4/Left8/Right12; pair string+two signed dwords.
#include "engine/CMemoryDataInputStream.h"
namespace X_007565e0 {
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
// CDataInputStream::ReadWithSrcChunkOverflow at00993ca0 is inherited through this method-only bridge.
struct CMemoryDataInputStreamAdapter: ::CMemoryDataInputStream {
 void ReadWithSrcChunkOverflow(unsigned char*,int);
 __forceinline void Read(void* out,int size) {
  __int64 next=(__int64)(unsigned)(position+size);
  if(next<0 || next>0x7fffffff) return;
  if(chunkLength>=size) {memcpy(out,(const void*)chunkBase,size);chunkBase+=size;chunkLength-=size;position+=size;}
  else ReadWithSrcChunkOverflow((unsigned char*)out,size);
 }
 __forceinline unsigned int ReadWord() {unsigned int v;Read(&v,4);return v;}
};


#pragma pack(push,1)
template<class Char>struct CBasicString {Char*PData;unsigned StringLength,DataLength;bool UseFastExtend;};
struct CCharStringData {CBasicString<char> Data;int RefCount;};
#pragma pack(pop)
void ReleaseStringData(CCharStringData*);
} // (hoisted to global scope: naming wave 9)
class CCharString { public:unsigned int Data;__declspec(noinline) CCharString():Data(0){}__declspec(noinline) CCharString(const CCharString&x):Data(0){if(this!=&x&&x.Data){Data=x.Data;++((X_007565e0::CCharStringData*)Data)->RefCount;}}__declspec(noinline) ~CCharString(){X_007565e0::ReleaseStringData((X_007565e0::CCharStringData*)Data);Data=0;}__declspec(noinline) const CCharString& operator=(const CCharString&x){if(this!=&x){X_007565e0::ReleaseStringData((X_007565e0::CCharStringData*)Data);Data=x.Data;if(Data)++((X_007565e0::CCharStringData*)Data)->RefCount;}return *this;}};
namespace X_007565e0 {
// Layout: method-only ABI view of the actual header receiver, not an original class.
// Slot6 returns CCharString by hidden result pointer; the flattened header holds its vptr at0.
// This data-free view supplies only virtual dispatch signatures, retaining all real stream fields.
struct CMemoryDataInputStreamVirtualSlots {virtual void v00();virtual void v01();virtual void v02();virtual void v03();virtual void v04();virtual void v05();virtual CCharString ReadString();};
namespace std {template<class First,class Second>struct pair {First first;Second second;pair(){}pair(const First&f,const Second&s):first(f),second(s){}};}
typedef std::pair<int,int> Value;
typedef std::pair<CCharString,Value> Pair;
struct _Rb_tree_node_base {unsigned int State;_Rb_tree_node_base* Parent;_Rb_tree_node_base* Left;_Rb_tree_node_base* Right;};
struct _Rb_tree_insert_result {_Rb_tree_node_base* Position;bool Added;_Rb_tree_insert_result(_Rb_tree_node_base* p,bool a):Position(p),Added(a){}_Rb_tree_insert_result(const _Rb_tree_insert_result& x):Position(x.Position),Added(x.Added){}};
struct map {_Rb_tree_node_base* Head;unsigned int Size;void _M_erase(_Rb_tree_node_base*);_Rb_tree_insert_result insert(const Pair&);};
template<class T>struct CPersistTraits {T DefaultVal;__declspec(noinline) void TransferBinaryIn(CMemoryDataInputStreamAdapter&in,T*p){p->first=(int)in.ReadWord();p->second=(int)in.ReadWord();}};
void __fastcall GFSerialiseMapBinaryIn(CMemoryDataInputStreamAdapter& in,map& out) {
 CCharString temporary;
 if(out.Size!=0){out._M_erase(out.Head->Parent);out.Head->Left=out.Head;out.Head->Parent=0;out.Head->Right=out.Head;out.Size=0;}
 int count=(int)in.ReadWord();
 for(;count>0;--count){
  Pair pair;pair.second.first=0;pair.second.second=0;
  pair.first=reinterpret_cast<CMemoryDataInputStreamVirtualSlots&>(in).ReadString();
  CPersistTraits<Value> traits;traits.TransferBinaryIn(in,&pair.second);
  out.insert(Pair(pair.first,pair.second));
 }
}

}