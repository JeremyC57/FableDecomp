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
struct CCharString {unsigned int Data;__declspec(noinline) CCharString():Data(0){}__declspec(noinline) CCharString(const CCharString&x):Data(0){if(this!=&x&&x.Data){Data=x.Data;++((CCharStringData*)Data)->RefCount;}}__declspec(noinline) ~CCharString(){ReleaseStringData((CCharStringData*)Data);Data=0;}__declspec(noinline) CCharString& operator=(const CCharString&x){if(this!=&x){ReleaseStringData((CCharStringData*)Data);Data=x.Data;if(Data)++((CCharStringData*)Data)->RefCount;}return *this;}};
// Layout: method-only ABI view of the actual header receiver, not an original class.
// Slot6 returns CCharString by hidden result pointer; the flattened header holds its vptr at0.
// This data-free view supplies only virtual dispatch signatures, retaining all real stream fields.
struct CMemoryDataInputStreamVirtualSlots {virtual void v00();virtual void v01();virtual void v02();virtual void v03();virtual void v04();virtual void v05();virtual CCharString ReadString();};
namespace _STL {template<class First,class Second>struct pair {First first;Second second;pair(){}pair(const First&f,const Second&s):first(f),second(s){}};}
typedef _STL::pair<int,int> Value;
typedef _STL::pair<CCharString,Value> Pair;
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

#include <cstdio>
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
#include <cstring>
namespace X_007565e0 {
#pragma pack(push,1)
typedef CCharStringData StringBlock;
#pragma pack(pop)
static int blocks,slowCalls,erased,liveNodes,duplicates,addedCount;static _Rb_tree_node_base* lastPosition;static bool lastAdded;
static StringBlock* block(const CCharString& s){return (StringBlock*)s.Data;}
static const char* text(const CCharString& s){return s.Data?block(s)->Data.PData:"";}
void ReleaseStringData(CCharStringData*p){if(p&&--p->RefCount<=0){if(p->Data.PData){delete[] p->Data.PData;p->Data.PData=0;}p->Data.StringLength=0;p->Data.DataLength&=0x80000000u;delete p;--blocks;}}
static CCharString make(const char* value){CCharString s;StringBlock* p=new StringBlock;p->Data.StringLength=(unsigned int)std::strlen(value);p->Data.DataLength=p->Data.StringLength+1;p->Data.UseFastExtend=0;p->RefCount=1;p->Data.PData=new char[p->Data.DataLength];std::strcpy(p->Data.PData,value);s.Data=(unsigned int)p;++blocks;return s;}
void CMemoryDataInputStreamVirtualSlots::v00(){}void CMemoryDataInputStreamVirtualSlots::v01(){}void CMemoryDataInputStreamVirtualSlots::v02(){}void CMemoryDataInputStreamVirtualSlots::v03(){}void CMemoryDataInputStreamVirtualSlots::v04(){}void CMemoryDataInputStreamVirtualSlots::v05(){}
CCharString CMemoryDataInputStreamVirtualSlots::ReadString(){CMemoryDataInputStreamAdapter& in=reinterpret_cast<CMemoryDataInputStreamAdapter&>(*this);unsigned int id=in.ReadWord();return make(id==1?"alpha":id==2?"beta":"seed");}
void CMemoryDataInputStreamAdapter::ReadWithSrcChunkOverflow(unsigned char* p,int n){++slowCalls;memcpy(p,(const void*)chunkBase,n);chunkBase+=n;chunkLength=0;position+=n;}

struct TreeNode:_Rb_tree_node_base {Pair Item;TreeNode(const Pair& p):Item(p){State=0;Parent=Left=Right=0;}};
void map::_M_erase(_Rb_tree_node_base* p){if(!p)return;_M_erase(p->Left);_M_erase(p->Right);delete (TreeNode*)p;--liveNodes;++erased;}
_Rb_tree_insert_result map::insert(const Pair& p){_Rb_tree_node_base** link=&Head->Parent;_Rb_tree_node_base* parent=Head;while(*link){int c=std::strcmp(text(p.first),text(((TreeNode*)*link)->Item.first));if(!c){++duplicates;lastPosition=*link;lastAdded=false;return _Rb_tree_insert_result(*link,false);}parent=*link;link=c<0?&(*link)->Left:&(*link)->Right;}TreeNode* n=new TreeNode(p);n->Parent=parent;*link=n;++Size;++liveNodes;++addedCount;lastPosition=n;lastAdded=true;return _Rb_tree_insert_result(n,true);}
static TreeNode* find(map& m,const char* key){_Rb_tree_node_base* p=m.Head->Parent;while(p){int c=std::strcmp(key,text(((TreeNode*)p)->Item.first));if(!c)return (TreeNode*)p;p=c<0?p->Left:p->Right;}return 0;}
}
int main(){using namespace X_007565e0;if(sizeof(StringBlock)!=17||sizeof(Value)!=8||sizeof(Pair)!=12||sizeof(CPersistTraits<Value>)!=8)return 1;{CCharString original=make("shared");CCharString* copies[300];for(int i=0;i<300;++i)copies[i]=new CCharString(original);if(block(original)->RefCount!=301)return 2;for(int j=0;j<46;++j)delete copies[j];if(block(original)->RefCount!=255)return 3;for(int k=46;k<300;++k)delete copies[k];if(block(original)->RefCount!=1)return 4;}{CCharString abnormal=make("signed-release");block(abnormal)->RefCount=0;}{CCharString own=make("self");own=own;if(block(own)->RefCount!=1)return 10;CCharString copy(own);if(block(own)->RefCount!=2)return 11;own=CCharString();if(own.Data||block(copy)->RefCount!=1)return 12;}if(blocks)return 5;_Rb_tree_node_base head;std::memset(&head,0,sizeof head);head.Left=head.Right=&head;map m={&head,0};{Pair seed;seed.first=make("seed");seed.second.first=-7;seed.second.second=31;m.insert(seed);}unsigned int words[]={3,2,7,9,1,3,5,2,99,101};for(int mode=0;mode<2;++mode){CMemoryDataInputStreamAdapter in;std::memset(&in,0,sizeof in);CMemoryDataInputStreamVirtualSlots v;/* Fixture installs the ABI view vptr on actual flattened header storage; no original type identity is inferred. */in.__vftable=*(void**)&v;in.chunkBase=(long)words;in.chunkLength=mode?0:sizeof words;GFSerialiseMapBinaryIn(in,m);if(m.Size!=2||in.position!=sizeof words||find(m,"alpha")->Item.second.first!=3||find(m,"alpha")->Item.second.second!=5||find(m,"beta")->Item.second.first!=7||find(m,"beta")->Item.second.second!=9||lastAdded||lastPosition!=find(m,"beta")||blocks!=2)return 6;if(block(find(m,"alpha")->Item.first)->RefCount!=1||block(find(m,"beta")->Item.first)->RefCount!=1)return 7;}if(slowCalls!=10||duplicates!=2||addedCount!=5)return 8;unsigned int zero=0;CMemoryDataInputStreamAdapter empty;std::memset(&empty,0,sizeof empty);CMemoryDataInputStreamVirtualSlots v;empty.__vftable=*(void**)&v;empty.chunkBase=(long)&zero;empty.chunkLength=4;GFSerialiseMapBinaryIn(empty,m);if(m.Size||liveNodes||blocks||head.Parent||head.Left!=&head||head.Right!=&head||erased!=5)return 9;std::printf("007565e0_TEST PASS\n");return 0;}