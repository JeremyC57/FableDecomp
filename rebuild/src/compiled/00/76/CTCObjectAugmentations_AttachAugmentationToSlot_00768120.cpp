// CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot) -- fetch the thing def for the global index; when it carries an augmentation sub-def, store it in the slot; then re-attach the augmentation particles and sounds
// Leaf: AttachAugmentationToSlot
// Retail: Fable.exe (TLC, VC7.1) @ 0x00768120, 166 bytes (`ret 8`; PDB ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CGameDefinitionManager::GetDef<T> @ 0x0042AF3C, CDefClassBase::
//   GetSubDef<CAugmentationDef> @ 0x0042B248, AttachAugmentationParticles() @ 0x007677E0, SetupAugmentationSounds() @ 0x00766C00
//   (manifest "CTCObjectAugmentations_UpdateParticleMorphs"; the body looks sounds up through CDefStringTable::GetString), the
//   def release through vtable slot 1.
// Tried: RAII out pointer + holder (extra zero store), plain out slot + holder + entry reference, non-const def types.
//   Next: the augmentation slot as a plain CDefSlot released by hand at the end (`if (p) { if (--p->m_refCount == 0) ... }`
//   on a pointer local read after SetupAugmentationSounds); lane gap otherwise (callee-saved colouring, chunk 161 0x69A8B0 family).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
};
struct CDefBase : CDefVirtual {
    mutable long m_refCount;                            // +0x04
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline ~CDefPointer() { T* object = Object; ReleaseDef(object); }
    CDefPointer& operator=(T* object)
    {
        if (Object != object) {
            if (Object) {
                ReleaseDef(Object);
                Object = 0;
            }
            Object = object;
            if (object)
                ++object->m_refCount;
        }
        return *this;
    }
};
template <class T> struct CDefHolder {                  // owns the reference GetDef handed over
    T* Object;
    __forceinline ~CDefHolder() { ReleaseDef(Object); }
};
struct CAugmentationDef : CDefBase { };
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out) const;   // 0x0042B248 (T = const CAugmentationDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
struct CThingBaseDef : CDefClassBase { };
struct CGameDefinitionManager {
    template <class T> bool GetDef(long index, CDefPointer<T>& out) const;   // 0x0042AF3C (T = const CThingBaseDef)
};
void* FindDef_s7r_1();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CGameDefinitionManager::GetDef(long p0, CDefPointer<T>& out) const
{
    void* def = FindDef_s7r_1();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CThing;
struct CTCObjectAugmentations {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CDefPointer<const CDefBase> PDef;                   // +0x0c
    bool          LoadedFromSaveGame;                   // +0x10
    vector< CDefPointer<const CThingBaseDef> > Augmentations;   // +0x14
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void AttachAugmentationParticles();                 // 0x007677E0
    void SetupAugmentationSounds();                     // 0x00766C00
    void AttachAugmentationToSlot(long defIndex, long slot);
};

void CTCObjectAugmentations::AttachAugmentationToSlot(long defIndex, long slot)
{
    CDefPointer<const CThingBaseDef> fetched;
    GetDefinitionManager().GetDef(defIndex, fetched);
    CDefHolder<const CThingBaseDef> thingDef = { fetched.Object };
    fetched.Object = 0;
    CDefPointer<const CAugmentationDef> augmentation;
    if (thingDef.Object->GetSubDef(augmentation))
        Augmentations[slot] = thingDef.Object;
    AttachAugmentationParticles();
    SetupAugmentationSounds();
}