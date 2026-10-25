// CChunkedFileSuperChunk::AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk) -- parent a chunk to this super chunk and append a counted reference to it to SubChunks
// Leaf: ?AddSubChunk@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA84F0, 69 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb `?AddSubChunk@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z`; the landed
//   COptimisedMesh::CopyTo 0x00ACD5A0 already declares 0x00AA84F0 as AddSubChunk, and the other callers
//   (CEngineWorldMap::CreateNewBankMap 0x00AC0C00, COptimisedPrimitive::CopyTo 0x00ACC490) add mesh-file chunks. The
//   catalog's "vector_push_back" is descriptive.
// Callees: vector<CCountedPointer<CChunkedFileChunk> >::_M_insert_overflow(pos, x, tag, 1, true) 0x00AA94E0.
// Layout: retail CChunkedFileSuperChunk = CChunkedFileChunk {vptr, ChunkType[5] +4, Parent +0xC} + SubChunks +0x10 and
//   RegisteredSubChunkTypes +0x1C as 12-byte STLport vectors (engine/CChunkedFileSuperChunk.h has the 16-byte Ego_r
//   arrays at +0x10/+0x20, so `this` is modelled locally as in the landed siblings 0x00AA7EA0/0x00AA78C0).
//   CCountedPointer = {Data +0, Info +4}, the copy bumps Info->RefCount.
// Shape: the inline private `chunk->SetParent(this)` store first, then STLport `SubChunks.push_back(chunk)` inline: the
//   placement copy under its null test and `add [ecx+4],8`, or the out-of-line overflow with the empty tag homed in the
//   dead `chunk` argument slot.
inline void* operator new(unsigned int, void* where) { return where; }

struct CCPPointerInfo {
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void*         Data;                                 // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    T* operator->() const { return Data; }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00AA94E0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

template <class T> class CArray : public std::vector<T> { };   // retail CArray<T>: the 12-byte STLport vector

class CDataInputStream;
class CDataOutputStream;
class CChunkedFileLoader;
class CChunkedFileSaver;
class CChunkedFileSuperChunk;

class CChunkedFileChunk {                               // retail layout = engine/CChunkedFileChunk.h + its vtable
public:
    virtual ~CChunkedFileChunk();
    char                    ChunkType[5];               // +0x04 four-character code + terminator
    CChunkedFileSuperChunk* Parent;                     // +0x0c
    void SetParent(CChunkedFileSuperChunk* parent) { Parent = parent; }
};

class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10 (Ego_r +0x10, 16-byte debug vector)
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c (Ego_r +0x20)
    unsigned long CurrentChunkIndex;                                        // +0x28 (Ego_r +0x30)
    void AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk);
    void RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk);
};

void CChunkedFileSuperChunk::AddSubChunk(const CCountedPointer<CChunkedFileChunk>& chunk)
{
    chunk->SetParent(this);
    SubChunks.push_back(chunk);
}