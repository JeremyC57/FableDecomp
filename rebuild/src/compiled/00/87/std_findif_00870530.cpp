// std::__find_if(const CThingComponentSet::CEntry* first, const CThingComponentSet::CEntry* last, CPred_EntryIsDriver pred, const random_access_iterator_tag&) -- STLport 4x-unrolled find_if over the packed 9-byte component entries: the first entry whose registered TC info exists and has a driver type, else last
// Leaf: ??$__find_if@PBVCEntry@CThingComponentSet@@VCPred_EntryIsDriver@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870530, 544 bytes incl. int3 padding, /Gr `__fastcall` (first in ecx,
//   last in edx, the empty predicate by value and the tag reference on the stack), `ret 8`.
// Identity: the catalog's "CWorldDriver_FindClosestMeshVolume" is descriptive. Ego_r/FableWin PDB name set:
//   std::find_if<const CThingComponentSet::CEntry*, CPred_EntryIsDriver> and `bool CPred_EntryIsDriver::operator()(const
//   CThingComponentSet::CEntry&) const`. Callers: the landed CThingComponentSet::GetDriverInfo 0x008709C0 and HasDriver
//   0x00870980 (`find_if(begin, begin + count, CPred_EntryIsDriver())`).
// Layout (Ego_r): CThingComponentSet::CEntry is 9 bytes, packed: CDefString Name +0, ulong DriverType +4, bool Override
//   +8. CTCBase::CRegisteredTCInfo (12 bytes): AllocFunc +0, ulong DriverType +4, CCharString Name +8.
// Callees: CDefStringTable::GetString(long) const 0x009D49B0 on the global table 0x013CA828 (CDefString's inline
//   `operator CCharString() const`), CTCBase::GetPTCInfo(const CCharString&) 0x00686F60 (static __fastcall, landed),
//   ~CCharString 0x0099EAE0.
// Shape: STLport 4.6 `__find_if(first, last, pred, const random_access_iterator_tag&)` verbatim (trip count (last -
//   first) >> 2 over the 4-way unrolled body, then `switch (last - first)` cases 3/2/1); the inlined operator() converts
//   the entry's def name to a CCharString temporary, looks the TC up, destroys the temporary, then `if (info == 0) return false;
//   return info->DriverType != 0;` (the early return keeps the `setne al ; test al,al` materialised bool; a single `&&`
//   expression folds it into a plain `jne`).
class CCharString {
public:
    void* PString;                                          // +0x00
    ~CCharString();                                         // 0x0099EAE0
};

class CDefStringTable {
public:
    CCharString GetString(long table_pos) const;            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828

class CDefString {
public:
    long TablePos;                                          // +0x00
    operator CCharString() const { return g_DefStringTable.GetString(TablePos); }
};

class CThing;
class CTCBase {
public:
    class CRegisteredTCInfo {                               // 12 bytes
    public:
        CTCBase* (__cdecl* AllocFunc)(CThing*);             // +0x00
        unsigned long DriverType;                           // +0x04
        CCharString   Name;                                 // +0x08
    };
    static const CRegisteredTCInfo* __fastcall GetPTCInfo(const CCharString& name);    // 0x00686F60
};

class CThingComponentSet {
public:
#pragma pack(push, 1)
    class CEntry {                                          // 9 bytes
    public:
        CDefString    Name;                                 // +0x00
        unsigned long DriverType;                           // +0x04
        bool          Override;                             // +0x08
    };
#pragma pack(pop)
};

class CPred_EntryIsDriver {
public:
    bool operator()(const CThingComponentSet::CEntry& entry) const
    {
        const CTCBase::CRegisteredTCInfo* info = CTCBase::GetPTCInfo(entry.Name);
        if (info == 0)
            return false;
        return info->DriverType != 0;
    }
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template const CThingComponentSet::CEntry* __fastcall
__find_if<const CThingComponentSet::CEntry*, CPred_EntryIsDriver>(
    const CThingComponentSet::CEntry*, const CThingComponentSet::CEntry*, CPred_EntryIsDriver,
    const random_access_iterator_tag&);

} // namespace std