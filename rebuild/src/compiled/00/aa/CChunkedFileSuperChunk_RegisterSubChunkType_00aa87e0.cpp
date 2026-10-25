// CChunkedFileSuperChunk::RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk) -- append a prototype chunk (a type the super chunk can instantiate while reading) to RegisteredSubChunkTypes
// Leaf: ?RegisterSubChunkType@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA87E0, 80 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb `?RegisterSubChunkType@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z`
//   (the donor puts it right before CChunkedFileSuperChunk::ReadFromFile, as retail does: 0x00AA8830); same body as
//   AddSubChunk 0x00AA84F0 minus the SetParent store, on the second array (+0x1C) with the same overflow 0x00AA94E0.
//   Callers register prototypes: the C3DMeshFileXLoader ctor 0x00AA5C80 (three times), the GUI var-transfer Add
//   helpers 0x00A98BE0.. and C3DMeshFileLoader 0x00A9AD00 (identical-body folding may share it). The catalog's
//   "C3DMeshFileXLoader_AddMeshToQueue" is a guess from one caller.
// Callees: vector<CCountedPointer<CChunkedFileChunk> >::_M_insert_overflow(pos, x, tag, 1, true) 0x00AA94E0.
// Layout: as AddSubChunk -- retail CChunkedFileSuperChunk with 12-byte STLport arrays at +0x10/+0x1C (the engine header
//   carries the 16-byte Ego_r arrays), modelled locally.
// Shape: STLport `RegisteredSubChunkTypes.push_back(chunk)` inline: `this` advanced to the member, the placement copy
//   under its null test and `add [ecx+4],8`, or the overflow call with the empty tag homed in the dead `chunk` slot.
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

void CChunkedFileSuperChunk::RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunk)
{
    RegisteredSubChunkTypes.push_back(chunk);
}