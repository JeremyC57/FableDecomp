// CThing::IsAffectedByBulletTime() -- whether bullet time slows this thing: in a pause mode script-controlled / flagged things, dead-type things, unpausable and fading things decide; carried things defer to their carrier; outside pause modes a hero-owned override and the main player's bullet-time immunity decide
// Grade: DIFFER(580v598) -- component pointer loaded into edx/ecx/edi where retail reuses eax (`mov eax,[eax+4]` then movzx/lea/mov ecx,eax) on the 0xf9/0x4b lookups; shared `return false` block placed before the fade block
// Leaf: ?IsAffectedByBulletTime@CThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C9920, length 608 bytes, `ret`.
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0, CTCCarryable::GetCarrier @ 0x00705170
//          (label "CGuiControlListBox::GetCurrentSelectedEntryData" is propagated), CThing::IsUnderScriptControl
//          @ 0x004C8C90, CTCFadeOutAndIn::IsFading @ 0x0049D310.
// Globals: g_pThingManager @ 0x013B8A1C (PlayerManager +0x30, NonScriptedEntitesPauseMode +0x54,
//          AllEntitesPauseMode +0x55).
// Layout: retail CThing interface bitset +0x20, CVectorMap +0x44, category byte +0x10, Dead +0x91 bit 0,
//         PausedDuringCutscenes +0x92 bit 7, NotPausedDuringCutscenes +0x93 bit 0, IsAffectedByBulletTimeOverride
//         +0x93 bit 5 (Ego_r.pdb minus 0x10). Interfaces: 0x26, 0x47 carryable, 0x4b (follower: leader pointer +0x1c,
//         flag byte +0x45), 0x64, 0x92, 0xf2, 0xf9 fade.
// Shape: the carrier / leader deferrals are tail calls of this same function, which VC7.1 turns into the loop back to
//        the top.
struct CTCBase;
struct CThing;
struct CBaseIntelligentPointer {
    void* _vf;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CPlayer {
    unsigned char _pad00[0x34];
    CBaseIntelligentPointer CharacterThing;             // +0x34
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                     // 0x00449960
};
struct CThingManager {
    unsigned char _pad00[0x30];
    CPlayerManager* PlayerManager;                      // +0x30
    unsigned char _pad34[0x54 - 0x34];
    bool NonScriptedEntitesPauseMode;                   // +0x54
    bool AllEntitesPauseMode;                           // +0x55
};
extern CThingManager* g_pThingManager;                  // 0x013B8A1C

struct CTCCarryable {
    CThing* GetCarrier() const;                         // 0x00705170
};
struct CTCFollower {
    unsigned char _pad00[0x1c];
    CBaseIntelligentPointer Leader;                     // +0x1c
    unsigned char _pad24[0x45 - 0x24];
    unsigned char _bit45 : 1;                           // +0x45
    unsigned char IgnoresBulletTime : 1;                //       bit 1
};
struct CTCFadeOutAndIn {
    bool IsFading() const;                              // 0x0049D310
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    void* _vf;
    unsigned char _pad04[0x10 - 0x04];
    unsigned char Category;                             // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _bits91 : 7;
    unsigned char _bits92 : 7;                          // +0x92
    unsigned char PausedDuringCutscenes : 1;            //       bit 7
    unsigned char NotPausedDuringCutscenes : 1;         // +0x93 bit 0
    unsigned char _bits93 : 4;
    unsigned char IsAffectedByBulletTimeOverride : 1;   //       bit 5
    bool IsUnderScriptControl();                        // 0x004C8C90
    bool IsAffectedByBulletTime();
    __forceinline bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    template <class T> __forceinline void GetInterface(long type, T** out) const
    {
        long key = type;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        *out = (T*)(it == end || type < it->type ? end : it)->component;
    }
};

bool CThing::IsAffectedByBulletTime()
{
    if (g_pThingManager->NonScriptedEntitesPauseMode || g_pThingManager->AllEntitesPauseMode) {
        if (g_pThingManager->NonScriptedEntitesPauseMode) {
            if (NotPausedDuringCutscenes && !IsUnderScriptControl())
                return false;
            if (PausedDuringCutscenes && IsUnderScriptControl())
                return true;
        }
        if (Category == 2)
            return false;
        if (HasTC(0x26))
            return false;
        if (HasTC(0xf2))
            return false;
        if (HasTC(0x64))
            return false;
        if (HasTC(0xf9)) {
            CTCFadeOutAndIn* fade;
            GetInterface(0xf9, &fade);
            return !fade->IsFading();
        }
        if (HasTC(0x47)) {
            CThing* carrier = ((CTCCarryable*)GetTC(0x47))->GetCarrier();
            if (carrier)
                return carrier->IsAffectedByBulletTime();
        }
        if (HasTC(0x4b)) {
            CTCFollower* follower;
            GetInterface(0x4b, &follower);
            return !follower->IgnoresBulletTime;
        }
        if (g_pThingManager->NonScriptedEntitesPauseMode)
            return !IsUnderScriptControl();
        return true;
    }
    if (HasTC(0x64))
        return false;
    CPlayer* mainPlayer = g_pThingManager->PlayerManager->GetMainPlayer();
    CThing* player = mainPlayer->CharacterThing.GetPItem();
    if (player && !player->Dead && !IsAffectedByBulletTimeOverride && player->HasTC(0x92))
        return false;
    if (Category == 2)
        return false;
    if (HasTC(0x47)) {
        CThing* carrier = ((CTCCarryable*)GetTC(0x47))->GetCarrier();
        if (carrier)
            return carrier->IsAffectedByBulletTime();
    }
    if (HasTC(0x4b)) {
        CTCFollower* follower;
        GetInterface(0x4b, &follower);
        const CBaseIntelligentPointer& leader = follower->Leader;
        if (leader.GetPItem() && leader.GetPItem() != this)
            return leader.GetPItem()->IsAffectedByBulletTime();
    }
    return true;
}
