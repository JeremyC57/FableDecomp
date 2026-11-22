// Leaf: ?ScalarDeletingDestructor@CEngineScreenEffectOutlineGlow@@QAEPAV1@I@Z
// Callees (NF269, 2026-11-22): the landscape memory manager through NEngineInternal::GPLandscapeRenderer (0x01436EA8, was the stand-in
//   ?DAT_01436ea8@@3PAEA) -> MemoryManager (+0x6B0); 0x00BDA0D0 is the public ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z
//   (Ego_r.pdb 009c2640 / FableWin.pdb 02cb7450), was the stand-in ?InsertNode@LandscapeLayerList@@QAEXH@Z; the opt-in below
//   (candidates/outline_glow_compiled.h) takes both from the shared headers and types +0x10 as that vertex buffer.
#define FABLE_RETAIL_OUTLINE_GLOW_COMPILED_DELETEVERTEXBUFFER
#include <new>
#include "candidates/outline_glow_compiled.h"

CEngineScreenEffectOutlineGlow*
CEngineScreenEffectOutlineGlow::ScalarDeletingDestructor(unsigned int freeFlags)
{
    vfptr = reinterpret_cast<void**>(0x012A8134);

    if (Blur2Texture != 0)
    {
        operator delete(Blur2Texture);
        Blur2Texture = 0;
    }

    if (field_10 != 0)
    {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(field_10);
        field_10 = 0;
    }

    vfptr = reinterpret_cast<void**>(0x0129B860);

    if ((freeFlags & 1U) != 0)
    {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(this);
    }

    return this;
}
