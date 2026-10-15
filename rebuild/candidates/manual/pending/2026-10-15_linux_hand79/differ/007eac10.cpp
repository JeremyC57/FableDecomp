// CTCActionUseBase::OnCreate() -- run the base hook, look the parent thing's def up by its global index and fetch its action-use sub-def into PActionUseDef
// Leaf: OnCreate
// Grade: DIFFER(ours v retail) -- 93v81: retail keeps the local def pointer's object in esi from the GetSubDef
//        `this` through the release (`mov esi,[esp+8] ; mov ecx,esi ; call ; test esi,esi ; dec [esi+4]`) and stores
//        the null initialiser after the argument pushes; the lane re-reads the address-taken frame slot after the
//        call (and uses the DecRef/GetRefCount release shape) and stores the zero first -- the GOTCHAS 24k "CDefPointer
//        local re-read from its slot" gap, same as CTCCameraCollision slot 4 0x007E3040. A probe that copies the
//        object into a raw local and releases it by hand gives 83v81 (edi instead of esi, zero store still early).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EAC10, length 81 bytes (+ int3 padding).
// Callees: CTCBase's slot-4 hook @ 0x00686850 (an empty body called non-virtually; manifest "$E2"),
// CTCBase_Calls::GetDefinitionManager() @ 0x006869D0 (the manifest label is a propagated _Cons_val),
// CDefinitionManager::GetDef<T>(long, CDefPointer<T>*) @ 0x0042AF3C, CDefClassBase::GetSubDef<
// CActionUseDef>(CDefPointer<T>*) const @ 0x007EAE40, and the thing def's virtual slot 1 (`call [eax+4]`)
// when the local pointer drops the last count.
//
// Layout: CThing::DefGlobalIndex is the ushort at +0x8c in retail (`movzx` widens it to the long index);
// CTCActionUseBase::PActionUseDef (CDefPointer<CActionUseDef>) is +0x0c.
// Shape: the same body as CTCCameraCollision's slot 4 (0x007E3040): a local CDefPointer in the `push ecx`
// slot whose null store is scheduled right before the GetDef call, the sub-def lookup through the
// pointer's raw `operator->` (the object loaded into esi once and reused by the destructor), and the
// destructor's null-tested release on a local copy (`dec [esi+4] ; jne ; call [eax+4]`, no null store).
// Header note: engine/CDefClassBase.h, CActionUseDef.h and CDefinitionManager.h hold layouts only, so the
// def classes are modelled locally with a _Retail suffix.
template <class T> struct CDefPointer;
struct CActionUseDef_Retail;

struct CDefClassBase_Retail
{
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    bool GetSubDef(CDefPointer<CActionUseDef_Retail>* out) const;   // GetSubDef<CActionUseDef> @ 0x007EAE40
};

template <class T>
struct CDefPointer
{
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0)
        {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CThingDef : CDefClassBase_Retail {};
struct CActionUseDef_Retail : CDefClassBase_Retail {};

struct CDefinitionManager_Retail
{
    bool GetDef(long globalIndex, CDefPointer<CThingDef>* out);   // GetDef<T> @ 0x0042AF3C
};

struct CThing
{
    unsigned char  _pad_0x00[0x8c];
    unsigned short DefGlobalIndex;                  // +0x8c
};

#pragma pack(push, 1)
struct CTCBase_Calls                                   // engine/CTCBase.h layout (the header holds the layout only)
{
    void*   __vftable;                              // +0x00
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    unsigned char _pad_0x0a[0x2];                   // +0x0a
    void OnCreate();                                // 0x00686850 (empty)
    CDefinitionManager_Retail* GetDefinitionManager() const;   // 0x006869D0
};

struct CTCActionUseBase : CTCBase_Calls
{
    CDefPointer<CActionUseDef_Retail> PActionUseDef;       // +0x0c
    bool                       Usable;              // +0x10
    void OnCreate();
};
#pragma pack(pop)

void CTCActionUseBase::OnCreate()
{
    CTCBase_Calls::OnCreate();
    CDefPointer<CThingDef> thingDef;
    GetDefinitionManager()->GetDef(ParentThing->DefGlobalIndex, &thingDef);
    thingDef->GetSubDef(&PActionUseDef);
}
