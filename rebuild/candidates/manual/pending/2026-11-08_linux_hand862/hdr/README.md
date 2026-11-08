# chunk 862 header overlay

## engine/NUISystem_CComponent.h -- new opt-in FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS (default path unchanged)
- What: under the macro, `unsigned char Deletion[8]` (+0xd4) becomes `NUISystem::CDeletion Deletion` and
  `unsigned char PositionChildren[12]` (+0x11c) becomes `std::set<NUISystem::CComponent *> PositionChildren`; the namespace is
  closed around `#include "stlport/fable_map.h"` and `#include "engine/NUISystem_CDeletion.h"` after the forward declarations.
- Evidence: Ego_r.pdb member types (struct_layouts_egor.tsv: CDeletion Deletion +0xd4, set<CComponent*> PositionChildren +0x11c);
  retail Update 0x00531EC0 reads `(*it)->Deletion.Method` at [ecx+0xd4] and walks PositionChildren from the set header's
  leftmost node with the expanded `_M_increment` (0x00531EFC, 0x0053219B). Sizes equal the opaque arrays (8 / 12), so the class
  layout and sizeof(CComponent) == 0x134 are unchanged.
- Opt-in only: without the macro the preprocessed header is identical to the repo copy. Used by differ/00531ec0.cpp (with
  FABLE_RETAIL_STLP_SET / FABLE_RETAIL_STLP_LIST).
- Caveat: engine/NUISystem_CDeletion.h includes NUISystem_CComponent.h, so a TU that defines the macro must include
  NUISystem_CComponent.h first (as the row does), not NUISystem_CDeletion.h.
- Includers rechecked read-only with this overlay (`vland.py --recheck-landed`, tmp/recheck_includers.log): all 9 direct landed
  includers MATCH / RELOCATION_MATCH + PASS (SIBLINGS.tsv). Header includers (NUISystem_CDeletion.h, NUISystem_CActionParamSLONG.h,
  CFrontEndManager.h, NUISystem_CChangingStateComponent.h, NUISystem_CActionParamFloat.h, CTCInventoryExperience.h,
  NUISystem_CObservable.h) do not define the macro.

No other header is changed. Experiments that did not help (CDeletion inline ctors / dtor, CUIScreenUnitConverter ConvertX /
ConvertY declarations, STLport list clear / _Construct / push_back respellings) were removed from the overlay; see NOTES.md.
