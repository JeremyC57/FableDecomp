#ifndef FABLE_ENGINE_NUISYSTEM_CUISCREENUNITCONVERTER_H
#define FABLE_ENGINE_NUISYSTEM_CUISCREENUNITCONVERTER_H
// NUISystem::CUIScreenUnitConverter (chunk 735, row 0x00577AC0): FableWin.pdb ?GetBackBufferWidth@CUIScreenUnitConverter@
// NUISystem@@SIMXZ and ?GetBackBufferHeight@CUIScreenUnitConverter@NUISystem@@SIMXZ (static __fastcall, float, no
// arguments). Retail 0x0041CC14 / 0x0041CC2B are the two 23-byte static float getters (a byte flag, then one of two float
// globals) next to CManager::GetUIScale 0x0041CF47; the row scales the first with GetUIScale().X (centre x) and the second
// with GetUIScale().Y (centre y). Both landed under the propagated label CTCLook::GetHeadAngularVelocityMaxXY.
namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static float __fastcall GetBackBufferWidth();      // 0x0041CC14
    static float __fastcall GetBackBufferHeight();     // 0x0041CC2B
};
}
#endif
