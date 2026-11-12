// C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk&) -- build the engine material from a
//   file material chunk: index, name, two-sided flag, self-illumination from the optional extended-properties chunk, a fresh
//   CMapFilenames filled from the material's map chunks by map type (-1/1 diffuse, 5 illumination, 6 = transparent, 8 bump,
//   9 reflection), bank indices cleared, then the map flags recomputed.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC53F0, 755 bytes, `ret 4`; jump table 0x00AC56C0 + byte index table 0x00AC56D8.
// Identity: Ego_r.pdb + FableWin.pdb ??0C3DMeshFileEngineMaterialChunk@@QAE@ABVC3DMeshFileMaterialChunk@@@Z; vtable 0x0129EFC8.
// Callees: CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) 0x00AA8360 ("EMTL"), CCharString() 0x0099E4B0 / copy
//   0x0099EC30 / operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, C3DMeshFileMaterialChunk::HasExtendedProperties 0x00ABFCD0,
//   GetExtendedProperties 0x00AC0B70, C3DMeshFileMaterialExtChunk::GetSelfIllumination 0x00ABFDC0, FindFirstMap 0x00AC0E50,
//   FindNextMap 0x00AC0EE0, CalculateMapFlags 0x00AC5260, operator new 0x00BFEA1A, operator delete 0x00BFE9BC.
// Shape: the `and esp,-8` frame and the register choice (material in edi, map.Info in ebx, FindNextMap's Data spilled) only
//   come out when CalculateMapFlags' REAL body (landed row 0x00AC5260, defined earlier in this retail TU) is visible to the
//   compiler but kept out of line (#pragma auto_inline(off)); with a declaration only, VC7.1 allocates differently.
//   The case values come from the index table (dossier's 7/8 mapping was off by one: 8 = bump, 9 = reflection) and the
//   case blocks are in retail's code order (Transparent, Diffuse, Bump, Reflection, Illumination). The extended
//   properties are held in a named local (retail reads its Data back from the frame slot).
// Leaf: ??0C3DMeshFileEngineMaterialChunk@@QAE@ABVC3DMeshFileMaterialChunk@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK
#define FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK_FROM_MATERIAL
#define FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALCHUNK_METHODS
#define FABLE_RETAIL_C3DMESHFILEMAPCHUNK_METHODS
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK_METHODS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DMeshFileEngineMaterialChunk.h"
#include "engine/C3DMeshFileMaterialChunk.h"
#include "engine/C3DMeshFileMapChunk.h"
#include "engine/C3DMeshFileMaterialExtChunk.h"

// CalculateMapFlags 0x00AC5260: the landed row's real body, out of line (same retail TU, defined earlier).
#pragma auto_inline(off)
void C3DMeshFileEngineMaterialChunk::CalculateMapFlags()
{
    MapFlags = 0;
    if (!MapFilenames.IsNull()) {
        if (MapFilenames->DiffuseMap.GetLength() != 0)
            MapFlags |= 1;
        if (MapFilenames->BumpMap.GetLength() != 0)
            MapFlags |= 2;
        if (MapFilenames->ReflectionMap.GetLength() != 0)
            MapFlags |= 4;
        if (MapFilenames->IlluminationMap.GetLength() != 0)
            MapFlags |= 8;
    }
    else {
        if (DiffuseMapBankIndex)
            MapFlags |= 1;
        if (BumpMapBankIndex)
            MapFlags |= 2;
        if (ReflectionMapBankIndex)
            MapFlags |= 4;
        if (IlluminationMapBankIndex)
            MapFlags |= 8;
    }
}
#pragma auto_inline(on)

C3DMeshFileEngineMaterialChunk::C3DMeshFileEngineMaterialChunk(const C3DMeshFileMaterialChunk& material)
    : CChunkedFileSuperChunk("EMTL"), DegenerateTriangles(false)
{
    MaterialIndex = material.GetMaterialIndex();
    Name = material.GetName();
    TwoSided = material.GetTwoSided();
    SelfIllumination = 0;
    if (material.HasExtendedProperties()) {
        CCountedPointer<const C3DMeshFileMaterialExtChunk> ext = material.GetExtendedProperties();
        SelfIllumination = ext->GetSelfIllumination();
    }
    Transparent = false;
    BooleanAlpha = false;
    MapFilenames.Reset(new CMapFilenames);
    CCountedPointer<const C3DMeshFileMapChunk> map = material.FindFirstMap();
    while (map.Data) {
        switch (map->GetType()) {
        case 6:
            Transparent = true;
            break;
        case -1:
        case 1:
            MapFilenames->DiffuseMap = map->GetFilename();
            break;
        case 8:
            MapFilenames->BumpMap = map->GetFilename();
            break;
        case 9:
            MapFilenames->ReflectionMap = map->GetFilename();
            break;
        case 5:
            MapFilenames->IlluminationMap = map->GetFilename();
            break;
        }
        map = material.FindNextMap(map);
    }
    DiffuseMapBankIndex = 0;
    BumpMapBankIndex = 0;
    ReflectionMapBankIndex = 0;
    IlluminationMapBankIndex = 0;
    CalculateMapFlags();
}