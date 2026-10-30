// CTexture::GenerateMipmaps @ 009f9e60
// thiscall(this, ulong a, ulong b); this in ecx, a=[esp+4], b=[esp+8]; ret 8.
// Harness rewrites __fastcall->__fastcall, so add a dummy EDX param to keep
// both real args on the stack (matching retail's [esp+4]/[esp+8] reads).
// body: D3DXFilterTexture(texture, 0 (palette), b (SrcLevel), a (Filter)); __stdcall, callee pops 16 bytes.

#include "engine/CTexture.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned long ulong;


extern "C" long __stdcall D3DXFilterTexture(void* pBaseTexture, const void* pPalette, unsigned int SrcLevel, ulong Filter);   // 0x00BFEFCE d3dx9_25 import thunk

void __fastcall GenerateMipmaps(CTexture* texture, int /*edx*/, ulong a, ulong b)
{
    D3DXFilterTexture(texture->PD3DTexture, 0, b, a);
}