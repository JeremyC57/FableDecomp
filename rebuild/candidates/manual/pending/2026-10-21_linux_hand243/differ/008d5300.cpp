// CCreatureAction_ReloadMissileWeapon::PerformAction() -- when the creature carries a weapon with a missile component whose weapon def names an ammunition def and a carry slot, create a fresh ammunition thing at the creature's position, keep it in PAmmo, and (if it came alive) replace whatever sits in that carry slot with it (the old occupant is removed and killed)
// Grade: DIFFER(379v383) -- frame and call sequence identical; VC hoists &PCreature into ebp at entry (push ebp at the top) where retail spills `this` and re-forms this+8 for each GetPItem with ebp/edi pushed inside the branch; carrying in edi instead of ebx (register-role rotation)
// Leaf: PerformAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D5300, length 383 bytes (+ int3 padding), `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCCarrying::
// GetThingInPrimarySlot @ 0x006A1440, CDefClassBase::GetSubDef<CWeaponDef> @ 0x0042B1A6 (out CDefPointer, bool), CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing @ 0x004C73D0 (a position query taking the player index, two zero words and the empty name string; the
// manifest "GetPosition" label is the no-argument overload), CThing::Create @ 0x00703210 (`__fastcall` def index, position), CIntelligentPointer
// <const CThing>::CIntelligentPointer(CThing*) @ 0x0065E133, operator= @ 0x007B82C0, ~CBaseIntelligentPointer @ 0x00A01C10, IS_THING_ALIVE
// (const CIntelligentPointer&) @ 0x0078B2A0 and (CThing* const&) @ 0x0040DCE0 (`__fastcall`), CTCCarrying::GetThingInCarrySlot(long) @ 0x006A11F0,
// ::RemoveThing @ 0x006A19B0, CThing::Kill(bool) @ 0x004C9B80, CTCCarrying::PutThingInCarrySlot(CThing*, long, bool) @ 0x006A1F80, the def's
// virtual slot 1 on the last reference. 0x0122D70E is the empty string.
// Layout: retail CCreatureActionBase = 0xa8 bytes (PCreature +8); PAmmo +0xa8 (CIntelligentPointer<const CThing>); CThing presence bits +0x20
// (CTCCarrying = interface 0x46, the missile interface = 0x48), interface map +0x44, def +0x70, player index +0x90; CWeaponDef AmmoCarrySlot
// +0x94, AmmoDefIndex +0xa4.
// Shape: `this` spills to the frame (ebp/edi hold the def words); the carrying component is the ternary GetTC in ebx; the empty name string is
// a scoped local passed to the position query; the weapon def releases with the short `dec ; jne ; call` form.
struct CThing;
struct CTCBase;
struct C3DVector { float x, y, z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CWeaponDef {
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x94 - 0x08];
    long AmmoCarrySlot;                               // +0x94
    unsigned char _pad98[0xa4 - 0x98];
    long AmmoDefIndex;                                // +0xa4
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CWeaponDef>* out);    // 0x0042B1A6
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    const C3DVector* GetNamedPosition(long player, long a, long b, const CCharString& name);   // 0x004C73D0
    void Kill(bool immediately);                      // 0x004C9B80
    static CThing* __fastcall Create(long defIndex, const C3DVector* position);   // 0x00703210
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +0x04
    CThing* GetPItem() const;                         // 0x00A01B50
    ~CBaseIntelligentPointer();                       // 0x00A01C10
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(CThing* thing);    // 0x0065E133
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other);   // 0x007B82C0
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerConstThing& p);   // 0x0078B2A0
bool __fastcall IS_THING_ALIVE(CThing* const& thing);                    // 0x0040DCE0
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
    CThing* GetThingInCarrySlot(long slot);           // 0x006A11F0
    void RemoveThing(CThing* thing);                  // 0x006A19B0
    void PutThingInCarrySlot(CThing* thing, long slot, bool flag);   // 0x006A1F80
};
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};
struct CCreatureAction_ReloadMissileWeapon : CCreatureActionBase {
    CIntelligentPointerConstThing PAmmo;              // +0xa8
    void PerformAction();
};

void CCreatureAction_ReloadMissileWeapon::PerformAction()
{
    CThing* creature = PCreature.GetPItem();
    if (creature->HasTC(0x46)) {
        CTCCarrying* carrying = creature->GetTC<CTCCarrying>(0x46);
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon && weapon->HasTC(0x48)) {
            CDefPointer<CWeaponDef> weaponDef;
            if (weapon->m_pDef->GetSubDef(&weaponDef)) {
                long slot = weaponDef->AmmoCarrySlot;
                long ammoDef = weaponDef->AmmoDefIndex;
                if (ammoDef > 0 && slot > 0) {
                    {
                        CCharString name("", -1);
                        CThing* owner = PCreature.GetPItem();
                        CIntelligentPointerConstThing ammo(CThing::Create(ammoDef, PCreature.GetPItem()->GetNamedPosition(owner->m_player, 0, 0, name)));
                        PAmmo = ammo;
                    }
                    if (IS_THING_ALIVE(PAmmo)) {
                        CThing* existing = carrying->GetThingInCarrySlot(slot);
                        if (IS_THING_ALIVE(existing)) {
                            carrying->RemoveThing(existing);
                            existing->Kill(true);
                        }
                        carrying->PutThingInCarrySlot(PAmmo.GetPItem(), slot, true);
                    }
                }
            }
        }
    }
}
