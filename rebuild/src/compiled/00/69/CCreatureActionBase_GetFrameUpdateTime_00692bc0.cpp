// CCreatureActionBase::GetFrameUpdateTime() const -- the creature's animation speed multiplier times one frame (1 / frame rate)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692BC0, length 36 bytes. Vtable slot 62 of the CCreatureActionBase family
// (rtti-vtables.tsv lists it through CCreatureAction_StartSneaking, 0x01258DFC).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThingCreatureBase::GetAnimationSpeedMultiplier
// @ 0x006655A0. Globals: the long frame rate @ 0x01375550 (`fild`), the float 1.0f @ 0x0122DED8 (`fdivr`: 1.0f / rate).
// Shape: the frame time is a named float local (`fstp [esp]` into the one-dword frame before the call -- a named float
// local for a value that must survive a call, GOTCHAS), then the call result is multiplied by it (`fmul [esp]`).
class CThingCreatureBase_Calls { public:         // engine/CThingCreatureBase.h exists but is not used: only this call is modelled
    float GetAnimationSpeedMultiplier() const;   // 0x006655A0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

extern long g_frameRate;                    // 0x01375550: frames per second (integer)

// engine/CCreatureActionBase.h exists but is not used: it is the PDB build's flat pack(1) layout (0xb4 bytes, three
// containers 4 bytes wider than retail's 0xa8, no vtable or methods); the retail `this` is modelled locally.
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThingCreatureBase_Calls> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    float GetFrameUpdateTime() const;
};

float CCreatureActionBase::GetFrameUpdateTime() const
{
    float frameTime = 1.0f / (float)g_frameRate;
    return PCreature.GetPItem()->GetAnimationSpeedMultiplier() * frameTime;
}