// C3DMeshFileUniqueVerticesChunk::CreateNewInstance() const -- a new "UNIV" C3DMeshFileUniqueVerticesChunk passed through InitNewInstance (the chunk prototype's factory)
// Leaf: ?CreateNewInstance@C3DMeshFileUniqueVerticesChunk@@UBEPAVCChunkedFileChunk@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABC5F0, 83 bytes, thiscall, plain `ret`.
// Identity (proven): slot 9 of vtable 0x0129EC6C (RTTI .?AVC3DMeshFileUniqueVerticesChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass), and the
//   object it builds stores that same vtable; FableWin.pdb ?CreateNewInstance@C3DMeshFileUniqueVerticesChunk@@UBEPAVCChunkedFileChunk@@XZ; Ego_r.pdb member
//   UniqueVerticesList (CArray<unsigned long>) at +0x10.
// Callees: operator new 0x00BFEA1A (0x1C bytes), CChunkedFileChunk::CChunkedFileChunk(const char*) 0x00AA7590 (literal 0x0129EB0C
//   "UNIV"), virtual InitNewInstance(CChunkedFileChunk*) const (slot 10, +0x28).
// Shape (family K052, representative 0x00AA46B0): the landed G048 factory 0x00A98AB0 with a CChunkedFileChunk base and the
//   member CArray's inline default constructor (three zero stores after the vtable store).
class CChunkedFileSuperChunk;
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();                            // slot 0
    virtual void v01(); virtual void v02();
    virtual int IsSuperChunk() const;                        // slot 3
    virtual int IsUnknownType() const;                       // slot 4
    virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual CChunkedFileChunk* CreateNewInstance() const = 0;   // slot 9
protected:
    CChunkedFileChunk(const char* chunkType);                // 0x00AA7590
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;   // slot 10 (+0x28)
    char ChunkType[5];                                       // +0x04
    CChunkedFileSuperChunk* Parent;                          // +0x0C
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
};
}
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray<T>: an STLport vector<T>, 12 bytes in retail
class C3DVertex2;
class C3DMeshFileUniqueVerticesChunk : public CChunkedFileChunk {
public:
    C3DMeshFileUniqueVerticesChunk() : CChunkedFileChunk("UNIV") { }
    virtual CChunkedFileChunk* CreateNewInstance() const;
    CArray<unsigned long> UniqueVerticesList;                                     // +0x10
};
CChunkedFileChunk* C3DMeshFileUniqueVerticesChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileUniqueVerticesChunk);
}