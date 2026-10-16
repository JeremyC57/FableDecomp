// CTCHighlightItem::FrameUpdate() -- while waiting, starts running once the def's delay has elapsed; while running, moves; counts frames either way
// Leaf: FrameUpdate
// Grade: DIFFER(117v117) -- same CDefPointer zero-store lane gap (zero stored before `mov ecx,[eax+70h]`), plus an eax/edx swap in the release's vtable load. Tried the same three forms as 0x0077CB60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077CD50, 117 bytes (+11 int3); default /O2 TU, no frame.
// Catalog label "CTCHighlightItem::vfunc_9" (the CTCBase FrameUpdate slot).
// Callees: CThingDef::GetSubDef<CHighlightItemDef> @ 0x0077D080, DoStartRunning() @ 0x0077CBE0,
//          CThing::UpdateGraphicAppearance(long) @ 0x004C7590 (15), DoMovement() @ 0x0077C960.
// Globals: the frames-per-second long @ 0x01375550.
// Shape: `(float)LifeTimer > def->StartDelaySeconds * fps` (`fild timer ; fild fps ; fmul [def+44h] ; fcompp ;
// test ah,5 ; jp`); the running arm has its own `inc ; ret` tail.
// Layout: CTCHighlightItem (PDB, matches retail): StartRunning +0x0C, Running +0x0D, LifeTimer +0x10, StartPos +0x14,
// PParticleEmitter +0x20 (CIntelligentPointer<CThing>); CTCBase ParentThing +0x04; retail CThing def object +0x70.
// The def pointer is read once into edi after GetSubDef and released by hand (`dec [edi+4] ; jne ; call [vtbl+4]`).

struct C3DVector {
    float X, Y, Z;
};

struct CHighlightItemDef {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04): called when the last reference goes
    long          RefCount;                          // +0x04
    unsigned char _pad_0x08[0x24];
    long          EndEffectDef;                      // +0x2C (0 = none)
    unsigned char _pad_0x30[0x10];
    bool          WaitForCameraIdle;                 // +0x40
    unsigned char _pad_0x41[0x3];
    float         StartDelaySeconds;                 // +0x44
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
};

struct CThingDef {
    bool GetSubDef(CDefPointer<const CHighlightItemDef>* out) const;   // 0x0077D080
};

template <class T> struct CDefPointerMember {
    T* Object;
    T* operator->() const { return Object; }
};

struct CThing {
    unsigned char _pad_0x00[0x70];
    CDefPointerMember<CThingDef> PDef;               // +0x70 CDefPointer<const CThingDef>
    const C3DVector& GetPosition() const;            // 0x004C73D0
    void Kill(bool immediately);                     // 0x004C9B80
    void UpdateGraphicAppearance(long reason);       // 0x004C7590
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                        // 0x00A01B50
};

CThing* __fastcall CreateThing(long defIndex, const C3DVector& position, long flags);   // 0x006E0880

struct CGameSubsystem {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsCameraIdle();                     // slot 9 (+0x24)
};

extern long g_GameFramesPerSecond;                   // 0x01375550

struct CTCHighlightItem {
    void*         __vftable;                         // +0x00
    CThing*       ParentThing;                       // +0x04 CTCBase
    bool          ToBeRemoved;                       // +0x08
    bool          IsInGlobalUpdateTCs;               // +0x09
    unsigned char _pad_0x0a[0x2];
    bool          StartRunning;                      // +0x0C
    bool          Running;                           // +0x0D
    long          LifeTimer;                         // +0x10
    C3DVector     StartPos;                          // +0x14
    CBaseIntelligentPointer PParticleEmitter;        // +0x20 CIntelligentPointer<CThing>

    CGameSubsystem* GetGameSubsystem() const;        // 0x00686D60 (CTCBase: main game component +0x1C)
    void DoStartRunning();                           // 0x0077CBE0
    void DoMovement();                               // 0x0077C960
    void OnInitialActivate();
    void DoStopRunning();
    void FrameUpdate();
};

void CTCHighlightItem::FrameUpdate()
{
    if (StartRunning) {
        CThingDef* thingDef = ParentThing->PDef.operator->();
        CDefPointer<const CHighlightItemDef> pdef;
        thingDef->GetSubDef(&pdef);
        const CHighlightItemDef* def = pdef.Object;
        if (LifeTimer > def->StartDelaySeconds * g_GameFramesPerSecond)
            DoStartRunning();
        ParentThing->UpdateGraphicAppearance(15);
        ++LifeTimer;
        if (--((CHighlightItemDef*)def)->RefCount == 0)
            ((CHighlightItemDef*)def)->Destroy();
    } else if (Running) {
        DoMovement();
        ++LifeTimer;
    }
}
