#ifndef FABLE_ENGINE_N3DPRIMITIVE_CSTATICBLOCK_H
#define FABLE_ENGINE_N3DPRIMITIVE_CSTATICBLOCK_H
#include "rebuild_abi.h"
#include <stddef.h>
class CTexture;
#ifdef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
namespace N3DPrimitive {
class CBaseBlock {
public:
    unsigned long GetChangeFlags() const { return ChangeFlags; }
private:
    unsigned long PrimitiveCount,StartIndex;
    bool IsStrip,DegenerateTriangles;
    unsigned char ChangeFlags;
};
class CStaticBlock : public CBaseBlock {
public:
    unsigned long DiffuseTextureFixupOffset,BumpTextureFixupOffset;
    const CTexture* DiffuseTexture;
    const CTexture* BumpTexture;
    long GetMaterialIndex() const { return MaterialIndex; }
private:
    long MaterialIndex;
};
}
#else
namespace N3DPrimitive {
// PDB complete base12/derived32 fields; native indexed stride32 and flags+a.
class CBaseBlock {public:unsigned long PrimitiveCount,StartIndex;bool IsStrip,DegenerateTriangles;unsigned char ChangeFlags;
#ifdef FABLE_RETAIL_CBASEBLOCK_ACCESSORS
    // Retail opt-in (chunk F2, row 0x00BCFB00): FableWin.pdb ?GetPrimitiveCount@CBaseBlock@N3DPrimitive@@QBEKXZ, ?GetStartIndex@..@@QBEKXZ
    // and ?GetPrimitiveType@..@@QBE?AW4EPrimitiveType@@XZ (inline in retail: the row reads +0 / +4 and forms 4 + (IsStrip != 0)).
    // The includer supplies engine/EPrimitiveType.h. No layout change.
    unsigned long GetPrimitiveCount() const { return PrimitiveCount; }
    unsigned long GetStartIndex() const { return StartIndex; }
    EPrimitiveType GetPrimitiveType() const { return IsStrip ? PT_TRIANGLE_STRIP : PT_TRIANGLE_LIST; }
#endif
#ifdef FABLE_RETAIL_CBASEBLOCK_SAVEBINARY
    // Retail opt-in (naming agent NF21, 2026-11-11, row 0x00A8E6C0): Ego_r.pdb 0x00BD0F70 / FableWin.pdb 0x032FECF0
    // ?SaveBinary@CBaseBlock@N3DPrimitive@@QAEXAAVCDataOutputStream@@@Z (public). Declaration only; no layout change.
    void SaveBinary(class CDataOutputStream& stream);   // 0x00A8E6C0
#endif
};
class CStaticBlock:public CBaseBlock {public:unsigned long DiffuseTextureFixupOffset,BumpTextureFixupOffset;CTexture* DiffuseTexture;CTexture* BumpTexture;long MaterialIndex;};
}
#endif
FABLE_STATIC_ASSERT(sizeof(N3DPrimitive::CBaseBlock)==12);
FABLE_STATIC_ASSERT(sizeof(N3DPrimitive::CStaticBlock)==32);
#ifndef FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
FABLE_STATIC_ASSERT(offsetof(N3DPrimitive::CStaticBlock,ChangeFlags)==0x0a);
#endif
FABLE_STATIC_ASSERT(offsetof(N3DPrimitive::CStaticBlock,DiffuseTexture)==0x14);
FABLE_STATIC_ASSERT(offsetof(N3DPrimitive::CStaticBlock,BumpTexture)==0x18);
#endif
