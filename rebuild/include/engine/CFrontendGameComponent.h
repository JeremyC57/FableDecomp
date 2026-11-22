#ifndef FABLE_ENGINE_CFRONTENDGAMECOMPONENT_H
#define FABLE_ENGINE_CFRONTENDGAMECOMPONENT_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). The developer front end (allocation 0x1E60, 0x0042F258). Ego_r.pdb
// ??0CFrontendGameComponent@@QAE@AAVCGame@@ABVCFrontendGameComponentInit@@@Z (retail 0x00496070); CFrontendGameComponentInit is
// Ego_r.pdb's one-long struct (Nowt +0x00) with the inline default ctor ??0CFrontendGameComponentInit@@QAE@XZ (Run 0x0042F270 stores
// the zero in place after the null test of the allocation); the body (Nowt = 0) is FITTED.
#include "engine/CGameComponent.h"
class CFrontendGameComponentInit {
public:
    CFrontendGameComponentInit() : Nowt(0) { }
    long Nowt;                                                                // +0x00
};
class CFrontendGameComponent : public CGameComponent {
public:
    CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init);   // 0x00496070
    virtual bool Run(CGameComponent** next_component);
    virtual void ChangeTextureColourDepth(long depth);
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
#ifdef FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETPROFILENAMES
    // Retail opt-in (row 0x004959E0): the static ?GetProfileNames@CFrontendGameComponent@@... (the row's Leaf line has the full
    // symbol), `__fastcall` with the vector in ecx. Declaration only. The includer has std::vector and CWideString in scope.
    static void __fastcall GetProfileNames(std::vector<CWideString>& names);   // 0x004959E0
#endif
#ifdef FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETSAVENAMES
    // Retail opt-in (NF197, 2026-11-20, row 0x00495830): Ego_r.pdb 0x0049E440 + FableWin.pdb 0x01A46960
    // ?GetSaveNames@CFrontendGameComponent@@SIXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z (public static
    // __fastcall, the vector in ecx; SetMode 0x00495FE0 passes &SaveNames +0x1E1C). Declaration only; the includer has std::vector
    // and CWideString in scope.
    static void __fastcall GetSaveNames(std::vector<CWideString>& names);      // 0x00495830
#endif
#ifdef FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING
    // Retail opt-in (NF191, row 0x004944C0): Ego_r.pdb 0x0049D0E0 + FableWin.pdb 0x01A46A80 ?ClearString@CFrontendGameComponent@@AAEXXZ
    // (private). Ego_r.pdb layout: char String[128] +0x168, long StringPos +0x1E8; the rest stays opaque. Same size as the default path.
private:
    void ClearString();                                                      // 0x004944C0
    unsigned char _body_010[0x168 - 0x10];
    char String[128];                                                         // +0x168
    long StringPos;                                                           // +0x1E8
    unsigned char _body_1ec[0x1e60 - 0x1ec];
};
#elif defined(FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS)
    // Retail opt-in (NF226, 2026-11-22, q12568; row 0x00494840): Ego_r.pdb 0x0049D3B0 + FableWin.pdb 0x01A41C50
    // ?InitGraphics@CFrontendGameComponent@@AAE_NXZ (private). Ego_r.pdb layout: CCountedPointer<const CFontBank> MainFont +0x1E04
    // (retail InitGraphics stores +0x1E04 / +0x1E08); the rest stays opaque. Same size as the default path. The includer includes
    // engine/CCountedPointer.h first. Not with FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING (one layout branch per TU).
private:
    bool InitGraphics();                                                      // 0x00494840
    unsigned char _body_010[0x1e04 - 0x10];
    CCountedPointer<const class CFontBank> MainFont;                          // +0x1E04
    unsigned char _body_1e0c[0x1e60 - 0x1e0c];
};
#else
private:
    unsigned char _body[0x1e60 - 0x10];
};
#endif
#endif
