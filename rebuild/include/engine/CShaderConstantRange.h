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
    CShaderConstantRange(const CShaderConstantRange& other);             // ??0CShaderConstantRange@@QAE@ABV0@@Z
    virtual ~CShaderConstantRange();                                     // ??1CShaderConstantRange@@UAE@XZ
    CShaderConstantRange& operator=(const CShaderConstantRange& other);  // ??4CShaderConstantRange@@QAEAAV0@ABV0@@Z
    bool operator!=(const CShaderConstantRange& other) const;            // ??9CShaderConstantRange@@QBE_NABV0@@Z
    long GetEnd() const;                                                 // ?GetEnd@CShaderConstantRange@@QBEJXZ
    long          Offset;                                                // +0x04
    unsigned long Count;                                                 // +0x08
};
FABLE_STATIC_ASSERT(sizeof(CShaderConstantRange) == 0xc);
#endif // FABLE_ENGINE_CSHADERCONSTANTRANGE_H
