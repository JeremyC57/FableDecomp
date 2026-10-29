// CTCWife::SetAttractionIcon(bool show, float scale, unsigned char alpha) -- shows (or clears) the wife's attraction emote icon: the just-married icon while the marriage frame has passed, else the ring or no-ring courting icon; refreshes the player GUI when the icon's visibility changes
// Leaf: SetAttractionIcon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CFC90, 261 bytes (`ret 0Ch`; Ego_r.pdb `?SetAttractionIcon@CTCWife@@AAEX_NME@Z`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x45, CTCEmoteIcon: bit 5 of the bitset
//          word +0x28); GFGetCurrentFrame 0x0049D870 (`__fastcall`, no arguments); CTCBase::GetDefinitionManager() 0x006869D0
//          (x3); CTCEmoteIcon::SetEmoteIcon(EEmotePriority, long, float, float, unsigned char) 0x0071AAB0;
//          CPlayerGui::UpdateMiniMapMarkers(bool) 0x004378C0 on g_pPlayerGui 0x013B8790.
// Layout: engine/CTCWife.h holds up to the GiftsReceived map, then retail sits 4 bytes below the PDB (as the landed
//          IsWillingToMarry 0x006CFEC0): FrameGotMarriedToThePlayer +0x20, ReceivedWeddingRing +0x91 (PDB +0x95), so a
//          local model; CTCEmoteIcon::EmoteIcon +0x14 (retail: CWideString EmoteText below it is 4 bytes, PDB offset
//          +0x14 too); the global definitions' +0xd0 record holds the three wife icon ids at +0x11c/+0x120/+0x124.
// Shape: the emote component is fetched with the bare one-ternary GetTC after the presence test; the old visibility is a
//        named bool (setne bl); the three SetEmoteIcon calls cross-jump into one call site (each arm pushes its own
//        arguments); `this` is loaded into ecx before the frame branch since both arms fetch the definitions; the married
//        arm fetches the definitions into a named local first (the call precedes the argument loads); the frame test is
//        an inline bool helper, and the clear arm's priority is a named bool's `? 3 : 4` (setle/neg/sbb; the bare
//        expression folds to setg+3).
class CThing;
class CTCBase;
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};            // 0x0049D870

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->first ? map->last : it)->second;
    }
};

class CTCEmoteIcon { public:
    unsigned char _pad_0x00[0x14];
    long          EmoteIcon;                    // +0x14
    void SetEmoteIcon(long priority, long icon, float activeTime, float scale, unsigned char alpha);   // 0x0071AAB0
};
struct CWifeIconDefs {                          // the global definitions' +0xd0 record (descriptive)
    unsigned char _pad_0x00[0x11c];
    long          WifeAttractionIcon;           // +0x11c
    long          WifeAttractionIconWithRing;   // +0x120
    long          WifeAttractionIconMarried;    // +0x124
};
class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xd0];
    CWifeIconDefs* Defs;                        // +0xd0
};
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool flag);       // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCWife : public CTCBase { public:
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x18];
    long          FrameGotMarriedToThePlayer;   // +0x20
    unsigned char _pad_0x24[0x6d];
    bool          ReceivedWeddingRing;          // +0x91
    bool IsMarriageFrameReached() const { return FrameGotMarriedToThePlayer <= CWorld::GetFrame(); }   // inline helper (descriptive)
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);
};

void CTCWife::SetAttractionIcon(bool show, float scale, unsigned char alpha)
{
    CThing* parent = ParentThing;
    if (!parent->HasTC(0x45))
        return;
    CTCEmoteIcon* icon = (CTCEmoteIcon*)parent->GetTC(0x45);
    bool wasShowing = icon->EmoteIcon != 0;
    if (show) {
        if (IsMarriageFrameReached()) {
            const CGameDefinitionManager* definitions = (&GetDefinitionManager());
            icon->SetEmoteIcon(3, definitions->Defs->WifeAttractionIconMarried, -1.0f, scale, alpha);
        } else
            icon->SetEmoteIcon(4, ReceivedWeddingRing ? GetDefinitionManager().Defs->WifeAttractionIconWithRing
                                                      : GetDefinitionManager().Defs->WifeAttractionIcon,
                               -1.0f, scale, alpha);
    } else {
        bool married = IsMarriageFrameReached();
        icon->SetEmoteIcon(married ? 3 : 4, 0, -1.0f, 1.0f, 0xff);
    }
    if (show != wasShowing)
        g_pPlayerGui->UpdateMiniMapMarkers(false);
}