# Landing 2026-11-09b: chunk 890 session 5 (expert, instrumented), CProgressDisplay::DrawRetailDisplay

0x00498490 CProgressDisplay::DrawRetailDisplay (5,101 bytes, `ret 0Ch`): RELOCATION_MATCH + PASS (VC7.1 and g++ -std=c++03
-pedantic), after five sessions (17 grades). Hand-back: `pending/2026-11-08_linux_hand890/` (TRACE.md sections B..G).

- Name: FableWin.pdb + Ego_r.pdb `?DrawRetailDisplay@CProgressDisplay@@AAEXABVCCharString@@M_N@Z` (private); the landed caller
  0x00499880 binds a `DrawProgress` spelling no PDB has (queued as land0811ad).
- What it does: draws the retail loading screen -- black screen, the tiled backdrop, the progress bar from the GUI def's pieces,
  the seal, and the formatted loading text; the bar length scales with the progress fraction.
- Found by instrumentation: the frame packer (every local in retail's slot via C2DBoxF four-float construction, an inner block for
  formattedWindow, a named window box); the x87 load order (VC7.1 loads the operand stored most recently: constructor spellings);
  the EndsWith lea (early-return GetAsCharArray, FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN); the last instruction from c2's
  scheduler replayed in Python -- retail's source parenthesises the half-width, and the parenthesis is a scheduled no-code tuple.
- Inline device: FABLE_FORCEINLINE CCharString::EndsWith (FABLE_RETAIL_CCHARSTRING_ENDSWITH; no out-of-line copy in Ego_r.pdb).
- Headers: the chunk's overlay merged (24 headers, additive opt-ins, hdrcheck default and every existing opt-in unchanged; new
  CProgressDisplay.h, CNewFrontendGameComponent.h, CTCFireballSpell.h). Some opt-ins serve the still-parked 0x0056AD40.
- Names: labels `ghidra_out/labels_landing_2026-11-09b.tsv`; 3 respellings queued as land0809b (ChangeGraphicsBankScene 0x00437570,
  ForceClearGui 0x004382C0 landed as PrepareForLoad -- one name on two addresses, Deactivate 0x00625920 landed as RequestClose).
  Unqueued candidate: 0x0042A2DB CCountedPointer<CList>::SetStaticCast<CComponent> conflicts with the 0x0042A2CA binding (check
  chunk 735's ShareData names first).
- Still parked: 0x0056AD40 CTCBoastUI::Activate at DIFFER(4275v4284), one inline device left.
