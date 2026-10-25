// CThingSoundEmitter::RenderUpdate(const CInterpolationInfo* info) -- per render frame: nothing when dead; the CThing base update first; when the attached thing has died either stop the loops (keep-playing emitters) or kill the emitter; kill it when its sound has finished; otherwise follow the attached thing's interpolated position and update the sound
// Leaf: ?RenderUpdate@CThingSoundEmitter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836AD0, 245 bytes, thiscall `ret 4`; CThingSoundEmitter vtable 0x01272C24 slot 21.
// Identity: Ego_r.pdb ?RenderUpdate@CThingSoundEmitter@@UAE_NPBVCInterpolationInfo@@@Z; CTCPhysicsBase::GetPosition(float) const and
//   CThing::GetPhysicsTC() const are Ego_r.pdb/FableWin.pdb names; the catalog's
//   "CThingFilter_IsUnconscious_Apply" is a label propagated onto the vtable slot.
// Callees: CThing::RenderUpdate 0x004C7460 (the base `return false`; the catalog's CThingFilter_IsUnconscious name is
//   folded onto it), CBaseIntelligentPointer::GetPItem 0x00A01B50, IS_THING_ALIVE<CIntelligentPointer<const CThing> >
//   0x0069CE20 (__fastcall, landed), CTCPhysicsBase::GetPosition(float) const 0x0049D150 (landed as GetInterpolatedPosition),
//   CThing::Kill(bool) 0x004C9B80, the sound object's virtual slots 11 (+0x2c), 14 (+0x38), 16 (+0x40), 19 (+0x4c).
// Layout: retail CThing (0x98 bytes): PhysicsTC +0x60, the Dead bit = bit 0 of +0x91 (Ego_r.pdb +0x70/+0xA1: the
//   PDB's CThing is 0x10 longer); CThingSoundEmitter as the landed ctor 0x008367B0 (sound object +0x98,
//   AttachedToThing +0x9c, PAttachedThing +0x9d unaligned, ContinuePlayingAfterAttachedThingDeath +0xa5);
//   CInterpolationInfo.Current.WFInterpolate +4 (Ego_r.pdb).
// Shape: the whole body under `if (!Dead) { ... } return false;` (one shared return-false tail; early returns per test
//   shrink-wrap the pushes); `CThing::RenderUpdate(info) == true` (`cmp al,1`); IS_THING_ALIVE is a plain __fastcall
//   template that VC7.1 inlines at its first call only (the attach test) and calls out of line at the second; the
//   physics component through the inline CThing::GetPhysicsTC() const accessor (`mov eax,[eax+60h]` ... `mov ecx,eax`);
//   the position ASSIGNED to a local (`pos = ...GetPosition(t)`), so the 12 bytes are copied from the call's
//   temporary into the local passed to the sound object (initialising `pos` lets VC7.1 construct it in place).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CTCPhysicsBase {
public:
    C3DVector GetPosition(float t) const;                // 0x0049D150 (the landed GetInterpolatedPosition)
};

struct CInterpolationInfoSet {
    float GTPredictedTimeSinceLastRenderFrame;           // +0x00
    float WFInterpolate;                                 // +0x04
};
struct CInterpolationInfo {
    CInterpolationInfoSet Current;                       // +0x00
};

class CThing {
public:
    virtual ~CThing();
    unsigned char   _pad04[0x5c];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char   _pad64[0x2d];
    unsigned char   Dead : 1;                            // +0x91 bit 0
    unsigned char   CreatedByOwnerThingCreation : 1;
    unsigned char   IncludeInThingScans : 1;
    unsigned char   Activated : 1;
    unsigned char   GloballyLoaded : 1;
    unsigned char   InLimbo : 1;
    unsigned char   KillLocked : 1;
    unsigned char   CreatedBySerialisation : 1;
    unsigned char   _pad92[0x6];
    virtual bool RenderUpdate(const CInterpolationInfo* info);   // 0x004C7460
    void Kill(bool immediately);                         // 0x004C9B80
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    void* m_pItem;                                       // +0x04
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& p)   // 0x0069CE20
{
    if (p.GetPItem() != 0) {
        if (!p->Dead)
            return true;
    }
    return false;
}

class CSoundEmitterObject {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void StopLoops();                            // slot 11 (+0x2c)
    virtual void v12(); virtual void v13();
    virtual void Update();                               // slot 14 (+0x38)
    virtual void v15();
    virtual void SetPosition(const C3DVector& pos);      // slot 16 (+0x40)
    virtual void v17(); virtual void v18();
    virtual bool IsPlaying();                            // slot 19 (+0x4c)
};

class CThingSoundEmitter : public CThing {
public:
    CSoundEmitterObject*              SoundObject;       // +0x98
    bool                              AttachedToThing;   // +0x9c
    CIntelligentPointer<const CThing> PAttachedThing;    // +0x9d
    bool                              ContinuePlayingAfterAttachedThingDeath;   // +0xa5
    C3DVector                         Pos;               // +0xa6
    virtual bool RenderUpdate(const CInterpolationInfo* info);
};
#pragma pack(pop)

bool CThingSoundEmitter::RenderUpdate(const CInterpolationInfo* info)
{
    if (!Dead) {
        if (CThing::RenderUpdate(info) == true)
            return true;
        if (AttachedToThing && !IS_THING_ALIVE(PAttachedThing)) {
            if (!ContinuePlayingAfterAttachedThingDeath) {
                Kill(true);
                return true;
            }
            SoundObject->StopLoops();
        }
        if (!SoundObject->IsPlaying()) {
            Kill(true);
            return true;
        }
        if (AttachedToThing && IS_THING_ALIVE(PAttachedThing)) {
            C3DVector pos;
            pos = PAttachedThing->GetPhysicsTC()->GetPosition(info->Current.WFInterpolate);
            SoundObject->SetPosition(pos);
        }
        SoundObject->Update();
    }
    return false;
}