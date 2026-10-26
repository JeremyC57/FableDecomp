// CTCFollowed::RemoveEmoteIcon(CThing& follower) -- clear the follower emote icon over a follower whose emote-icon component still shows the global follower icon
// Leaf: ?RemoveEmoteIcon@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D5050, length 108 bytes (+ int3 padding), thiscall `ret 4`.
// Ego_r.pdb `?RemoveEmoteIcon@CTCFollowed@@IAEXAAVCThing@@@Z`; callers 0x006D5242 (RemoveFollower 0x006D5180),
// 0x006D556E (FrameUpdate 0x006D53C0), 0x006D57F9 (QuitAIFollower 0x006D56E0), all landed CTCFollowed members.
// The manifest label "GUI_SetEmoteIcon" is descriptive. The twin of the landed AddEmoteIcon 0x006D4FD0 (model reused).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined lookup of the emote-icon
// component, interface 0x45 = bit 5 of +0x28), CTCBase::GetDefinitions() @ 0x006869D0, CTCEmoteIcon::SetEmoteIcon(
// EEmotePriority, long, float, float, unsigned char) @ 0x0071AAB0.
// Layout: CTCEmoteIcon EmoteIcon +0x14; the definitions holder's +0xd0 global def and its +0x12c follower icon index.
// Shape: the component is a named local taken right after the lookup (esi); its current icon is a named local read into edi
// BEFORE the GetDefinitions call (written inline in the `==` VC loads it after the call); SetEmoteIcon(priority 1, icon 0, 0.0f, 1.0f, 0xFF) clears it.
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

enum EEmotePriority { EMOTE_PRIORITY_FOLLOWER = 1 };

class CTCEmoteIcon { public:
    unsigned char _pad00[0x14];
    long          EmoteIcon;                                // +0x14
    void SetEmoteIcon(EEmotePriority priority, long icon, float activeTime, float scale, unsigned char alpha);   // 0x0071AAB0
};

struct CGlobalGameDef {
    unsigned char _pad000[0x12c];
    long          FollowerEmoteIcon;                        // +0x12c
};

struct CDefinitions {
    unsigned char   _pad00[0xd0];
    CGlobalGameDef* PGlobalDef;                             // +0xd0
};

class CTCFollowed { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    CDefinitions* GetDefinitions();                         // 0x006869D0 (CTCBase)
    void RemoveEmoteIcon(CThing& follower);
};

void CTCFollowed::RemoveEmoteIcon(CThing& follower)
{
    if (follower.HasInterface(0x45))
    {
        CTCEmoteIcon* icon = (CTCEmoteIcon*)follower.GetInterface(0x45);
        long shown = icon->EmoteIcon;
        if (shown == GetDefinitions()->PGlobalDef->FollowerEmoteIcon)
            icon->SetEmoteIcon(EMOTE_PRIORITY_FOLLOWER, 0, 0.0f, 1.0f, 0xff);
    }
}