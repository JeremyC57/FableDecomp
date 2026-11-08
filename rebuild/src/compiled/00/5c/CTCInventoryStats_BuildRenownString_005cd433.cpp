// CTCInventoryStats::BuildRenownString() -- the localised renown-class text of the hero: TEXT_GUI_STATS_RENOWNCLASS_LEVEL<n> looked up in the game-text data bank, n = the renown component's class level
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pEngineGlobals (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
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
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || wanted < it->first)
                it = map->end();
            *out = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE T* GetTC(long wanted)
    {
        T* component;
        GetInterface(&component, wanted);
        return component;
    }
};
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    ~CCharString();                                               // 0x0099EAE0
    const CCharString& operator+=(const CCharString& other);            // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);             // 0x0099F830
class CWideString { public:
    void* m_pStringData;
};
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;       // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x14];
    CDataBank*    m_pTextBank;                                    // +0x14
};

struct CTCHeroRenown {                                // interface type 4
    unsigned char _pad_0x00[0x70];
    long RenownClassLevel;                            // +0x70
};

class CTCInventoryStats { public:
    void*   __vftable;
    CThing* ParentThing;                              // +0x04
    CWideString BuildRenownString();
};

CWideString CTCInventoryStats::BuildRenownString()
{
    CTCHeroRenown* renown = ParentThing->GetTC<CTCHeroRenown>(4);
    CCharString symbol("TEXT_GUI_STATS_RENOWNCLASS_LEVEL");
    symbol += GFIntToCharString(renown->RenownClassLevel);
    return CMainGameComponent::Get()->m_pTextBank->GetTextBySymbol(symbol);
}