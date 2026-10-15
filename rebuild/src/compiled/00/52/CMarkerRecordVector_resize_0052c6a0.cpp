// CMarkerRecordVector::resize(unsigned long n) -- the one-argument std::vector::resize: grow or shrink to n records, new ones default-constructed (a temporary passed to resize(n, value))
// Leaf: ?resize@CMarkerRecordVector@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052C6A0, length 88 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0 and ~CCharString @ 0x0099EAE0 (the temporary's name),
// vector::resize(size_type, const T&) @ 0x0052C630 (24-byte elements: the 0x2AAAAAAB / `sar edx,2` divide; manifest
// "vector<CTCInventoryMap::CUIMiniMapMarker>::resize"), free @ 0x00BFEA14 (the temporary's inner vector storage).
// Identity: the catalog's "CThingDeadCreature::WorldUpdateBulletTime" is a propagated label: `this` is handed straight to
// the two-argument resize, so this is the STLport `resize(size_type n) { resize(n, T()); }` overload of the same vector.
// The class and record names are descriptive (the element type is only known by its 24-byte layout).
// Layout (local model): the record {CCharString name +0, vector {start +4, finish +8, end of storage +0xc}, long +0x10,
// bool +0x14} = 0x18 bytes, built in the frame.
// Shape: the record's name is constructed first (out of line); the zero stores of the inner vector, the long and the bool
// are scheduled after the argument pushes (ebx = 0); after the call the temporary dies in reverse member order: the
// inner vector frees its storage when non-null (`push eax ; call free ; add esp,4`), then the name's destructor.
#include <stdlib.h>

struct CCharString {
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
};

struct CMarkerPointVector {                     // std::vector<...> with a malloc-based allocator
    void* m_start;
    void* m_finish;
    void* m_endOfStorage;
    CMarkerPointVector() : m_start(0), m_finish(0), m_endOfStorage(0) { }
    ~CMarkerPointVector()
    {
        if (m_start)
            free(m_start);
    }
};

struct SMarkerRecord {                          // 0x18 bytes
    CCharString        m_name;                  // +0x00
    CMarkerPointVector m_points;                // +0x04
    long               m_value;                 // +0x10
    bool               m_flag;                  // +0x14
    SMarkerRecord() : m_value(0), m_flag(false) { }
};

struct CMarkerRecordVector {                    // std::vector<SMarkerRecord>
    SMarkerRecord* m_start;
    SMarkerRecord* m_finish;
    SMarkerRecord* m_endOfStorage;
    void resize(unsigned long n, const SMarkerRecord& value);   // 0x0052C630
    void resize(unsigned long n);
};

void CMarkerRecordVector::resize(unsigned long n)
{
    resize(n, SMarkerRecord());
}