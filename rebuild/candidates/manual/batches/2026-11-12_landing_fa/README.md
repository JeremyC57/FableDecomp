# 2026-11-12 landing fa -- 0x00881A40 NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags (wf13, after repair)

Source: `pending/2026-11-12_linux_handWF13/`. The row matched in workflow wf13 with three verifier findings. An Opus repair
(`lane_tools/workflows/repair_and_verify_row.js`) fixed all three and both verifiers then passed it without findings:
- `stlport/fable_map.h`: the one rewritten line (the `#ifdef FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE` line becomes an
  `#if defined(..._DECL) / #elif defined(..._NOINLINE) && defined(_MSC_VER) / #elif defined(...)` chain) is documented; the default
  and the existing opt-in path are token-identical (hdrcheck2), the two new macros have no other user.
- The `__declspec(noinline)` find body now cites retail: the out-of-line call at 0x00881ABB and the node kept in edi at
  0x00881AC0 / 0x00881AC4 / 0x00881AC8 (`mov edi,[esp+10h]` / `cmp edi,[ebx]` / `add edi,14h`). With the body hidden VC7.1 keeps
  the node in eax; fully inline is 609 bytes.
- NAME_FIXES gives the full STLport symbol of 0x00883860. CThingGraphicByPassFlagState is a `class` (both PDBs use key V).
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. Headers: stlport/fable_map.h, NScreenEffect_CScreenEffectPriorityStack.h,
NGraphicAppearance_CTCGraphicAppearance.h. Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fa): 0x00883860.
