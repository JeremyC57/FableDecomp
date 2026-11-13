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
private:
    unsigned char _body[0x1e60 - 0x10];
};
#endif
