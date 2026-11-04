// CMirroredTextureHighlightsChunk::CreateNewInstance() const -- Construct a new chunk and pass it through the original initialization hook.
// Leaf: ?CreateNewInstance@CMirroredTextureHighlightsChunk@@UBEPAVCChunkedFileChunk@@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own vtable129e0e0 slot9 and exact PDB const virtual method. Literal MTEH, protected base constructor00aa7590 and virtual InitNewInstance slot10.
// Layout: shared flattened headers aliased; real CBaseClassNonCopyable/Chunk/SuperChunk bases and original named arrays restored. Base16 or44, final28 or56. UnusedSlot names represent untouched virtual slots, not recovered names.
#define CChunkedFileSuperChunk CChunkedFileSuperChunk_HeaderLayout
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
#define CChunkedFileChunk CChunkedFileChunk_HeaderLayout
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
#define CMirroredTextureHighlightsChunk CMirroredTextureHighlightsChunk_HeaderLayout
#include "engine/CMirroredTextureHighlightsChunk.h"
#undef CMirroredTextureHighlightsChunk
#include <new>
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable:public CBaseClass{};
class CChunkedFileSuperChunk;class CChunkedFileChunk:public CBaseClassNonCopyable{public:char ChunkType[5];CChunkedFileSuperChunk* Parent;
virtual void UnusedSlot01(){}
virtual void UnusedSlot02(){}
virtual void UnusedSlot03(){}
virtual void UnusedSlot04(){}
virtual void UnusedSlot05(){}
virtual void UnusedSlot06(){}
virtual void UnusedSlot07(){}
virtual void UnusedSlot08(){}
virtual CChunkedFileChunk* CreateNewInstance()const=0;
protected:CChunkedFileChunk(const char*);virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* p)const{return p;}
};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
#include "engine/CArray.h"
class CChunkedFileSuperChunk:public CChunkedFileChunk{public:CArray<CCountedPointer<CChunkedFileChunk> > SubChunks,RegisteredSubChunkTypes;unsigned long CurrentChunkIndex;protected:CChunkedFileSuperChunk(const char*);};
class C3DTriangle2;
class CMirroredTextureHighlightsChunk:public CChunkedFileChunk{public:CArray<unsigned long> TriangleVertexIndexList;CMirroredTextureHighlightsChunk():CChunkedFileChunk("MTEH"){}virtual CChunkedFileChunk* CreateNewInstance()const;};
CChunkedFileChunk* CMirroredTextureHighlightsChunk::CreateNewInstance()const{return InitNewInstance(new CMirroredTextureHighlightsChunk);}