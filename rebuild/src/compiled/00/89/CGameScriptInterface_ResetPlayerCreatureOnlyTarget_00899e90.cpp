// CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const -- clear the hero creature's only-target (interface 8 component)
// Leaf: ?ResetPlayerCreatureOnlyTarget@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe @ 0x00899E90, 93 bytes, plain `ret`; Ego_r.pdb ?ResetPlayerCreatureOnlyTarget@CGameScriptInterface@@UBEXXZ.
// Body: hero = Component (+0x08)->PPlayerManager (+0x1c)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0);
// if hero, not dead (bit 0 of +0x91) and holding interface 8 (bit 8 of UsingComponents +0x20), the component from the
// Components map (LowerBound 0x0040F020, key in the `push ecx` slot) gets 0x008ABC90(NULL).
// Layout: CThing as the matched 0x0089AFF0 / 0x00897AA0 rows; CMainGameComponent PPlayerManager +0x1c (Ego_r.pdb, retail
// agrees below +0x24). The interface-8 component (modelled CTCTargetingBase at 0x0089D8B0) and the callee name
// (0x008ABC90, takes one pointer and wraps it in a counted handle) are not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 8 };
    void SetOnlyTarget(CThing* target);                  // 0x008ABC90 (name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c (scoped_ptr)
};
class CGameScriptInterface {
public:
    virtual void ResetPlayerCreatureOnlyTarget() const;
    void*               World;                           // +0x04
    CMainGameComponent* Component;                       // +0x08
};
void CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const
{
    CThing* hero = Component->PPlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCTargetingBase* tc;
        if (hero->GetTC(&tc))
            tc->SetOnlyTarget(0);
    }
}