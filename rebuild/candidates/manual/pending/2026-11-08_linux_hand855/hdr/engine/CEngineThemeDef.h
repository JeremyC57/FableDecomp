#ifndef FABLE_ENGINE_CENGINETHEMEDEF_H
#define FABLE_ENGINE_CENGINETHEMEDEF_H
// CEngineThemeDef retail opt-in (chunk 855, row 0x00642AA0 CMinimapData::PopulateMinimapThemeList). Ego_r.pdb CEngineThemeDef
// (0xa8 bytes; the generated engine/_quarantine/CEngineThemeDef.h is not usable: unreconciled), here only the member that row
// reads: MinimapTheme (EMinimapThemeType) at retail +0x90 (Ego_r +0x94; ghidra_out/def_schema.json retail_off 144), read
// `mov eax,[edi+90h] ; dec eax ; cmp eax,6` (0x00642BD9) as the landed-pending sibling CRegionMinimap::PopulateRegionMinimap
// 0x0066CF00 reads it. EMinimapThemeType's enumerators are in neither PDB name list: values are written as casts.
#ifdef FABLE_RETAIL_CENGINETHEMEDEF
#include "engine/CDefClassBase.h"
enum EMinimapThemeType { };
class CEngineThemeDef : public CDefClassBase {
public:
    unsigned char _pad_0x25[0x90 - 0x25];
    EMinimapThemeType MinimapTheme;                                      // +0x90
};
#endif
#endif
