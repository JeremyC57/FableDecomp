# 0x00BE3970 CLandscapePatchTesselator::TesselateEdge -- B24C4 (2026-11-24): ported, baseline only
- The hand873 draft used an overlay made from 2026-11-08 headers (pending/2026-11-08_linux_hand873/hdr_round2). Re-applied onto the
  CURRENT headers (hdr/): CPatchTesselationEdgeStrip.h FABLE_RETAIL_CPATCHTESSELATIONEDGESTRIP_FINDVERT (FindVert + PeekVertexCount,
  FableWin names; FindVert body kept __forceinline as hand873 had it -- to be justified or removed), CLandscapePatchTesselator.h
  FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR_EDGE_HELPERS (the three FableWin helper declarations), and the water rows' (B24C2)
  _quarantine/CEngineLandscapeMap.h FABLE_RETAIL_CENGINELANDSCAPEMAP_GETEDGESTRIP. The vertex layout and SetTargetVertexNormal now
  come from the repo's FABLE_RETAIL_LANDSCAPE_BUILD_DEFAULT_SECTION_PASS28 (replaces hand873's _HELPERS vertex class and
  _VERTEX_NORMAL). Source: v1.cpp = differ/00be3970.cpp (defines + include path changed only).
- Baseline grade (ATTEMPTS.log): DIFFER(2399v2390) hunks=51 -- the same 2399 bytes hand873 recorded (its "13 opcode hunks"; fg also
  counts register-only hunks). Residue as hand873: retail keeps the loop-top vertex pointer (`mov ecx,[esi+8] ; lea eax,[edi+edi*4] ;
  ... add ecx,eax ; mov [esp+38h],ecx`), ours keeps Verts.Data in edi and indexes it; everything downstream is register drift.
- Lessons to read first (hand873 NOTES / LESSONS in prior/): TE_AXIS variants, GetMainAxisVertexWorldPos spellings tried.
- Next: hand-author the loop top: a `CPatchTesselationEdgeStrip::CVertex& vert = strip->GetVert(i)` reference taken before the
  start/end tests (retail computes &Verts[i] once, stores it, then reads +8/+0xA through it), then FindVert's loop form.
