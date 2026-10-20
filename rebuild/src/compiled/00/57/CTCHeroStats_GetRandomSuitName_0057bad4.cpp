// CTCHeroStats::GetRandomSuitName() const -- picks one of the nine hero suit names with the thing's random seed
// Leaf: GetRandomSuitName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057BAD4, 185 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), no EH frame. Twin of the landed GetRandomHairName
// (0x0057BA5C) and GetRandomBeardName (0x0057B9D7) with a nine-entry table.
// Callees: std::vector<CCharString>::vector(size_type) @ 0x0057FCDD, CCharString::operator=(const char*)
//          @ 0x0099EFE0 (x9), CTCBase::GetPRandomSeed() @ 0x00686DA0, GFRandom(ulong range, ulong* seed)
//          @ 0x004CDAE0 (__fastcall), CCharString::CCharString(const CCharString&) @ 0x0099EC30 into the hidden
//          result, std::vector<CCharString>::~vector() @ 0x00414EA0.
// Shape: the vector's first pointer is read once into esi (a named `CCharString* names = nameList.begin()` local);
// the result is `names[GFRandom(9, seed)]` copied straight into the return slot.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    CCharString& operator=(const char* text);        // 0x0099EFE0
};

struct CCharStringVector {                           // std::vector<CCharString> {first, last, end} (12 bytes in retail)
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    explicit CCharStringVector(unsigned long count); // 0x0057FCDD
    ~CCharStringVector();                            // 0x00414EA0
    CCharString* begin() { return _M_start; }
};

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);  // 0x004CDAE0

struct CTCHeroStats {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04

    unsigned long* GetPRandomSeed() const;           // 0x00686DA0 (CTCBase)
    CCharString GetRandomSuitName() const;
};

CCharString CTCHeroStats::GetRandomSuitName() const
{
    CCharStringVector nameList(9);
    CCharString* names = nameList.begin();
    names[0] = "HERO_SUIT_NAKED";
    names[1] = "HERO_SUIT_OAK_VALE";
    names[2] = "HERO_SUIT_BANDIT";
    names[3] = "HERO_SUIT_STEALTH";
    names[4] = "HERO_SUIT_ARMOUR";
    names[5] = "HERO_SUIT_BRIGHT_ARMOUR";
    names[6] = "HERO_SUIT_DARK_ARMOUR";
    names[7] = "HERO_SUIT_WILL_ROBE";
    names[8] = "HERO_SUIT_LEATHER_ARMOUR";
    return names[GFRandom(9, GetPRandomSeed())];
}