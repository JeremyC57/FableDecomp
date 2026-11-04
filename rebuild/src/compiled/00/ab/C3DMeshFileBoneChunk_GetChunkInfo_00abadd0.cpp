// C3DMeshFileBoneChunk::GetChunkInfo() const -- Format the named chunk hierarchy into a returned string.
// Leaf: ?GetChunkInfo@C3DMeshFileBoneChunk@@UBE?BVCCharString@@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags: /GS
// Identity: own vtable129eb9c slot2, exact original PDB const virtual class-valued return. Format literal129e940 and CRT _snprintfBFEB96; original named index fields.
// Layout: active chunk headers flattened, alias them and restore original CBaseClassNonCopyable/Chunk/SuperChunk inheritance with genuine arrays. Retail SuperChunk44, Name44 and indices48..60; Bone indices44..56, LocalizationMatrix60, Name108. Shared submesh header confirms.
#define CChunkedFileSuperChunk CChunkedFileSuperChunk_HeaderLayout
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
#define CChunkedFileChunk CChunkedFileChunk_HeaderLayout
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
#include <stdio.h>
#ifndef _MSC_VER
#define _snprintf snprintf
#endif
class CCharStringData{public:char* PData;};class CCharString{public:CCharStringData* PStringData;CCharString():PStringData(0){}CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();const char* GetAsCharArray()const{return PStringData==0?"":PStringData->PData;}};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{};class CChunkedFileSuperChunk;
class CChunkedFileChunk:public CBaseClassNonCopyable{public:char ChunkType[5];CChunkedFileSuperChunk* Parent;};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
#include "engine/CArray.h"
class CChunkedFileSuperChunk:public CChunkedFileChunk{public:CArray<CCountedPointer<CChunkedFileChunk> > SubChunks,RegisteredSubChunkTypes;unsigned long CurrentChunkIndex;};
class C3DMeshFileBoneChunk:public CChunkedFileSuperChunk{public:long BoneIndex,ParentIndex,FirstChildIndex,NextSiblingIndex;float LocalizationMatrix[12];CCharString Name;virtual const CCharString GetChunkInfo()const;};
const CCharString C3DMeshFileBoneChunk::GetChunkInfo()const{char buffer[1024];_snprintf(buffer,1024,"Name: %s, Index: %li, Parent: %li, Child: %li, Sibling: %li",Name.GetAsCharArray(),BoneIndex,ParentIndex,FirstChildIndex,NextSiblingIndex);buffer[1023]=0;return CCharString(buffer);}