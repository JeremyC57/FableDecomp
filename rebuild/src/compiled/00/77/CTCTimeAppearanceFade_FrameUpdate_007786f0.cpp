// CTCTimeAppearanceFade::FrameUpdate() -- fades the owner's graphic appearance with the time of day: opaque between fade-in end and fade-out start, ramping up across the fade-in window and down across the fade-out window, invisible otherwise
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007786F0, 252 bytes (plain `ret`).
// Callees: CDefClassBase::GetSubDef<CTimeAppearanceFadeDef>(CDefPointer<const CTimeAppearanceFadeDef>*) 0x00778900 (on the
//          owner's def class, owner +0x70; this TU, see the stand-in below; manifest label "<CAICreatureWillPowerIndicatorDef>"
//          is propagated); CTCBase::GetWorld() 0x00686A70; GFGetValPositionInLimitsLooping(float, float, float, float,
//          float, float*) 0x00778610 (`__fastcall`: the out position in ecx, five floats on the stack, `ret 14h`);
//          __ftol2 0x00BFEA70; NGraphicAppearance::CTCGraphicAppearance::SetAlpha(unsigned char) 0x004BFAC0; the def's
//          slot 1 on release. Constants: 255.0f 0x01230014, 1.0f 0x0122DED8.
// Layout (retail): CThing PTCGraphicAppearance +0x64 (PDB +0x74), def class +0x70; CWorld::PGameTimeManager +0x1c,
//          CGameTimeManager::Time +0x08; CTimeAppearanceFadeDef FadeInStart +0x28, FadeInEnd +0x2c, FadeOutStart +0x30,
//          FadeOutEnd +0x34 (PDB offsets hold).
// Shape: the def class is fetched into a named local before the def pointer (retail loads it ahead of the sunk `push esi`); the time is a named float local (its frame word is written through eax after the pushes) and the position a
//        zero-initialised local homed in the dead def-out slot; the three window tests are an if/else chain whose
//        arms each push the alpha and join one SetAlpha call; the def release keeps its null test.
class CThing;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);         // 0x004BFAC0
};
}
// Stand-in body (the real 0x00778610 is in this TU, out of line, and also handles a window that wraps past maxLimit): it
// only touches eax/ecx and the x87 stack like the retail body, so VC7.1 knows edx survives the call (retail keeps the
// time in edx across all three calls).
#pragma auto_inline(off)
bool __fastcall GFGetValPositionInLimitsLooping(float value, float start, float end, float minLimit, float maxLimit,
                                                float* position)   // 0x00778610
{
    if (value < start || value > end)
        return false;
    *position = (value - start) / (end - start);
    return true;
}
#pragma auto_inline(on)

class CTimeAppearanceFadeDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          m_count;                      // +0x04
    unsigned char _pad_0x08[0x20];
    float         FadeInStart;                  // +0x28
    float         FadeInEnd;                    // +0x2c
    float         FadeOutStart;                 // +0x30
    float         FadeOutEnd;                   // +0x34
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CFadeDefPointer {                        // CDefPointer<const CTimeAppearanceFadeDef>
    CTimeAppearanceFadeDef* Object;
    CFadeDefPointer() : Object(0) {}
    ~CFadeDefPointer() { if (Object) Object->Release(); }
    CTimeAppearanceFadeDef* operator->() const { return Object; }
};

class CDefClassBase { public:
    bool GetSubDef(CFadeDefPointer* out);       // 0x00778900
};
CTimeAppearanceFadeDef* FindRef_CDefClassBase_GetSubDef_0(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (the real 0x00778900 is in this TU, out of line): it only writes the def through the out parameter, so
// VC7.1 knows the caller's address-taken pointer does not escape the call.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CFadeDefPointer* out)
{
    CTimeAppearanceFadeDef* def = FindRef_CDefClassBase_GetSubDef_0(this);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char   _pad_0x00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char   _pad_0x68[0x8];
    CDefClassBase*  DefClass;                   // +0x70
};
class CGameTimeManager { public:
    void*  World;                               // +0x00
    void*  GameComponent;                       // +0x04
    float  Time;                                // +0x08
};
class CWorld { public:
    unsigned char     _pad_0x00[0x1c];
    CGameTimeManager* PGameTimeManager;         // +0x1c
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                         // 0x00686A70
};
struct CTCTimeAppearanceFade : CTCBase {
    virtual void FrameUpdate();
};

void CTCTimeAppearanceFade::FrameUpdate()
{
    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
    if (!appearance)
        return;
    CDefClassBase* defClass = ParentThing->DefClass;
    CFadeDefPointer def;
    if (defClass->GetSubDef(&def)) {
        float time = GetWorld()->PGameTimeManager->Time;
        float position = 0.0f;
        if (GFGetValPositionInLimitsLooping(time, def->FadeInEnd, def->FadeOutStart, 0.0f, 1.0f, &position))
            appearance->SetAlpha(0xff);
        else if (GFGetValPositionInLimitsLooping(time, def->FadeInStart, def->FadeInEnd, 0.0f, 1.0f, &position))
            appearance->SetAlpha((unsigned char)(long)(position * 255.0f));
        else if (GFGetValPositionInLimitsLooping(time, def->FadeOutStart, def->FadeOutEnd, 0.0f, 1.0f, &position))
            appearance->SetAlpha((unsigned char)(long)((1.0f - position) * 255.0f));
        else
            appearance->SetAlpha(0);
    }
}