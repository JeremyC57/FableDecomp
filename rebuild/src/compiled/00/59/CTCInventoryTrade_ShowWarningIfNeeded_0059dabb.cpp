// CTCInventoryTrade::ShowWarningIfNeeded() -- after a trade: hands a pending trophy use to the hero's inventory component and clears it, or, when flagged, shows the "weapon held" shop warning as a subtitle
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059DABB, length 199 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x0040F020 (the inline GetInterface, type 0x11 = CTCInventory),
// CTCInventory virtual slot 33 (+0x84, the trophy use taking the def index and an in/out flag), CCharString(const char*, long)
// @ 0x0099EBF0 ("TEXT_GUI_SHOP_WEAPON_HELD_WARNING" 0x01252FF4) and ~CCharString @ 0x0099EAE0, CThing::GetDisplayEngine()
// @ 0x004C7A70 (the font bank at +0x1614C), CSubtitleRenderer::SetText(CFontBank*, const CCharString&, unsigned long, float, bool, bool)
// @ 0x00492ED1 on the renderer embedded at (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) (0x013B8790) +0x18.
// Layout: retail CTCInventoryTrade ParentThing +0x04, WarningNeeded +0x178, TrophyUseNeeded +0x17C (PDB +0x1A4/+0x1A8).
// Shape: the flag local is the byte [ebp-1]; the lookup key is an immediate in [ebp-8] (no spare register), which the warning
// name then reuses; WarningNeeded is addressed once (`lea`) for its test and clear; the display engine is a named local so its call
// precedes the argument pushes; the colour 0x122 and time 0.0f (`fldz`) are immediates on the argument stack.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CFontBank_Retail;
struct CDisplayEngine_Retail {
    unsigned char     _pad00[0x1614c];
    CFontBank_Retail* m_pFontBank;                   // +0x1614c
};
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            CInterfaceMap::value_type* found = (it == map->end() || wanted < it->first) ? map->end() : it;
            *out = (T*)((void*)found->second);
            return true;
        }
        return false;
    }
    CDisplayEngine_Retail* GetDisplayEngine();       // 0x004C7A70
};
class CTCInventory { public:                                // interface type 0x11
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual void UseTrophy(long trophyDefIndex, bool* fromShop);   // slot 33 (+0x84)
};
struct CSubtitleRenderer_Retail {                    // embedded at (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get())+0x18
    void SetText(CFontBank_Retail* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char           _pad00[0x18];
    CSubtitleRenderer_Retail m_subtitles;            // +0x18
};

namespace NInventory {
struct CTCInventoryTrade {
    unsigned char _pad00[0x04];
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[0x178 - 0x08];
    bool          WarningNeeded;                     // +0x178
    long          TrophyUseNeeded;                   // +0x17c
    void ShowWarningIfNeeded();
};
void CTCInventoryTrade::ShowWarningIfNeeded()
{
    if (TrophyUseNeeded != -1) {
        bool fromShop = true;
        CTCInventory* inventory;
        if (ParentThing->GetInterface(&inventory, 0x11))
            inventory->UseTrophy(TrophyUseNeeded, &fromShop);
        TrophyUseNeeded = -1;
        WarningNeeded = false;
    } else if (WarningNeeded) {
        WarningNeeded = false;
        CCharString warning("TEXT_GUI_SHOP_WEAPON_HELD_WARNING");
        CDisplayEngine_Retail* engine = ParentThing->GetDisplayEngine();
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().m_subtitles.SetText(engine->m_pFontBank, warning, 0x122, 0.0f, true, true);
    }
}
}