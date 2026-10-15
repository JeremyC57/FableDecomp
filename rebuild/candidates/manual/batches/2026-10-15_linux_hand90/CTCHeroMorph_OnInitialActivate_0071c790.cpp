// CTCHeroMorph::OnInitialActivate() -- marks the hero morph changed, builds the custom tattoos and uploads the texture morphs to the engine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071C790, 39 bytes (+9 int3 padding). CTCHeroMorph vtable 0x01265CF4 slot 6.
// Callees: CTCHeroMorph::InitialiseCustomTattoos() @ 0x0071C360,
//          CTCAppearanceMorphBase::UploadTextureMorphsToEngine(CIEngine*, set<long>*, bool, float) @ 0x006CA7F0.
// Globals: the application object @ 0x013B86A0 (+0x28 display, its +0x2C the CIEngine).
//
// Shape: the Changed byte stored before the first call; the engine chain loaded through eax/ecx/edx and pushed
// after the three zero immediates (null set, false, 0.0f -- a float literal 0 is `push 0`).
// Layout: Changed is +0x3D in retail (PDB +0x55): the two particle-emitter lists ahead of it are 4-byte head
// pointers (PDB 12) and the Tattoos vector / CustomToBankIndex map are 4 bytes smaller each; a local model.

struct CIEngine;
struct CDisplay_Retail {
    unsigned char _pad_0x00[0x2C];
    CIEngine*     m_pEngine;                 // +0x2C
};
struct CApplication_Retail {
    unsigned char    _pad_0x00[0x28];
    CDisplay_Retail* m_pDisplay;             // +0x28
};
extern CApplication_Retail* g_pApplication;  // 0x013B86A0

struct CLongSet;                             // std::set<long>

struct CTCHeroMorph_Retail {
    void*         __vftable;                 // +0x00
    unsigned char _pad_0x04[0x39];           // +0x04 CTCBase, mesh morph set, emitter lists, tattoos, ...
    bool          Changed;                   // +0x3D (PDB +0x55)

    void UploadTextureMorphsToEngine(CIEngine* engine, CLongSet* morphs, bool force, float blend);   // 0x006CA7F0
    void InitialiseCustomTattoos();          // 0x0071C360
    void OnInitialActivate();
};

void CTCHeroMorph_Retail::OnInitialActivate()
{
    Changed = true;
    InitialiseCustomTattoos();
    UploadTextureMorphsToEngine(g_pApplication->m_pDisplay->m_pEngine, 0, false, 0.0f);
}
