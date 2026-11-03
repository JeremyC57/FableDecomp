#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUTEMPTY_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUTEMPTY_H
#include "rebuild_abi.h"
#include "engine/CVertexShaderConstantLayout.h"
// New 2026-11-03 (Codex pass 13 row 0x009DA9F0; Ego_r.pdb / FableWin.pdb declarations, Codex evidence/009da9f0_*_types.json).
class CVertexShaderConstantLayoutEmpty : public CVertexShaderConstantLayout {
public: virtual CCharString PeekName() const;
public: virtual ~CVertexShaderConstantLayoutEmpty();
public:CVertexShaderConstantLayoutEmpty();
};
#endif
