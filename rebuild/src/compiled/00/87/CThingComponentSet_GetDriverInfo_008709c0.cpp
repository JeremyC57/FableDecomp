// CThingComponentSet::GetDriverInfo() const -- the first component entry that is a driver (the set's driver must exist)
// Leaf: ?GetDriverInfo@CThingComponentSet@@QBEABVCEntry@1@XZ
// Retail 0x008709C0, 0x28 bytes (+ int3 padding to 0x30), thiscall `ret`. FableWin/Ego_r name:
// ?GetDriverInfo@CThingComponentSet@@QBEABVCEntry@1@XZ. No direct caller (reached through a vtable/def table).
// Callee: std::find_if<const CEntry*, CPred_EntryIsDriver>(first, last, pred, const random_access_iterator_tag&)
// @ 0x00870530 (/Gr: first in ecx, last in edx, the empty predicate by value and the tag reference on the stack, `ret 8`;
// the 4-way unrolled random-access find_if with CPred_EntryIsDriver::operator() inlined). HasDriver (0x00870980) is the
// same call followed by `!= end`.
// Layout (Ego_r): CThingComponentSet {CSmallVector<CEntry, 8> Entries +0}; CSmallVector {CEntry* Start +0, signed char
// Size +4}; CEntry is 9 bytes (packed).
// Shape: `return *find_if(begin, begin + count, CPred_EntryIsDriver());` -- the 3-argument STLport find_if inline, its
// random_access_iterator_tag temporary in a frame byte (user constructor: no store), the predicate value-initialised
// (`mov byte ptr [esp+8],0`, then pushed as a dword).
// Manifest label "CWorldDriver_FindClosestMeshVolume_DefaultCall_008709c0" is a descriptive auto label.
class CDefString
{
public:
    long TablePos;                                   // +0x00
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
    const random_access_iterator_tag&);                                                         // 0x00870530

template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return find_if(__first, __last, __pred, random_access_iterator_tag());
}
}

template <class T, int N> class CSmallVector
{
public:
    T*          Start;                               // +0x00
    signed char Size;                                // +0x04

    const T* GetAsCArray() const { return Start; }
};

class CThingComponentSet
{
public:
#pragma pack(push, 1)
    class CEntry                                     // 9 bytes
    {
    public:
        CDefString    Name;                          // +0x00
        unsigned long DriverType;                    // +0x04
        bool          Override;                      // +0x08
    };
#pragma pack(pop)

    CSmallVector<CEntry, 8> Entries;                 // +0x00

    long GetNoComponents() const { return Entries.Size; }
    const CEntry& GetDriverInfo() const;
};

class CPred_EntryIsDriver
{
public:
    bool operator()(const CThingComponentSet::CEntry& entry) const;
};

const CThingComponentSet::CEntry& CThingComponentSet::GetDriverInfo() const
{
    const CEntry* begin = Entries.GetAsCArray();
    return *std::find_if(begin, begin + GetNoComponents(), CPred_EntryIsDriver());
}