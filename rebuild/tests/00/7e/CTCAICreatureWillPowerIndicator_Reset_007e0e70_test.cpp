// Behaviour test for 007e0e70: src/007e0e70.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCAICreatureWillPowerIndicator::Reset(float)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(pop)
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

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
    CBaseIntelligentPointer PEmitter;           // +0x19
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

// ---- stubs and driver ----
static void* g_def = 0; static int g_deleted = 0, g_kill = 0; static bool g_killArg = false;
static CThing* g_item = 0;
void* FindDef_CDefClassBase_GetSubDef_0() { return g_def; }
void CDefPointeeBase::Destroy() {}
void CDefPointeeBase::Delete() { ++g_deleted; }
long g_FramesPerSecond = 30;
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
void CThing::Kill(bool immediately) { ++g_kill; g_killArg = immediately; }
void CTCAICreatureWillPowerIndicator::v00() {}
int main()
{
    CAICreatureWillPowerIndicatorDef def; def.m_refCount = 3; def.RechargeTime = 2.0f; g_def = &def;
    CDefClassBase base;
    CThingWithDef owner; memset(&owner, 0, sizeof(owner)); owner.PDef = &base;
    CThing emitter; memset(&emitter, 0, sizeof(emitter));
    CTCAICreatureWillPowerIndicator w; w.ParentThing = &owner; w.PoweredUp = true; w.Timer = 0; w.ResetCount = 0; w.ShowEffect = false;
    w.PEmitter.__vftable = 0; w.PEmitter.m_pItem = 0;
    // explicit time: no def lookup, no emitter
    w.Reset(1.5f); CHECK(w.Timer == 45); CHECK(!w.PoweredUp); CHECK(w.ResetCount == 1); CHECK(def.m_refCount == 3); CHECK(g_kill == 0);
    // negative time: the def's recharge time, def released
    w.PoweredUp = true; w.Reset(-1.0f); CHECK(w.Timer == 60); CHECK(w.ResetCount == 2); CHECK(def.m_refCount == 2);
    // no sub-def: 5 seconds
    g_def = 0; w.Reset(-1.0f); CHECK(w.Timer == 150); CHECK(def.m_refCount == 2);
    // no thing def: 5 seconds
    owner.PDef = 0; g_def = &def; w.Reset(-2.0f); CHECK(w.Timer == 150); CHECK(def.m_refCount == 2);
    // last reference deleted; a live emitter is killed, a dead one is not
    owner.PDef = &base; def.m_refCount = 1; w.PEmitter.m_pItem = &emitter; w.Reset(-1.0f);
    CHECK(g_deleted == 1); CHECK(g_kill == 1); CHECK(g_killArg);
    emitter.Flags91 = 1; w.Reset(0.0f); CHECK(g_kill == 1); CHECK(w.Timer == 0); CHECK(w.ResetCount == 6);
    if (fails == 0) printf("007e0e70_TEST PASS\n"); else printf("007e0e70_TEST FAIL\n");
    return fails;
}