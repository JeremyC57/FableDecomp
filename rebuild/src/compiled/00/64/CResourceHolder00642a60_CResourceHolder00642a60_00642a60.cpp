// CResourceHolder_00642a60::~CResourceHolder_00642a60() -- free the buffer at +0x30, destroy the member at +0x24 out of line, destroy the polymorphic member at +0x14 (its vtable 0x0122F84C, then its base destructor out of line), then free the buffer at +8
// Leaf: ??1CResourceHolder_00642a60@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00642A60, length 58 bytes.
// Callees: free @ 0x00BFEA14 twice, the member destructor @ 0x009F9F70, the polymorphic member's base destructor @ 0x009F2E20. Shape: members destroyed in reverse
// declaration order; the inline derived destructor of the +0x14 member stores its vtable then tail-calls nothing (the base destructor is a plain call). The chunk
// label is propagated; the class is named by address.
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

// Naming (NF142, 2026-11-20, q6305): the CHolderBase_009f2e20 / CHolder_00642a60 stand-ins and the local CTexture model replaced by the shared
//   engine/CSurface.h (FABLE_RETAIL_CSURFACE_COPYFROMMEMORY: the inline virtual ~CSurface stores the CSurface vtable 0x0122F84C, ??_7CSurface@@6B@,
//   then calls ?Uninitialise@CSurface@@QAEXXZ 0x009F2E20: Ego_r.pdb 0x00B46AF0 + FableWin.pdb 0x030C99C0) and engine/CTexture.h (inline ~CTexture =
//   ?Uninitialise@CTexture@@QAEXXZ 0x009F9F70). The members are Ego_r.pdb CMinimapData's MinimapSurface +0x14 / MinimapTexture +0x24 (the class has no
//   shared header; the row's header-less stand-in CResourceHolder_00642a60 and its own leaf are unchanged; its caller 0x0082CD80 binds
//   ??1CMinimapData@@QAE@XZ: NAME_FIXES).
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#include "engine/CTexture.h"
#include "engine/CSurface.h"

struct CBuffer_00642a60 { void* m_pBlock; ~CBuffer_00642a60() { if (m_pBlock) free(m_pBlock); } };

struct CResourceHolder_00642a60 {
    unsigned char    _pad[8];
    CBuffer_00642a60 m_bufferA;                    // +0x08
    unsigned char    _pad0c[8];
    CSurface         m_holder;                     // +0x14
    CTexture         m_member;                     // +0x24
    unsigned char    _pad2c[4];                    // +0x2c
    CBuffer_00642a60 m_bufferB;                    // +0x30
    ~CResourceHolder_00642a60();
};

CResourceHolder_00642a60::~CResourceHolder_00642a60()
{
}