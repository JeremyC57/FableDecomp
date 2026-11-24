// CGraphicDataBankInit::CGraphicDataBankInit() -- the graphic-bank load parameters' defaults: the seven pixel formats and both maxima -1,
//   GenerateMipmaps false, AllowDither true, AllowCompressedTextures false, AllowDXT1ForBooleanAlpha true
// Leaf: ??0CGraphicDataBankInit@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415B80, length 48 bytes; no frame, -1 in ecx (`or ecx,-1`), zero in dl, one in cl, `ret`.
// Names (NF224, 2026-11-22, q12561): Ego_r.pdb, FableWin.pdb and Ego_d.pdb ??0CGraphicDataBankInit@@QAE@XZ; the 4 landed callers
//   (0x00416C8A 0x0042DDB3 0x0041E3F6 0x005BFF07) bind that spelling. Was the made-up stand-in
//   SNavigationLayerDefaults_00415b80::SNavigationLayerDefaults_00415b80 (a row-local struct; the name is in no PDB) over the chunk
//   label CNavigationLayer::CNavigationLayer. Now on the shared engine/CGraphicDataBankInit.h (members by offset: +0x00..+0x18 the seven
//   CPixelFormat FormatIndex -1, +0x1C GenerateMipmaps, +0x20 / +0x24 MaxGraphicWidth / MaxGraphicHeight, +0x28 AllowDither,
//   +0x29 AllowCompressedTextures, +0x2A AllowDXT1ForBooleanAlpha).
// Fidelity: scaffolding, not retail (the 0x00BB1740 / 0x00485420 pattern): the constructor is the header's inline one, a pick-any
//   COMDAT wherever it is used; the anonymous-namespace helper below uses it so this TU emits that COMDAT. Internal to this TU; never called.
#include <new>
#include "engine/CGraphicDataBankInit.h"

namespace {
void ReferenceGraphicDataBankInitConstructor(void* where)
{
    new (where) CGraphicDataBankInit();
}
}
