// CTCHeroStats::GetRandomHairName() const -- picks one of four hair mesh names (one of them empty) with the thing's random seed
// Leaf: GetRandomHairName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057BA5C, 120 bytes; frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), no EH frame.
// Callees: std::vector<CCharString>::vector(size_type) @ 0x0057FCDD (one pushed argument), CCharString::operator=
//          (const char*) @ 0x0099EFE0 (x4), CTCBase::GetPRandomSeed() @ 0x00686DA0, GFRandom(ulong range, ulong* seed)
//          @ 0x004CDAE0 (__fastcall), CCharString::CCharString(const CCharString&) @ 0x0099EC30 into the hidden
//          result, std::vector<CCharString>::~vector() @ 0x00414EA0.
// Shape: the vector's first pointer is read once into esi (a named `CCharString* names = nameList.begin()` local;
// indexing the vector itself re-reads [ebp-0Ch] after every operator=); the result is
// `names[GFRandom(5, seed)]` copied straight into the return slot; the fourth name is the shared empty literal.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    CCharString& operator=(const char* text);        // 0x0099EFE0
};

struct CCharStringVector {                           // std::vector<CCharString> {first, last, end}
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    explicit CCharStringVector(unsigned long count); // 0x0057FCDD
    ~CCharStringVector();                            // 0x00414EA0
    CCharString* begin() { return _M_start; }
    CCharString& operator[](unsigned long n) { return *(begin() + n); }
};

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);  // 0x004CDAE0

struct CTCHeroStats {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04

    unsigned long* GetPRandomSeed() const;           // 0x00686DA0 (CTCBase)
    CCharString GetRandomHairName() const;
};

CCharString CTCHeroStats::GetRandomHairName() const
{
    CCharStringVector nameList(4);
    CCharString* names = nameList.begin();
    names[0] = "OBJECT_HERO_HAIR";
    names[1] = "OBJECT_HERO_HAIR_SHORT_01";
    names[2] = "OBJECT_HERO_HAIR_PONYTAIL_01";
    names[3] = "";
    return names[GFRandom(4, GetPRandomSeed())];
}