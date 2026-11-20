// std::vector<CSubtitleRenderer::CEmbeddedHighlight>::~vector() -- destroy each 0x14-byte highlight record (its CWideString Text), then
//   free the storage
// Leaf: ??1?$vector@VCEmbeddedHighlight@CSubtitleRenderer@@V?$allocator@VCEmbeddedHighlight@CSubtitleRenderer@@@std@@@std@@QAE@XZ
// Retail: Fable.exe @ 0x00442870, 49 bytes, plain `ret`; caller 0x00492358, the end of CSubtitleRenderer::ProcessEmbeddedHighlightTags
//   0x00491AD2 (a local vector at [ebp+1Ch], filled in a stride-0x14 loop just before). Catalog label
//   CDefendingCombatantInfo::CCombatWheel::ResetRings is wrong: the body is STLport ~vector with _Destroy inlined -- ~CWideString
//   (0x0099B510) on each element's +0x00 from _M_start to _M_finish, then ~_Vector_base: `if (_M_start) free(_M_start)` (0x00BFEA14),
//   _M_start re-read.
// Name fix (NF195, 2026-11-20, q6240): was the descriptive std::vector<CRecordWithWideStringAt0>::~vector over a local record and a
//   local CWideString / STLport model. The element is CSubtitleRenderer::CEmbeddedHighlight (Ego_r.pdb struct_layouts: Text CWideString
//   +0, CharIndex +4, C2DVector Offset +8, bool Split +0x10 = 0x14 bytes; CSubtitleRenderer::EmbeddedHighlights is a
//   vector<CEmbeddedHighlight>; the caller is ProcessEmbeddedHighlightTags, both PDBs); FableWin.pdb names this destructor
//   ??1?$vector@VCEmbeddedHighlight@CSubtitleRenderer@@...@std@@QAE@XZ (0x01A70FB7). Now on the shared headers (the 0x0049417D recipe:
//   engine/CSubtitleRenderer.h FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED, fable_string.h, stlport/fable_containers.h), with the new
//   add-only opt-in FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK (the shared header is pack(1), which made the record 0x11).
// Shape: /O2 (aligned loop head behind `mov edi,edi`); the explicit instantiation emits the destructor.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK   // the record is 0x14 bytes (Ego_r), not pack(1) 0x11
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CSubtitleRenderer.h"

template std::vector<CSubtitleRenderer::CEmbeddedHighlight>::~vector();
