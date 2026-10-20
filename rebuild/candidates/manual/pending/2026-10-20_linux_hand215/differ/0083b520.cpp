// CThingVillage::GetCurrentGraphic() -- the village's current map graphic: the state sub-def's graphic when it has one, else the village def's own graphic; cached in CurrentGraphic and returned by reference
// Grade: DIFFER(250v228) -- retail keeps the state def in ebx, `this` in edi and &CurrentGraphic in esi with a 4-byte frame shared by both def out pointers, releasing with `dec [reg+4] ; jne`; ours opens an 8-byte frame (scoping the first out pointer makes it 12), uses ebp and reloads the counts (the `--RefCount == 0` load/dec/store form)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083B520, length 228 bytes (240 with padding).
// Callees: CDefClassBase::GetSubDef<T> @ 0x0042B0A2 (on the base def, out pointer), CThing::GetDefinitionManager
// @ 0x004C7990 (manifest "CViewBase::GetDisplayEngine" is propagated), CDefinitionManager::GetDef<T>(long,
// CDefPointer<T>*) @ 0x0083B970, the defs' virtual Destroy (slot 1, +4).
// Layout: retail CThing PBaseDef +0x70, DefGlobalIndex (ushort) +0x8c (Ego_r.pdb - 0x10); CThingVillage
// CurrentGraphic +0xd0 (PDB 0xe8 - 0x18); CEngineGraphic is 14 packed bytes {BankIndex, AnimStep, RenderSizeX,
// AdditiveAlpha, Type}; the state sub-def holds one at +0x25 (unaligned), the village def at +0x70 (descriptive).
// Shape: the sub-def out pointer is read into a raw local right after the call and released by hand on both exits
// (the dereference drops the null test on the first); the village def is read through a by-value counted copy
// (`.Object`: add-ref and release share one null test, both count stores kept, as the landed 0x0070B4D0); BankIndex
// is tested unsigned (`> 0` -> `jbe`).
#pragma pack(push, 1)
struct CEngineGraphic {                                     // 14 bytes
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

struct CDefPointeeBase {
    virtual void Destroy0();
    virtual void Destroy();                                 // slot 1 (+0x04)
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CCountedDefPointer {              // the by-value CDefPointer<T> copy
    T* Object;
    CCountedDefPointer(T* object) : Object(object) { if (Object) Object->AddRef(); }
    ~CCountedDefPointer() { if (Object) Object->Release(); }
};

#pragma pack(push, 1)
struct CVillageStateDef : CDefPointeeBase {                 // the base def's graphic sub-def (descriptive)
    unsigned char  _pad08[0x25 - 0x08];
    CEngineGraphic Graphic;                                 // +0x25
};
#pragma pack(pop)
struct CThingVillageDef : CDefPointeeBase {
    unsigned char  _pad08[0x70 - 0x08];
    CEngineGraphic Graphic;                                 // +0x70 (descriptive)
};
struct CThingBaseDef : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CVillageStateDef>* out) const;   // 0x0042B0A2
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingVillageDef>* out);   // 0x0083B970
};

struct CThing {
    unsigned char  _pad00[0x70];
    CThingBaseDef* PBaseDef;                                // +0x70
    unsigned char  _pad74[0x8c - 0x74];
    unsigned short DefGlobalIndex;                          // +0x8c
    CDefinitionManager* GetDefinitionManager();            // 0x004C7990
};

struct CThingVillage : CThing {
    unsigned char  _pad90[0xd0 - 0x90];                    // (CThing is 0x90 bytes)
    CEngineGraphic CurrentGraphic;                          // +0xd0
    const CEngineGraphic& GetCurrentGraphic();
};

const CEngineGraphic& CThingVillage::GetCurrentGraphic()
{
    CDefPointer<CVillageStateDef> out;
    bool found = PBaseDef->GetSubDef(&out);
    CVillageStateDef* state = out.Object;
    if (found && state->Graphic.BankIndex > 0) {
        CurrentGraphic = state->Graphic;
        if (state) {
            if (--state->RefCount == 0)
                state->Destroy();
        }
        return CurrentGraphic;
    }
    {
        CDefPointer<CThingVillageDef> defOut;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &defOut);
        CThingVillageDef* def = CCountedDefPointer<CThingVillageDef>(defOut.Object).Object;
        CurrentGraphic = def->Graphic;
        if (def) {
            if (--def->RefCount == 0)
                def->Destroy();
        }
    }
    if (state) {
        if (--state->RefCount == 0)
            state->Destroy();
    }
    return CurrentGraphic;
}
