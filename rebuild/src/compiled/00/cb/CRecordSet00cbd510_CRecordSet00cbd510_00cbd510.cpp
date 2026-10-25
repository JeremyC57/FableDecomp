// CRecordSet_00cbd510::~CRecordSet_00cbd510() -- destroy the named-record vector, the two vector maps and the list, then the CBase part
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBD510, length 148 bytes (padded to 160).
// Leaf: ??1CRecordSet_00cbd510
// Label correction: the catalog "CParticleEmitter::DeleteAllParticles" is propagated and wrong. The body is the destructor of a
// CBase-derived class with no RTTI row: its only direct caller is the scalar deleting destructor 0x00CBD4F0, and nine `jmp`
// thunks at 0x00CDD100.. forward to it. The class is named by address.
// Callees: ~CCharString @ 0x0099EAE0, _Destroy_range over 12-byte {CCharString, CCountedPointer<..>} pairs @ 0x00CBB200 and
// <pair<long, CCountedPointer<CRegionConnectionNode> > > @ 0x00CBB1B0 (`__fastcall(first, last, const allocator&)`), the list's clear() @ 0x00CBB090, free @ 0x00BFEA14, CBase::~CBase @ 0x0099A300.
// Layout: CBase vptr +0, STLport list (header pointer) +4, two 16-byte CVectorMaps +8 / +0x18, two words +0x28/+0x2c, a vector of
// 24-byte records (CCharString at +0x10) +0x30.
// Shape: no vtable store, so the class is `__declspec(novtable)` over CBase's out-of-line virtual destructor; an empty user dtor,
// members destroyed in reverse order: the record vector's inline _Destroy loop (bounds by value, end in ebx), the maps through the
// range helper with an empty allocator<T>() temporary (both homed at byte 3 of the `push ecx` frame word), the list cleared and its
// header freed.
// Fixed (rename1, 2026-10-24): the 0x00CBB200 element was declared as a 16-byte pair<long, CCompositeSurfaceInfo> (the
//   helper's propagated manifest name); 0x00CBB200 (landed std::_Destroy_range) steps 12 bytes and destroys a CCharString at
//   +0 and a counted pointer {object +4, count block +8}, so the map at +0x18 holds {CCharString, CCountedPointer} pairs.
//   Declaration only: the out-of-line helper takes the bounds, so the bytes are unchanged.
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
struct CCharString {
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
namespace N_0099a300 { struct CBase {
    virtual ~CBase();                       // 0x0099A300
}; } using N_0099a300::CBase;
template <class T> struct allocator { allocator() {} };
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> void __fastcall _Destroy_range(T* first, T* last, const allocator<T>& alloc);
struct SNamedCountedPair { CCharString first; void* object; void* block; ~SNamedCountedPair(); };  // pair<CCharString, CCountedPointer<..> > (12 bytes)
struct SConnectionPair { long first; void* object; void* block; ~SConnectionPair(); };    // pair<long, CCountedPointer<CRegionConnectionNode> >
template <> void __fastcall _Destroy_range(SNamedCountedPair* first, SNamedCountedPair* last, const allocator<SNamedCountedPair>&);  // 0x00CBB200
template <> void __fastcall _Destroy_range(SConnectionPair* first, SConnectionPair* last, const allocator<SConnectionPair>&);  // 0x00CBB1B0
struct SNamedRecord {                       // 24 bytes, a CCharString at +0x10
    long        m_0;
    long        m_4;
    long        m_8;
    long        m_c;
    CCharString m_name;                     // +0x10
    long        m_14;
};
struct CNamedRecordVector {                 // STLport vector<SNamedRecord>, inline element destruction
    SNamedRecord* _M_start;
    SNamedRecord* _M_finish;
    SNamedRecord* _M_end_of_storage;
    ~CNamedRecordVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
template <class T> struct CRangeVector {    // STLport vector<T> whose elements go through _Destroy_range
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRangeVector()
    {
        _Destroy_range(_M_start, _M_finish, allocator<T>());
        if (_M_start)
            free(_M_start);
    }
};
template <class T> struct CVectorMap {      // retail 16 bytes
    CRangeVector<T> Data;                   // +0x00
    unsigned char   Compare;                // +0x0c
    bool            Dirty;                  // +0x0d
};
struct SListNode;
struct CEventList {                         // STLport list: the header node pointer
    SListNode* _M_node;
    void clear();                           // 0x00CBB090
    ~CEventList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct __declspec(novtable) CRecordSet_00cbd510 : CBase {
    CEventList                    m_list;           // +0x04
    CVectorMap<SConnectionPair>   m_connections;    // +0x08
    CVectorMap<SNamedCountedPair> m_namedPointers;  // +0x18
    long                          m_28;             // +0x28
    long                          m_2c;             // +0x2c
    CNamedRecordVector            m_records;        // +0x30
    ~CRecordSet_00cbd510();
};
CRecordSet_00cbd510::~CRecordSet_00cbd510()
{
}