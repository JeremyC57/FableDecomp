// CTCFollowed::AddEmoteIcon(CThing&) -- show the follower emote icon (from the global defs) over a follower that has an emote-icon component, when the parent wants follower icons and the follower is not already showing it
// Leaf: AddEmoteIcon
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
//
// Retail 0x006D4FD0, 117 bytes (`ret 4`). Ego_r.pdb `?AddEmoteIcon@CTCFollowed@@IAEXAAVCThing@@@Z`. Callees:
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of the emote-icon
// component, interface 0x45 = bit 5 of +0x28), CTCBase::GetDefinitionManager() @ 0x006869D0 (the global
// definitions holder; manifest "_Cons_val<...>", propagated), CTCEmoteIcon::SetEmoteIcon(EEmotePriority,
// long, float, float, unsigned char) @ 0x0071AAB0.
// Layout: CTCEmoteIcon EmoteIcon +0x14 (Ego_r.pdb, no shift); the definitions holder's +0xd0 def and its
// +0x12c follower icon index; a parent-thing flag byte at +0x6c (bit 0x20: follower icons wanted).
// Shape: the component is a named local taken right after the lookup (esi), the masked presence word
// tests as a byte; SetEmoteIcon(priority 1, icon, -1.0f, 0.5f, 0x80) reuses the compared index in eax.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad54[0x18];
    unsigned char Flags6c;                                  // +0x6c (0x20: show follower emote icons)
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
};

class CTCEmoteIcon { public:
    enum EEmotePriority { EMOTE_PRIORITY_DEBUG_HIGH=0,EMOTE_PRIORITY_FOLLOWING=1,EMOTE_PRIORITY_TROPHY_MINIGAME=2,EMOTE_PRIORITY_WIFE=3,EMOTE_PRIORITY_ATTRACTION=4,EMOTE_PRIORITY_EXPRESSION=5,EMOTE_PRIORITY_BRIBE=6,EMOTE_PRIORITY_DEBUG_LOW=7,EMOTE_PRIORITY_NONE=8 };   // nested (Ego_r ?SetEmoteIcon@CTCEmoteIcon@@QAEXW4EEmotePriority@1@JMME@Z)
    unsigned char _pad00[0x14];
    long          EmoteIcon;                                // +0x14
    void SetEmoteIcon(EEmotePriority priority, long icon, float activeTime, float scale, unsigned char alpha);   // 0x0071AAB0
};

struct CGlobalGameDef {
    unsigned char _pad000[0x12c];
    long          FollowerEmoteIcon;                        // +0x12c
};

class CGameDefinitionManager { public:
    unsigned char   _pad00[0xd0];
    CGlobalGameDef* PGlobalDef;                             // +0xd0
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCFollowed : public CTCBase { public:
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void AddEmoteIcon(CThing& follower);
};

void CTCFollowed::AddEmoteIcon(CThing& follower)
{
    if (follower.HasInterface(0x45))
    {
        CTCEmoteIcon* icon = (CTCEmoteIcon*)follower.GetInterface(0x45);
        if (ParentThing->Flags6c & 0x20)
        {
            long followerIcon = GetDefinitionManager().PGlobalDef->FollowerEmoteIcon;
            if (icon->EmoteIcon != followerIcon)
                icon->SetEmoteIcon(CTCEmoteIcon::EMOTE_PRIORITY_FOLLOWING, followerIcon, -1.0f, 0.5f, 0x80);
        }
    }
}