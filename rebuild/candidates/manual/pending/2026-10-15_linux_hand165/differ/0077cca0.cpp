// CTCHighlightItem::DoStopRunning() -- kills the particle emitter, spawns the def's end effect at the thing and clears Running
// Leaf: DoStopRunning
// Grade: DIFFER(122v117) -- same CDefPointer zero-store lane gap; ours also keeps the def in ebp (extra push ebp) and CSEs `lea edi,[def+2Ch]` for the end-effect read. Tried the same three forms as 0x0077CB60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077CCA0, 117 bytes; default /O2 TU, no frame.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (twice), CThing::Kill(bool) @ 0x004C9B80,
//          CThingDef::GetSubDef<CHighlightItemDef> @ 0x0077D080, CThing::GetPosition() @ 0x004C73D0,
//          the __fastcall thing factory @ 0x006E0880 (def index in ecx, position in edx, flags pushed).
// Shape: the emitter is null-tested through one GetPItem call and killed through a second; the def is used without
// a null test; the factory's trailing 0 is pushed before GetPosition (right-to-left).
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

void CTCHighlightItem::DoStopRunning()
{
    if (Running) {
        if (PParticleEmitter.GetPItem())
            PParticleEmitter.GetPItem()->Kill(true);
        CThingDef* thingDef = ParentThing->PDef.operator->();
        CDefPointer<const CHighlightItemDef> pdef;
        thingDef->GetSubDef(&pdef);
        const CHighlightItemDef* def = pdef.Object;
        if (def->EndEffectDef)
            CreateThing(def->EndEffectDef, ParentThing->GetPosition(), 0);
        Running = false;
        if (--((CHighlightItemDef*)def)->RefCount == 0)
            ((CHighlightItemDef*)def)->Destroy();
    }
}
