#ifndef FABLE_CENVIRONMENTTHEMEPRIORITYSTACK_H
#define FABLE_CENVIRONMENTTHEMEPRIORITYSTACK_H
// New header (chunk 1021, row 0x006B3E60 CEnvironment::SetWeightedThemes). Ego_r.pdb NEnvironmentInternal::CEnvironmentThemePriorityStack
// (struct_layouts_egor.tsv: 16 bytes in the donor, one member `CArray<CEnvironmentThemePriorityStackEntry> Ents`; 12 bytes in retail,
// the STLport vector). FableWin.pdb ?GetEntry@CEnvironmentThemePriorityStack@NEnvironmentInternal@@QAEAAVCEnvironmentThemePriorityStackEntry@2@J@Z
// is inline (no Ego_r.pdb copy; 0x006B3F66..0x006B3F81 expands it). The out-of-line members (ProcessTransitions 0x00887B00,
// CalculateTheme 0x008889C0, ctor 0x008883C0) are landed with local models; declarations only here.
#include "engine/CArray.h"
#include "engine/CEnvironmentThemePriorityStackEntry.h"
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStack {
    CArray<CEnvironmentThemePriorityStackEntry> Ents;
public:
    CEnvironmentThemePriorityStackEntry& GetEntry(long index) { return Ents[index]; }
    void ProcessTransitions();
    void CalculateTheme(CBlendedEnvironmentTheme&,float,const CEnvironmentThemeResourcePallette&);
};
}
#endif
