// NThingShot::CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic() -- loads the shot's multi-arrow sub-def into a local def pointer and returns its weapon-trail graphic id
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: GetWeaponTrailGraphic
// Grade: DIFFER(49v42) -- the release: retail `mov ecx,[ebp-4] ; mov esi,[ecx+3Ch] ; dec dword ptr [ecx+4] ; jne`, the lane `mov eax,[ebp-4] ; mov esi,[eax+3Ch] ; mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0 ; jne` (the count's address is materialised). Tried: member `--Object->m_refCount`, local pointer copy, an inline Release() on the pointee, speed TU with frame pointer (load/dec/store). Next: drop the `const` sub-def type / `mutable` count (they may force the address form), or a non-template def pointer.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BD2A7, length 42 bytes. Vtable 0x0126DF34
// (NThingShot::CTCShotDefinitionMultiArrow) slot 32, referenced from 0x0126DFB4; Ego_r.pdb names the virtual
// `?GetWeaponTrailGraphic@CTCShotDefinitionMultiArrow@NThingShot@@UBEJXZ` (const, returns long). The catalog label
// "CTCTavernGame::GetBanter" is propagated over this body and rejected.
// Callee: CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) @ 0x007BE7EA (bool, result unused; manifest names a
// byte-identical instantiation "GetSubDef<CAbilityDef>"), called on the def object held at +0x38; the sub-def's virtual
// slot 1 destroys it when the last reference goes.
// Layout: the def object pointer at +0x38 (CTCShotDefinition), the graphic id at +0x3c of the sub-def (descriptive).
// Shape: a frame-pointer size TU (`push ebp ; mov ebp,esp`, `and dword ptr [ebp-4],0`, `leave`): `optimize("y",off)` +
// `("s",on)`. The local def pointer's release reads the count through the object with no null test (the def is known
// valid after the read of +0x3c): the destructor is an unconditional release through a LOCAL copy of the pointer (the
// member form re-reads the slot for the compare: `dec [eax+4] ; mov ecx,[ebp-4] ; cmp [ecx+4],0`).
struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy() const;            // slot 1 (+4)
    mutable long m_refCount;                 // +0x04
    __forceinline void Release() const
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CMultiArrowDef : CDefPointee {
    unsigned char _pad_0x08[0x34];
    long          WeaponTrailGraphic;        // +0x3c
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer()
    {
        Object->Release();
    }
    T* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<const CMultiArrowDef>* out);   // 0x007BE7EA
};

struct CTCShotDefinitionMultiArrow {
    virtual void v00();
    unsigned char  _pad_0x04[0x34];
    CDefClassBase* PDef;                     // +0x38
    long GetWeaponTrailGraphic() const;
};

long CTCShotDefinitionMultiArrow::GetWeaponTrailGraphic() const
{
    CDefPointer<const CMultiArrowDef> def;
    PDef->GetSubDef(&def);
    return def->WeaponTrailGraphic;
}
