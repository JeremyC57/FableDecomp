// CTCAICreatureWillPowerIndicator::Reset(float) -- restart the recharge timer (a negative time means the def's RechargeTime, 5 s without a def) and kill the power-up emitter
// Grade: DIFFER(ours v retail) -- inline null-tested def release: ours mov edx,[eax+4];add eax,4;dec edx;test;mov [eax],edx where retail has dec [eax+4];jne (retry-8 lane gap), plus mov eax vs mov ecx for the first ParentThing load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0E70, length 168 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef> @ 0x007E1230 (on the thing's def, +0x70),
// the def's virtual Delete (slot 1) when its reference count drops to zero, __ftol2 @ 0x00BFEA70,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80.
// Globals: 0.0f pool 0x0122DEDC, the frames-per-second counter 0x01375550.
// Layout = Ego_r.pdb CTCAICreatureWillPowerIndicator (36 bytes): CTCBase {vptr, ParentThing +4, flags +8},
// PoweredUp +0xC, Timer +0x10, ResetCount +0x14, ShowEffect +0x18, PEmitter +0x19 (unaligned: the intelligent
// pointer is declared packed). CAICreatureWillPowerIndicatorDef::RechargeTime is +0x28.
// Shape: the time parameter itself is overwritten with the def value (`mov [esp+0Ch],ecx`); the def is released
// by an inline holder taken from the out pointer once, so its object stays in eax on both arms (the null-def
// arm knows it is zero); `(long)(time * fps)` loads the integer global first; the emitter test is the inline
// IsThingAlive (GetPItem, null test, Dead bit 0x91) before the third GetPItem call for Kill(true).
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(pop)
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {          // the def taken out of the out pointer, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
struct CAICreatureWillPowerIndicatorDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    float RechargeTime;                         // +0x28
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out) const;   // 0x007E1230
};
struct CThingWithDef {
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                        // +0x70
};
extern long g_FramesPerSecond;                  // 0x01375550

class CTCAICreatureWillPowerIndicator {
public:
    virtual void v00();
    CThingWithDef* ParentThing;                 // +0x04
    unsigned char  _flags08[4];                 // +0x08 CTCBase flags
    bool  PoweredUp;                            // +0x0c
    long  Timer;                                // +0x10
    long  ResetCount;                           // +0x14
    bool  ShowEffect;                           // +0x18
    CBaseIntelligentPointer PEmitter;           // +0x19
    void Reset(float time);
};

void CTCAICreatureWillPowerIndicator::Reset(float time)
{
    if (time < 0.0f) {
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CAICreatureWillPowerIndicatorDef> out;
        if (thingDef != 0 && thingDef->GetSubDef(&out))
            time = out.Object->RechargeTime;
        else
            time = 5.0f;
        CAICreatureWillPowerIndicatorDef* def = out.Object;
        if (def && --def->m_refCount == 0)
            def->Delete();
    }
    Timer = (long)(time * g_FramesPerSecond);
    PoweredUp = false;
    ++ResetCount;
    if (IsThingAlive(PEmitter))
        PEmitter.GetPItem()->Kill(true);
}
