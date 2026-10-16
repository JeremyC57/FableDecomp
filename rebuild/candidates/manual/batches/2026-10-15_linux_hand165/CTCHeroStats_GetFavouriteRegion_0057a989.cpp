// CTCHeroStats::GetFavouriteRegion(long&) const -- the display text of the region the hero spent most time in (empty text and -1 when none)
// Leaf: GetFavouriteRegion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A989, 146 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 8` (hidden result + one argument).
// Callees: CTCBase::GetWorldMap() @ 0x00686D20, CWorldMap::GetRegion(long) @ 0x004FC180, CRegion::GetDisplayName()
//          @ 0x006BBF20 (returns a CCharString reference), CDataBank::GetTextBySymbol(const CCharString&) @ 0x009C95E0
//          (by value through the hidden result pointer), CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Globals: the engine globals @ 0x013B86A0 (+0x14 the text bank), the empty CWideString @ 0x013BCA24.
// Shape: `for (unsigned i = 0; i != RegionTime.size(); ++i)` over the 4-byte times (`test eax,0FFFFFFFCh ; je`, the
// element walked by a strength-reduced pointer) keeps the largest time (ebx, starts -1) and its index (edi, -1);
// the text bank is a named local read before the `best != -1` test (its load sits above the `je`). The found
// path does not write the out index; the not-found path stores -1 and copies the empty wide string.
// Layout: retail RegionTime vector {first, last} at +0x124 (PDB +0x130).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CWideStringData;

struct CCharString {
    CCharStringData* PStringData;
};

struct CWideString {
    CWideStringData* PStringData;
    CWideString(const CWideString& other);           // 0x0099B720
};

extern const CWideString g_EmptyWideString;          // 0x013BCA24

struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);  // 0x009C95E0
};

struct CEngineGlobals {
    unsigned char _pad_0x00[0x14];
    CDataBank*    TextBank;                          // +0x14
};

extern CEngineGlobals* g_pEngineGlobals;             // 0x013B86A0

struct CRegion {
    const CCharString& GetDisplayName() const;       // 0x006BBF20
};

struct CWorldMap {
    CRegion& GetRegion(long index);                  // 0x004FC180
};

struct CLongVector {                                 // std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    long* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    const long& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CTCHeroStats {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x11C];
    CLongVector   RegionTime;                        // +0x124 (PDB +0x130)

    CWorldMap* GetWorldMap() const;                  // 0x00686D20 (CTCBase)
    CWideString GetFavouriteRegion(long& region) const;
};

CWideString CTCHeroStats::GetFavouriteRegion(long& region) const
{
    long bestTime = -1;
    long best = -1;
    for (unsigned long i = 0; i != RegionTime.size(); ++i) {
        if (RegionTime[i] > bestTime) {
            bestTime = RegionTime[i];
            best = i;
        }
    }
    CDataBank* textBank = g_pEngineGlobals->TextBank;
    if (best != -1)
        return textBank->GetTextBySymbol(GetWorldMap()->GetRegion(best).GetDisplayName());
    region = -1;
    return g_EmptyWideString;
}
