#ifndef FABLETLC_OUTLINE_GLOW_COMPILED_H
#define FABLETLC_OUTLINE_GLOW_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

#ifdef FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER
// Retail opt-in (NF269, 2026-11-22; rows 0x00BEAAA0 / 0x00BEB4D0): reach the landscape memory manager the way the landed L129 rows do,
// through NEngineInternal::GPLandscapeRenderer (0x01436EA8, ?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A) ->
// MemoryManager (+0x6B0), on the retail branch of engine/CEngineLandscapeMemoryManager.h: FreeVirtual 0x00BD9B90 and the public
// DeleteVertexBuffer 0x00BDA0D0 (?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z, Ego_r.pdb 009c2640 /
// FableWin.pdb 02cb7450), in place of the stand-ins DAT_01436ea8 / LandscapeLayerList::InsertNode(int) / the local manager below. The
// member at +0x10 is the vertex buffer it frees (CWaterPatchMesh::VertexBuffer +0x10, engine/CWaterPatchMesh.h).
#ifndef FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#endif
#ifndef FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#endif
#ifndef FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#endif
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#else
extern fable_u8* DAT_01436ea8;

class LandscapeLayerList
{
public:
    void InsertNode(int node);
};

class CEngineLandscapeMemoryManager
{
public:
    void FreeVirtual(void* allocation);
};
#endif // FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER

class CEngineScreenEffectOutlineGlow
{
public:
    void** vfptr;
    fable_u8 m_Pad04[0x0C];
#ifdef FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER
    CVertexBufferWin32* field_10;                   // +0x10: the vertex buffer (DeleteVertexBuffer 0x00BDA0D0)
#else
    int field_10;
#endif
    void* Blur2Texture;

    ~CEngineScreenEffectOutlineGlow();
    CEngineScreenEffectOutlineGlow* ScalarDeletingDestructor(unsigned int freeFlags);
};

FABLE_STATIC_ASSERT_AT(33, offsetof(CEngineScreenEffectOutlineGlow, field_10) == 0x10);
FABLE_STATIC_ASSERT_AT(34, offsetof(CEngineScreenEffectOutlineGlow, Blur2Texture) == 0x14);
FABLE_STATIC_ASSERT_AT(35, sizeof(CEngineScreenEffectOutlineGlow) == 0x18);

#endif
