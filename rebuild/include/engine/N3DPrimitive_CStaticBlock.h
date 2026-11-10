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
class CBaseBlock {public:unsigned long PrimitiveCount,StartIndex;bool IsStrip,DegenerateTriangles;unsigned char ChangeFlags;};
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
