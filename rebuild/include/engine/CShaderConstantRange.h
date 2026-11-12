#ifndef FABLE_ENGINE_CSHADERCONSTANTRANGE_H
#define FABLE_ENGINE_CSHADERCONSTANTRANGE_H
// CShaderConstantRange: one retail model (chunk 723b; Codex pass 13 agrees: Ego_r TPI 0x40B7D). Ego_r.pdb class; RTTI CShaderConstantRange : CBaseClass, polymorphic (??1CShaderConstantRange@@UAE@XZ, ??_E), 0xC bytes: vptr,
// long Offset +4, unsigned long Count +8. Constructors / operators from the Ego_r.pdb name set.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
class CShaderConstantRange : public CBaseClass {
public:
    CShaderConstantRange();                                              // ??0CShaderConstantRange@@QAE@XZ
    CShaderConstantRange(long offset, unsigned long count);              // ??0CShaderConstantRange@@QAE@JK@Z
#ifdef FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY
    // Retail opt-in (chunk F2, row 0x00BCFB00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer): the copy
    // constructor and the destructor are the implicit ones, as the row's expanded GetBoneMatrices() temporary shows:
    // `mov [tmp],offset vftable ; Offset ; Count` with no CBaseClass constructor call (0x00BCFB79), then only the base destructor
    // ~CBaseClass 0x0099A2E0 with no vftable store ahead of it (0x00BCFBAE; a user-written empty body re-stores it). No layout change.
#else
    CShaderConstantRange(const CShaderConstantRange& other);             // ??0CShaderConstantRange@@QAE@ABV0@@Z
    virtual ~CShaderConstantRange();                                     // ??1CShaderConstantRange@@UAE@XZ
#endif
    CShaderConstantRange& operator=(const CShaderConstantRange& other);  // ??4CShaderConstantRange@@QAEAAV0@ABV0@@Z
    bool operator!=(const CShaderConstantRange& other) const;            // ??9CShaderConstantRange@@QBE_NABV0@@Z
    long GetEnd() const;                                                 // ?GetEnd@CShaderConstantRange@@QBEJXZ
    long          Offset;                                                // +0x04
    unsigned long Count;                                                 // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CShaderConstantRange) == 0xc);
#endif // FABLE_ENGINE_CSHADERCONSTANTRANGE_H
