# Hand chunk 912 (CPersistContext .. CQ_HeroSoulsArenaScript): 33 of 33 byte-exact

(The chunk holds 33 rows, not 40.)

## What matched (33)
- **CPersistContext::TransferVectorOfSubComponents<T>** (0x00F33B90 CFireHeartPatternDef 0x34, 0x00F349F0 CArenaRoundDef 0x38,
  0x00F351F0 CHeroSoulsRoundDef 0x38): the landed Transfer<CCreatureEntry> (0x00F274D0) model reused as-is. Only the binary-load
  arm differs: `value.resize(count)` is the ONE-argument out-of-line resize (0x00F33D90 / 0x00F34BF0 / 0x00F353F0), no default
  temporary. Real name from the PDB (`??$TransferVectorOfSubComponents@V<T>@@@CPersistContext@@...`); the text-load helper
  0x009BC120 is TransferVectorOfSubComponentsLoadText and the wrapper is CPersistContext_TransferableVectorOfSubComponents<T>.
- **CPersistContext_TransferableVectorOfSubComponents<T>** Size / AreDefaultValsApplied / SetDefaultValsApplied / Transfer for
  CArenaWaveDef (0x3C, 0x00F28010..80), CHeroSoulsWaveDef (0x38, 0x00F28780..F0), CFireHeartPatternDef (0x34, 0x00F33D00..70),
  CArenaRoundDef (0x38, 0x00F34B60..D0), CHeroSoulsRoundDef (0x38, 0x00F35360..D0). Slot order from the vtables 0x012FB598,
  0x012FB5AC, 0x01301C40, 0x01301C68, 0x01301C7C; method names from the PDB. The +0x24 byte is the CDefClassBase bitfield group
  Setup:1, Template:1, DefaultValsApplied:1 (bit 2). SetDefaultValsApplied needs the element bound to a named reference
  (`T& def = (*TransVector)[index]; def.DefaultValsApplied = applied;`): the direct form folds +0x24 into a lea and loads the
  vector before the index (37 v 38).
- **CPersistContext_TransferableVector<CDefString>** Size (0x00F33470), TransferIn (0x00F33480), TransferOut (0x00F334A0):
  the two hooks TAIL-JUMP into CPersistTraits<CDefString>::TransferIn 0x007E3820 / TransferOut 0x007E3930 with `this` = the
  dead index slot. Matched only with a same-TU `#pragma auto_inline(off)` body of the traits method above the hook (traits
  is an empty specialisation); with the method only declared VC keeps `push x3 ; call ; ret 0Ch`.
- **CPersistContext_TransferableComponent<T>::TransferIn/Out**: EScriptingStateGroups (0x007AD7B0 / 0x007AD7D0, the long-forwarding
  inline enum traits), CRGBColour (0x006421C0: the traits' DefaultVal ctor written `A = 255; R = 0; G = 0; B = 0;` gives retail's
  +2/+1/+0 store order with the late alpha store), CFaction (0x006BACE0: traits holds a 0x1C CFaction with out-of-line ctor/dtor).
- **CPlayerManager::GetMultiplayerColour** (0x00449B60): switch with `return CRGBColour(r, g, b, a)` per arm, matched first try
  (header-derived view for the method since engine/CPlayerManager.h is data-only).
- **NScript::CQ_HeroSoulsArenaScript::GetJackSpeech** (0x00D80AE0): an unused local built as a COPY of a temporary -- an inline
  helper returning a named CCharString local (no NRVO in VC7.1) gives the ctor / copy-ctor / dtor prelude; `T s = T("..")` and
  `T s = ".."` are elided.
- **NScript::CQ_HeroSoulsArenaScript::Alloc** (0x00D80BA0): static __fastcall factory, landed quest-script model (novtable
  CLevelScriptBase<T> carrying Game/MasterScriptData, CTimer via g_pGameScriptInterface->RegisterTimer) plus the STLport
  `_Vector_base` + `_STLP_alloc_proxy` vector for Rounds (fixes the `pop edi` position between the zero stores).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x00F33B90 / 0x00F349F0 / 0x00F351F0 `CPersistContext::Transfer<signed_char>` -> `CPersistContext::TransferVectorOfSubComponents<CFireHeartPatternDef / CArenaRoundDef / CHeroSoulsRoundDef>`.
- All 20 `CPersistContext_TransferableVector::vfunc_0/2/3/4` rows at 0x00F28010..0x00F353D0 -> `CPersistContext_TransferableVectorOfSubComponents<T>::Size / AreDefaultValsApplied / SetDefaultValsApplied / Transfer` (T per family above).
- 0x00F33470 / 0x00F33480 / 0x00F334A0 `CPersistContext_TransferableVector::vfunc_0/3/2` -> `CPersistContext_TransferableVector<CDefString>::Size / TransferIn / TransferOut`.
- Callees: 0x00F33D90 / 0x00F34BF0 / 0x00F353F0 (`Transfer<CAnimationSet>`) are vector<T>::resize(n); 0x009E8C10 (`CPersistTraits<NInventory::CItem>::TransferIn`) is CPersistTraits<CRGBColour>::TransferIn; 0x006BA420 (`std::pair<CCharString,CCharString>::pair`) is CFaction::CFaction; 0x006BA6D0 (`~CPatchTesselationEdgeStrip`) is CFaction::~CFaction; 0x009E2560 (`CTCInventoryBase::GetItemDescription`) is the __fastcall long writer TransferOutLong; 0x0099EAE0 (`_Cons_val<...>`) is CCharString::~CCharString; 0x00F27EA0 / 0x00F28610 (`Transfer<signed_char>`) are TransferVectorOfSubComponents<CArenaWaveDef / CHeroSoulsWaveDef> ("Waves"); 0x00F27E60 / 0x00F285D0 (gapscan) are their owner round defs' Transfer.
