// CTCDegradableBase::OnCreate() -- clear the last hit frame, fetch the parent thing's definition, take its degradable sub-def, clear the current stage and remember the parent as the physical thing
// Grade: DIFFER(98v98) -- the temporary def pointer's zero store sits before the pushes (retail after them, just before the GetDefinitionManager call) and the def object is loaded via ecx then copied to edi (retail mov edi,[esp+8] ; mov ecx,edi); a forceinline GetParentDef helper fixes the edi load but turns the release into the counted load/dec/store form
// Leaf: OnCreate
//
// Retail 0x008EC870, 98 bytes (plain `ret`; vtable 0x0126A6A4 slot 4, Ego_r.pdb
// `?OnCreate@CTCDegradableBase@@MAEXXZ`; the catalog "CTCDegradableBarrier::vfunc_4" is the slot name).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (manifest "_Cons_val<...>", propagated),
// CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C (manifest name is a byte-twin template),
// CDefClassBase::GetSubDef<CDegradableDef>(CDefPointer*) @ 0x008ECDA0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", propagated); the def's virtual slot 1 when the
// temporary def pointer drops its last reference.
// Layout = Ego_r.pdb CTCDegradableBase (0x1c): LastWorldFrameHit +0xc, PDefinition +0x10,
// PCurrentDegradableInfo +0x14, PParentThingPhysical +0x18; CThing's definition index is the word at +0x8c.
// Shape: the temporary CDefPointer is zeroed after the argument pushes (declared, then the call chain);
// its object is read into a local (edi) right after GetDef, and the release is written by hand on that
// local (null test, `dec [edi+4] ; jne`, virtual destroy).
struct CThing {
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                                // +0x8c
};

struct CDefClassBase;
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CDegradableDef;
struct CDegradableInfo;
struct CThingPhysical;

struct CDefClassBase {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    bool GetSubDef(CDefPointer<const CDegradableDef>* out); // 0x008ECDA0
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0
};

struct CTCDegradableBase : CTCBase {
    long                             LastWorldFrameHit;     // +0x0c
    CDefPointer<const CDegradableDef> PDefinition;          // +0x10
    const CDegradableInfo*           PCurrentDegradableInfo;// +0x14
    CThingPhysical*                  PParentThingPhysical;  // +0x18
    void OnCreate();
};

void CTCDegradableBase::OnCreate()
{
    LastWorldFrameHit = 0;
    CDefPointer<CDefClassBase> def;
    GetDefinitionManager()->GetDef(ParentThing->DefIndex, &def);
    CDefClassBase* object = def.Object;
    object->GetSubDef(&PDefinition);
    PCurrentDegradableInfo = 0;
    PParentThingPhysical = (CThingPhysical*)ParentThing;
    if (object != 0 && --object->RefCount == 0)
        object->DestroySelf();
}
