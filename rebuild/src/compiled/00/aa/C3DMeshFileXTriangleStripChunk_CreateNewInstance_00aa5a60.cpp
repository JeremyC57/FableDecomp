// C3DMeshFileXTriangleStripChunk::CreateNewInstance() const -- Construct a new chunk and pass it through the original initialization hook.
// Leaf: ?CreateNewInstance@C3DMeshFileXTriangleStripChunk@@UBEPAVCChunkedFileChunk@@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: own vtable129e380 slot9 and exact PDB const virtual method. Literal XTST, protected base constructor00aa7590 and virtual InitNewInstance slot10.
// Layout: shared flattened headers aliased; real CBaseClassNonCopyable/Chunk/SuperChunk bases and original named arrays restored. Base16 or44, final28 or56. UnusedSlot names represent untouched virtual slots, not recovered names.
#define CChunkedFileSuperChunk CChunkedFileSuperChunk_HeaderLayout
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
#define CChunkedFileChunk CChunkedFileChunk_HeaderLayout
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
#define C3DMeshFileXTriangleStripChunk C3DMeshFileXTriangleStripChunk_HeaderLayout
#include "engine/C3DMeshFileXTriangleStripChunk.h"
#undef C3DMeshFileXTriangleStripChunk
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
class C3DMeshFileXTriangleStripChunk:public CChunkedFileChunk{public:CArray<unsigned short> StripVertices;C3DMeshFileXTriangleStripChunk():CChunkedFileChunk("XTST"){}virtual CChunkedFileChunk* CreateNewInstance()const;};
CChunkedFileChunk* C3DMeshFileXTriangleStripChunk::CreateNewInstance()const{return InitNewInstance(new C3DMeshFileXTriangleStripChunk);}