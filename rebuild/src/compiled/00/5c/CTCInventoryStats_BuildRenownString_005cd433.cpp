// CTCInventoryStats::BuildRenownString() -- the localised renown-class text of the hero: TEXT_GUI_STATS_RENOWNCLASS_LEVEL<n> looked up in the game-text data bank, n = the renown component's class level
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CD433, length 145 bytes (`ret 4`: hidden CWideString result). Frame-pointer size TU:
// optimize("y",off) + ("s",on). Ego_r.pdb: ?BuildRenownString@CTCInventoryStats@@QAE?AVCWideString@@XZ.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface for interface 4,
// the hero renown component), CCharString(const char*, long) @ 0x0099EBF0, GFIntToCharString(long) @ 0x0099F830 (`__fastcall`,
// PDB ?GFIntToCharString@@YI?AVCCharString@@J@Z; hidden result in ecx, value in edx), CCharString::operator+=(const CCharString&)
// @ 0x0099F0A0, ~CCharString @ 0x0099EAE0, CDataBank::GetTextBySymbol(const CCharString&) @ 0x009C95E0 (engine globals
// @ 0x013B86A0 +0x14). String: "TEXT_GUI_STATS_RENOWNCLASS_LEVEL" @ 0x01254C44.
// Shape: the component comes from an inline accessor returning an unset local on the miss path (retail reads the dead hidden-result
// slot [ebp+8]); the int-string temporary reuses the key's frame word [ebp-8].
// Layout: retail CTCBase ParentThing +0x04; the renown component's class level at +0x70 (retail offset).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);            // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);             // 0x0099F830
struct CWideString {
    void* m_pStringData;
};
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);       // 0x009C95E0
};
struct CEngineGlobals {
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                    // +0x14
};
extern CEngineGlobals* g_pEngineGlobals;                          // 0x013B86A0

struct CTCHeroRenown {                                // interface type 4
    unsigned char _pad_0x00[0x70];
    long RenownClassLevel;                            // +0x70
};

struct CTCInventoryStats {
    void*   __vftable;
    CThing* ParentThing;                              // +0x04
    CWideString BuildRenownString();
};

CWideString CTCInventoryStats::BuildRenownString()
{
    CTCHeroRenown* renown = ParentThing->GetTC<CTCHeroRenown>(4);
    CCharString symbol("TEXT_GUI_STATS_RENOWNCLASS_LEVEL");
    symbol += GFIntToCharString(renown->RenownClassLevel);
    return g_pEngineGlobals->m_pTextBank->GetTextBySymbol(symbol);
}