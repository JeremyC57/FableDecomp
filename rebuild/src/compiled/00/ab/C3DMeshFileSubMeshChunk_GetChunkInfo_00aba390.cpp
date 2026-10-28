// C3DMeshFileSubMeshChunk::GetChunkInfo() const -- Format the named chunk hierarchy into a returned string.
// Leaf: ?GetChunkInfo@C3DMeshFileSubMeshChunk@@UBE?BVCCharString@@XZ
// Flags: /GS
// Identity: own vtable129eb18 slot2, exact original PDB const virtual class-valued return. Format literal129e940 and CRT _snprintfBFEB96; original named index fields.
// Layout: active chunk headers flattened, alias them and restore original CBaseClassNonCopyable/Chunk/SuperChunk inheritance with genuine arrays. Retail SuperChunk44, Name44 and indices48..60; Bone indices44..56, LocalizationMatrix60, Name108. Shared submesh header confirms.
#define CChunkedFileSuperChunk CChunkedFileSuperChunk_HeaderLayout
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
#define CChunkedFileChunk CChunkedFileChunk_HeaderLayout
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
#define C3DMeshFileSubMeshChunk C3DMeshFileSubMeshChunk_HeaderLayout
#include "engine/C3DMeshFileSubMeshChunk.h"
#undef C3DMeshFileSubMeshChunk
#include <stdio.h>
#ifndef _MSC_VER
#define _snprintf snprintf
#endif
class CCharStringData{public:char* PData;};class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();const char* GetAsCharArray()const{return PStringData==0?"":PStringData->PData;}};
class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{};class CChunkedFileSuperChunk;
class CChunkedFileChunk:public CBaseClassNonCopyable{public:char ChunkType[5];CChunkedFileSuperChunk* Parent;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};}
template<class T>class CArray:public std::vector<T>{};
class CChunkedFileSuperChunk:public CChunkedFileChunk{public:CArray<CCountedPointer<CChunkedFileChunk> > SubChunks,RegisteredSubChunkTypes;unsigned long CurrentChunkIndex;};
class C3DMeshFileSubMeshChunk:public CChunkedFileSuperChunk{public:CCharString Name;long SubMeshIndex,ParentIndex,FirstChildIndex,NextSiblingIndex;virtual const CCharString GetChunkInfo()const;};
const CCharString C3DMeshFileSubMeshChunk::GetChunkInfo()const{char buffer[1024];_snprintf(buffer,1024,"Name: %s, Index: %li, Parent: %li, Child: %li, Sibling: %li",Name.GetAsCharArray(),SubMeshIndex,ParentIndex,FirstChildIndex,NextSiblingIndex);buffer[1023]=0;return CCharString(buffer);}