// NThingShot::CTCShotDefinitionBase::Update() -- per-frame shot update: latch "passed target" once, run the movement/collision steps, strip dead entries from the hit-things list, check for a collision and, while the shot lives on, trigger its sub-shots once the shot definition's delay has elapsed (killing the shot when the definition says so), then run the post-update steps and wake the physics
// Leaf: ?Update@CTCShotDefinitionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C21A0, 333 bytes (plain `ret`).
// Callees: CTCShotDefinitionBase::HasPassedTarget 0x007BF3D0; virtual slots 35..41 of this (+0x8c .. +0xa4); CBaseIntelligentPointer
//          ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<const CThing> vtable 0x01237F1C);
//          STLport find(first, last, value, random_access_iterator_tag) 0x007379D0 and remove_copy 0x00737AD0 (both __fastcall,
//          as the landed CTCSolidToThing::FrameUpdate 0x00770E80), vector::erase(first, last) 0x00738D70;
//          CTCShotDefinitionBase::OnCollisionProper(bool*) 0x007C1000; CTCBase::GetWorld 0x00686A70; CWorld::GetWorldFrame 0x0049D870;
//          CThing::Kill(bool) 0x004C9B80; CTCPhysicsBase::SetAsMoving(bool) 0x006B0130. Global: frames per second 0x01375550.
// Layout (Ego_r.pdb CTCShotDefinitionBase; retail is PDB - 4 after the 12-byte retail vector): ParentThing +0x04,
//          HitThingsList +0x14, PShotDef +0x38, StartFrame +0x4c, SubShotsTriggered +0x50, PassedTarget +0x64. The shot
//          definition (CThingShotDef, descriptive): sub-shot count +0x98, sub-shot delay (seconds) +0xa0, kill-on-sub-shots +0xa8.
//          Retail CThing PhysicsTC +0x60.
// Shape: `v.erase(remove(v.begin(), v.end(), CPtr(0)), v.end())` (erase's end() read first); the collision flag and the
//          iterator tag share a frame byte; the world frame comes from the static GetWorldFrame through GetWorld().
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
class CTCPhysicsBase { public:
    void SetAsMoving(bool moving);              // 0x006B0130
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    void Kill(bool removeNow);                  // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    const CThing* PItem;                        // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
typedef CIntelligentPointerConstThing CPtr;
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CPtr* __fastcall find(CPtr* first, CPtr* last, const CPtr& value, const random_access_iterator_tag& tag);   // 0x007379D0
CPtr* __fastcall remove_copy(CPtr* first, CPtr* last, CPtr* result, const CPtr& value);                  // 0x00737AD0
inline CPtr* remove(CPtr* first, CPtr* last, const CPtr& value)
{
    first = find(first, last, value, random_access_iterator_tag());
    if (first == last)
        return first;
    CPtr* next = first;
    return remove_copy(++next, last, first, value);
}
struct CPtrVector {                             // STLport vector<CIntelligentPointer<const CThing> > (12 bytes)
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* _M_end_of_storage;
    CPtr* begin() { return _M_start; }
    CPtr* end() { return _M_finish; }
    CPtr* erase(CPtr* first, CPtr* last);       // 0x00738D70
};
class CThingShotDef { public:                          // descriptive
    unsigned char _pad00[0x98];
    long NumSubShots;                           // +0x98
    unsigned char _pad9c[0xa0 - 0x9c];
    float SubShotDelay;                         // +0xa0 (seconds)
    unsigned char _pada4[0xa8 - 0xa4];
    bool KillOnSubShots;                        // +0xa8
};
extern long g_framesPerSecond;                  // 0x01375550
class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void UpdateMovement();              // slot 35 (+0x8c)
    virtual void UpdateOrientation();           // slot 36 (+0x90)
    virtual void CheckForCollisions(bool* collided);   // slot 37 (+0x94)
    virtual void PostUpdate();                  // slot 38 (+0x98)
    virtual void UpdateSound();                 // slot 39 (+0x9c)
    virtual void TriggerSubShots();             // slot 40 (+0xa0)
    virtual void OnPassedTarget();              // slot 41 (+0xa4)
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

struct CTCShotDefinitionBase : public CTCBase {
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x14 - 0x08];
    CPtrVector HitThingsList;                   // +0x14
    unsigned char _pad20[0x38 - 0x20];
    CThingShotDef* PShotDef;                    // +0x38 (CDefPointer<CThingShotDef const>)
    unsigned char _pad3c[0x4c - 0x3c];
    long StartFrame;                            // +0x4c
    bool SubShotsTriggered;                     // +0x50
    unsigned char _pad51[0x64 - 0x51];
    bool PassedTarget;                          // +0x64
    bool HasPassedTarget();                     // 0x007BF3D0
    void OnCollisionProper(bool* collided);     // 0x007C1000
    void Update();
};

void CTCShotDefinitionBase::Update()
{
    if (!PassedTarget && HasPassedTarget()) {
        OnPassedTarget();
        PassedTarget = true;
    }
    UpdateMovement();
    UpdateOrientation();
    {
        HitThingsList.erase(remove(HitThingsList.begin(), HitThingsList.end(), CPtr(0)), HitThingsList.end());
    }
    {
        bool collided;
        CheckForCollisions(&collided);
        OnCollisionProper(&collided);
        if (collided)
            return;
    }
    long frame = GetWorld().GetWorldFrame();
    if (PShotDef->NumSubShots > 0 && !SubShotsTriggered
        && (float)(frame - StartFrame) / g_framesPerSecond >= PShotDef->SubShotDelay) {
        TriggerSubShots();
        SubShotsTriggered = true;
        if (PShotDef->KillOnSubShots)
            ParentThing->Kill(true);
    }
    PostUpdate();
    UpdateSound();
    ParentThing->PhysicsTC->SetAsMoving(true);
}