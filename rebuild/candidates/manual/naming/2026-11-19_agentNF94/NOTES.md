# NF94 notes (round 8, naming queue part 2 of 3)

## Read first (coordinator)
- 0x0059C747 (q11226) is now on the shared headers and binds ?GetCategoryIndexForTradeType@CTCInventoryTrade@@QAEJXZ through a same-TU strong
  `auto_inline(off)` copy, the landed 0x0059C6B0 / 0x0059C6DD / 0x0059C710 form. That is a FOURTH strong definition of the PDB symbol (the landed
  owner 0x0059B6F4 still defines the `_Retail` name; the old 0x0059C747 also defined the `_Retail` name strongly, so the owner and 0x0059C747
  already clashed before). The one-definition question stays q11204's (coordinator decision); q11230 (land the held owner) is blocked on it.
  The held port was NOT copied into rows/.

## Per entry
- q11226 0x0059C747: Ego_r.pdb 0x00608E95 + FableWin.pdb 0x023146C3 ?RemoveItemFromInventory@CTCInventoryTrade@@UAEXJJ@Z, vtable 0x01253034 slot 79.
  Needed FABLE_RETAIL_CTCINVENTORYBASE_VTABLE (declares the override in CTCInventoryTrade.h; already used by 9 landed trade rows) and a new overlay
  opt-in FABLE_RETAIL_CCATEGORY_REMOVEITEM (CCategory::RemoveItem 0x005BF221, Ego_r 0x00621FFD + FableWin 0x0236130C). Test unchanged: it is a
  standalone behaviour model that does not link the row (as the siblings' tests). RELOCATION_MATCH + PASS.
- q11230 0x0059B6F4: blocked, see above.
- q11238 0x00440590: STLport vector<long>::erase(first, last) (descriptive-by-library). Self row now `template ... vector<long>::erase(...)` over
  stlport/fable_containers.h (first grade). Exe call sites: 0x0047FEEB (no catalog row covers it), 0x00485D1F, 0x005815FF, 0x00597E4E, 0x005BEB08,
  0x005BEE94, 0x0064D5A1, 0x0064D5EF, 0x0066EA91. Binder 0x00485D1A was the generated placeholder `CClass_00485d1a::ApplyPair_00485d1a`; the landed
  0x00771DE0 already binds it as vector<long>::clear, so it was renamed too (RENAMES s1). 0x0066EA85 moved onto NHeroInformationScreens_CManager.h
  (new overlay opt-in) and now also binds resize 0x004433F0 with the std spelling. Claimed extras 0x0064D593 / 0x0064D5E1 / 0x005BEAF5 / 0x00597DF7:
  not-a-binder. Seen, not mine: 0x00590D32 binds 0x004433F0 as ?reserve@?$CPODVector@J@@QAEXK@Z (it is in my rows for q11291; looked at there).
- q11252 0x007DA230 GetHandValue: the body is now inline in engine/CTCTavernGameCardBlackjack.h (opt-in GETHANDVALUE_INLINE) and the owner row
  emits it as a pick-any COMDAT (`// Fidelity: scaffolding, not retail`, the nf86 / nf88 pattern; MATCH, COMDAT checked LINK_ONCE_DISCARD).
  0x007DA900 HasPlayerWonHand moved onto the header (it carried the same body in class on its local model; RELOCATION_MATCH). Retail structure
  (Ego_d TU order + call shapes): GetHandValue was an ordinary .cpp function between the score getters (which call it, 0x007DA435 / 0x007DA55B)
  and HasPlayerWonHand / HasPlayerDrawnHand (which inline it); the header inline is the rows-as-TUs spelling. **Coordinator:** the landed
  0x007DA400 GetCurrentScore and 0x007DA520 GetRequiredScore each define ?GetHandValue@CTCTavernGameCardBlackjack@@...@Z strongly after their
  callers (same-TU body); they already clashed with each other, and now clash with the pick-any owner too (same one-definition class as q11204).
- q11257 0x013D2D70: Ego_d.pdb ?ClothMemoryPool@N3DMeshAllocators@@3PAVCMovableResourceMemoryPool@@A (checked in Ego_d.pdb itself) + the FrameStart /
  BuildMesh save-set-restore bodies. Seen, not applied (one source): 0x013D2D6C, bound ?g_MeshDataMemoryPool@@ by 0x00A24520, is probably
  Ego_d's ?DataMemoryPool@N3DMeshAllocators@@3PAVCMovableResourceMemoryPool@@A (same namespace, adjacent); not in my entries.
- q11249 / q11318 were taken with their sibling q11249 (same vector), ahead of queue order; q11318 is blocked: port (STATUS 0x00747B80).
- q11280..q11283 (CThingSearchTools / NThingMap): 0x0049C6F0 is now CThingSearchTools::GetAllThingMapsInArea (overlay CThingMapManager.h opt-in
  GETALLACTIVETHINGMAPSINAREA for its tail-forward callee) and 0x00436500 the out-of-line copy of the header inline GetMapwhoArea (scaffolding
  through a never-instantiated derived class, the nf88 protected-member form). The rough-area binders (0x00518130, 0x005185F0, 0x0063FAE0,
  0x0078954D) and 0x00637DA0 model 12-20 classes locally: blocked: port. **Table family:** 33 landed rows read NThingMap::LayerInfo at +4
  through a local `struct CLayerInfo` + `extern CLayerInfo LayerInfo_0137dad8[]` (bound ?LayerInfo_0137dad8@@3PAUCLayerInfo@@A at
  0x0137DADC); respelling them needs the shared NThingMap::CLayerInfo (NThingMap_CThingMap.h CTHINGMAP_LAYERINFO, MAP_VALIDATE branch), whose
  other classes clash with those rows' local CThingMap models: a coordinated port, recorded in NAME_FIXES only.

## q11315 (0x009C37A0) follow-ups, not in my entries
- 0x009C3AE0 still binds three callees with non-PDB spellings from its local CFileInstaller model: `?AllocateBuffer@CFileInstaller@@QAEPAUCBufferChunk@@_N@Z` (PDB `?AllocateBuffer@CFileInstaller@@IAEPAVCBufferChunk@1@_N@Z`, Ego_r 0x00B12060 / FableWin 0x030476C0), `?FreeBuffer@CFileInstaller@@QAEXPAUCBufferChunk@@@Z` (PDB `IAEXPAVCBufferChunk@1@@Z`) and `?SetDirtyDiscError@CFileInstaller@@SIX_N@Z` (PDB `KIX_N@Z`, protected). The CBufferChunk nesting is a type change on a local model: a port onto engine/CFileInstaller.h. Its own name is also public in the model (PDB `IAE`).

## q11312 / q11314 (0x013CA7F8 CMovieSubtitles::PFont)
- Blocked: port. 0x009BD910 (Play) models CMovieSubtitles and about ten other classes locally; 0x0122A700 is the generated Release_013ca7f8 body, retail the atexit destructor thunk of PFont registered by 0x01224080 (generated CStatic_0122a700 row). Fix as a pair port onto a definition of the static.

## q11339 (0x008F0B20 GetCardDef) caller map
Rows reaching its 25 exe call sites (owner.py), with what they declare:
- local `CDefPointer` (non-template) model: 0x007D4E30, 0x007D4ED0, 0x007D4F40, 0x007D5A00, 0x007D60C0, 0x007D6150, 0x007D70E0, 0x007DA760 (declared on CTCTavernGameCardBlackjack).
- local `CCardDefPointer`: 0x007D5560, 0x007D5890.
- non-const `CDefPointer<CTavernGameCardBaseDef>` with a same-TU stand-in body: 0x008F0B80.
- PDB spelling but a same-TU stand-in body: 0x008F0BD0 (header), 0x008F0EB0, 0x008F10B0, 0x008F2280.
- header (already right): 0x007D6870. Not checked for spelling (no declaration grep hit; likely header or partial rows): 0x007D5070, 0x007D71E0, 0x007D7520, 0x007DAB60, 0x007DAEF0, 0x008F1250, 0x008F17D0, 0x008F1D40.

## q11338 (0x008C3510 GetSubDef<CMazeBattleDef>)
- 0x008C2810 and 0x00954B50 carry local def / pointer models and (0x008C2810) a same-TU stand-in body of GetSubDef: port.

## q11361 (0x006A3A70) follow-ups
- 0x006A3DA0 binds the ported push_back 0x006A3CD0 as `?PushBack@CKeyedThingVector@@QAEXABUSKeyedThing@@@Z`; 0x00814260 binds the STLport spelling the port now defines. Not in my entries.
- 0x006A2540 CTCCarrying::OnCreate: port (about a dozen local classes, auto_inline(off) stand-in).

## q11368 (0x004C10E0) follow-ups
- 0x004884D0 defines `?UpdateFreeCamera@CPlayer_Methods@@QAEXXZ` (a CPlayer_Methods : CPlayer helper struct); PDB `?UpdateFreeCamera@CPlayer@@AAEXXZ`. Not in my entries.
- 0x006D5B50: port (local C3DVector etc.); it also binds 0x004C1060 as `?GetHelperPointOrCentre@CTCGraphicAppearance@@QBE?AVC3DVector@@M@Z` (PDB GetFocalPos, NGraphicAppearance).

## q11369 (0x005E1210 __adjust_heap) family
- The anim-component sort TU (0x005E1C30 __make_heap, 0x005E2860 sort_heap, 0x005E2F40 __partial_sort, 0x005E37D0 __introsort_loop) is landed as non-template global functions over local models, and they bind each other by those spellings (reloc_bindings: ?__make_heap@@YIX..., ?__pop_heap@@YIX..., ?sort_heap@@YIX..., ?__partial_sort@@YIX...). Porting one member changes its own leaf and breaks the next one's binding, so the family needs one port. A ready draft for 0x005E1C30 (FABLE_RETAIL_STLP_HEAP + _MAKE_HEAP_BODY, /Gr, RELOCATION_MATCH) is in work/005e1c30_a.cpp. 0x005E1B60 (__pop_heap) is ported and keeps its template leaf (unchanged); 0x005E2F40 still binds it as ?__pop_heap@@ (pre-existing).

## Rule slips (NF94)
- Two Bash calls hung on stdin and were stopped from the background (TaskStop): an empty `grep $f` when no file matched (q11346 probe), and st.py with a last argument `-` (reads stdin). Before that, two STATUS notes (0x009C31E0, 0x0066A920) had been written empty for the same reason; repaired.
- One grade ran without sourcing wine/env.sh (NO_RESULT, no compile) on 0x0051F010.
- 0x005E1B60: a `// Flags: /Gr` line I added is not read by vland (it uses the catalog flags), so b8 graded the wrong symbol (DIFFER 128v126, test PASS). Redone with FABLE_RETAIL_STLP_HEAP (explicit FABLE_STLP_FASTCALL text) and no Flags line. The unstaged 0x005E1C30 draft carries the same Flags line and needs the same change before use.

## q11410 / q11411 / q11412 (CSalSwFilter family, 0x00C6B720 / 0x00C6BBD0 / 0x00C6BC80 / 0x00C6BD00)
- CODAApi.lib (sdk/gamecoda/tree/CODA/lib/Win32, `strings`) has the whole CSalSwFilter member set: ClearHistory (Q), SetLP / SetEQ (Q), ProcessLP / ProcessEQ (Q), ProcessLPFade / ProcessEQFade (A), SetFrequency(unsigned int, float) / SetQ(float) / SetGain(float) (A), IsHistoryClear() const (A).
- The landed rows model it as CStateVariableFilter (SetParameters 0x00C6BC80, SetParametersWithGain 0x00C6BD00, SetCutoff 0x00C6BBD0 defined in both TUs as an auto_inline(off) stand-in) and ClearHistory 0x00C6B720 as SBodyRecord_00c55030::Reset; 0x00C6B720 is also bound as ?Reset@SRampedVector@@ (0x00C551D0) and ?Reset@CVoiceFilter@@ (0x00C5C2D0). Port: overlay CSalSwFilter.h with the private helpers, then move the four rows and three binders together.

## q11427 (0x00769D50) particle heap family
- 0x00769D50 (__adjust_heap), 0x00769470 (__push_heap), 0x0076AF10 (sort_heap, landed as Dinkumware _Sort_heap) and 0x0076A480 (__make_heap) of pair<unsigned long, C3DVector> with NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>::CCompareVector<unsigned long> (the comparator 0x0076A590 already uses) bind each other with three different local spellings; one family port.
- (q11412) the binder 0x00C55030 binds 0x00C6B720 as `?Reset@SBodyRecord_00c55030@@QAEXXZ` (WORKLIST); part of the same family port.
- One Bash call exited 2 (an apostrophe inside a single-quoted st.py note, q11441); redone through a quoted heredoc.
- Lock 005e1c30 (a rows.txt lock) was removed by mistake together with my own extra claim 005e2f40 when I dropped the 0x005E1C30 port; restored (owner NF94). 005e2f40 stays released (unchanged row).
- Binder completeness: after the advisor's review I ran every exe site of the done entries through owner.py and grepped the in-row owners (work/sitecheck.py). It found one more wrong binder, 0x006C3CA0 (q11421, fixed); the site results are in the APPLIED notes.

## Counts in APPLIED column 4
- binders_respelled is derived from RENAMES section 2 (distinct binder rows per target address), so an entry whose self row was the only change shows 0, and q11230 shows 1 (0x0059C747 respelled its binding to 0x0059B6F4).
- REJECTED.tsv holds one partial rejection (q11424: the YA calling-convention letter of the queued spelling); no entry has status rejected.
