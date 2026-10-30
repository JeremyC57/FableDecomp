// C3DMeshFileVerticesChunk::GetChunkInfo() const -- "Number of vertices: %u" over the vertex list, returned as a CCharString
// Leaf: ?GetChunkInfo@C3DMeshFileVerticesChunk@@UBE?BVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABBD70, 92 bytes, thiscall, `ret 4` (hidden result), /GS frame (cookie 0x0139C8A8,
//   check 0x00BFE9F9).
// Flags: /GS
// Identity (proven): Ego_r.pdb C3DMeshFileVerticesChunk::GetChunkInfo (the const virtual of every CChunkedFileChunk;
//   siblings landed, e.g. C3DMeshFileBoneChunk 0x00ABADD0); VertexList +0x10 is CArray<C3DVertex2> (32-byte elements:
//   `sar 5`); the format literal at 0x0129EBF4 reads "Number of vertices: %u" in the retail image.
// Callees: sprintf 0x00BFEB10 (CRT), CCharString::CCharString(const char*, long = -1) 0x0099EBF0.
#define CChunkedFileChunk CChunkedFileChunk_HeaderLayout
#include "engine/CChunkedFileChunk.h"
#undef CChunkedFileChunk
#define C3DMeshFileVerticesChunk C3DMeshFileVerticesChunk_HeaderLayout
#include "engine/C3DMeshFileVerticesChunk.h"
#undef C3DMeshFileVerticesChunk
#include <stdio.h>

class CCharStringData { public: char* PData; };
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable : public CBaseClass { };
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {
public:
    char                    ChunkType[5];                 // +0x04
    CChunkedFileSuperChunk* Parent;                       // +0x0c
    virtual const CCharString GetChunkInfo() const;
};

namespace std {
template <class T> class allocator { };
template <class V, class T, class A> class _STLP_alloc_proxy : public A { public: V _M_data; };
template <class T, class A> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
};
}
template <class T> class CArray : public std::vector<T> { };

class C3DVertex2 { public: unsigned char _body[0x20]; };   // 32 bytes (position, normal, uv)

class C3DMeshFileVerticesChunk : public CChunkedFileChunk {
public:
    CArray<C3DVertex2> VertexList;                        // +0x10
    virtual const CCharString GetChunkInfo() const;
};

const CCharString C3DMeshFileVerticesChunk::GetChunkInfo() const
{
    char buffer[128];
    sprintf(buffer, "Number of vertices: %u", VertexList.size());
    return CCharString(buffer);
}