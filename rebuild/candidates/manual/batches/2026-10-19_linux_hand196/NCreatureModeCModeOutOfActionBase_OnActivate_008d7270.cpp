// NCreatureMode::CModeOutOfActionBase::OnActivate() -- knocked out of action: set the recovery countdown in frames, suspend the creature's animation in state 1, spawn the mode's particle emitter at the head when the creature allows effects, start the derived mode's anim, then the stand-mode activation
// Leaf: OnActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7270, 171 bytes (`ret`; vtable 0x0126743C slot 5).
// Callees: own virtuals slot 14 (+0x38, the recovery time in seconds, float), slot 13 (+0x34, the emitter type; -1 = none in
// the base 0x0073DA20) and slot 12 (+0x30, empty in the base 0x0073DA10); __ftol2 0x00BFEA70; CThingAnimatable::SetSuspendAnimation
// (bool, long) 0x00833A10; GetHeadPosWithOffset(CThing*) 0x0085EE50 (`__fastcall`, C3DVector by hidden pointer in ecx);
// CreateEffectThing(long, const C3DVector*, bool) 0x006E0880 (`__fastcall`); CBaseIntelligentPointer ctor 0x00A01B10, GetPItem
// 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10; CModeStandBase::OnActivate 0x008621A0 (qualified base call).
// Globals: the frames-per-second long 0x01375550.
// Layout (PDB CModeOutOfActionBase 0x78, byte-packed tail): RecoveryCount +0x68, Recoverable +0x6c, PEmitter +0x6d.
// Thing: state byte +0x10, effects-allowed flag bit 3 of +0x6c.
// Shape: the frame rate is read into a local before the virtual call (spilled to the frame, then `fimul`); the new
// emitter goes through a CIntelligentPointer<CThing> temporary assigned to PEmitter (inline operator= = SetPItem(GetPItem())),
// destroyed by the implicit dtor (out-of-line base dtor, no vtable re-store).
struct CThingAnimatable {                  // empty here: only its member is called (thing base at offset 0)
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

struct CThing : CThingAnimatable {
    unsigned char _pad00[0x10];
    unsigned char m_state;                 // +0x10 creature state byte (compared with 1)
    unsigned char _pad11[0x5b];
    unsigned char m_flags6c;               // +0x6c bit 3: effects allowed
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

C3DVector __fastcall GetHeadPosWithOffset(CThing* thing);   // 0x0085EE50
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

extern long g_framesPerSecond;             // 0x01375550

struct CBaseIntelligentPointer {           // vptr + item
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CThing* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);           // 0x00A01B90
};

#pragma pack(push, 1)
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
#pragma pack(pop)

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnActivate();             // slot 5, 0x008621A0
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void StartOutOfActionAnim();   // slot 12 (+0x30)
    virtual long GetEmitterType();         // slot 13 (+0x34)
    virtual float GetRecoveryTime();       // slot 14 (+0x38)
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {

#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeStandBase {   // vtable 0x0126743C
    long                     RecoveryCount;  // +0x68
    bool                     Recoverable;    // +0x6c
    CIntelligentPointerThing PEmitter;       // +0x6d
    virtual void OnActivate();
};
#pragma pack(pop)

void CModeOutOfActionBase::OnActivate()
{
    long framesPerSecond = g_framesPerSecond;
    RecoveryCount = (long)(framesPerSecond * GetRecoveryTime());
    if (ParentThing->m_state == 1)
        ParentThing->SetSuspendAnimation(true, 0);
    long emitterType = GetEmitterType();
    if (emitterType >= 0 && (ParentThing->m_flags6c & 8)) {
        C3DVector position = GetHeadPosWithOffset(ParentThing);
        PEmitter = CIntelligentPointerThing(CreateEffectThing(emitterType, &position, false));
    }
    StartOutOfActionAnim();
    CModeStandBase::OnActivate();
}

}
