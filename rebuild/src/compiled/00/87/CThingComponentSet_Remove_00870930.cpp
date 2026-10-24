// CThingComponentSet::Remove(const CCharString&) -- erase every component entry whose name equals the given name
// Leaf: ?Remove@CThingComponentSet@@QAEXABVCCharString@@@Z
// Retail 0x00870930, 0x48 bytes (+ int3 padding to 0x50), thiscall `ret 4`. FableWin/Ego_r name:
// ?Remove@CThingComponentSet@@QAEXABVCCharString@@@Z. No direct caller (reached through the def/editor tables).
// Callees: CDefString::operator==(const CCharString&) const @ 0x004B7980, CSmallVector<CThingComponentSet::CEntry, 8>::
// erase(CEntry*) @ 0x008709F0 (catalog name; returns the entry now at the erased position).
// Layout (Ego_r): CThingComponentSet {CSmallVector<CEntry, 8> Entries +0}; CSmallVector {CEntry* Start +0, signed char
// Size +4}; CEntry (9 bytes, packed) {CDefString Name +0, unsigned long DriverType +4, bool Override +8}.
// Shape: `for (it = begin; it != end; ) if (it->Name == name) it = Entries.erase(it); else ++it;` with end recomputed
// every pass as Start + Size (`movsx` of the signed-char size, `* 9`).
// Manifest label "CSmallVector_EraseByName" is a descriptive auto label.
class CCharString;

class CDefString
{
public:
    long TablePos;                                   // +0x00
    bool operator==(const CCharString& name) const;  // 0x004B7980
};

template <class T, int N> class CSmallVector
{
public:
    T*          Start;                               // +0x00
    signed char Size;                                // +0x04

    T* GetAsCArray() { return Start; }
    T* erase(T* position);                           // 0x008709F0
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
    void Remove(const CCharString& name);
};

void CThingComponentSet::Remove(const CCharString& name)
{
    for (CEntry* it = Entries.GetAsCArray(); it != Entries.GetAsCArray() + GetNoComponents(); )
    {
        if (it->Name == name)
            it = Entries.erase(it);
        else
            ++it;
    }
}