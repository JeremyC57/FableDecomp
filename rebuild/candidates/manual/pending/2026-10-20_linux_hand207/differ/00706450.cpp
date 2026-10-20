// Grade: DIFFER(216v187) -- the loop-invariant zero for the two out slots is hoisted into ebx (so the entries vector spills to an 8-byte frame); retail re-zeroes eax each pass and keeps the entries pointer in ebx
// CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, EHeroAttachableAppearanceModifierType type) const -- whether any worn modifier of the type has an appearance def covering one of the body areas
// Leaf: IsWearingAppearanceModifierCoveringBodyArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706450, length 185 bytes (+ int3 padding). __thiscall, `ret 8`.
//         Ego_r.pdb: `?IsWearingAppearanceModifierCoveringBodyArea@CTCHeroAttachableAppearanceModifiers@@QBE_NJW4EHero
//         AttachableAppearanceModifierType@@@Z`. Label: the catalog's "CTCGold::GetGold" is propagated; the row sits in the
//         appearance-modifier TU (0x00706370 / 0x00706880 AddAppearanceModifier) and its caller (CTCHeroStats, 0x0057D462)
//         passes (def +0x40 flags, 2).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const
//          T>*) const @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const T>*) const @
//          0x0057FF92, the defs' virtual slot 1 (destroy on the last release).
// Shape: a pointer walk over AppearanceModifiers[type] (end re-read each pass); per entry two zeroed out slots (the thing
//        def in the dead `type` slot, the appearance def in the push-ecx word), each read once into a local released
//        by hand with `dec [reg+4] ; jne`; the hit path returns true after both releases (the appearance def's null test folded
//        away after the flag read), the miss path skips the never-filled appearance def.
// Layout: local model -- engine/CTCHeroAttachableAppearanceModifiers.h carries the 16-byte PDB containers; retail
//         AppearanceModifiers +0x30 (12-byte vector of 12-byte vectors of 8-byte CModifierEntry {def index, ...});
//         retail CAppearanceModifierDef::CoversBodyAreaFlags +0x40 (PDB +0x48).
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0057FF92 for CAppearanceModifierDef
};

class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x40 - 0x08];
    long CoversBodyAreaFlags;                       // +0x40
};
class CThingObjectDef : public CDefClassBase { };

template<class T> inline void ReleaseDef(const T* def)    // the inlined CDefPointer release on a def read once
{
    if (def && --def->RefCount == 0)
        def->Destroy();
}

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 for CThingObjectDef
};
class CDefinitionHolder : public CDefinitionManager { };

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    long Data;                                      // +0x04
};

class CThing;
class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x30 - 0x08];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0 (CTCBase)
    bool IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, long type) const;
};

bool CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, long type) const
{
    const CRetailVector<CModifierEntry>& entries = AppearanceModifiers._M_start[type];
    for (const CModifierEntry* it = entries._M_start; it != entries._M_finish; ++it) {
        CDefPointer<const CThingObjectDef> thingOut;
        CDefPointer<const CAppearanceModifierDef> appearanceOut;
        bool found = GetDefinitions()->GetDef(it->DefIndex, &thingOut);
        const CThingObjectDef* thingDef = thingOut.Object;
        if (found) {
            found = thingDef->GetSubDef(&appearanceOut);
            const CAppearanceModifierDef* appearanceDef = appearanceOut.Object;
            if (found && (appearanceDef->CoversBodyAreaFlags & bodyAreaFlags)) {
                ReleaseDef(appearanceDef);
                ReleaseDef(thingDef);
                return true;
            }
            ReleaseDef(appearanceDef);
        }
        ReleaseDef(thingDef);
    }
    return false;
}
