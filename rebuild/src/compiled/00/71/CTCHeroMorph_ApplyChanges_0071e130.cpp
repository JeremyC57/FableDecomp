// CTCHeroMorph::ApplyChanges() -- when the morph changed: tells the hero thing (CMessageOnMorphChanged), rebuilds the texture morphs and flags the graphic appearance
// Leaf: ApplyChanges
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071E130, 77 bytes (+3 int3 padding). CTCHeroMorph vtable 0x01265CF4
// slot 28 (manifest CTCHeroMorph::vfunc_28; the CTCAppearanceMorphBase default in that slot, 0x0071B7A0, is an empty `ret`).
// Callees: CBaseClass::CBaseClass() @ 0x0099A2D0 / CBaseClass::~CBaseClass() @ 0x0099A2E0 (the message's root base),
//          CThing::SendMessage(CMessageBase*) @ 0x004C7820, CTCHeroMorph::UpdateTextureMorphs() @ 0x0071DE80,
//          CTCGraphicAppearance::SetAsChanged(unsigned long) @ 0x004BF9E0.
// Vtable: NTCMessages::CMessageOnMorphChanged @ 0x01265BB0.
//
// Shape: a one-word `push ecx` frame holding the message; its base constructor runs first, the parent thing is
// loaded into ecx and the message address pushed BEFORE the derived vtable store (the inline constructor's store
// sinks to just before the call); right after the call only the out-of-line base destructor runs (the message's
// implicit destructor stores nothing) -- the message lives in its own inner block (declared in the `if` body its
// destructor sinks to the end of the block, after SetAsChanged). Then UpdateTextureMorphs(), ParentThing->+0x64 (graphic appearance)->SetAsChanged(8)
// and the Changed byte cleared.
// Layout: Changed is +0x3D in retail (PDB +0x55, see OnInitialActivate 0x0071C790); a local model.

class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};

struct CMessageOnMorphChanged : CBaseClass {       // NTCMessages::CMessageOnMorphChanged
    CMessageOnMorphChanged() {}
};

struct CTCGraphicAppearance {
    void SetAsChanged(unsigned long flags);   // 0x004BF9E0
};

class CThing { public:
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    void SendMessage(CBaseClass* message);         // 0x004C7820
};

struct CTCHeroMorph_Retail {
    void*         __vftable;                  // +0x00
    CThing*       ParentThing;                // +0x04 CTCBase
    unsigned char _pad_0x08[0x35];            // +0x08
    bool          Changed;                    // +0x3D (PDB +0x55)

    void UpdateTextureMorphs();               // 0x0071DE80
    void ApplyChanges();
};

void CTCHeroMorph_Retail::ApplyChanges()
{
    if (Changed) {
        {
            CMessageOnMorphChanged message;
            ParentThing->SendMessage(&message);
        }
        UpdateTextureMorphs();
        ParentThing->m_pGraphicAppearance->SetAsChanged(8);
        Changed = false;
    }
}