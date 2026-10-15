// CTCFadeOutAndIn::FinalCleanup() -- a fade still in progress is finished at once: one frame left, then one more FrameUpdate
// Leaf: FinalCleanup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3B50, length 27 bytes (vtable 0x0125DA0C slot 2, the CTCBase
// "FinalCleanup" slot; the next override in the same vtable, slot 9 = 0x006C3F40, is FrameUpdate).
// Layout: PDB CTCFadeOutAndIn (40 bytes): FadingOut +0xc, FadingIn +0xd, Alpha +0x10, FramesLeftForFade +0x14.
// Shape: the inlined IsFading (`FadingIn || FadingOut`, 0x0049D310) folded into the `if`; the virtual FrameUpdate on
// `this` is the last statement, so it becomes `mov eax,[ecx] ; ... ; jmp dword ptr [eax+24h]` with the frame-count
// store scheduled between. engine/CTCBase.h is a flat struct, so the polymorphic base is modelled locally (slots 0..9).
class CThing;

class CTCBase_Retail {
public:
    virtual void v00();
    virtual void OnSerialise();                    // slot 1
    virtual void FinalCleanup();                   // slot 2
    virtual void v03();
    virtual void OnCreate();                       // slot 4
    virtual void v05();
    virtual void OnInitialActivate();              // slot 6
    virtual void v07();
    virtual void v08();
    virtual void FrameUpdate();                    // slot 9 (+0x24)
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

class CTCFadeOutAndIn : public CTCBase_Retail {
public:
    bool  FadingOut;                               // +0x0c
    bool  FadingIn;                                // +0x0d
    float Alpha;                                   // +0x10
    long  FramesLeftForFade;                       // +0x14
    bool IsFading() const { return FadingIn || FadingOut; }
    virtual void FinalCleanup();
};

void CTCFadeOutAndIn::FinalCleanup()
{
    if (IsFading()) {
        FramesLeftForFade = 1;
        FrameUpdate();
    }
}
