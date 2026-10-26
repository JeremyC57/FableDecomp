// CTCFadeOutAndIn::FrameUpdate() -- step the fade one frame: move Alpha towards 1 (fading in) or 0 (fading out) and push the new alpha / drawable state to every fading thing
// Leaf: FrameUpdate
//
// Retail 0x006C3F40, 265 bytes (CTCFadeOutAndIn vtable 0x0125DA0C slot 9; the catalog calls it vfunc_9, the landed
// FinalCleanup 0x006C3B50 calls the same slot as FrameUpdate). Callees: CThing::RemoveTC(long) @ 0x004C9840 (label
// "CBaseObject::OnPropertyChanged" is propagated), CTCFadeOutAndIn::UpdateFadingThings() @ 0x006C3E80 (Ego_r.pdb
// `?UpdateFadingThings@CTCFadeOutAndIn@@AAEXXZ`, private; landed under that name -- the earlier descriptive
// "RemoveDeadFadingThings" declaration bound a symbol nothing defines, FINDINGS chunk 386), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, __ftol2 @ 0x00BFEA70,
// CTCGraphicAppearance::SetAlpha(unsigned char) @ 0x004BFAC0, CThing::SetAsDrawable(bool) (virtual slot 28, +0x70).
// Floats: 0x0122DED8 = 1.0f, 0x0122DEE0 = -1.0f, 0x01230014 = 255.0f, 0x0129BA3C = 1e-4f (an extern variable).
// Layout = Ego_r.pdb CTCFadeOutAndIn (0x28): FadingOut +0xc, FadingIn +0xd, Alpha +0x10, FramesLeftForFade +0x14,
// FadingThings +0x18 (12-byte STLport vector of 8-byte CIntelligentPointer<CThing>); CThing graphic appearance +0x64.
// Shape: the remaining-frames count is a local (fild from the frame word); the fade amount and direction are two
// float locals set in an if/else and combined as `Alpha += direction * (amount / frames)` (the parenthesised quotient keeps retail's x87 order); the loop reads the end
// pointer every pass and calls GetPItem afresh for each use.
class CThing;

struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);                     // 0x004BFAC0
};

class CThing {
public:
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27)
#undef V
    virtual void SetAsDrawable(bool drawable);              // slot 28 (+0x70)
    unsigned char _pad04[0x64 - 0x04];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    void RemoveTC(long interfaceType);                      // 0x004C9840
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

template <class T> struct SRetailAllocProxy {               // STLport _STLP_alloc_proxy
    T* _M_data;
};
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
};

extern float g_epsilon_0129ba3c;                            // 0x0129BA3C (1e-4f)

class CTCBase_Retail {
public:
    virtual void v00();
    virtual void OnSerialise();                             // slot 1
    virtual void FinalCleanup();                            // slot 2
    virtual void v03();
    virtual void OnCreate();                                // slot 4
    virtual void v05();
    virtual void OnInitialActivate();                       // slot 6
    virtual void v07();
    virtual void v08();
    virtual void FrameUpdate();                             // slot 9 (+0x24)
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCFadeOutAndIn : public CTCBase_Retail {
public:
    bool  FadingOut;                                        // +0x0c
    bool  FadingIn;                                         // +0x0d
    float Alpha;                                            // +0x10
    long  FramesLeftForFade;                                // +0x14
    CRetailVector<CIntelligentPointer<CThing> > FadingThings;    // +0x18
    virtual void FrameUpdate();
private:
    void UpdateFadingThings();                              // 0x006C3E80
};

void CTCFadeOutAndIn::FrameUpdate()
{
    if (ParentThing->PGraphicAppearance == 0) {
        FadingOut = false;
        FadingIn = false;
        ParentThing->RemoveTC(0xf9);
        return;
    }
    UpdateFadingThings();
    if (!FadingOut && !FadingIn)
        return;
    long frames = FramesLeftForFade;
    if (frames <= 0) {
        if (FadingIn) {
            FadingIn = false;
            ParentThing->RemoveTC(0xf9);
            return;
        }
        FadingOut = false;
        return;
    }
    float amount;
    float direction;
    if (FadingIn) {
        amount = 1.0f - Alpha;
        direction = 1.0f;
    } else {
        amount = Alpha;
        direction = -1.0f;
    }
    FramesLeftForFade = frames - 1;
    Alpha += direction * (amount / frames);
    for (CIntelligentPointer<CThing>* it = FadingThings._M_start; it != FadingThings._M_finish; ++it) {
        CTCGraphicAppearance* appearance = it->GetPItem()->PGraphicAppearance;
        if (appearance) {
            appearance->SetAlpha((unsigned char)(Alpha * 255.0f));
            it->GetPItem()->SetAsDrawable(Alpha > g_epsilon_0129ba3c ? true : false);
        }
    }
}