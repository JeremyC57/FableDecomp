// CGameScriptInterface::GetHeroTargetedThing() const -- the hero creature's current target as a script thing, or a null script thing
// Leaf: ?GetHeroTargetedThing@CGameScriptInterface@@UBE?AVCScriptThing@@XZ
// Retail: Fable.exe @ 0x0089A490, 124 bytes, `ret 4` (hidden result); Ego_r.pdb same symbol.
// Body: hero = PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPPlayerCharacter() (0x00487DC0, `add ecx,2Ch; jmp` = CPlayer::ControlledCreature +0x2c); if hero: PeekTC of
// interface 8 (const LowerBound 0x004365B0, key in the frame) with its bool result IGNORED -- when the hero lacks the
// interface the component pointer is uninitialised, and VC7.1 reuses the hidden-result load for it (`mov ecx,[esp+0xc]`);
// target = component->slot 33 (+0x84); if target, return target->GetScriptThing() (0x004C7E50, labelled GFPredicateAnd by
// folding, as the landed 0x004AFA60 / 0x006E7480 rows model it); otherwise return CGameScriptThing::CreateNullThing() (0x008CFF40, the null
// script thing; the manifest's operator&& label is propagated).
// Layout: Ego_r.pdb CGameScriptInterface PlayerManager +0x14; CThing as the matched 0x00899E90 row. The interface-8 class
// is CTCTargetingBase (landed 0x006E5690 OnCleanup; vtable 0x01271E74); the slot-33 name is not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form, with the end-or-mismatch choice written as one ternary
// whose ->component is stored (`*out = ((it == last || 8 < it->type) ? last : it)->component`), which loads the component
// straight into ecx as retail does (the if/assign form carries it in eax, 126 bytes). Same fix as 0x00899B10.
class CScriptThing;
struct CTCBase;
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
    CScriptThing GetScriptThing();                       // 0x004C7E50 (by value through the hidden result pointer)
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
        *out = (const T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
        return true;
    }
};
class CScriptThing {
public:
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing();
    void* Data;
};
class CGameScriptThing : public CScriptThing {      // Ego_r.pdb: CGameScriptThing derives from CScriptThing
public:
    static CScriptThing __fastcall CreateNullThing(); // 0x008CFF40 (?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ)
};
class CTCTargetingBase {                                // interface 8 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 8 };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTarget() const;                   // slot 33 (+0x84, name not confirmed)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CGameScriptInterface {
public:
    virtual CScriptThing GetHeroTargetedThing() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};
CScriptThing CGameScriptInterface::GetHeroTargetedThing() const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    if (hero)
    {
        const CTCTargetingBase* tc;
        hero->PeekTC(&tc);
        CThing* target = tc->GetTarget();
        if (target)
            return target->GetScriptThing();
    }
    return CGameScriptThing::CreateNullThing();
}