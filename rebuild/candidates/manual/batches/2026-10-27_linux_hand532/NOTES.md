# Hand chunk 532 (_global .. _global): 3 of 10 byte-exact

## What matched (3)
- **std::_Rb_tree (STLport 4.6), 0x0052A2D0**: `insert_unique(iterator, const value_type&)` for the NPC name map
  `map<unsigned __int64, pair<CCharString, CWideString> >` (CThingManager). Shape: the stock STLport 4.6 hinted insert,
  begin / end / interior hint cases, with the `_KeyOfValue` / `_M_key_compare` on a 64-bit key; it matches with the
  real `_Rb_tree_iterator` `operator++` / `operator--` (0x00527480 / 0x00527AC0) out of line and `_M_insert` private.
- **std::_Rb_tree (STLport 4.6), 0x009EF4D0**: the same template for `map<char, CCharString>` (console key binding
  map). Same shape; the signed-char compare (`cmp bl,[esi+10h] ; jle`) needs the key typed `char`.
- **CThingAICreature, 0x008334F0**: `GetGatherPosInBuilding(CIntelligentPointer<const CThingBuilding>) const`: the
  building's GATHER_POSITION helper point, else (creature has TC 7, CTCCreatureNavigation) the building position when
  navigable, else the nearest navigable position within the building physics radius, else the first navigable cell of
  an 11x11 half-unit grid. Shape lessons: the helper-name CCharString is a scoped named local (`{ CCharString n(...);
  found = ...; }`) so its dtor runs before the `found` test; the grid loops are guarded by `if (!found)` and test
  `!found && x <= 5.0f` in the condition; `pos = building->GetPos()` copies through the C3DVector copy constructor
  (fld/fstp triples, not a 12-byte mov); `CTCPhysicsBase` from the shared header.

