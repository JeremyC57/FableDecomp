// Grade: DIFFER(356v356) -- only the entry-count down-counter's home differs: retail keeps it in the dead stream-argument slot ([esp+6Ch], frame 0x58), ours gives it a frame slot (frame 0x5c), shifting every esp offset; 7 loop/read forms tried
// CLocalDetailCacheMap::CObjectTypeCollectionPalette::Load(CDataInputStream& stream) -- rebuild the palette from a stream: a flag byte and an entry count, then for each entry load a local-detail object collection type and register it with the engine's local detail generator, keeping the registered pointer
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDEDD0, length 354 bytes (padded to 368), `ret 4`.
// Ego_r.pdb: `?Load@CObjectTypeCollectionPalette@CLocalDetailCacheMap@@QAEXAAVCDataInputStream@@@Z`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined Read of the landed 0x006FF810), memmove @ 0x00BFEAE6
//   (the inlined STLport clear()), CBaseClassNonCopyable ctor @ 0x0099A2F0 / dtor @ 0x0099A300, CLocalDetailObjectCollectionType::
//   Load(CDataInputStream&, bool) @ 0x00BE27B0, CEngineLocalDetailGenerator::RegisterObjectCollectionType @ 0x00B59C70 on the
//   generator @ 0x01436E88, vector::_M_insert_overflow @ 0x00BDFC00 (push_back growth).
// Vtables: CLocalDetailObjectCollectionType 0x012A7D94 (its ??_G 0x00BDF740 runs only the base dtor: no user destructor).
// Layout: PDB CObjectTypeCollectionPalette (16) with the 12-byte retail vector: Entries +0 (CArray<const CLocalDetailObjectCollectionType*>).
//   The frame temporary CLocalDetailObjectCollectionType is 0x44 bytes in retail (frame 0x58); its ctor zeroes the three mesh pointers.
// Shape: both header values come from by-value read helpers (homed in the dead stream slot); the per-entry type is a frame
//   temporary built and destroyed each pass; the push_back value is a named pointer local.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

inline void* operator new(unsigned int, void* p) { return p; }

struct CDataInputStream {
    void*                m_pVtable;        // +0x00
    unsigned long        m_position;       // +0x04
    long                 m_08;
    const unsigned char* m_pData;          // +0x0c current chunk read pointer
    long                 m_10;
    long                 m_remaining;      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned char ReadUChar() { unsigned char value; Read(&value, 1); return value; }
    __forceinline long ReadLong() { long value; Read(&value, 4); return value; }
};

struct __true_type { __true_type() { } };
template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class T> inline T* __copy_trivial(const T* first, const T* last, T* result)
{
    if (last == first)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (T*)((char*)memmove(result, first, n) + n);
}

template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last)
    {
        T* i = __copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fillLength, bool atEnd);   // 0x00BDFC00
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                       // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();              // 0x0099A300
};

class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable {   // vtable 0x012A7D94
public:
    void*         Mesh;                            // +0x04 C3DMeshInfo*
    void*         ZSpriteMesh;                     // +0x08
    void*         ShadowMesh;                      // +0x0c
    long          MeshPolyCount;                   // +0x10
    long          MeshBankIndex;                   // +0x14
    long          ZSpriteMeshBankIndex;            // +0x18
    long          ShadowMeshBankIndex;             // +0x1c
    unsigned char PlainDataBlock[0x34];            // +0x20 (0x54-byte object as the PDB)
    CLocalDetailObjectCollectionType() : Mesh(0), ZSpriteMesh(0), ShadowMesh(0) { }
    void Load(CDataInputStream& stream, bool flag);   // 0x00BE27B0
};

class CEngineLocalDetailGenerator {
public:
    const CLocalDetailObjectCollectionType* RegisterObjectCollectionType(const CLocalDetailObjectCollectionType& type);   // 0x00B59C70
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;   // 0x01436E88

namespace CLocalDetailCacheMap {

class CObjectTypeCollectionPalette {
public:
    vector<const CLocalDetailObjectCollectionType*> Entries;   // +0x00
    void Load(CDataInputStream& stream);
};

void CObjectTypeCollectionPalette::Load(CDataInputStream& stream)
{
    bool flag = stream.ReadUChar() != 0;
    long count = stream.ReadLong();
    Entries.clear();
    for (; count > 0; --count) {
        CLocalDetailObjectCollectionType type;
        type.Load(stream, flag);
        const CLocalDetailObjectCollectionType* registered = g_pLocalDetailGenerator->RegisterObjectCollectionType(type);
        Entries.push_back(registered);
    }
}

}
