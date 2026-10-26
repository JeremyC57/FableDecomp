// CTCAICreatureWillPowerIndicator::Reset(float) -- restart the recharge timer (a negative time means the def's RechargeTime, 5 s without a def) and kill the power-up emitter
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
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
};
#pragma pack(push, 1)
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
#pragma pack(pop)
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1 (+0x04)
    long m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
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
class CAICreatureWillPowerIndicatorDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    float RechargeTime;                         // +0x28
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out) const;   // 0x007E1230
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
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
    CIntelligentPointer<CThing> PEmitter;           // +0x19
    void Reset(float time);
};

void CTCAICreatureWillPowerIndicator::Reset(float time)
{
    if (time < 0.0f) {
        CDefPointer<CAICreatureWillPowerIndicatorDef> out;
        CDefClassBase* thingDef = ParentThing->PDef;
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