## Parked for Fable 5.1 (7)
- 005458a0 NUISystem::CSpellContainer::AssignSpell DIFFER(669v621): retail calls the CDeletion copy ctor 0x0042CD84 and ~_List_base 0x0042ABCA out of line in the already-assigned branch (inline in the final assignment); ours inlines both in both branches and holds slot in ebp (retail edi) | tried: make_pair / operator forms, a user CDeletion copy ctor, CreateComponent on the CManager base | next: same-TU noinline bodies for 0x0042CD84 / 0x0042ABCA under their real names (VC inlining budget)
- 0055e2c0 CryptoMatic DIFFER(545v549): retail runs strlen before the prologue pushes and holds keyLength in esi, the index in ebp and the destination in edi; ours holds the key in esi; the result local sits in another frame word and the four key-word stores are ordered differently | tried: declaration reorder, key-pointer spelling (g3, worse) | next: lane gap: register allocation (try the strlen as an inline loop on a `const char*` copy before any other local)
- 005fcbe0 CTCInventoryMap::ConstructPastQuestsList DIFFER(649v637): CDefPointer release is `mov edi,[eax+4] ; add eax,4 ; dec edi ; ... mov [eax],edi` instead of retail's `dec dword [ecx+4] ; jne` | tried: local-copy destructor, __forceinline, mutable / int count, the `!--` form | next: a same-TU noinline GetDef 0x0042B312 body (the landed sibling 0x005FC2D0 matched with one)
- 00753630 CTCLook::InitialiseTurningViaAnimation DIFFER(532v528): x87 operand order only: the length test squares Y before X and `EPSILON * EPSILON < length` gives `test ah,41h ; jne` (the g2 `length > EPSILON*EPSILON` gives retail's `ah,5 ; jp`), the dot product loads toPos.Z before facing.Z | tried: four operand orders, g2 comparison form | next: `float s = toPos.X * toPos.X; s += toPos.Y * toPos.Y;` with `s > EPSILON * EPSILON` (GOTCHAS chunk 205 left-square-first form)
- 007634e0 NShape::CBasicShape::UpdateSubdivisions DIFFER(659v627): retail keeps &Subdivisions in ebp for the whole body, the new triangle's Data/Info in esi/edi and the iterator in the dead order slot, frame 0x30; ours keeps this in edi, iterator in ebx, re-reads Info (release is `dec ; reload ; cmp`), frame 0x34, two induction pointers in erase(begin())'s copy | tried: reference local for Subdivisions (folded away), loop forms | next: lane gap: register allocation (try `ltcg_grade.py`; a block-scoped piece vector to share the counted pointer's slot)
- 0077dc40 CTCHeroCentre::OnInitialActivate DIFFER(591v626): retail keeps the copied list's header node in esi across the out-of-line `_M_insert_dispatch` 0x00618100 (LTCG knows the callee does not read it), homes the list on the spilled-this word, the comparator on the found flag's word; ours stores and re-reads the header, shrink-wraps ebx/ebp, inline list/vector teardown differs | tried: copy-ctor vs assign forms of the list copy, push_back forms | next: lane gap: LTCG (same-TU auto_inline(off) bodies for _M_insert_dispatch and the push_back overflow helper, graded with ltcg_grade.py)
- 00834530 CombatStressTestDebug DIFFER(558v558): the empty CThingFilter_None temporary gets its own frame slot (retail overlaps it with the seed parameter home: frame 0x24 vs 0x28, every local 4 bytes lower), and the vector size is computed before the empty test (retail loads _M_finish into ebp before and subtracts after) | tried: user ctor on the filter (stops zero-init), pair ctors (forces the hidden return pointer), count inside the test (g2: shrink-wraps ebp) | next: lane gap: LTCG frame packing (a named filter local declared at the top / ltcg_grade.py)

## Skipped
None.

## Names
### Proven (two sources each)
- 0x0052A2D0 `std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, pair<CCharString,CWideString> >, ...>::insert_unique(iterator, const value_type&)`: STLport 4.6 body shape byte-matched; Ego_r has this map's `operator[]` (`??A?$map@_KU?$pair@VCCharString@@VCWideString@@...`), the caller 0x0052B8B0.
- 0x009EF4D0 `std::_Rb_tree<char, pair<const char, CCharString>, ...>::insert_unique(iterator, const value_type&)`: byte-matched; Ego_r `??A?$map@DVCCharString@@...` (the map's operator[]).
- 0x008334F0 `?GetGatherPosInBuilding@CThingAICreature@@QBE?AVC3DVector@@V?$CIntelligentPointer@$$CBVCThingBuilding@@@@@Z`: Ego_r donor order IsMessageEventWithinSight / IsMessageEventWithinHearing / GetSleepTimes / GetGatherPosInBuilding = retail 0x833230 / 0x8332E0 / 0x833390 / 0x8334F0; `ret 0xC` = hidden return + 8-byte CIntelligentPointer.
- 0x00834530 `?CombatStressTestDebug@?A0x...@@YIXAAVCThingAICreature@@ABVCThingSearchTools@@PAK@Z`: both PDBs; the landed caller WorldUpdate 0x00834760.
- 0x00753630 `?InitialiseTurningViaAnimation@CTCLook@@AAEXABVC3DVector@@J@Z`: Ego_r; the only user of CGetBestTurningAnim / for_each; CTCLook PDB member offsets.
- 0x007634E0 `?UpdateSubdivisions@CBasicShape@NShape@@UAEXXZ`: vtable 0x0126A31C slot 5; Ego_r order follows FractureShape 0x00763060.
- 0x0077DC40 `?OnInitialActivate@CTCHeroCentre@@UAEXXZ`: vtable 0x0126AF94 slot 6 (CTCBase OnInitialActivate slot); Ego_r order after GetQuestCards.
- 0x0055E2C0 `?CryptoMatic@?A0xf010c0d0@@YIHPBDPADH0D@Z`: Ego_r order EncipherMorsel 0x55E040 / DecipherMorsel 0x55E180 / CryptoMatic / EncryptBuffer 0x55E4F0 / DecryptBuffer 0x55E510; both wrappers call it.
- 0x005458A0 `?AssignSpell@CSpellContainer@NUISystem@@UAEJAAV?$CCountedPointer@VCComponent@NUISystem@@@@K@Z`: CSpellContainer vtable slot; Ego_r.
- 0x005FCBE0 `?ConstructPastQuestsList@CTCInventoryMap@@AAEPAVCComponent@NUISystem@@PAV23@@Z`: Ego_r; UI_TITLE_QUEST_PAST string use.
- Callees: 0x006CB8E0 `?GetNearestNavigablePosTo@CTCCreatureNavigation@@QBE_NABVC3DVector@@MAAV2@@Z` (Ego_r donor order GetNearestFleePosition / GetNearestNavigablePosTo / IsCurrentlyNavigating); 0x006CBA50 `?IsPosNavigable@CTCCreatureNavigation@@QBE_NABVC3DVector@@@Z`; 0x004C0560 `?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z`; 0x0070C050 `?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z`; the rest in NAME_FIXES.tsv.
### Probable
- 0x0129BA3C `?EPSILON@@3MB` (landed spelling; one source plus use as squared tolerance).
### Descriptive
- 0x013BADAC `g_CCharString_013badac` ("HeroCentreQuestCardPos" prefix string).
- 0x013B89FC `g_pQuestManager`.

## Label corrections
| Address | Catalog label (wrong) | Real |
|---|---|---|
| 0052a2d0 | CSplineList_Insert_Ordered | _Rb_tree::insert_unique (NPC name map) |
| 005458a0 | AnimationEntry_LoadOrFindIndex | NUISystem::CSpellContainer::AssignSpell |
| 005fcbe0 | ConstructQuestsScreen_PastQuests | CTCInventoryMap::ConstructPastQuestsList |
| 00753630 | CAnimateThing_SetupTurningAnimation | CTCLook::InitialiseTurningViaAnimation |
| 007634e0 | CPatchTesselation_GenerateTriangles | NShape::CBasicShape::UpdateSubdivisions |
| 0077dc40 | CTCCampfire_BuildPlayerRegion | CTCHeroCentre::OnInitialActivate |
| 008334f0 | AIStateHelper_GetGatherPoint | CThingAICreature::GetGatherPosInBuilding |
| 00834530 | AnimationBlend_UpdateCombatants | (anon)::CombatStressTestDebug |
| 009ef4d0 | CCharMap_InsertIfNew | _Rb_tree::insert_unique (char -> CCharString map) |
| 00662410 | AddCombinationBlend | CThingCreatureBase::IsInMeleeAsAttacker |
| 006cb8e0 | operator== CScriptThing | CTCCreatureNavigation::GetNearestNavigablePosTo |

## Header debt
- `engine/CThingAICreature.h`, `engine/CTCHeroCentre.h`, `engine/CTCLook.h` are flat pad classes; rows here model the
  CThing TC map (+0x20 bitset, +0x44 CVectorMap, +0x60 PhysicsTC, +0x64 PTCGraphicAppearance) locally.
- No `engine/CQuestManager.h`; 005fcbe0 models it locally.

## Review at landing
- NAME_FIXES.tsv (24 rows): the crypto TU names are swapped in landed code. 0x0055F090 is StdEncryptString and
  0x0055F140 is StdDecryptString; 0x0055E4F0 is EncryptBuffer (mode 0) and 0x0055E510 is DecryptBuffer (mode 1).
  This affects `rebuild/src/compiled/00/55/global_StdDecryptString_0055f090.cpp` and `global_StdEncryptString_0055f140.cpp`.
- Landed CUnproven iterator leaves at 0x00527480 / 0x009EE1F0 (`operator++`) and 0x00527AC0 (`operator--`) have real
  STLport spellings (NAME_FIXES).
- 0x0052B8B0 landed as `??ACNPCNameMap`: it is `std::map<...>::operator[]` (Ego_r spelling in NAME_FIXES).
- Anonymous-namespace leaves (CryptoMatic, CombatStressTestDebug, CGetBestTurningAnim) are matched by prefix
  `?Name@?A0x`; the hash is per TU.
