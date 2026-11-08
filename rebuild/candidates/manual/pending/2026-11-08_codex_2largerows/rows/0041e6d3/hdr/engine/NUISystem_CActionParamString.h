#ifndef FABLE_ENGINE_NUISYSTEM_CACTIONPARAMSTRING_H
#define FABLE_ENGINE_NUISYSTEM_CACTIONPARAMSTRING_H
// NUISystem::CActionParamString (new shared header, chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): the wide-string action
// parameter. FableWin.pdb ??0CActionParamString@NUISystem@@QAE@AAVCWideString@@@Z; Ego_r.pdb layout (struct_layouts_egor.tsv,
// 12 bytes): the CActionParam base (vptr +0, ActionParamType +4) and CWideString Value +8. Action reads Value in place
// (`add ecx,8 ; call ?ToCharString@CWideString@@QBE?AVCCharString@@XZ`, 0x0041F926 PLAY_MOVIE, 0x0041EEFF TAKE_BOAST).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
class CActionParamString : public CActionParam {
public:
    CActionParamString(CWideString& value);
    virtual ~CActionParamString();
    CWideString Value;                                                // +0x08
};
}
FABLE_STATIC_ASSERT(sizeof(NUISystem::CActionParamString) == 0xc);
#endif
