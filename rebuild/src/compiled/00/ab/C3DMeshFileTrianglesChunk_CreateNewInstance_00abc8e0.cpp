// C3DMeshFileTrianglesChunk::CreateNewInstance() const -- Construct a new chunk and pass it through the original initialization hook.
// Leaf: ?CreateNewInstance@C3DMeshFileTrianglesChunk@@UBEPAVCChunkedFileChunk@@XZ
// Identity: own vtable129ed34 slot9 and exact PDB const virtual method. Literal TRIS, protected base constructor00aa8360 and virtual InitNewInstance slot10.
// Layout: shared flattened headers aliased; real CBaseClassNonCopyable/Chunk/SuperChunk bases and original named arrays restored. Base16 or44, final28 or56. UnusedSlot names represent untouched virtual slots, not recovered names.
#define CChunkedFileSuperChunk CChunkedFileSuperChunk_HeaderLayout
#include "engine/CChunkedFileSuperChunk.h"
#undef CChunkedFileSuperChunk
#define CChunkedFileChunk CChunkedFileChunk_HeaderLayout
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
#define C3DMeshFileTrianglesChunk C3DMeshFileTrianglesChunk_HeaderLayout
#include "engine/C3DMeshFileTrianglesChunk.h"
#undef C3DMeshFileTrianglesChunk
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
namespace std{template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};}
template<class T>class CArray:public std::vector<T>{};
class CChunkedFileSuperChunk:public CChunkedFileChunk{public:CArray<CCountedPointer<CChunkedFileChunk> > SubChunks,RegisteredSubChunkTypes;unsigned long CurrentChunkIndex;protected:CChunkedFileSuperChunk(const char*);};
class C3DTriangle2;
class C3DMeshFileTrianglesChunk:public CChunkedFileSuperChunk{public:CArray<C3DTriangle2> TriangleList;C3DMeshFileTrianglesChunk():CChunkedFileSuperChunk("TRIS"){}virtual CChunkedFileChunk* CreateNewInstance()const;};
CChunkedFileChunk* C3DMeshFileTrianglesChunk::CreateNewInstance()const{return InitNewInstance(new C3DMeshFileTrianglesChunk);}