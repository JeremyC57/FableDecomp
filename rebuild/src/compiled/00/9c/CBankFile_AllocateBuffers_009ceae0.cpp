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