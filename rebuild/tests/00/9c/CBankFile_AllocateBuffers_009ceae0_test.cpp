// 009ceae0 CBankFile::AllocateBuffers
// Flags: /O2 /Oy /W3
// Leaf: AllocateBuffers
#define FABLE_RETAIL_BANK_FILE
#include "engine/CBankFile.h"
#include <stdlib.h>
#include <string.h>
namespace X_009ceae0{
struct CCharStringData;struct CCharString{CCharStringData*PStringData;CCharString();CCharString(const CCharString&);~CCharString();};
// Layout: PDB CRuntimeData12: DataOffset0,DataSize4,DataType8,Valid9.
struct CRuntimeData{unsigned long DataOffset,DataSize;unsigned char DataType;bool Valid;CRuntimeData():DataOffset(0),DataSize(0),DataType(0),Valid(false){}};
struct CBankFileEntryUpdateData;
template<class T>inline T Exchange(T&slot,T value){T old=slot;slot=value;return old;}
template<class T>__forceinline T*CopyRange(T*first,T*last,T*result){return first==last?result:(T*)((char*)memmove(result,first,(char*)last-(char*)first)+((char*)last-(char*)first));}
// Layout: retail STLport storage12 bytes. Empty-range copying and storage detachment follow landed CBankFile::InternalClose.
template<class T>struct CArray{T*Start,*Finish,*Storage;void resize(unsigned,const T&);void resize(unsigned);__forceinline void DeallocateMemory(){if((unsigned)(Storage-Start)>0){Finish=CopyRange(Finish,Finish,Start);T*p=Exchange(Start,(T*)0);Exchange(Finish,(T*)0);Storage=0;if(p)free(p);}}};
template<>void CArray<CRuntimeData>::DeallocateMemory();template<>void CArray<CCharString>::DeallocateMemory();
struct SymbolTree{struct Node{unsigned Colour;Node*Parent,*Left,*Right;CCharString Key;unsigned long Value;};Node*Head;unsigned Count;unsigned char Compare;void _M_erase(Node*);__forceinline void clear(){if(Count){_M_erase(Head->Parent);Head->Left=Head;Head->Parent=0;Head->Right=Head;Count=0;}}};
// Layout: actual CBankFile.h owns retail110 storage; data-free methods adapter. Header opt-in reviewed by main, preserving donor default.
struct CBankFileMethods: ::CBankFile{void AllocateBuffers(unsigned long);};
void CBankFileMethods::AllocateBuffers(unsigned long n){Size=n;CArray<CRuntimeData>&runtime=*(CArray<CRuntimeData>*)RuntimeData;runtime.DeallocateMemory();runtime.resize(n,CRuntimeData());CArray<CCharString>&symbols=*(CArray<CCharString>*)Symbols;symbols.DeallocateMemory();if(OpenFlags&8)symbols.resize(n);CArray<unsigned long>&checksums=*(CArray<unsigned long>*)Checksums;checksums.DeallocateMemory();if(OpenFlags&16)checksums.resize(n,0);CArray<CBankFileEntryUpdateData*>&updates=*(CArray<CBankFileEntryUpdateData*>*)UpdateData;updates.DeallocateMemory();if(OpenFlags&32)updates.resize(n,0);((SymbolTree*)SymbolIndices)->clear();((SymbolTree*)FilenameIndices)->clear();}
}

#include <stdio.h>
#include <assert.h>
#include <new>
#include <stddef.h>
namespace X_009ceae0{
struct CCharStringData{unsigned refs;char text[16];};static unsigned live;
CCharString::CCharString():PStringData(0){}CCharString::CCharString(const CCharString&r):PStringData(r.PStringData){if(PStringData)++PStringData->refs;}CCharString::~CCharString(){if(PStringData&&!--PStringData->refs){delete PStringData;--live;}}
template<class T>void Destroy(CArray<T>&a){for(T*p=a.Start;p!=a.Finish;++p)p->~T();free(a.Start);a.Start=a.Finish=a.Storage=0;}
template<>void CArray<CRuntimeData>::DeallocateMemory(){Destroy(*this);}template<>void CArray<CCharString>::DeallocateMemory(){Destroy(*this);}
template<class T>void CArray<T>::resize(unsigned n,const T&v){Destroy(*this);Start=n?(T*)malloc(n*sizeof(T)):0;for(unsigned i=0;i<n;++i)new(Start+i)T(v);Finish=Storage=Start+n;}
template<class T>void CArray<T>::resize(unsigned n){T v;resize(n,v);}
void SymbolTree::_M_erase(Node*p){if(!p)return;_M_erase(p->Left);_M_erase(p->Right);delete p;}
static void MakeTree(SymbolTree&t,bool populated){t.Head=new SymbolTree::Node;t.Head->Parent=0;t.Head->Left=t.Head->Right=t.Head;t.Count=0;if(populated){SymbolTree::Node*p=new SymbolTree::Node;p->Parent=t.Head;p->Left=p->Right=0;p->Key.PStringData=new CCharStringData;p->Key.PStringData->refs=1;++live;t.Head->Parent=t.Head->Left=t.Head->Right=p;t.Count=1;}}
}
int main(){using namespace X_009ceae0;assert(sizeof(::CBankFile)==0x110&&offsetof(::CBankFile,RuntimeData)==0x20&&offsetof(::CBankFile,OpenFlags)==0x78&&offsetof(::CBankFile,FilenameIndices)==0xc4);for(unsigned bits=0;bits<8;++bits){CBankFileMethods bank;memset(&bank,0,sizeof(bank));SymbolTree&symbols=*(SymbolTree*)bank.SymbolIndices;SymbolTree&files=*(SymbolTree*)bank.FilenameIndices;MakeTree(symbols,true);MakeTree(files,true);bank.OpenFlags=bits<<3;bank.AllocateBuffers(3);CArray<CRuntimeData>&r=*(CArray<CRuntimeData>*)bank.RuntimeData;CArray<CCharString>&s=*(CArray<CCharString>*)bank.Symbols;CArray<unsigned long>&c=*(CArray<unsigned long>*)bank.Checksums;CArray<CBankFileEntryUpdateData*>&u=*(CArray<CBankFileEntryUpdateData*>*)bank.UpdateData;assert(bank.Size==3&&r.Finish-r.Start==3&&s.Finish-s.Start==((bits&1)?3:0)&&c.Finish-c.Start==((bits&2)?3:0)&&u.Finish-u.Start==((bits&4)?3:0));for(unsigned i=0;i<3;++i)assert(r.Start[i].DataOffset==0&&r.Start[i].DataSize==0&&!r.Start[i].DataType&&!r.Start[i].Valid);assert(symbols.Count==0&&files.Count==0&&symbols.Head->Left==symbols.Head&&files.Head->Right==files.Head);bank.AllocateBuffers(0);assert(r.Start==r.Finish&&bank.Size==0);Destroy(r);Destroy(s);Destroy(c);Destroy(u);delete symbols.Head;delete files.Head;}assert(live==0);puts("009ceae0_TEST PASS");return 0;}