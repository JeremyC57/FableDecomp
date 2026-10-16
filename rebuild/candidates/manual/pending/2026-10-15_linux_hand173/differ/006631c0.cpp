// CThingCreatureBase::PreloadAnimation(long animation) -- fetch the creature's appearance sub-def and touch (preload) every variant of one animation type in its animation set (no group filter)
// Grade: DIFFER(172v160) -- lane gap: only the final def release differs: retail `mov ecx,[esp+4] ; test ; je ; dec [ecx+4] ; jne ; call [vtbl+4]` (esi already popped); an inline dtor re-reads the escaped slot after the dec (168 bytes), a hand release / local copy / Release() member gives the `mov esi,[eax+4] ; add eax,4 ; dec esi ; mov [eax],esi` form in the free esi. Body, filter temporaries and loop match.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006631C0, 160 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> @ 0x0042B0A2 (byte-twin label GetSubDef<CAbilityDef>),
//   CThing::GetWorld @ 0x004C7AB0, CAnimationSet::GetNumberOfAnimations(long, const CAnimFilter&) @ 0x005DC290
//   (auto label "AnimationDef_CountMatchingPreloads"), CAnimationSet::GetAnimation(long, long, const CAnimFilter&)
//   @ 0x005DC340 (auto label "Animation_FindNthValid"; returns the entry, animation index first), the mesh bank's
//   GetAnimation(long) @ 0x00A242C0, C3DAnimationInfo::TouchAnimation @ 0x00A26A50.
// Identity: the frame object whose only word is 0x0122FFDC is a CAnimFilterNoGroup temporary (RTTI locator at
//   vtable-4: .?AVCAnimFilterNoGroup@@; slot 0 0x0041CD13 the accept test, slot 1 0x00429A2F Clone), built anew for
//   each call (the vptr store is repeated in the loop).
// Layout: retail CThing PBaseDef +0x70; retail CWorld +0x3c = main mesh bank; CAppearanceDef Animation at +0x34
//   (see 0x004717C7); def ref count +4, virtual Destroy slot 1.
// Shape: GetSubDef's out CDefPointer (zero store) released by hand at the end (one read of the slot); the def pointer is re-read from its slot for the
//   set address; count in ebx, index in esi, bank in ebp.
struct C3DAnimationInfo {
    void TouchAnimation();                      // 0x00A26A50
};
struct CMeshBank {
    C3DAnimationInfo* GetAnimation(long index); // 0x00A242C0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshBank* PMainMeshBank;                   // +0x3c
};
struct CAnimationEntry {
    long AnimIndex;                             // +0x00
};
struct CAnimFilter {
    virtual bool Accept(const CAnimationEntry* entry) const = 0;
    virtual CAnimFilter* Clone() const = 0;
};
struct CAnimFilterNoGroup : CAnimFilter {      // vtable 0x0122FFDC
    virtual bool Accept(const CAnimationEntry* entry) const;    // 0x0041CD13
    virtual CAnimFilter* Clone() const;                         // 0x00429A2F
};
struct CAnimationSet {
    long GetNumberOfAnimations(long animation, const CAnimFilter& filter) const;                    // 0x005DC290
    const CAnimationEntry* GetAnimation(long animation, long index, const CAnimFilter& filter) const; // 0x005DC340
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CAppearanceDef : CDefClassBase {
    unsigned char _pad08[0x34 - 0x08];
    CAnimationSet Animation;                    // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CThingBaseDef {
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;      // 0x0042B0A2
};

struct CThingCreatureBase {
    unsigned char _pad00[0x70];
    const CThingBaseDef* PBaseDef;              // +0x70
    CWorld* GetWorld();                         // 0x004C7AB0
    void PreloadAnimation(long animation);
};

void CThingCreatureBase::PreloadAnimation(long animation)
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        CMeshBank* bank = GetWorld()->PMainMeshBank;
        const CAnimationSet* set = &def->Animation;
        long count = set->GetNumberOfAnimations(animation, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->GetAnimation(set->GetAnimation(animation, i, CAnimFilterNoGroup())->AnimIndex)->TouchAnimation();
    }
    const CDefClassBase* object = def.Object;
    if (object && --const_cast<CDefClassBase*>(object)->RefCount == 0)
        const_cast<CDefClassBase*>(object)->Destroy();
}
