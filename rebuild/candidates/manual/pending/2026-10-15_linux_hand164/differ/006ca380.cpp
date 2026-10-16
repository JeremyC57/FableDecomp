// CTCHero::SetWeaponsAsUsable(bool usable) -- record whether the hero may use weapons; when they become unusable and the hero is carrying a weapon, take it out of the carrying component's primary slot
// Grade: DIFFER(105v115) -- retail tail-jumps into RemoveThingFromPrimarySlot on both arms (`pop esi ; mov [esp+4],0 ; jmp`) and re-reads ParentThing into esi; ours emits `push 0 ; call ; pop esi ; ret 4` with one merged tail (tried void callee, `return void-call`, key as the inline parameter)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA380, length 113 bytes (+15 int3 padding), `ret 4` (tail `jmp` with the
// argument slot rewritten to `false`).
// Ego_r.pdb: ?SetWeaponsAsUsable@CTCHero@@QAEX_N@Z
// Callees: the `__fastcall` IsThingCarryingWeapon(CThing*) @ 0x006A1540, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined interface lookup), CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00.
// Shape: the carrying component (interface 0x46, bit 6 of CThing +0x28) comes from an inline accessor returning an unset
// local when the bit is clear; the key and the unset local both live in the dead `usable` slot; the final call tail-jumps.
// Layout: Ego_r.pdb CTCHero: WeaponsUsable +0x0c; CTCBase ParentThing +0x04; CThing bitset +0x20, interface map +0x44.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            tc = (T*)it->component;
        }
        return tc;
    }
};
bool __fastcall IsThingCarryingWeapon(CThing* thing);   // 0x006A1540
class CTCCarrying {
public:
    void RemoveThingFromPrimarySlot(bool b);            // 0x006A1E00 (manifest says CThing*; the tail jump wants void)
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHero : public CTCBase {
public:
    bool WeaponsUsable;                         // +0x0c
    bool WillUsable;                            // +0x0d
    void SetWeaponsAsUsable(bool usable);
};

void CTCHero::SetWeaponsAsUsable(bool usable)
{
    WeaponsUsable = usable;
    if (!usable && IsThingCarryingWeapon(ParentThing))
        return ParentThing->GetTC<CTCCarrying>(0x46)->RemoveThingFromPrimarySlot(false);
}
