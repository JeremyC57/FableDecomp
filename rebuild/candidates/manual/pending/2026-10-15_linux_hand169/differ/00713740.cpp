// CTCScriptedControl::OnRemove() (vtable slot 13) -- tell the thing's interface-7 component its state group is gone, flag the CTCAnimationComplex animation (interface 0x5A) as changed, then the empty CTCBase slot-13 base
// Leaf: vfunc_13
// Grade: DIFFER(144v135) -- only the flag store: retail `or byte ptr [eax+94h],1`, ours `mov cl,[eax+94h] ; add eax,94h ; or cl,1 ; mov [eax],cl` (unsigned char |=, char/long bitfields and optimize("s") tried); next: an inline CAnimateThing member setter or a non-reference GetAnimation (pointer) return
// Retail: Fable.exe 0x00713740, 135 bytes (plain `ret`). Vtable 0x012658E4 (CTCScriptedControl) slot 13 (manifest
// "CTCScriptedControl::vfunc_13"; the slot's CTCBase body @ 0x006868E0 is a bare `ret`, manifest "$E2" is wrong).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (x2, the inline GetInterface of the landed
// CAIStateGroupBase::CleanupMain 0x008FD050), the interface-7 component's OnStateGroupRemoved @ 0x006CB6B0 (manifest
// "CScriptThing::operator==" is propagated), CTCAnimationComplex::GetAnimation @ 0x0070B460 (returns CAnimateThing&;
// manifest "CWorld::DrawGetEnvironment" is propagated), CTCBase slot 13 @ 0x006868E0.
// Layout: CThing presence bitset +0x20 (type 7 = sign of byte +0x20, type 0x5A = bit 26 of +0x28), interface map
// +0x44; CAnimateThing flag byte +0x94 bit 0.
// Shape: each lookup re-reads ParentThing; the type key lives in the `push ecx` frame slot; the map pointer is a
// named local so the end is re-read through it.
struct CTCBaseAny;
struct CInterfaceMap {                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBaseAny* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CAnimateThing {
    unsigned char _pad00[0x94];
    unsigned char m_changed : 1;            // +0x94 bit 0: animation changed
    unsigned char m_flags94_1 : 7;
};
struct CTCBaseAny {
    void OnStateGroupRemoved();             // 0x006CB6B0 (interface 7 component)
    CAnimateThing& GetAnimation();          // 0x0070B460 (CTCAnimationComplex, interface 0x5A)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20
    CInterfaceMap m_interfaces;             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual void vfunc_13();                // slot 13: 0x006868E0 (empty)
    CThing* ParentThing;                    // +0x04
    bool    ToBeRemoved;                    // +0x08
    bool    IsInGlobalUpdateTCs;            // +0x09
};
struct CTCScriptedControl : CTCBase {       // vtable 0x012658E4
    virtual void vfunc_13();
};

void CTCScriptedControl::vfunc_13()
{
    if (ParentThing->HasInterface(7)) {
        long type = 7;
        CInterfaceMap* map = &ParentThing->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 7 < it->type)
            it = map->last;
        it->component->OnStateGroupRemoved();
    }
    if (ParentThing->HasInterface(0x5a)) {
        long type = 0x5a;
        CInterfaceMap* map = &ParentThing->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || 0x5a < it->type)
            it = map->last;
        it->component->GetAnimation().m_changed = 1;
    }
    CTCBase::vfunc_13();
}
