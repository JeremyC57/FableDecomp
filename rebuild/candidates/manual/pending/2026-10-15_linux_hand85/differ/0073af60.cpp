// CTCCreatureModeManager::SetupDefaultMode() -- add the default creature mode named by the thing's CCreatureModeDef sub-def
// Grade: DIFFER(104v95) -- the sub-def out-parameter local is re-read from its frame slot at every use (retail loads it once
// into edi right after GetSubDef, this in ebx); the lane also keeps this in edi and splits the release into dec/reload/test.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073AF60, length 95 bytes (+1 int3).
// Callees: CDefClassBase::GetSubDef<CCreatureModeDef>(CDefPointer<CCreatureModeDef>*) @ 0x0073DBE0 (bool, out-parameter;
// manifest label names another instantiation), CTCCreatureModeManager::AddMode(long) @ 0x0073AE10; the def releases are
// the pointee's virtual slot 1.
// Shape: the thing's base def (CThing +0x70 = PBaseDef; PDB +0x80) is copied into a local CDefPointer (add-ref `inc [esi+4]`),
// the sub-def pointer local is zero-constructed before the null test, and DefaultCreatureMode (+0x28, PDB name) is passed
// on. The sub-def release is `dec [edi+4] ; jne ; call [vt+4]`; the base-def release sits inside the non-null arm with no
// second null test (the null path skips both destructors by constant propagation).
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
};

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CCreatureModeDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 8];
    long DefaultCreatureMode;               // +0x28
    long InitialCreatureMode;               // +0x2c
    long DefaultWeaponCreatureMode;         // +0x30
};

struct CThingBaseDef : CDefPointeeBase {
    template <class T> bool GetSubDef(CDefPointer<T>* out) const;   // 0x0073DBE0 for T = CCreatureModeDef
};

struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<CThingBaseDef> PBaseDef;    // +0x70
};

struct CTCCreatureModeManager {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    bool AddMode(long modeType);            // 0x0073AE10
    void SetupDefaultMode();
};

void CTCCreatureModeManager::SetupDefaultMode()
{
    CDefPointer<CThingBaseDef> thingDef(ParentThing->PBaseDef);
    CDefPointer<CCreatureModeDef> modeDef;
    if (thingDef.Object) {
        if (thingDef->GetSubDef(&modeDef))
            AddMode(modeDef->DefaultCreatureMode);
    }
}
