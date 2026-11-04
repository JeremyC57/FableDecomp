// NUISystem::CMovie::SetMovie(CWideString& movie) -- set the movie component's movie name (virtual)
// Leaf: ?SetMovie@CMovie@NUISystem@@UAEXAAVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548510, 11 bytes: `add ecx,174h ; jmp 0x0099B7D0` (Movie = movie, tail call).
// Identity (two sources): slot 142 of CMovie's vtable 0x0124881C (rtti-vtables.tsv), which engine/CMovie.h (chunk 735) names from
//   Ego_r.pdb's order ?SetMovie@CMovie@NUISystem@@UAEXAAVCWideString@@@Z (slot 142) / ?IsPlaying@..UAE_NXZ (slot 143); the body assigns
//   the CWideString Movie, Ego_r +0x1A4 = retail +0x174 (the CMovie members sit 0x30 lower in retail; the constructor 0x005482D0 builds
//   a CWideString there). The catalog's "CMovie::CMovie SetMovie" class prefix was doubled.
// Callees: ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0.
// Header: engine/CMovie.h (FABLE_RETAIL_CMOVIE + FABLE_RETAIL_CMOVIE_MEMBERS).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMOVIE
#define FABLE_RETAIL_CMOVIE_MEMBERS
#include "engine/CMovie.h"
namespace NUISystem {
void CMovie::SetMovie(CWideString& movie)
{
    Movie = movie;
}
}