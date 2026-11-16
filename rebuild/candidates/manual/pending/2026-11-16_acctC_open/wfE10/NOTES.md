# wfE10 NOTES (agent wfE10, acctC lane) -- summary
No row matched (no src/, test/, payload.json, oracle.tsv). Every row now COMPILES on today's rebuild/include + hdr/ (all five drafts were
SRC_FAIL or stale before) and has a graded differ/ source:
  0x007A1B20 DIFFER(2828v2840) 10 hunks, registers equal   | 0x00507C30 DIFFER(7661v7661) 2 lines (one swapped reload pair)
  0x00502E90 DIFFER(13933v13622) inline-budget sites       | 0x00795300 (= CTCUnholyPower::FrameUpdate) / 0x0078FD60 DIFFER(9548v9268) twins
Tools in tools/: gv.sh (grade with prepended defines), sd2.py (structural diff, `m` masks registers), cmpcalls.py (call targets by
address via reloc_bindings), slotmap.py (retail -> ours frame offsets), port_optin.py (copy an opt-in block from a stale header).
Coordinator decisions: see the end of this file.

# wfE10 NOTES (agent wfE10, acctC lane)

## 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- DIFFER(2828v2840), 10 hunks, registers equal (differ/007a1b20.cpp)
Identity: Ego_r.pdb/FableWin.pdb ?FrameUpdate@CLightningBeam@CTCWillLightning@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@PAK@Z
(Ego_r 0x007A7370); extent 0x007A1B20..0x007A2638 incl. the 5-entry jump table, `ret 0Ch` (player by value = 8 bytes + seed).
Re-authored on today's shared headers (old draft used hdr_STALE). Overlay: CTCWillLightning.h BEAM_FRAMEUPDATE (method decls),
new CSpecialAbilitiesLightningSpellDef.h, CTCSoundPlayer PLAYUNATTACHEDCRITERIA, CHitParameters SETCREATEDUST (hdr/README.md).
What moved it (2752 -> 2832/2840, call list now identical by target):
- FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE (retail expands both Inserts, calls _M_insert_overflow 0x0076ADA0; landed sibling
  CTCLightningNode::FrameUpdate 0x007F2DC0 uses the same opt-in), FABLE_RETAIL_STLP_LIST_ERASE_NOINLINE (retail calls erase 0x007A38B0;
  landed same-TU erase row), CCountedPointer dtor plain inline (NOT VISIBLE_CALLED_DTOR: retail expands it in the end loop).
- named `CCharString criteria = PDef->EndAttackeeSound.GetString();` in its own block for both sound calls (GetString before pushes).
- `m_tpParticleHandle.Get() && !m_tpParticleHandle->IsDead()` spelled out (retail `test eax,eax` + `test [eax+91h],1`) and
  `CTCDParticleEmitter* emitter = 0;` (retail passes ebp=0 when GetTC fails). These gave the ebp zero / ebx=0x10 constant registers.
- C3DVECTOR_NONZERO_SQUARED_XYZ + C3DVECTOR_NORMALISE_PAREN_SQUARES_XYZ (squares shared between IsNonZero and Normalise, summed X,Y,Z):
  the plain-hit copy (0x007A2030..) now matches.
Open residue (retail address order):
1. frame 0xbc v ours 0xb4: retail keeps the spilled &m_lChildren (0x007A2573 `mov [esp+2Ch],esi`) in its own slot; ours packs it
   into the LIGHTNING_START pair slot 0x14; one more 4-byte slot missing (retail strings at 0x40 / 0x44). Tried: particle block in its
   own nested block (worse), one function-scope iterator, a named children reference (neutral).
2. decapitating-hit copy (0x007A1E53..0x007A1E9B): retail squares Z,Y,X (fstp X then `fld st0; fmul st1`) and compares with no fxch;
   ours squares X,Z,Y as in the plain-hit copy (which matches). Tried GetSquaredMagnitude / paren / ZYX spellings on that copy only.
3. CLOSED: `CThingGameObject* struck = static_cast<CThingGameObject*>(m_tcpTarget.Get());` between SetMoveBackOnHit and
   SetCreateDustParticleEffectOnHit (the old draft's order) puts the target GetPItem between the two stores as retail: 27 -> 10 hunks.
5. frame detail: retail criteria strings at [esp+40h] (attached) / [esp+2Ch] (unattached), ours both at [esp+14h]. Tried: reference-bound
   temporaries (neutral), pairs at function scope (frame 0xcc, worse), one criteria at case scope (2876, worse).
4. end-loop head pad (`lea ecx,[ecx]` v `mov edi,edi`) -- follows from the frame.
Grades used: ~35.

## 0x00507C30 CWorldMap::LoadFromFile -- DIFFER(7661v7661), 2 unmasked lines (differ/00507c30.cpp)
The H40i draft failed to compile on today's headers (its opt-ins lived only in pending/2026-11-11_linux_handH40i/hdr). Re-based as
additive opt-ins on copies of TODAY's headers: CWorldMap.h _LOADFROMFILE, CThingManager.h _SETCURRENTTHINGUID + _WORLDLOAD,
CThingMapManager.h _INIT, CRegion.h _LOADFROMFILE, CLevelLoader.h _CTOR, CEngineManager.h _CLEANUP (asserts pinned).
Name fix: the draft's descriptive CThingManager::InitialiseLoadedThings is ?ActivateThings@CThingManager@@QAEXABV?$vector@PAVCThing@@...@Z
0x0051E2F0 (Ego_r.pdb name + landed row CThingManager_ActivateThings_0051e2f0.cpp + landed caller 0x005223F0); spelled so now.
Bindings re-checked: CLevelLoader ctor (Ego_r ??0CLevelLoader@@QAE@AAVCThingManager@@AAVCWorldMap@@AAVCWorld@@@Z) and
CEngineManager::Cleanup (Ego_r 0x005A5930 right before AddEngine 0x005A5950 = retail 0x00640490 before AddEngine 0x006404D0).
Residue: ONLY the swapped pair of reloads at the parse-loop exit 0x00509395 / 0x0050939C (retail `mov ebp,[esp+1D0h]` (file) then
`mov edi,[esp+0CCh]` (buffer data); ours the other way round). Register-masked: 0 hunks, call targets unchanged.
Tried this session: k = 1..7 unused `float uK = 0.0f;` at entry (no change: not a symbol-count tie). Earlier agents' c2prio
measurement (line 3 of the source) says the order follows the split candidates' original ids; untried: c2prio_f.py (wfcommon C44).
Grades used: 10.

## 0x00502E90 CWorldMap::PopulateRegionWithVillagers -- DIFFER(13933v13622) (differ/00502e90.cpp)
The chunk-879 draft was SRC_FAIL on today's headers. Re-based (all additive, hdr/README.md): CThingAICreature.h _RELATIONS (inline
Set/Get Father/Mother/Spouse, SetHome/WorkBuilding unless CTCWIFE_MARRY), CThingCreatureBase.h _GETSEX (GetSex 0x00661FF0; GetCreatureType
via today's _CREATURETYPE), CThingCreatureDef.h _SEX_TYPED (Sex / CreatureType typed ESex / ECreatureType inside _MEMBERS), CThingBuilding.h
_RESIDENCY (GetIsAvailableForResidency const, the bound spelling), CTCVillage.h _GAYVILLAGERS, CWorld.h _POPULATE
(PopDontPopulateNextLoadedRegion), CWorldMap.h _POPULATE (decl), stlport/fable_map.h _TREE_ERASE_KEY_CALLS (equal_range / erase(first,last)
noinline with their STLport bodies, erase(key) forceinline in tree and map: retail 0x005034BE expands erase(key) and calls both halves).
Today's names: CMAINGAMECOMPONENT_WORLDSEED -> _SAMPLEBANK_WORLDSEED, CTHINGBUILDING_SIMDEF -> _PEEKSIMDEF, CWORLD_PEEKDEFINITIONMANAGER;
ECreatureType from engine/ECreatureType.h (HUMAN_CHILD / HUMAN_ADULT; the draft's local CREATURE_TYPE_* enum removed).
Residue (call list by target otherwise equal): (1) servant CreateRelevantPersonAndUpdateStats site expanded in ours, called in retail
(retail calls only at the servant and apprentice sites); (2) female push_back called (0x00517A10) in ours, expanded in retail
(_M_insert_overflow); frame / register colouring as in the chunk-879 notes. Tried: scoped inline_depth(0) on the servant call (13837,
apprentice site then expands and operator-> calls appear), /d2inlT60..80, /d2inlS20/30 (T70 13625v13622 but 315 hunks).
Grades used: ~20.

## 0x00795300 -- real name CTCUnholyPower::FrameUpdate (catalog said CTCDivineWrath) -- DIFFER(9548v9268) (differ/00795300.cpp)
Identity: rtti-vtables CTCUnholyPower vtable 0x0126BFDC slot 9 -> 0x00795300 (CTCDivineWrath slot 9 -> 0x0078FD60), Ego_r.pdb
?FrameUpdate@CTCUnholyPower@@UAEXXZ; acctB dossier (ref/00795300/notes/DOSSIER.md). NAME_FIXES.tsv has the row.
Twin: 0x0078FD60 CTCDivineWrath::FrameUpdate differs ONLY in 11 constants (RemoveTC 0x86/0x85, SpellExclusionChannel +0xB0/+0xA8,
LoopingSound +0xC4/+0xBC, AddDamageWithAttribute 0xA/9, CEventHitBySpecialAbility 0x13/0x12): one source text, two rows.
Re-based the acctB draft (its hdr overlay was stale): copied the purely-additive stale headers (CCreatureAction_BuildUp/Release/
ReleaseAndLoopUnholyPowerSpell, CTCDExperienceOrb, CTCUnholyPower, CTCWillResponse); ported blocks into today's CThingCreatureBase
(_LEAVEORBS), CHitParameters (_SPECIALABILITY), C3DVector (_SUBTRACT_NAMED_RESULT, as the first branch of the subtract chain),
C2DBoxI (_RADIUS_INLINE, first branch of the radius-ctor chain); GETSTATE_INLINE = today's CWILLBUTTONHELPERTIMER_STATE_INLINE.
Dropped CINTELLIGENTPOINTER_ASSERT_ASSUME / CDEFPOINTER_ASSERT_ASSUME (__assume in operator->: compiler-steering, not retail text).
Residue: see line 2 of the source. First structural fact: ours makes the zero register (edi) at entry, retail first at 0x0079570C
(release start) -- every early `test reg,reg` becomes `cmp reg,edi`, the cancel-sound arms cannot tail-merge (ours pushes edi before
the GetString call). Grades used: 8.

## 0x0078FD60 CTCDivineWrath::FrameUpdate -- DIFFER(9548v9268), 145 reg-masked hunks (differ/0078fd60.cpp)
Identity: rtti-vtables CTCDivineWrath slot 9 -> 0x0078FD60; Ego_r.pdb ?FrameUpdate@CTCDivineWrath@@UAEXXZ. The H40 draft (ref/) is
SRC_FAIL with a large header debt; instead the row is GENERATED from the twin 0x00795300 source by renames (UnholyPower -> DivineWrath,
ST_BUILD_DIVINE_WRATH / HERO_ABILITY_SKILL_DIVINE_WRATH_SPELL strings verified with rstr.py at 0x0126BF90 / 0x0126BF40,
HERO_ABILITY_DIVINE_WRATH_SPELL (18), damage attribute 9, TCI_DIVINE_WRATH 133). It grades EXACTLY like the twin (9548v9268,
145 hunks), confirming one source text. Overlay twins: CTCDivineWrath.h _IMPLICIT_INFO_DTORS / _INLINE_ACCESSORS (the class has no
DarkFacesEffect: 200 v 208 bytes), CCreatureAction_{BuildUp,Release,ReleaseAndLoop}DivineWrathSpell.h _CCREATUREACTION_DIVINEWRATH
(renamed copies of the UnholyPower opt-ins; ctors Ego_r 0x008F57C0 / 0x008F5810 / 0x008F5870 names, retail 0x008BD170 / 0x008BD1C0 /
0x008BD220). Any fix found for one row applies to both. Grades used: 1.


## Decisions for the coordinator
1. NAME FIXES (NAME_FIXES.tsv): 0x00795300 is ?FrameUpdate@CTCUnholyPower@@UAEXXZ (catalog: CTCDivineWrath::FrameUpdate);
   0x0051E2F0 is CThingManager::ActivateThings (H40g draft's descriptive InitialiseLoadedThings).
2. Overlay edits that touch existing lines (preprocessed default text unchanged): C2DBoxI.h and C3DVector.h get a NEW first branch in an
   existing #if chain (old `#if`/`#ifdef` line becomes `#elif defined(...)`); CThingCreatureDef.h wraps the two opt-in lines Sex /
   CreatureType inside the existing _MEMBERS opt-in in `#ifdef _SEX_TYPED ... #else <old> #endif`. Accept, or move to separate names.
3. stlport/fable_map.h _TREE_ERASE_KEY_CALLS is an inline control in a shared header (noinline equal_range / erase(range), forceinline
   erase(key)) evidenced by retail 0x005034BE; it is a header opt-in, not a per-row pragma.
4. The acctB CINTELLIGENTPOINTER/CDEFPOINTER _ASSERT_ASSUME (__assume in operator->) opt-ins were dropped as compiler steering.
5. 0x00502E90 is an inline-budget residue (owner rule 2026-11-08 allows /d2inlT/S or scoped pragmas): no N found yet that fixes both
   the servant CreateRelevantPersonAndUpdateStats call and the female push_back expansion.

# SUCCESSOR (wfE10-2) -- appended; earlier sections above stay valid unless contradicted here
## 0x007A1B20 -- now 8 hunks, frame 0xbc = retail (differ/007a1b20.cpp)
- CLOSED frame / string slots: the two EndAttackeeSound criteria are IMPLICIT conversion temporaries
  `sound->PlayAttachedCriteria(PDef->EndAttackeeSound, true, false, false, -1)` / `PlayUnattachedCriteria(PDef->EndAttackeeSound, m_vEndPos, false, false)`
  with the existing per-TU opt-in FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (forwarder expanded, GetString 0x00415DD0 called: checked
  in the obj; without the opt-in the call goes to ??B 0x00415D70 = wrong target). Evidence: Xbox Anniversary PDB (S_REGREL32 list of
  CTCWillLightning::CLightningBeam::FrameUpdate: pTCLightning, lbChild, ptc_elec, hit_direction, hpParams, ptc_entity_events x2, hit_params,
  sound_player x2, vtStartPos, vtEndPos, upParams, lbChild) has NO criteria local. Explicit unnamed `.GetString()` puts GetString after the pushes.
- Open: decap copy x87 square order only (retail 0x007A1E68 fstp X, squares Z,Y,X; ours fst X, squares X,Z,Y like the plain copy which matches)
  + end-loop pad (follows: retail decap block 4 bytes longer). Tried (no change / worse): decap-only spellings `C3DVector d; d = a - b;` (2860),
  `d(a - b)`, `-(b - a)` (2844), `d(a); d -= b` (needs SUBTRACT_ASSIGN; 3016), `C3DVector(a) - b` (3016), inverted IsNonZero if (10 hunks),
  `d = d.Normalise()` (neutral). Comparator log (tools/lk on tmp/b1.cpp -> tmp/lk_b1.json): decap direction float records ids 796/797/798,
  plain 923/924/925 -- the same relative pattern, so the difference must come from records retail creates differently in the decap copy.
## 0x00507C30 -- still DIFFER(7661v7661) 2 lines, but the call target is now right (differ/00507c30.cpp, line 2 has the c2prio measurement)
- The old source called vector<char>(unsigned) (bound to 0x00411910) where retail calls 0x00410F30: q.py hid it. 0x00410F30 = CArray<char>(unsigned)
  out of line (NAME_FIXES.tsv), with a visible body: new opt-in FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE (hdr/README.md). The body-less
  existing SIZE_CTOR_OUTOFLINE grades 6 masked hunks (buffer not kept in edi, `mov esi,1` constant): c2 then assumes &buffer is captured.
- With it, the named `text`, `buffer[...] = 0` and no-text spellings all grade the same 2 lines (they used to break it). k=1..7 dead long
  locals at entry: no change. c2prio: the reload order = ascending split-piece id; buffer local (#2) -> #62 (free list), file #65 -> #295,
  this #67 -> #296+. Retail needs the buffer piece between file and this. Not closed.
## 0x00502E90 -- unchanged DIFFER(13933v13622) (successor tries)
- inline_depth(0) around BOTH the servant and the apprentice CreateRelevantPersonAndUpdateStats calls: 14080 (operator-> of peopleDef and the
  other inline args become calls; retail inlines peopleDef-> ([edi+68h]) and calls CThing::GetPos 0x004C73D0 at that site). /d2inlT90/110/130,
  /d2inlS40/60: 14120 / 14534 / 15372 / 14162 / 14115, call diffs 18..32 (worse than baseline 7 diff regions).
- Reading of the budget: ours expands the servant site (more headroom there than retail) and then has none left for the female push_back;
  retail must have spent more IL BEFORE the servant site. fwsize (tmp/fw_v0.txt) lists the earlier helpers whose FableWin bodies are larger
  (CThingAICreatureInit ctor/dtor +9i, CIntelligentPointer ctors +9i, CDefPointer operator-> assert bodies): the place to look next.
## 0x00795300 / 0x0078FD60 twins -- DIFFER(9472v9268), 139 masked hunks (was 9548 / 145)
- Zero register now made late as retail (ours 0x00795724 v retail 0x0079570C): the GetAllThingsInRoughArea<> body must be VISIBLE
  (existing opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY + CTCMAPWHO / _SEARCHID, includes CTCMapwho.h / NThingMap_CThingMap.h before
  CThingSearchTools.h with `class CTCMapwho;` first, as the landed CWorldMap::IsLineOfSight 0x0063E6F0). Same lesson as 0x00507C30: c2 then
  knows &things is not captured, so the dead `_M_data = 0` store of the list ctor before malloc goes (retail 0x00795516 has none).
- Cancel-sound arms: both must be implicit conversions `PlaySoundW(PDef->CancelSound[SpellLevel], false)` (the 2nd arm's temporary is built
  BEFORE `push 0` in retail; explicit .GetString() pushes first). Arm 2 must call GetString 0x00415DD0, arm 1 expands it. Default header:
  arm 2 calls ??B 0x00415D70 (wrong); new FORCEINLINE_FORWARDER: both expand; + scoped inline_depth(1) on arm 2: no change; inline_depth(0):
  calls ??B and expands operator-> / vector[] (wrong). Untried: a budget flag (/d2inl*) with FORWARDER, or arm 1 through a different helper.
- Remaining after that (cmpcalls): extra PlaySound call (no tail merge), CEventDied AddEvent duplicated, retail calls list dtor 0x006F09D0,
  CIntelligentPointer copy 0x0078E5B0 and IS_THING_ALIVE in the spell-over loop where ours expands them; frame 0x248 v 0x230.
- Xbox Anniversary PDB locals of CTCUnholyPower::FrameUpdate (weak third source; that build reworked the function): creatures, search_area,
  ptc_hero_stats, findit, something, pspecial_effects, effect, objects, ptc_experience, pdef, hit_param, point_of_hit, direction,
  ptc_entity_events, finalaction, effectname, info, effect_pos, it, ptc_nymph, ptc_cmm, ptc_reward, ptc_deed_log, pcreature_special_effects,
  pmy_enemy_tc, focus -- usable for local naming; no cancel-sound criteria local (consistent with the implicit conversion).

## Decisions for the coordinator (successor additions)
6. New overlay opt-ins: engine/CArray.h FABLE_RETAIL_CARRAY_SIZE_CTOR_VISIBLE_NOINLINE (0x00507C30) and engine/CDefString.h
   FABLE_RETAIL_CDEFSTRING_FORCEINLINE_FORWARDER (twins) -- both per-TU inline controls with byte evidence (hdr/README.md); both are new first
   branches of existing #if chains (default text unchanged; no FABLE_STATIC_ASSERT in either header).
7. NAME FIX (probable): 0x00410F30 is ??0?$CArray@D@@QAE@I@Z, not the landed descriptive vector<signed char>(unsigned) leaf (NAME_FIXES.tsv).
   The landed 0x00507C30-era differ sources calling vector<char>(unsigned) would have bound the wrong address (0x00411910).
8. 0x007A1B20 uses the existing FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION (implicit criteria conversions; Xbox PDB has no criteria local).
- (0x00507C30 more) `long size = (length == -1) ? file.GetLength() : length;`, `long size = length; if (size == -1) ...`, a named
  `size = file.GetLength() + 1` for the buffer: all the same 2 lines (insensitive with the visible-noinline ctor).

# THIRD AUTHOR (wfE10-3) -- appended
## 0x00507C30 (wfE10-3)
- NEW FACT (c2prio_f.py on probe/r3.cpp -> tmp/prio_r3.txt): local NAMES change c2's original candidate ids (hash-order walk with a free list):
  renaming to the Xbox-PDB names (file_length, map_id, map_uid, map_posx, map_posy, Sea, loaded_on_player_proximity, level_name,
  level_filename, level_script_name, parsed_item, buffer->text) swapped the ids of mapY (5 -> 70) and mapUID (70 -> 5); the buffer-data
  local stays #2 and still gets a free-list piece (#104) below file (#295). Grade unchanged (2 lines). 12 names for the `file` param: no change
  (param ids fixed). Dead `long uK = 0;` k=1..4 before the length test and before the parse loop: no change.
- Model: pieces are handed walking parents in ascending original id; the local (#2) always comes first and takes the free-list top, so
  the reload order file < local < this needs file to get a smaller piece (free list) than the local, i.e. file walked before the local.
## 0x00502E90 (wfE10-3) -- inline trace (tools/inl.sh = C26 c2 inliner trace, tools/inl/an.py) measured; the model is now explicit
- Trace of differ (tmp/inl_p0.txt): T=74, expand iff sc < 74. Servant site = decision 481 (sz 336, a 99, g 8342, pen 0, sc 29 -> expanded);
  apprentice 521 (g 8913, a+g > 9000 -> step penalty 168, sc 151 -> called). Fill-loop push_backs: male 569 sc 124, FEMALE 584 sc 97,
  children 599 sc 159, all with pen 25 (a+g > 9000) -> all called. Without pen they would be 75.6 / 59 / 95 = exactly retail's P O P.
- probe/lc1.cpp: `bool listCreatures = true;` before the servant loop, passed at the servant + apprentice sites only (instead of the literal
  `true`): the constant-argument savings vanish (sav [40,0,..], c1 296, sc 214) -> both CALLED, g at the fill loop drops by ~494 -> pen 0 ->
  male called / female EXPANDED / children called: the key-call pattern (tools/kc.sh) now equals retail's EXACTLY at those sites
  (`| C | R | R | C C P C C O C C P`). Grade 14064 (longer): side effects -- the fill-loop CThingAICreatureInit ctor (0x004FACD0) sites
  538/553/566 now expand (retail calls all five), a 0x00512210 helper expands at two sites (retail calls), one Create call lost (cdiff #118).
- Contradiction found: retail CALLS all five fill-loop init ctors (needs the step penalty there: g+17 > 9000) yet EXPANDS the female push_back
  a few decisions later (needs NO penalty, or a higher site frequency f > 0.281 v 0.2142). With identical male/female branch structure
  (retail 0x00504A01.. / 0x00504BB2.. are the same shape) the plain model cannot give both: some cost input of the female site differs in
  retail (its f, or its callee: e.g. `females.insert(females.end(), creature)` would fold to the same bytes with a different price). Untried.
- tools: kc.sh (key-call pattern from the last obj), cdiff.py (call targets by address v retail), fd.py (first differing instr), anch.py.
- (wfE10-3) differ/00502e90.cpp now uses the SHARED engine/CThingAICreatureInit.h (+ existing opt-in FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY) instead of a row-local class model: same grade 13933v13622. Other row-local classes remain (CVillagePeopleDef: no shared header, the landed 0x004FF750 does the same; CSimBuildingDef, CTCDummyVillager, CTCDNone: check engine/ headers before landing). The lc1 probe (variable listCreatures) is saved as differ/00502e90_lc1_probe.cpp (a probe, not a candidate).
- (wfE10-3) More measured constraints for 0x00502E90 (from the p0 / lc1 traces): retail CALLS SetHomeBuilding (0x00512210) at two house sites
  where p0 expands it with sc 72 (pen 39, g 9662): retail needs sc >= 74 there, i.e. a+g about 730 LARGER than p0 (p0 already includes the
  494 of the expanded servant). So retail's accumulated inline size g is larger all along (>= +560 before the servant site, >= +1200 v lc1 by
  the house loop): some helper expanded at many sites has a bigger IL body in retail (candidates from fwsize tmp/fw_v0.txt: CIntelligentPointer
  <CThingBuilding/CThingAICreature>(T*) ctors and copy ctor +9i, CThingAICreatureInit ctor/dtor +9i, Set{Home,Work,Father,Mother,Spouse}
  +10i). The female push_back must ALSO have a different price (it is expanded although a step penalty applies everywhere late): unexplained.
  `females.insert(females.end(), creature)`: called (worse). Next: helper-text level (bigger-IL but same-code bodies of the frequent helpers).
## Decisions for the coordinator (wfE10-3 additions)
9. No row matched in this pass; no header / opt-in changes (hdr/ untouched), no new names. differ/00502e90.cpp switched from a row-local
   CThingAICreatureInit model to the shared header + existing FABLE_RETAIL_CTHINGAICREATUREINIT_CTOR_BODY (same grade).
10. 0x00502E90: if a variable `listCreatures` local at the servant/apprentice sites is ever proposed, it is a FITTED inline-price device
   (no evidence in retail beyond the decisions); it fixes those sites but not the row (see the wfE10-3 section).
11. Grades used this pass: about 45 (007a1b20 11, 00507c30 22, 00502e90 12) + 3 c2 traces (c2prio x1, inliner x2).

# FOURTH AUTHOR (wfE10-4) -- appended
## 0x00507C30 CWorldMap::LoadFromFile -- RELOCATION_MATCH, test PASS, vland RELOCATION_MATCH PASS, addmatch done (src/00507c30.cpp)
- What closed it: device d (owner acceptance test d) -- 3 dead release asserts `FABLE_RETAIL_ASSERT(...)` (overlay fable_assert.h with
  FABLE_RETAIL_ASSERT_DEAD_ARM) right before `CMapInfo& info = Maps[newMap];` in the EndMap block. Their dead arms' IL records re-key c2's
  split-piece numbering (c2prio trace tmp/507_prio_a.txt: the reload order at the loop exit = ascending piece id; pieces are handed out
  from a LIFO free list in a hash-order walk), so the exit reloads become ebp(file) / edi(buffer data) / ebx(this) as retail 0x00509395.
  Conditions FITTED (any condition grades the same); count measured 0..7 at that site, only 3 matches. k unused int locals 1..7 at 5 sites:
  no effect (unused locals are dropped before ids are assigned).
- WARNING (fragility): the matching count/position is hash-walk sensitive: every TU change (an added #include of engine/CPlayerGui.h or
  engine/CExceptionBase.h, a renamed declaration) moved it (2 at entry -> 4 at EndMap -> 3 at EndMap). A future header change that adds
  symbols to this TU may need a re-sweep: tools/507swa.py <src> endmap,after,tail,entry (strips the asserts, writes N=0..7 variants).
- Names fixed in the row (two sources each): 0x0041A060 NResourceDirectoryNames::GetLevelDir (Ego_r 0x0042B8E0; landed CWorldMap::
  SaveMapsToString 0x004FB990 declares it so) was a descriptive GetLevelsDirectory; 0x00438570 CPlayerGui::PeekGuiDef (Ego_r 0x0044B230,
  header opt-in FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF) + CPlayerGuiDef::RegionGraphFileName (+0xA94, FABLE_RETAIL_CPLAYERGUIDEF_CLASS) replaced
  a raw +0xA94 cast of a descriptive GetPlayerGUIDef; 0x009BA390 CStringParser::ReadLineAsString (Ego_r 0x00B0EE30; FABLE_RETAIL_CSTRINGPARSER
  _READLINE) replaced ReadAsStringUntilWhitespace (WRONG TARGET 0x009BA4A0 hidden by q.py masking); globals g_bUseCompiledWorldFiles /
  g_WADBuildEnabled / g_bUseCompiledGlobalThings (landed spellings) replaced GLoadBinaryWorld / GSaveBinaryWorld / GLoadGlobalThings...;
  NProfileTimer from fable_profile.h; CGenericException : CExceptionBase (shared engine/CExceptionBase.h; ThrowInfo __TI2, landed
  CWStringParser::ReadAsFloat model) instead of a base-less local class. The buffer is `CArray<char> text` (Xbox PDB local `text`).
- Header changes: none (existing opt-ins only; fable_assert.h already in hdr/).
- Test: test/00507c30.cpp embeds the row's function text verbatim over a mock engine (3 runs: text world, binary world, missing quote);
  a mutant without `info.Used = false` fails.
## 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate -- RELOCATION_MATCH, test PASS, vland RELOCATION_MATCH PASS, addmatch done (src/007a1b20.cpp)
- What closed it: device d -- ONE dead release assert right after the decapitating hit's `C3DVector direction = ...`:
  `FABLE_RETAIL_ASSERT(direction.X == direction.X && direction.Y == direction.Y && direction.Z == direction.Z);` (NaN check; overlay
  fable_assert.h + FABLE_RETAIL_ASSERT_DEAD_ARM). The dead arm's IL reads the components and changes the x87 stacker's choice for that
  copy only: retail 0x007A1E68 fstp X / squares Z,Y,X, now ours too; closes the 4-byte gap and the end-loop pad (all 8 hunks).
  Measured: asserts with no direction read (1..7 at function / case 3 / clock / before either copy's IsNonZero): inert; `direction ==
  direction`, IsNonZero(), Y-only and Z-only NaN tests: no; X-only and the 3-component test (1..4 copies): match. Condition FITTED.
- Rest of the source is the wfE10-2 differ unchanged (opt-ins listed in hdr/README.md wfE10 section); line 3 now says `ret 0Ch`.
- vland RELOC_NAME warnings (labels, not our spellings): 0x0079F730 is ?AllChildrenInactive@CLightningBeam@CTCWillLightning@@QAE_NXZ
  (Ego_r 0x007A4D20 / FableWin 0x026B2D00; the label 'CLightningBeam::AllBeamsInactive' mixes it with CTCWillLightning::AllBeamsInactive);
  0x0129BA3C DELTA_NOTIONAL_ZERO (label g_NormaliseEpsilon); 0x007A3200 CCountedPointer<CLightningBeam> dtor.
- Test: test/007a1b20.cpp embeds the row's function text verbatim over a mock engine (all 5 states, both hit kinds, zero direction,
  blocked hit, end sounds, child erase); the mutant with a +Y zero-direction fallback fails.
## Decisions for the coordinator (wfE10-4)
12. Both matches use device d (dead release asserts through the overlay fable_assert.h, FABLE_RETAIL_ASSERT_DEAD_ARM), counts and
    conditions stated in each source header (FITTED conditions): 0x00507C30 three asserts before Maps[newMap] in EndMap; 0x007A1B20 one
    NaN-check assert after the decap direction. fable_assert.h is an acctC overlay header (not in rebuild/include): it must land with them.
13. 0x00507C30 name fixes in the row (see its section): NResourceDirectoryNames::GetLevelDir, CPlayerGui::PeekGuiDef +
    CPlayerGuiDef::RegionGraphFileName, CStringParser::ReadLineAsString (was a WRONG CALL TARGET 0x009BA4A0 hidden by q.py), landed global
    spellings, shared CExceptionBase. NAME_FIXES.tsv: 0x00410F30 CArray<char> ctor, 0x006BC4A0 CRegion::AddMap, 0x0050BE40
    CCountedPointer<CAFile>::DeleteData, 0x0079F730 CLightningBeam::AllChildrenInactive.
14. No header / opt-in changes in this pass.

# FIFTH AUTHOR (wfE10-5) -- appended
## 0x00502E90 (wfE10-5) -- DIFFER(13572v13622), call list by target = retail except one cross-jump merge (differ/00502e90.cpp)
- NEW evidenced fix: the erase(key) at 0x005034BE: our hand count loop `for(i=p.first;i!=p.second;++i) ++n;` survived as a 30-instruction
  node walk; STLport 4.6 _tree.c's own `n = distance(p.first, p.second)` (with FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE) lets VC7.1 drop it
  as retail does. New nested opt-in FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE in hdr/stlport/fable_map.h (hdr/README.md).
- Inline set now = retail at every key site (tools/502kc.sh: I=init ctor C=Create P=push_back R=CreateRelevant H=SetHomeBuilding |=DP)
  with scoped `#pragma inline_depth(0)` (device b) around: servant + apprentice R calls (servant def indices hoisted to named locals
  like the apprentice/shop sites, so peopleDef-> stays inline), the 5 fill-loop `CThingAICreatureInit init(...)` decls, the
  `candidates(males)` copy, the 3 house-loop SetHomeBuilding calls (back() hoisted to a named local: retail inlines back()).
  Trace (tools/inl.sh, tmp/inl_502p2d.txt): ours g stays below retail's all through the late part (retail needs pen>0 at the vector
  copy, the house SetHomeBuildings, the fill-loop ctors) -- the per-site pragmas are the device for that.  /d2inlT72/73 instead of the
  house/candidates pragmas: map::find (0x00513420) then expands (pen falls with T) -- rejected.
- Remaining: (1) teacher-site R expansion: ours 4 Creates (the fallback `else if (maleDefIndex)` copy not cross-jumped into the first
  male copy: ours pushes edx/eax in the first copy's overflow path, retail ecx/edx -> retail jmp 0x00503810); shopkeeper site 2 Creates
  v retail 3. (2) frame 0x19c v 0x194 (pre-existing, also in the base); 223 masked hunks, mostly slot/colour.
- Anniversary PDB locals (weak) of this function are in tmp/anniv_syms.txt line 75020 (men_shuffled = candidates, ptc_talk, init x14).
## 0x00795300 / 0x0078FD60 twins (wfE10-5) -- DIFFER(9363v9268), 104 masked hunks, call list = retail but one dtor (differ/*.cpp)
- tools/795patch.py applies the same edits to either twin (both grade 9363v9268 / 104).
- (1) Visible CDefClassBase::GetSubDef<T> body (text of the 0x00502E90 row) + existing FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME and
  _GETEFFECT_BODY (landed same-TU CTCUnholyPower ctor 0x007941D0): retail keeps creature_effects in ebp after GetSubDef 0x00669580
  (0x00796CA9); ours had a zero register in ebp there (cmp ebp,reg / AddEvent of CEventDied tail-duplicated). Fixed both. 9405 -> 9363.
- (2) Stamina cancel arm: `{ CCharString sound = PDef->CancelSound[SpellLevel].GetString(); PlaySoundW(sound, false); }` (FITTED): the
  GetString call 0x00415DD0 (an explicit call at f 0.03 is refused, sc ~92) with the temporary built BEFORE `push 0`, and the arms
  tail-merge (arm 1 jmp into arm 2's PlaySoundW). Implicit conversion: GetString expands at both arms (nested decisions under the
  forceinline forwarder get f=1.0, sc 17); inline_depth(0) suppresses the forceinline forwarder (??B called); inline_depth(1) inert.
- (3) Device d: three dead `FABLE_RETAIL_ASSERT(PDef->DamagePerHit[SpellLevel] >= 0.0f);` right after `float damage = ...` before the
  spell-over loop. Their condition's inline calls (CDefPointer operator->, vector[]) are expanded by c2's inliner although dead, so g
  rises and the step penalty reaches the loop-top decisions as in retail: IP copy 0x0078E5B0, IS_THING_ALIVE 0x006961F0, bitset test
  (0x00410DE0), SetLeaveExperienceOrbsOverride 0x0078DDB0 x2 called. 0, 1 and 2 asserts leave them expanded (measured).
  LESSON: dead release asserts with inline-call conditions are an inline-BUDGET lever (they add g without code).
- Open: frame 0x240 v 0x230 (16 bytes); release-loop ~name (VICTIM_EFFECT) cross-jumped in ours (18 v 19 ~CCharString calls); the
  scoped inline_depth(0) on the spell-over copy/IS_THING_ALIVE (tried before the asserts) also calls the tree iterator operator->.
- (wfE10-5, later) + new opt-in FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST (FableWin four-field initialiser as the first branch of
  the box-ctor chain; 9363 -> 9349) + existing FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY (visible by-name GetDef<T> body: the
  HERO_ABILITY def stays in esi across AddTrainableStatExperience and its release loses the null test, retail 0x007959B2) -> 9303v9268,
  85 masked hunks. Tested and rejected: __assume(Object) in CDefPointer operator-> (new opt-in tried in a scratch copy only, not kept:
  9365 / 211 hunks, and the null test stayed -- it was the reload, not the assert). Assert count re-measured after each change: 3 (4 equal).
- Twin source edits are all in tools/795patch.py (run on ref drafts 795_base / 78f_base = the old differ sources).
- (wfE10-5) Twin probe, NOT kept: one shared `CThingFilter_OverlappingCircle circle` for the whole release part (both GetAllThingsInRoughArea
  calls) gives 9277 (and 9247v9268 with 4 asserts, 88 hunks, call list still right) -- shorter than retail, frame still 0x240: retail most
  likely builds the circle twice. Frame 0x240 v 0x230 open (our /FAs slot table: tools/795fr.sh; this at retail 0x38, ours 0x4c).
- (wfE10-5) 0x00502E90 differ now also declares noGuards before noGhosts (retail loads NumGhosts +0xA4 into ecx, NumGuards into eax at
  0x00502F5B; registers now equal there, same hunk count). k = 1..7 unused locals at entry: byte-identical (inert).
- (wfE10-5) Landing blockers still in 0x00502E90: row-local class models with shared headers (CShopDef, CSimBuildingDef, CTCBuyableHouse,
  CTCThingOwner, CTCTalk, CTCDummyVillager); twins: row-local NEntityEvents::CEventDied etc. (check engine/ headers before landing).
## Decisions for the coordinator (wfE10-5)
15. New overlay opt-ins (additive; default text unchanged; hdr/README.md): stlport/fable_map.h FABLE_RETAIL_STLP_TREE_ERASE_KEY_DISTANCE
    (STLport 4.6 text, row 0x00502E90) and engine/C2DBoxI.h FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST (FableWin text as a new first
    branch, twins). Matched rows 0x007A1B20 / 0x00507C30 regraded after both: still RELOCATION_MATCH.
16. Devices in the open rows (none matched): 0x00502E90 uses 10 scoped `#pragma inline_depth(0)` regions (+2 hoisted locals for the
    servant def indices and 3 for back()); the twins use 3 dead asserts (device d) and one FITTED named-temporary block. If the owner
    prefers, the 502 pragmas might be replaceable by device-d asserts (the twins showed asserts move g): untested.
17. __assume(Object) in CDefPointer operator-> (acctB idea) re-tested in a scratch header: worse (211 hunks), not kept.
- (wfE10-5, last) Twins now use the SHARED opt-in FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (engine/CDefinitionManager.h, FableWin
  0x01CCD0F0 text, noinline) instead of a row-local template body: identical grade 9303v9268 / 85. (0x00502E90 still carries the older
  row-local GetSubDef<T> body: switch it to the same opt-in before landing.) FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY: inert.
- Grades this pass (wfE10-5): 0x00502E90 ~45 + 2 inliner traces; twins ~80 + 1 inliner trace.

## wfE10-6 re-verify (2026-11-15) -- 0x00507C30 / 0x007A1B20
- Assigned again as "parked" by the coordinator, but both were already closed by wfE10-4. Re-verified against today's rebuild/include +
  hdr/ overlay (after the other agent's additive opt-ins): q.py RELOCATION_MATCH both (no flags); vt.py PASS both; read-only vland
  RELOCATION_MATCH PASS both (WINS 2); payload.json source/test equal to src/ and test/. Nothing re-authored.
- vland RELOC_NAME warnings on 0x007A1B20 for 0x007A3200 (CCountedPointer<CLightningBeam> dtor) and 0x0129BA3C (DELTA_NOTIONAL_ZERO) are
  Ghidra-label disagreements only: rebuild/oracles/reloc_bindings.tsv already binds those exact spellings to those addresses. The other
  warnings (0x0079F730, 0x00410F30, 0x006BC4A0, 0x0050BE40) are already in NAME_FIXES.tsv.

## wfE10-7 (successor) -- 0x00795300 / 0x0078FD60 twins
- NEW (measured): (1) the two `CThingFilter_CanBeHitByEpicSpell can_be_hit;` named objects -> value-initialised temporaries in the
  Bind2 argument, `CFilter_Bind2<...>(CThingFilter_OverlappingCircle(pos, radius), CThingFilter_CanBeHitByEpicSpell())`, with the row-local
  class WITHOUT the std::unary_function base (as the landed 0x006A8D30 row models it; POD, so `T()` zero-fills its byte): gives retail's
  `mov byte [esp+0x1f],0` stores (0x00795579 / 0x00795A19) and frame 0x230 (was 0x240). (2) behaviour fix: the keep test of the
  spell-over erase loop is `offset.GetSquaredMagnitude() > reach * reach + 5.0f` (retail 0x00796294 `test ah,5 ; jnp reset`: keep only
  when strictly greater); the old `<=` inverted the branch. (3) the hit sites: retail builds `GetPos() - dir` X first with each store
  right after its fsub and no copy (0x00795B30.., 0x0079665C.., 0x0079704x) = the landed SpiritAttack 0x00854DD0 idiom: DEFAULT
  operator- (drop FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT) + visible noinline 9-arg CHitParameters ctor (HIT_CTOR + HIT_CTOR_BODY +
  CTOR instead of SPECIALABILITY; new opt-in CHITPARAMETERS_EPICSPELL_SETTERS supplies the two setters). Named result + ctor body: the
  temporary is still copied (z through ecx), 9340. (4) device-d assert count must be re-swept after each change (tools/asw.sh).
- (wfE10-7) Best now differ/00795300.cpp = differ/0078fd60.cpp: DIFFER(9284v9268), 63 masked hunks, call list = retail but the one cross-jumped
  ~CCharString (tools/795patch7.py turns a wfE10-5 source into it). Matched rows 0x007A1B20 / 0x00507C30 regraded after the CHitParameters.h
  addition: still RELOCATION_MATCH.
- (wfE10-7) Measured but not kept: Xbox-PDB local names (creatures, search_area, something, hit_param, direction, effectname, ...; tools/xren.py):
  byte-identical (local names do not move these ties). point_of_hit named local: no change. SQUAREDMAGNITUDE_XYZ/ZYX/none: identical.
  Normalise opt-ins: PAREN_SQUARES_XYZ best (fixes dir site 3); site 2 (0x007965C2, retail Z,Y,X with `fst/fmul`) still X,Z,Y.
  k unused locals at entry: inert. Dead-assert count re-swept 0..20 on each base: 8 best (spell-over loop top called as retail).
- (wfE10-7) Sweep-2 `++it` arm placement (retail 0x007961D9 right after the test, ours last before the latch): FIXED by writing the test as
  `if (!IS_THING_ALIVE(creature))` (or IS_THING_DEAD) -- the bool-returning inline function changes c2's block order -- but that site's
  extra inline layer then makes the sweep-2 map erase (0x00797A60, retail calls) expand and the bitset test call 0x004118C0 (9335, 81
  hunks); no assert count 0..20 or asserts before the loop restores it; `creature == 0 || ...` / `!creature.Get() || ...` keep the old
  placement. Open lead: an inline control for the erase only (scoped inline_depth(0) also un-inlines the iterator copy: 9291).
- (wfE10-7) Inliner traces (tools/inl.sh, tmp/inl_d7.txt = best source, tmp/inl_t2.txt = IS_THING_ALIVE probe): sweep-2 erase decision
  sc = c1*(1/f+6)/7 with c1 73: best f 0.6152 -> sc 79 > T 74 (called, = retail); probe f 1.2305 -> sc 68 (expanded). The bool-function
  test makes the else arm 1/2 instead of 1/4 likely. /d2inlT67 on the probe calls the erase and, with 4 dead asserts, gives frame 0x230,
  retail x87 orders at the range test and dir site 2, and no block hunk (9202v9268, 66 hunks) -- but small-c1 helpers then lose their
  forced inline (map::find wrapper c1 9 called at 0x007955F2 / sweep 1) and HasTC 0x004118C0 is called where retail calls
  bitset::test 0x00410DE0 (the latter is in the probe at default T too). T68 = T67; T70/72 re-expand the erase. Rejected; kept as
  differ/00795300_isalive_probe.cpp. Next idea: a spelling of the sweep-2 dead/null test that keeps f(else) = 1/4 (two branches) but
  lets c2 put the ++it arm first (retail), or find why retail's erase arm, not the ++it arm, falls into the latch.
- (wfE10-7) 0x00502E90 not reopened this pass (no measured lead beyond the wfE10-5 note: device-d asserts as g lever might replace some of its
  10 inline_depth pragmas -- untested; the six row-local class models are still landing blockers). Grades this pass: twins ~200 + 2 inliner traces.
## Decisions for the coordinator (wfE10-7)
18. New overlay opt-in engine/CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_EPICSPELL_SETTERS (two inline setters, additive; lets a TU use
    HIT_CTOR + HIT_CTOR_BODY instead of SPECIALABILITY). Matched rows regraded: still RELOCATION_MATCH.
19. Behaviour fix in the twins: the spell-over keep test is `!(mag > reach*reach + 5.0f)` (retail jnp); the wfE10-5 source had `<=` (NaN differs)
    and an intermediate draft `>` (inverted).
20. Twins' row-local CThingFilter_CanBeHitByEpicSpell no longer derives from std::unary_function (matches landed 0x006A8D30 model; needed for
    the value-initialised temporary's zero byte). No shared header exists for this class.
21. Twins still use device d (8 dead asserts, count re-swept) and one FITTED named temporary (stamina cancel sound).

## wfE10-8 (successor) -- 0x00795300 / 0x0078FD60 twins (work file probe/n8/F1.cpp; differ/ updated at milestones)
- (1) EVIDENCED: release (lit) loop `goto next_creature` -> `continue` (label removed): the ~CCharString cross-jump at retail 0x0079600A
  disappears; call list by target = retail. (2) EVIDENCED: retail rebuilds the address of a CIntelligentPointer temporary with `lea`
  after its ctor call (0x00795640, 0x00795E20, 0x00795F0C), ours used the ctor's eax: implicit conversions
  `LastFrameCreaturesHit[creature].ActualCreature = creature;` / `.Effect = CTCSpecialAbilities::CreateParticleEffect(..)` /
  `creature_effect = ..` / `eit->Effect = ..` (no explicit CIntelligentPointer<T>(..)). (3) sweep-2 test `if (!IS_THING_ALIVE(creature))`
  (retail 0x007961D9 ++it arm first) + FITTED guard `it = begin(); if (it != end()) for (; it != end(); )`: the guard's 1/2 halves the
  loop's c2 frequency (body 9x of 0.3125 -> of 0.156), so the erase 0x00797A60 is called, the keep-arm ++ 0x00793A00 called and the
  dead/erase-arm ++ expanded, all as retail (model: sc = c1(1/f+6)/7 v 74; erase c1 73, ++ c1 56; needs erase-arm f in (0.31, 0.91]).
  `if (!empty())` on the lit loop also works but leaves a double je. (4) device d re-swept: 6 asserts (6 = 8 byte-identical; 5 differs).
  Result F1: DIFFER(9313v9268), 51 masked hunks, call list = retail. Remaining: frame 0x238 v 0x230 + many frame offsets past 0x7F
  (disp32: most of the +45 bytes), box ctor register picks, ga/PeekLightingChannel spill, x87 order at 0x0079626E / 0x007965C2.
- (wfE10-8, cont.) FRAME: c2 packer replay (tools/pk/pk.py trace + tools/psim.py simulator with K&R qsort from tools/pk/sim.py; votes of
  retail slots from efm.py via tools/n8fm.sh; tools/psearch2.py searches weight changes) showed ours had one 4-byte slot too many.
  (5) EVIDENCED by the packer model + retail 0x00796CBE/0x00796D57 (`effect = 0` per arm, effect shares creature_effects' slot): the
  explode part declares `long effect = 0; bool found_effect = ..` inside EACH arm (two light nodes instead of one weight-6 node):
  frame 0x238 -> 0x230 = retail (i1: 9258v9268, 51 hunks). (6) FITTED: one function-scope `CThingCreatureBase* creature;` for the
  sweep-1 and release loops (retail shares their slot -544 and keeps `effects` alone at -480; the only way found to make effects
  conflict with the sweep-1 creature): k1 9273v9268, 53 hunks, differ/00795300.cpp. Remaining frame residue = qsort tie orders
  (n/eend slot v creature slot at 10250, names v dir 3000, found/slot10/owner 2250, box v pos 1500, info 350) + murder$ (spell-over)
  slot: tools/qsearch.py shows single slot-creation-order moves that fix all ties (e.g. the n/eend slot created ~8th). Not found yet.
- (wfE10-8) Twin generator tools/mktwin.py (renames + twin header) replaces 795patch7.py; T_k1 grades identical (9273 / 53).
- (wfE10-8) Landing hygiene in both twins (grade unchanged, byte-identical): local CMessageEventDef -> engine/CMessageEventDef.h; local
  CEventBase / CEventHitBySpecialAbility -> engine/NEntityEvents_CEventHitBy.h (the remaining local CEventKilledCreature / CEventDied
  have no header; their pads are now _pad08 over the header's 8-byte CEventBase, else `new` sizes 0x1C); (EHeroAbility)0x13 ->
  HERO_ABILITY_UNHOLY_POWER_SPELL (engine/EHeroAbility.h). Still row-local (no header anywhere): CThingFilter_CanBeHitByEpicSpell,
  CTCDRumble, CTCNymph, EQuakeStrength/EQuakeLength, CEventKilledCreature, CEventDied.
- (wfE10-8) Device d re-swept on k1: 8 asserts best (50 hunks; also fixes the 0x00796664 push order and half of the Normalise site 2).
- (wfE10-8) Zero-register residue measured with c2prio_f.py --trace (tmp/prioT_ka8.txt): ours colours const-0 pieces #44/#402 into ebp
  late (prio -34, ebp free) in the damage-loop / spell-over murder blocks, because the two `this` pieces there (#415, #89) are
  "spill 0 <= 0: not worth a register" (1 ref each?); retail keeps a this-piece in ebp at 0x00796A62 (GetDefinitionManager + GetWorld)
  and murder.Object in ebp at 0x00797372. Next: find why our this-pieces carry spill 0 (--blocks) -- the split points differ.
- (wfE10-8) Tried, no effect: GetSquaredMagnitude XYZ / ZYX / default at the range test (c2 reassociates; order stays Z,Y,X v retail
  X,Z,Y), unnamed `(a - b).GetSquaredMagnitude()`, named dist_sq; Normalise PAREN_SQUARES_ZYX (= XYZ), LOCALS(_XYZ), UNPAREN (worse);
  effects declared before IsCharging (early zero store appears); FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE (respawn ctor called);
  STLP_MALLOC_ALLOC_DEALLOCATE / ALLOC_PROXY_REBIND (erase c1 unchanged 73); mid-expression inline_depth pragmas (both call nodes
  sample the pragma at the closing paren); inline_depth(1)/(2) regions are inert (only 0 acts per site).
- (wfE10-8) Device c: one unused `long unused0 = 0;` at entry fixes Normalise site 2 x87 order (k=1..3 identical, 4..7 worse); asserts re-swept with it: 8 still best. Best now probe/n8/u2.cpp = differ/00795300.cpp: DIFFER(9273v9268), 48 masked hunks.
- (wfE10-8) BOX SITES: `long x = (long)pos.X; long y = (long)pos.Y; C2DBoxI box(C2DCoordI(x, y), (long)(radius + 20.0f));` (both sites;
  named x/y, the coord AND the radius as argument expressions) removes every box-ctor / Bind2-argument / ga-spill register hunk
  (0x007954F8.., 0x00795553.., 0x0079567F.., 0x007958DA..): 48 -> 37. Spellings that do NOT: named `C2DCoordI centre`, named `long r`
  with the temporary coord (radius ftol hoisted), `C2DBoxI box(C2DCoordI(centre), r)`, 4-long ctor, all-in-one temporaries.
  Then device d re-swept: 6 asserts (32 hunks); device c (unused local) no longer needed (byte-identical without it).
  Best: probe/n8/V1.cpp = differ/00795300.cpp, DIFFER(9273v9268), 32 masked hunks; twin T_V1 = differ/0078fd60.cpp, same.
- (wfE10-8) 0x00502E90: only probed (2 grades + 1 inliner trace, probe/n9/): without its 14 inline_depth(0) regions the CreateRelevant
  sites sit at f=5.625 (= 9x of 0.625: the code after the VillageBuildings `for (...; ++b)` loop stays at its 5x, playbook rule).
  Lead for a successor: loop/guard shapes (for v while, `continue`, guards) as on 0x00795300 may replace the pragmas; 223 masked hunks,
  six row-local class models with shared headers still block landing. Not reworked this pass.
## Decisions for the coordinator (wfE10-8)
22. Twins 0x00795300 / 0x0078FD60: FITTED spellings (marked in the source header): sweep-2 guard `if (it != end())` (c2 loop
    frequency), one function-scope `CThingCreatureBase* creature` for sweep 1 + release loop (frame packer). Device d: 6 dead asserts.
    No new header opt-ins this pass; matched rows 0x007A1B20 / 0x00507C30 untouched (no header changed).
23. Remaining twin residue is register allocation (const-0 pieces in ebp where retail keeps `this` / murder.Object in ebp), one x87
    order (range test), frame-slot qsort ties that likely follow from the RA residue (packer weights count spill reloads).

## wfE10-9 (successor) -- 0x00795300 / 0x0078FD60 twins re-measured; 0x00502E90 not reopened (probe/n10/, ~47 grades + 1 c2 --blocks trace + 1 frame map)
- Base re-graded: differ/00795300.cpp = probe/n10/B0.cpp DIFFER(9273v9268), 32 masked hunks; NEW metric tools/rawd.py (instructions differing
  with esp offsets kept, calls/relocs masked): 346 -- the frame-slot residue is most of the raw difference.
- BYTE-IDENTICAL to B0 (the function is insensitive to IL-surface spellings here, playbook "125 identical edits"): k = 1..3 unused `long`/`float`
  locals at entry and before `float reach` (k >= 4 at entry worse, 9277); murder block `killer != NULL`, named `bool found = GetDef(..)`,
  nested `if (killer) if (..IsPlayerCharacter()) if (GetDef)`, braces around the AddMessage arm; Kill / murder / killer statement order (V1..V3:
  scheduling only, 33 hunks); eend declared before eit / in one declarator (Q1/Q3). Worse: one combined
  `if (killer && IsPlayerCharacter() && GetDef(..))` (9337..9390), for-form of the HitEffects respawn loop (9375), any extra dead assert in the
  murder blocks (acts as the global device-d count: 7 asserts = 9277).
- MEASURED root of the zero-register residue (tmp/prioB_B0.txt, c2prio_f --trace --blocks): in the damage-loop murder block the two `this`
  references are c2 blocks 240 (line 500: CCharString ctor + GetDefinitionManager + GetDef) and 241 (GetWorld + AddMessage), adjacent, w 2;
  `this` (#6, prio -318) is split and its pieces there (#460, #24) are skipped "spill 0 <= 0" = 1-reference pieces, so the two refs land in
  DIFFERENT pieces; retail keeps both refs in one ebp piece (0x00796A62 load, 0x00796A70 / 0x00796A96). With ebp free, the const-0 pieces
  (#44/#402, prio -34) take it (ours `xor ebp,ebp` before Kill). In the spell-over copy retail's const-0 piece is only {murder = 0 store,
  AddMessage push} (0x00797372 xor after the Kill call; Kill push imm; killer compare `test esi,esi` even with ebp = 0 live), then
  murder.Object takes ebp (0x0079740B / 0x0079741A) through the ~CDefPointer at 0x0079768B. So retail's piece regions are NOT whole c2 blocks
  of ours: the lever is the split pass's region formation (c2.dll 0x1078B2AA / 0x1078B51C; hand883 LESSONS: backward liveness, then forward
  reach with KILL), which c2prio_f.py does not log per piece. Frame map (tools/n8fm.sh): the swapped slots -544/-540 (eend/n/stats v
  creature/effect/it/objects) and tv4735 (an RA spill temp) v spell-over murder (retail murder shares the -544 slot) follow from that RA.
- Next idea (untried, tooling): log each split piece's block set and seed order at the piece constructors (0x1078B494 parent esi,
  0x1078BF46 parent ebp) as acctB r3011 NOTES "Ranked untried ideas 1" proposes; then pick the spelling that gives retail's pieces.
- 0x00502E90: not reopened (223 masked hunks, frame 0x19c v 0x194, six row-local class models with shared headers still block landing; the
  loop-shape lead of wfE10-8 needs a full re-port first). Parked.

## wfE10-10 (successor) -- twins 0x00795300 / 0x0078FD60 (probe/n11/)
- (1) FITTED, measured: the range-test x87 square order (retail 0x0079626E..0x0079627C squares X, Z, Y) closes ONLY with the sum written in the
  row as `(offset.Y * offset.Y + offset.Z * offset.Z + offset.X * offset.X)` (12 in-row orders x 2 parenthesisations graded: YZX 29 hunks, YXZ 30,
  XZY/ZYX 31, XYZ/ZXY 32). Through GetSquaredMagnitude's inline body every order (XYZ, ZYX, default XZY, and a scratch YZX opt-in, removed
  again) squares Z, Y, X. differ/ both twins: DIFFER(9273v9268) 29 masked hunks, rawdiff 338 (was 32 / 346).
- Byte-identical (no effect): landed OnDie idiom `bool found; { CCharString name(..); found = GetDefinitionManager().GetDef(name, &murder); }`
  in either / both murder blocks; k = 1, 2, 4, 7 unused `long` locals at entry. Device d re-swept on the new base: 6 (= 8) best; 4/5/7/10/12 worse.
- Also byte-identical on the new base (no effect): `creature.Get()->Kill(false)`, `creature.Get()->GetPLastAttackedByCreature()`, killer declared
  then assigned; eit/eend declared at function scope (before / after `creature`) or in two declarations. Worse: `CDefPointer murder` declared
  before the Kill call (32 hunks: retail stores murder = 0 after Kill).
- c2 trace of the new base (tmp/prio_B1.txt, c2prio_f --trace --blocks): unchanged colouring -- `this` pieces #460 / #24 skipped "spill 0"
  (one reference each), then const-0 pieces #95 / #465 (prio -34, spill > 0) take the still-free ebp in the two murder blocks; retail's
  damage copy keeps one `this` piece (two references: GetDefinitionManager 0x00796A70, GetWorld 0x00796A96) in ebp, and retail's spell-over
  const-0 piece starts AFTER the Kill call (0x00797372) and merges with murder.Object. Root = split-pass region formation (as wfE10-9);
  no source spelling found that moves it (~45 grades this pass incl. the 12-order sweep).
- 0x00502E90: re-graded only (W0 = differ: DIFFER(13572v13622), 223 masked hunks, rawdiff 2841; first hunks = frame 0x19c v 0x194 and the
  entry zero-store run 0x00502F69..0x0050301D in a different order / register, i.e. function-wide RA). Not reworked: no lead that a pass of
  ~40 grades could close; parked.

## wfE10-11 (successor) -- twins 0x00795300 / 0x0078FD60 (probe/n12/, ~30 grades); 0x00502E90 not reopened
- Base re-graded: differ/00795300.cpp = probe/n12/B0.cpp DIFFER(9273v9268), 29 masked hunks (unchanged). differ/ files NOT changed this pass.
- NEW, measured, no gain: (1) TU inline flag of the landed same-TU siblings (CTCUnholyPower::FinalCleanup 0x00795090 /
  CTCDivineWrath::FinalCleanup 0x0078FAF0 carry `// Flags: /d2inlT62`): /d2inlT56..66 on FrameUpdate = 98..154 hunks (smaller code:
  helpers retail expands get called); with T62 the device-d count re-swept 0/2/4/6/8 -> best 98 hunks. The siblings' T62 is a per-row
  inline device, not a TU fact this row shares. (2) device d with a condition that has NO inline helper (`FABLE_RETAIL_ASSERT(killer)`
  after the killer declaration, 1/2/3 per murder block, or one block only): 1 or 2 extra = 34 hunks, 3+3 = 29 (identical); the extra
  dead arm re-keys only the Normalise x87 / push order after the damage loop (0x00796FC8, 0x0079706A), never the murder blocks.
  (3) device c: k = 3/5/6 unused longs at entry (3 identical, 5/6 = 34 hunks); k = 1/2/4 unused longs right after each Kill(false):
  byte-identical. (4) device-d count 3/9/11/14 at the spell-over loop top: all worse (6 stays best). (5) GETDEF_NAME_BODY removed
  (diagnostic only): 57 hunks -- the visible GetDef body is in effect, so `murder` is not address-exposed; ours loses murder.Object's
  ebp piece in the spell-over copy to the const-0 piece, an RA (piece) residue, not exposure. `CThing* killer` does not compile
  (IsPlayerCharacter is a CThingCreatureBase virtual).
- Retail fact re-read for a successor: in the spell-over copy retail's const-0 piece holds ONLY {murder=0 store 0x00797378, AddMessage
  extra_data push 0x007973F8}; the killer null test between them (0x0079738E) is `test esi,esi` and Kill's bool push (0x00797360) an
  immediate, i.e. retail's piece excludes those references while ours ({Kill push, murder store, killer cmp, AddMessage push, wife=0
  store, deeds killer cmp}) includes them. Damage copy: retail has no const-0 register at all; `this` piece (0x00796A62..0x00796A96) in ebp.
- (wfE10-11, cont.) PROGRESS, measured: 29 -> 21 masked hunks (rawdiff 338 -> 329; U 48 -> 40; length 9273 -> 9277): a c2 BLOCK
  BOUNDARY right after the damage-loop Kill(false) gives retail's damage murder-block registers (no const-0 register; `this` piece in ebp
  at 0x00796A62..0x00796A96; push 0 / test esi,esi immediates). User labels (`l:;`, `goto l; l:`) and `do {..} while (0)` are folded
  before the allocator (byte-identical); the boundary that survives is a real if/else join: FITTED
  `if (creature->GetTC(TCI_CONTAINER, &container)) { container->OnDie(); creature->Kill(false); } else creature->Kill(false);`
  (VC7.1 tail-merges the two Kill calls into one copy at the container `je` target, exactly retail's shape 0x007969D9 -> 0x00796A08;
  arms swapped = identical). Device d re-swept on it: 4 = 6 (smallest kept), 0..3 / 5 / 7 / 8 worse; device c k = 1..5 inert.
  differ/00795300.cpp + twin differ/0078fd60.cpp (tools/mktwin.py) updated: both DIFFER(9277v9268), 21 masked hunks.
- Still open (21 hunks): (1) Kill `push 0` / `lea ecx` order at 0x00796A08 (ours lea first inside the merged copy); (2) spell-over murder
  block 0x00797360..0x00797697 (retail: const-0 piece only {murder=0 0x00797378, extra_data push 0x007973F8}, then murder.Object in
  ebp 0x0079740B..0x00797697; ours const-0 piece from before Kill). The same if/else in the spell-over copy (S1, H1/H3) makes `this`
  take ebp there (retail's DAMAGE pattern, not its spell-over pattern): 24 hunks; with murder declared before the container block 25.
  (3) frame-slot ties -> disp sizes (+9 bytes) and the alignment-padding hunks that follow.
- Lead for a successor: find the natural source form that puts a block boundary after Kill in the damage copy (the if/else is FITTED)
  and, in the spell-over copy, the structure that starts the const-0 piece after Kill while keeping `this` in 1-ref pieces.

## wfE10-12 (successor, 2026-11-15) -- twins 0x00795300 / 0x0078FD60 (probe/n13/, 18 grades + 1 packer trace); 0x00502E90 not reopened
- Base re-graded: differ/00795300.cpp = probe/n13/B0.cpp DIFFER(9277v9268), 21 masked hunks, U 40, rawdiff 329 (unchanged). differ/ NOT changed.
- NEW MEASURED FACT: the frame-slot permutation (tools/slotmap.py: retail 0x20<->ours 0x24, 0x6c->0x70, box 0x78..0x84 -> 0x84..0x90, ...) is
  IDENTICAL across variants with different murder-block register allocation (B0, H1 = spell-over if/else, F2, C4 = natural damage form).
  So the slot ties are NOT a consequence of the RA residue (contradicts the wfE10-8 hypothesis): they are an independent residue (packer
  weights / creation order) and must be closed separately; they cause the +9 bytes (disp32) and every alignment-padding hunk.
- Tried, no gain (all hand-reasoned, one question each):
  (1) template `CTCContainer* container; creature->GetTC(&container)` (PDB has ??$GetTC@VCTCContainer@@; opt-in FABLE_RETAIL_CTCCONTAINER_ITEMS
      for StatGetInterfaceType) instead of GetTC(TCI_CONTAINER, &CTCBase*): byte-identical in both copies (C2/C3 = 21; natural damage C1/C4 = 34).
  (2) spell-over if/else Kill in both arms (H1) on today's base: 24 (`this` takes ebp, as wfE10-11).
  (3) spell-over frequency: nested `if (health<=0) if (explodable)` identical; `if (it != endit)` guard on the spell-over loop 23; `if
      (!IS_THING_ALIVE(creature)) continue;` 24. The murder-block hunks never move.
  (4) damage boundary spellings: ternary `GetTC ? (OnDie(), Kill(false)) : Kill(false)` = identical to the if/else (21, lea-first order
      stays); named `bool has_container` + trailing test 39.
  (5) dead release assert (device d) right after Kill to make a c2 block boundary: natural damage + assert(creature.Get()) 29 (= natural
      base: the `if (0)` arm is gone before the allocator); assert(creature->IsDead()) 34; spell-over 26 (only the Normalise x87 / push
      order re-keys, as wfE10-11 found).
  (6) device c k = 1..3 unused longs right BEFORE the spell-over container/Kill: byte-identical. Two sweeps in a row inert -> stopped.
- Retail fact for a successor: the two kill blocks are textually the same; the only retail difference before Kill is that the spell-over
  HasTC(TCI_CONTAINER) is a bitset::test CALL (0x00797326) where the damage copy tests the bit inline (0x007969D2) -- ours already matches both.
- State: near_miss-structure unchanged (21 masked hunks). Residue groups, all independent: (a) damage Kill `push 0`/`lea ecx` order under
  the FITTED if/else (0x00796A08); (b) spell-over const-0 piece before Kill / murder.Object not in ebp (0x00797360..0x00797697);
  (c) frame-slot tie order (RA-invariant, see above).

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).

## wfE10-13 (successor, 2026-11-16) -- twins (probe/n14/); frame-packer re-measure first
- Base re-graded: probe/n14/B0.cpp = differ/00795300.cpp DIFFER(9277v9268), 21 masked hunks (unchanged).
- Packer trace of today's base (tools/pk/pk.py -> tmp/pkn14_B0.json; tools/pkshow.py prints slots/members/weights): 26 slots; the
  K&R qsort model (tools/pk/sim.py) reproduces our postsort exactly. Ties (equal density): S1 {creature,it,effect,objects} v S15
  {n,eend,TC-key temps,stats} at 10250; S11 names v S18 dirs 3000; S4 owner / S9 / S10 (CCharString temps) 2250; S17 v S19 1500;
  S22 v S23 info 350. Against the inherited `want` pairs (wfE10-8, older slot ids) all five tie groups come out reversed; in the
  model, S15 created at presort position <= 9 with w40 fixes all of them (tools/tiesim.py). Being re-validated against today's
  retail offsets before any source change.
- (wfE10-13) BREAKTHROUGH, EVIDENCED: q.py masks jump TARGETS; retail's killer-null / HasTC(0x51)-false jumps (0x00796B0E/0x00796B1B,
  0x00797473/0x00797485) land PAST the wife->Murdered() call (0x00796C09 / 0x0079757F), the PeekTC/faction-null jumps land on the wife test:
  `if (wife) wife->Murdered();` is INSIDE `if (killer && killer->GetTC(&deeds)) { ..PeekTC/faction/PostDeed x2..; if (wife) wife->Murdered(); }`
  (same shape as landed CThingAICreature::OnDie 0x00832E60). W1 = 21 -> 10 masked hunks, instruction count equal, spell-over murder.Object
  now in ebp as retail (its frame weight 6 moves it into the creature/it slot = retail 0x20; the S1/S15 tie is fixed).
- (wfE10-13) With it the FITTED damage if/else is NOT needed: natural `if (GetTC(TCI_CONTAINER,&container)) container->OnDie();
  creature->Kill(false);` + device d re-swept (6 = 8 best; 0..3/5/7 worse) = W2a6: DIFFER(9295v9268), 8 masked hunks = only alignment
  padding; rawdiff 196. differ/00795300.cpp = probe/n14/W2a6.cpp. Remaining residue = frame-slot order only.
- (wfE10-13) Frame now (tools/slotvote.py: votes retail offsets per packer slot; tools/pkshow.py; tools/tsearch.py): slot composition =
  retail everywhere; order ties still reversed: (S5 radius/murder v S2 this) 3500, (S3 effects v S13 damage) 2500, (S10 CCharString temps,
  S4 owner, S9) 2250, (S19 effect_pos/box v S17 pos) 1500, (S23 v S22 info) 350. Natural separate sweep-1 / release `creature` locals
  (N1): sweep-1 creature joins the effects slot (retail keeps effects alone) -> worse frame (rawdiff 363); FITTED shared creature kept.
- (wfE10-13) EVIDENCED by masked jump targets (new tools/jt.py: aligned jumps whose targets do not align): a creature_effects def
  without VICTIM_EXPLODE_EFFECT falls through to the spell `effects` arm (retail 0x00796CAD and 0x00796CE9 both jump to 0x00796D40):
  `bool done = false; if (GetSubDef(&creature_effects)) { long effect = 0; if (..GetEffectFromString..) { ..; done = true; } }
  if (!done && effects.Get()) { long effect = 0; if (..) {..} }` (flag name FITTED). E2: DIFFER(9291v9268), 8 masked hunks (alignment
  only), jt.py clean. differ/ both twins updated (mktwin.py). The `&&`-chain / comma spellings of the same CFG are far worse (47-50 hunks).
- (wfE10-13) Frame: code is now instruction-identical, so slot weights should equal retail's unless c2's IL-level frequency estimates
  differ; ties remain (5 groups reversed). Tried, no gain: natural separate creature locals (N1), FITTED removals (stamina block F1 15,
  sweep-2 guard F2 32, GetSquaredMagnitude F3 11), damage/radius order (V1 10), `things` declared earlier (V3/V4: length 9288 but RA
  breaks, 27-30), combined `||` continue in the lit-loop arms (G2 identical, G1 11).
- (wfE10-13) Frame model state: tools/replay.py replays our packer exactly from the --iset trace (tmp/pkn14_E2i.json; rule = latest
  compatible slot, size <= 2*slot); per-node retail votes tmp/votes_E2.json; base layout puts 57/69 voted nodes at retail offsets;
  hill-climbing node order inside equal (size, weight) groups reaches only 61/69 (tools/tieexp.py), so the remaining ties are not a
  pure symbol-order question; most likely retail's IL frequency estimates make some tied weights unequal (S5>S2, S3>S13, S10>=S4>=S9,
  S19>S17, info$14377>info$14337). Not closed. ~30 grades this pass.
- (wfE10-13) 0x00502E90 lead (not worked): tools/jt.py reports 73 aligned jumps with non-aligned targets on differ/00502e90.cpp
  (some are false positives from relocated operands), i.e. hidden statement-nesting errors like the twins' -- check them first if
  the row is reopened (after its six row-local class models move to shared headers).
- (wfE10-13, final checks) Call targets: tools/cmpcalls.py on differ/00795300.cpp: 430 v 430 calls, no target difference (strict mode
  shows only unbound row-local names v retail addresses). jt.py normalisation fixed (relocated `[0]` / disp32 operands): both twins
  0 mismatched jump targets; 0x00502E90 = 67 (corrected from 73). So the twins' only residue is the frame-slot order (disp8 v disp32,
  +23 bytes, alignment padding). Device c on the new base: k = 1..3 unused longs at entry byte-identical, 4..7 worse (13 hunks); none kept.
  The 61/69 hill-climb ceiling of tools/tieexp.py may be partly vote noise (several nodes have 1-2 votes; depth mismatches add more):
  a successor should list the still-wrong nodes before trusting it.
- (wfE10-13) Behaviour test, when the twins match, must cover: killer null (or no deed log) with a wife present -> Murdered NOT called;
  creature_effects def present but without VICTIM_EXPLODE_EFFECT -> falls through to the spell `effects` arm; a mutant missing either
  decision must fail.
## Decisions for the coordinator (wfE10-13)
- Twins: FITTED damage-loop if/else REMOVED (natural form now matches); NEW FITTED `bool done` flag (CFG evidenced by retail jumps
  0x00796CAD / 0x00796CE9 -> 0x00796D40, spelling fitted); device d count 6 (was 4); no header opt-ins, no name fixes.
- Lane-wide: run tools/jt.py (masked jump targets) + a call-target check on every near-miss before register work: here a hidden
  nesting error looked like a register residue for ~600 grades.

## wfE10-14 (successor, 2026-11-16) -- 0x00502E90 reopened (probe/n20/), DIFFER(13572v13622)/223 -> DIFFER(13568v13622)/159, frame 0x194 = retail
- EVIDENCED (jt.py masked jump targets): every list loop in retail caches its end: people loop (0x0050308A esi = VillagePeople node),
  buildings loop (0x00503346 end stored [esp+1c]), tavern / games owner loops (0x005035CA ebx = ObjectList node). Named
  `...::iterator end = list.end();` before each loop. The people loop must increment AFTER the villager pointer's dtor
  (retail 0x0050331A dtor, 0x00503326 ++it): body in a nested `{ }` block and `++it;` after it (a for-increment instead moves
  c2's frequency estimate of the later loops 5x and expands the house push_back / list size there: 13952).
- EVIDENCED (wfB5 lesson): STLport random_shuffle (2- and 3-arg) bodies + RandFunc::operator() inline (= landed 0x0049B1E0 /
  0x00777080 texts) defined in the row: c2 then keeps houses begin/end in esi/edi across the shuffles and sort as retail
  0x005053E6..0x0050545F; the frame dropped to 0x194 = retail. Still called out of line (as retail).
- FITTED: houses search `bool found` flag (retail 0x00505266 jumps straight into the found block: no h != end re-test);
  `if (freeSpaces)` before `while ((females.size() || males.size() || children.size()) && freeSpaces)` with a trailing
  `if (!freeSpaces) break;` (retail: entry test 0x00505539, top test after the sizes 0x0050557F, bottom test 0x005056CC).
- Tried, no gain: `adults->erase(a)` without assignment (identical), while/for-&& search spellings (160), std::find (calls find).
- Open: (1) teacher / shop CreateRelevant expansions cross-jump the other way (ours 4 / 2 Creates v retail 3 / 3): the first male
  copy's push_back overflow block uses edx/eax in ours v ecx/edx in retail, so the fallback copy can't merge. (2) adults loop
  (seg 12, +50 bytes): ours a memory-homed with an a+4 IV, h in ebx; retail a in edi, work in ebx, h in memory [esp+48].
  (3) colouring elsewhere. Segment lengths per DisplayProgress: tools/502seg.py <q.txt>.
- (wfE10-14) Correction: the nested-block + trailing `++it` people loop is FITTED (b1, a plain for-increment, gives the same loop-1
  bytes; b2 kept for its downstream inline effect). EVIDENCED are only the cached ends and the dtor-before-increment order.
- (wfE10-14) Marriage loops also cache their ends (retail 0x00505881 [esp+94], 0x00505958 ebx): f1 = 144 hunks, DIFFER(13528v13622),
  saved as differ/00502e90.cpp. Map loop / final VillagePeople loop / emptyHouses loop with cached ends: no gain (145/146/144).
- (wfE10-14) PROBE-ONLY LEAD (probe/n20/p1.cpp, stand-in bodies, NOT a candidate): defining GetRelevantBuilding 0x005029A0,
  AddToWorkBuildingList 0x005003E0, AddToRelevantBuilding 0x00502B00 with non-capturing bodies above the row (auto_inline off)
  gives 128 hunks and retail's houses begin/end in esi/edi across the shuffles / sort (0x005053E6). The three are landed
  (rebuild/src/compiled/00/50/global_*.cpp, local-model style): their real texts must be ported onto shared headers in the row.
- (wfE10-14) Six row-local class models REPLACED by shared headers (grade unchanged, 144): engine/CSimBuildingDef.h (FABLE_RETAIL_CSIMBUILDINGDEF),
  CShopDef.h, CTCBuyableHouse.h (+ NEW opt-in FABLE_RETAIL_CTCBUYABLEHOUSE_GETPDEF), CTCThingOwner.h (GetObjectList()), CTCTalk.h (+ NEW
  opt-in FABLE_RETAIL_CTCTALK_GETDIALOGUEVOICE), CTCDummyVillager.h; row-local GetSubDef<T> body -> shared FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY.
  Remaining local classes have no engine/ header: CVillagePeopleDef (as landed 0x004FF750), CBuyableHouseDef (only _quarantine; landed
  CTCBuyableHouse rows model it locally), CTCDNone, CIsHouseLargerThan, RandFunc.
- (wfE10-14) `CIsHouseLargerThan larger;` named uninitialised comparator (EVIDENCED: retail 0x0050544E pushes the slot with no zero store).
- (wfE10-14) The three same-TU helpers are now DEFINED in the row with their landed texts on shared headers: GetRelevantBuilding 0x005029A0,
  AddToWorkBuildingList 0x005003E0, AddToRelevantBuilding 0x00502B00 (only change: `creature->SetHomeBuilding(*chosen)` for the private
  member assignment, `!def.IsNull()` for `def.Object`). Each graded from the same file at its own address: RELOCATION_MATCH x3; the row
  still calls them 13 / 10 / 3 times as retail (no inline control needed). Row: 127 masked hunks, DIFFER(13490v13622) (differ/).

## wfE10-14 (successor, 2026-11-16) -- twins 0x00795300 / 0x0078FD60: c2 frame-item LIST ORDER rule found (probe/n15/)
- Base re-graded: differ/00795300.cpp = probe/n15/B0.cpp DIFFER(9291v9268), frame 0x230 both, 8 alignment-only hunks.
- NEW MEASURED FACT (c2 13.10.3077 RE + probe): the packer's sorted item list (0x108a78a4) is built by 0x10788642 (edx = item,
  ebx = 1 per memory reference): a new item is appended after all items of equal size (list sorted by size ascending); each
  further reference adds 1 and moves the item ahead of same-size items with STRICTLY smaller weight (stops at >=, 0x10789aa8 jge).
  So the order inside a (size, weight) group = the order in which items REACHED their final weight over the packer-time reference
  walk; the walk is in final listing order. Probe tools/pk/00795300_pkgw.py (+ runner 00795300_pkw.py) logs every add ("w" events,
  item address); tools/00795300_hsim.py rebuilds the list from that sequence: model == observed list exactly (139 items).
- Hidden references (counted by the packer, absent from the final code): creature$13706 1, effects 1, murder$14655 1, wife$14522 1,
  wife$14661 1, CancelSound temp $T23151 2 (cross-jumped tail), ga 1, effect$14554 1 (done-flag arm), dir$14433 1, dir$14627 1
  (tools/00795300_hcnt.py, 00795300_hid2.py). Retail's code is instruction-identical, so retail's packer input can differ from ours
  only through hidden references, node identity (scope / named v temporary) or interference.
- Slot level (tools/00795300_ts2/ts3/ts5.py): NO weight-only change of up to 11 tied slots reaches retail's slot order: retail's slot
  CREATION order differs (e.g. S12/S13/S14 = lit / damage / things creators reversed is the Kendall-3 nearest of 227 working orders).
  Retail post-sort (packer slot ids of our build): [0,16,7,1,15,6,12,14,5,2,11,18,8,20,3,13,10,4,9,19,17,21,23,22,24,25] (69/69 votes).
- (wfE10-14) inline_depth(0) regions re-measured after the helper port (drop one at a time, probe/n20/r00..r12): the init-ctor regions
  #1 and #3 (of 5) and the child SetHomeBuilding region are INERT (byte-identical without them) and were removed; the other 10 each
  change the key-call pattern or expand a retail call (candidates copy 0x00514350) when dropped. Without any: 199 hunks.
- (wfE10-14) Byte-identical / inert: Anniversary 360 local names for 23 row locals (tools/502ren.py), k = 1..7 unused longs at entry.
- (wfE10-14) Length: -101 of the -132 bytes are encodings of equal instructions (disp8 v disp32 esp offsets: same frame size 0x194,
  different slot order) -- tools/502enc.py <q.txt>. Remaining structure: teacher/shop cross-jump (+47/-35), adults loop RA.
- (wfE10-14) Anniversary PDB (third source) S_REGREL32 list of this function also has `hand_placed_villagers` (= originalVillagers),
  `school_building`, `num_warehouse_workers_places_left`, `men_shuffled` (= candidates), `rand_func`, `homes` (= houses) -- usable names.
- (wfE10-14) More EVIDENCED fixes: servant def indices written inline in the CreateRelevant call (`peopleDef->MaleServant,
  peopleDef->FemaleServant`; retail loads +0x68/+0x6C right before the call 0x0050463C) with the scoped pragma moved INSIDE the call
  parentheses (`... true` / `#pragma inline_depth(0)` / `);` / `#pragma inline_depth()`: the pragma acts where the call closes, so
  operator-> in the arguments stays inline) + the child SetHomeBuilding pragma back (the budget moved); apprentice coin through
  `bool evil = (GFRandom(...) & 1) != 0;` (0x0050479E stores the bool), female index assigned before the male one in both arms
  (0x005047A4 loads +0x7C first); house-loop def test `(*h)->PeekPSimDefinition().Get()` (0x005054A9 keeps Object, tests it at
  0x005054CF: no setne bool) -- this also re-ordered frame slots (encoding delta -101 -> -35); shared engine/GFRandom.h (Codex text)
  instead of the row-local GFRandom (retail stores the coin at 0x00505C56 / 0x00505FD9; helpers still RELOCATION_MATCH).
  -> DIFFER(13602v13622), 111 masked hunks (differ/). Inert: pointer tests at the two other IsNull sites; RESULT_LOCAL GFRandom opt-in.
- (wfE10-14) CORRECTION: the servant "inline arguments + pragma inside the parentheses" spelling (111 hunks) was WRONG: the pragma
  still covers the whole statement, so peopleDef's operator-> is CALLED twice there (wrong call list). Reverted to the hoisted named
  indices (FITTED, retail loads +0x68/+0x6C at the call with peopleDef.Object hoisted in edi before the loop) + the child
  SetHomeBuilding region (now needed again): DIFFER(13586v13622), 115 masked hunks, call list = retail except the teacher/shop
  cross-jump (differ/). Device d (0..6 dead asserts before the servant loop, no pragma): inert, servant stays expanded.
  Open idea: a per-row /d2inlT<N> or a different region boundary that calls CreateRelevant but inlines operator-> in its args.
- (wfE10-14) Full pin: tools/00795300_slotvote2.py (slotvote + `$T` / `tv` temporaries) gives a retail offset for 139/139 packer
  items (tmp/00795300_votes_all.json): retail slot COMPOSITION = ours for every item (incl. all temporaries and the three tv temps);
  only the slot ORDER differs (ours 113/139 items at retail offsets). So slot weights are equal unless hidden references differ.
- (wfE10-14) Searches on the exact model (score = items at retail offsets, 139 max), none reaches 139:
  node order inside (size, weight) groups by annealing 133 (tools/00795300_sa2.py); single hidden-reference add/remove 128, greedy
  131 (+1 ref on effect_pos$14331 and on murder$14507 fixes S17/S19 and S2/S5, then S11/S18 flips; tools/00795300_hs3.py, diag.py);
  every combination of <= 3 natural changes (+1 ref at first/last use of any tie-slot member, removal of any of our hidden refs)
  < 135 (tools/00795300_comb.py); same-slot node merges 124 (tools/00795300_merge.py); moving any run of the walk (block-order
  hypothesis) 124 (tools/00795300_segmv2.py); reversed / first-reference rules 38-53. => retail's packer input differs from ours in
  several independent places that leave no trace in the final bytes; no single source fact found.
- (wfE10-14) Graded hand variants this pass (all 2710 instructions unless noted): named `C3DVector point_of_hit` at each / all three
  CHitParameters sites (Anniversary 360 locals point_of_hit / direction): byte-identical (P1-P4); `PerEffectInfo info;` declared before
  effect_pos (360 order) in either / both victim arms: rawdiff 199 / 202 (worse, I1-I3); declare-then-assign effect_pos in the victim
  arms: +1 instruction (E1-E3); separate loop-local creature pointers on today's base (N2, 360 `something` x2): 2708 instructions
  (one jmp-over-padding lost = frame side effect), rawdiff 361, build-up creature joins the effects slot (retail: creature/it slot);
  dead asserts at the release damage / build-up radius: odd counts +1 instruction, even counts byte-identical (A1-A5).
- (wfE10-14) Anniversary 360 PDB has CTCUnholyPower::FrameUpdate (flat local list, no S_BLOCK32, tmp/00795300_anniv_syms.txt): an
  edited build (CTCNymph, ptc_cmm, point_of_hit, findit, GUMPH), useful only as names / candidate list; nothing in it closed a tie.
- State: unchanged best = differ/00795300.cpp (B0) DIFFER(9291v9268), 8 alignment-only hunks; twin differ/0078fd60.cpp likewise.
  Parked: residue = frame-slot creation order of the c2 packer (5 tie groups), caused by packer-time references invisible in retail.
- (wfE10-14, last) The wfE10-3 lc1 idea re-tested on the new base: `bool listCreatures = true;` before the servant loop, passed at the
  servant and apprentice CreateRelevant calls, servant indices inline in the arguments, both pragma regions removed -> both CALLED,
  peopleDef hoisted in edi and +0x6C loaded at the call as retail (only a push/load order swap left), 491 v 491 calls,
  tools/cmpcalls.py: targets equal except the teacher/shop cross-jump shift. DIFFER(13581v13622), 113 masked hunks, 9 pragma
  regions (differ/). FITTED (device a, an ordinary named local; the constant-argument inline credit is the measured reason).
- (wfE10-14) /d2inlT50..73 with the servant region removed (no listCreatures): servant stays expanded at every T -- not a lever.
- Next for a successor: frame-slot order (tools/502enc.py: -35 bytes of encodings), adults-loop RA (a memory-homed + a+4 IV v retail
  a in edi, h in [esp+48]), teacher cross-jump (first male copy's push_back overflow temps edx/eax v retail ecx/edx).
- (wfE10-14, last) A 3-change packer input DOES reproduce retail's frame exactly (139/139 items; tools/00795300_hs4.py, saved walk
  tmp/00795300_hs4_seq.json): +1 packer reference on effect_pos$14331 (lit-loop arm 1, S19 24->25), +1 on murder$14507 (damage-block
  CDefPointer, S5 14->15, reach time unchanged), -1 on the arm-2 `creature_effect = CreateParticleEffect(..)` CIntelligentPointer
  temporary $T23188 (S18 36->35). Read as slot facts: retail S19 > S17 (density 1500 tie broken), S5 > S2 (`this`), S18 < S11; the
  other two tie groups (S3/S13, S10/S4/S9) then fall out of the qsort. Removing our dir$14433 / dir$14627 hidden refs instead of
  the $T23188 one gives 119 (their reach order moves). No source spelling found that adds / removes exactly these packer-time
  references with the final bytes unchanged (they are hidden references; the arm-1 / arm-2 victim effect blocks of the lit loop
  and the damage-block murder def are where a successor should look: same-final-code spellings of those statements).
- Also graded: the other C3DVector::Normalise texts in the overlay (LOCALS, LOCALS_XYZ, SQUARE_HELPER, UNPAREN, default) change code;
  PAREN_SQUARES_ZYX is byte-identical to the kept XYZ. ~35 grades + ~10 probes this pass.
- (wfE10-14) CORRECTION of the slot-level bullet above ("NO weight-only change ... creation order differs"): WRONG -- ts5 never
  included S11 / S18 and ts3 stopped at two slots. tools/00795300_ts7.py (presort fixed, all 26 slots, <= 3 changes, + on any slot,
  - only on slots holding a hidden-reference symbol) finds exactly two solutions: {S5 +1, S19 +1, S11 +1} and {S5 +1, S19 +1, S18 -1}.
  So retail's slot creation order can equal ours; retail needs ONE more packer-time reference in S5 (radius$14029 / murder$14507 /
  things$13844 / tv4735) and in S19 (effect_pos$14331 / effect_pos$14567 / $T23219 / box$13735), and one more in S11 (names) or one
  fewer in S18 (dirs / $T23188).
- (wfE10-14) Node-level realisation (exact model, 139/139): +1 on box$13735 right after its last use AND +1 on things$13844 right
  after its last use (both arguments of the build-up `GetAllThingsInRoughArea<CThingCreatureBase>(box, .., things)` call, ONE
  statement) fixes S5 and S19 (131); the remaining S11/S18 tie then closes only by one fewer reference on the arm-2 victim-effect
  CIntelligentPointer temporary $T23188 (`creature_effect = CTCSpecialAbilities::CreateParticleEffect(effect, effect_pos);`, lit loop,
  any of its 3 references); +1 on any S11 member at any position does not (reach order moves). Lead for a successor: a same-final-code
  spelling of the build-up GetAllThingsInRoughArea statement that gives box and things one extra packer-time reference each, and of
  the arm-2 creature_effect assignment with one fewer (ROUGHAREABODY is already defined in the row). Graded this pass, no gain:
  swapped info.Effect / info.Position (Q1 / Q2: code order changes), arm 1 through a creature_effect reference (Q3: -7 instructions).
- Note: tmp/q_E1..E3.txt now hold this pass's probe/n15 variants (not the n14 E2 base); the base grade is tmp/q_B0.txt.

## wfE10-15 (successor, 2026-11-16) -- 0x00502E90 only (probe/00502e90_n30/, tools/502_30*.py, probe/00502e90_anniv/)
- Base re-graded: differ/00502e90.cpp = DIFFER(13581v13622), 113 masked hunks (R 3658 v O 3664 instructions).
- NEW SOURCE: Anniversary 360 PPC listing + line table of this function (module 74 world_map.cpp, sec5 0x01211AF0, 11708 bytes,
  422 line entries; probe/00502e90_anniv/502ppc.py -> 502ppc.txt, runs.py = per-line runs). DisplayProgress = bl 0x922290 (20 calls),
  GetRelevantBuilding = bl 0x12108a8 (13), AddToWorkBuildingList = bl 0x120c960 (10). Adults/work-home loop = lines 5163..5204:
  5165 a loop; 5168 `bool erased=false` + work = GetWorkBuilding; 5169 find; 5172/5174 h search; 5177 SetHomeBuilding; 5180 push_back;
  5183 simDef; 5184 size == LiveSpaces; 5186 houses.erase(h); 5188 erased = true; 5190 simDef dtor; 5195 if (erased) 5197 erase(a) else 5201 ++a.
- INERT (byte-identical q output, 15 grades): shop maleDefIndex/femaleDefIndex declaration order (2 forms); the 360 erased-flag form of
  the adults loop (found block inside the h loop, `if (erased) a = erase(a); else ++a;`, with / without assignment, h in / out of the for);
  h declared at a-loop body top / before the a loop / before the outer loop; a declared outside the for; ONE shared `a` and / or ONE
  shared `adults` for both adults loops (c2 builds candidates per web: scope / sharing of named locals is irrelevant here).
- c2prio trace (tmp/30prio_a0.txt, --blocks tmp/30prio_a0b.txt): adults loop a (#154) prio 36 (split eax + memory), h (#156) 80 -> ebx,
  work (#155) 188 -> edi, and an a+4 strength-reduced IV temp #476 (lines 834/857/861) prio -176 -> memory. Retail: a in edi, work in
  ebx, h memory-homed [esp+48] (stored at 0x0050525E entry, 0x0050526F not-found exit, 0x005052B3 found), NO a+4 IV (erase computes
  lea eax,[edi+4] at 0x005053BF).
- (wfE10-15) PROGRESS -> DIFFER(13575v13622), 108 masked hunks (differ/00502e90.cpp; helpers 0x005029A0 / 0x005003E0 / 0x00502B00 still
  RELOCATION_MATCH from the file):
  (1) EVIDENCED, structural: the final-loop `std::find(originalVillagers..., villager)` must be STLport's inline find() forwarding to the
      out-of-line `__find(first, last, val, input_iterator_tag())`: retail 0x00506173..0x00506190 pushes the tag temporary's address
      (`lea ecx,[esp+0xf] ; push ecx`) and calls 0x00511BF0 = landed std_find_00511bf0.cpp (ret 0Ch). The row-local declaration-only
      `find` made us call find() with two stack words (a different symbol; q.py masks it). Row text now: declared __find + inline find
      (needs FABLE_RETAIL_STLP_FIND_IF_INPUT for the list iterator_category). -4 hunks.
  (2) EVIDENCED (TIP 0x006D0830): same-TU COMDAT GetDef<CThingCreatureDef> 0x00513160 made visible with the existing
      FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (still called): keeperDef now in ecx and released in place
      (`dec [ecx+4] ; jne`, retail 0x00503ADB); -1 hunk, -3 unmasked.
  (3) FITTED: shop else arm `maleDefIndex = 0; femaleDefIndex = shopDef->PreferredShopkeeper;` (instruction order unchanged: c2 schedules
      it back): maleDefIndex / femaleDefIndex tie at prio 98, the key (last writing tuple, from the else arm) picked female first; now
      retail's edi = male / ebx = female (0x00503A70).
- (wfE10-15) Cross-jump model (teacher 4 v 3 / shop 2 v 3 Creates): c2's LOCAL scratch registers rotate eax -> ecx -> edx; inside each
  expanded Create copy the &init lea takes X, the push_back overflow leas X+1, X+2. Retail teacher copy 1 X = eax (merges with copy 4),
  retail shop copy 1 X = ecx (no merge); ours the other way round. The last local scratch before copy 1 decides: teacher = the
  peopleDef.Object load `mov eax,[esp+0x14]` (retail: apparently a split piece of candidate peopleDef #56, so no rotation step; ours a
  scratch -> next ecx); shop = the keeperDef release's vtable load. Probe t1 (teacher def indices as named locals loaded before GetPos)
  flips the teacher to 3 Creates (111 hunks) but moves the loads before the GetPos call (not retail) -- lead only.
- (wfE10-15) Name question (coordinator): retail 0x00513420 is bound as map<...>::find (from 0x005003E0) while our row / helpers call
  _Rb_tree<...>::find (map::find inline); the bytes cannot tell the two apart (map holds _M_t at +0); vland would flag the symbol.
- (wfE10-15) Also graded, no gain: FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (129 hunks) / _COUNTREF (119); h-value form of the adults
  search (`if (h != houses.end())`, same bytes in the loop, no exit store appears).
- (wfE10-15) Fill-loop AddToRelevantBuilding sites (retail 0x00505791..0x005057ED): retail calls Component.PeekDefinitionManager()
  (0x00415FBC) BEFORE loading / pushing World ([esi+8]); ours pushes World first, then calls. `*&World` / `(CWorld&)World`: inert.
  Ego_r layout lists World / Component as pointers (`CWorld *`, `CMainGameComponent *`), the retail header branch has references:
  a `*World` pointer member might evaluate late -- untested (needs a header opt-in; check the PDB type record first).
- (wfE10-15) Servant site 0x00504622..0x00504640 and the fill-loop `push 0` sites: push / load scheduling only (no count change).
- (wfE10-15) No header / opt-in changes (only existing opt-ins newly defined in the row: FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY,
  FABLE_RETAIL_STLP_FIND_IF_INPUT). ~45 grades + 2 c2prio traces this pass. Next for a successor: (a) the a+4 IV in the adults loop
  (retail has none: find the source fact that stops c2's strength reduction of `position + 1` in erase), (b) teacher / shop scratch
  rotation (make the teacher's peopleDef load a split piece as retail, or find the first block where the rotation state diverges),
  (c) frame-slot order (-35 bytes of disp8/disp32; tools/502enc.py), (d) the World-argument evaluation order above.
- (wfE10-15, final) differ/00502e90.cpp = DIFFER(13587v13622), 107 masked / 172 unmasked hunks (was 113 / 197), 491 v 491 calls (cmpcalls:
  only the teacher / shop cross-jump shift). Every same-TU callee body the row makes visible now grades RELOCATION_MATCH from the row file:
  GetRelevantBuilding 0x005029A0, AddToWorkBuildingList 0x005003E0, AddToRelevantBuilding 0x00502B00, GetSubDef<CVillagePeopleDef>
  0x00512FE0, GetSubDef<CShopDef> 0x005130A0 (needed the NEW additive opt-in FABLE_RETAIL_CSHOPDEF_STATGETTYPENAME_INLINE, hdr/README.md;
  before it the shared GetSubDef text CALLED StatGetTypeName: DIFFER 171v182 -- a pre-existing landing blocker), GetDef<CThingCreatureDef>
  0x00513160 (needs FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD, else DIFFER 116v108), __find 0x00511BF0 (defined with the landed text
  through FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW: -1 masked / -19 unmasked hunks, encodings -47 -> -41).
- (wfE10-15) Still declaration-only same-TU callees: std::sort<CThingBuilding**, CIsHouseLargerThan> 0x0051C400 (by-value arguments, its
  body branch FABLE_RETAIL_STLP_SORT_INLINE excludes the FASTCALL declaration: not tried), _Rb_tree erase(iterator) / insert_unique
  (referenced only from emitted COMDAT bodies). Undefined non-template callees are all outside the TU window (engine code).
- (wfE10-15) CORRECTION: the scratch-register rotation model above is a HYPOTHESIS, unverified (the teacher case needs retail's
  peopleDef load to be a split piece; the out-of-line 0x004FF750 check was inconclusive). The candidate ids / line numbers quoted from
  tmp/30prio_a0*.txt refer to the pre-change source probe/00502e90_n30/502_a0.cpp.

## wfE10-16 (successor, 2026-11-16) -- twins 0x00795300 / 0x0078FD60 only (probe/00795300_n16/, probe/00795300_anniv/, ~26 grades + 1 packer probe)
- Base re-graded: differ/00795300.cpp = probe/00795300_n16/B0.cpp DIFFER(9291v9268), 8 alignment-only hunks, rawdiff 196 (unchanged).
  New grader tools/00795300_g16.sh (grade + masked hunks + rawdiff + instruction count, one line per variant).
- MEASURED (packer probe tmp/00795300_pk16_A1.json): dead release asserts add NO packer-time references -- FABLE_RETAIL_ASSERT(box.TLX <= box.BRX)
  after the build-up GetAllThingsInRoughArea call leaves the walk at 629 events and every item weight equal (box 6/5, things 3/3); the
  `if (0)` arm is gone before the packer. Device d cannot realise the "+1 on box / things" lead (its +1 instruction is a layout side effect).
- Byte-IDENTICAL natural spellings (q output equal to B0): damage-loop `LastFrameCreaturesHit.erase(it++);` instead of old_it/++it/erase
  (the sweep-1 site is NOT: 2692 instructions); `C3DVector dir(a - b);` at all three dir sites; build-up named
  `MapOfCreaturesToPerCreatureInfo::iterator findit = LastFrameCreaturesHit.find(creature); if (findit != ..end()) continue;` (360 name
  `findit`, lines 321/322); nested `if (!done) { if (effects.Get()) {..} }` (360 lines 855/857 are separate); a named
  `CThing* new_effect = CreateParticleEffect(..)` then the assignment, in arm 1 and / or arm 2 (360 lines 526/527, 585/586 split).
  Same-TU callee audit: the only undefined same-TU callees are CTCUnholyPower members 0x007939D0 PerEffectInfo(), 0x00793CC0 Attackable,
  0x00793D20 SetPTarget, 0x00793E00 ResetCreatureHitState, 0x00793EC0 UnsheatheWeapon, 0x007940F0 PlaySoundW, 0x00794C10 FinishMove; all
  templates (GetDef 0x00792350 / 0x007923C0, map / tree / vector ops, IP ctors, IS_THING_ALIVE, GetAllThingsInRoughArea) are defined.
  PerEffectInfo() defined above the row with its landed (empty) text: byte-identical (still called). The others take no frame address
  (Attackable / ResetCreatureHitState by value) or pass it straight to an external call (PlaySoundW -> CTCSound::PlaySound), so their
  bodies cannot change escape facts; not ported.
- Worse: C2DBoxI RADIUS_INLINE (Initialise) text 2711 instr / rawdiff 213; `C3DVector dir; dir = a - b;` (+1..+7 instructions);
  `dir = pos; dir -= owner pos` (SUBTRACT_ASSIGN) at the damage / spell-over sites +4/+5 instr, object site 2710 instr but frame worse.
- NEW THIRD SOURCE (Anniversary 360 Test, edited build; retail bytes win): module 85 Unity_tc_unholy_powerEtAl.cpp, sec5 0x01146268,
  11760 bytes, 329 line entries; probe/00795300_anniv/00795300_ppc.py -> 00795300_ppc.txt (PPC + line), 00795300_runs.py -> 00795300_runs.txt
  (per-line runs with named bl targets, bl_names.txt from the PDB publics). Map: build-up 296 radius, 297 pos (PeekPos in 360), 298 box,
  300 things ctor, 303 GetAllThingsInRoughArea, 321 findit, 322 continue; lit loop 479..599 (arm 1 509 name, 512 GetEffectFromString,
  514 effect_pos, 523 Z, 526 Create, 527 map[] assignment, 534 info, 535 Position, 536 Effect, 537 push_back; arm 2 556..599 same
  shape without map[]); sweep-1 erase 645 ++ / 646 erase (= ours); damage loop 767..889 (836 GetSubDef with flag = false, 851 flag = true,
  855 `if (!flag)`, 857 `if (effects.Get())`, 884 ++ / 885 erase); spell-over 904..1052. No statement in 360 differs from ours in a way
  that could add / remove a packer-time reference without changing the PC code.
- Pruned lead: the "-1 reference on the arm-2 creature_effect temporary $T23188" leg of the wfE10-14 realisation cannot be a removed
  reference: pkshow gives it weight 3 = its three final references, and retail shows the same three leas (0x007960EF, 0x007960FC,
  0x00796108). Retail cannot count fewer than its final code shows, so that leg could only be an item-identity difference (which item
  is placed first among equal weights), not a spelling with one reference fewer. The S11 / S18 leg is NOT located.
- State: parked, unchanged best differ/00795300.cpp (B0) DIFFER(9291v9268); twin differ/0078fd60.cpp unchanged, same residue. differ/ files
  NOT edited (the identical natural spellings above may replace the FITTED old_it form if the coordinator prefers them: damage-loop
  erase(it++) and findit are byte-identical on today's base).
- (wfE10-16, CORRECTION) The 360 arm order at both erase sweeps is NOT ours: line 650 (sweep 1, a lone `bne` to the cross-jumped `++it`
  of line 613) and line 889 (damage loop, `it++` placed right after the test at 767) show `if (<remove>) { ..erase } else { ++it; }`.
  Graded on today's base: sweep 1 swapped (R1), damage loop swapped (R2), both (R3) -- all three byte-IDENTICAL to B0 (q output equal),
  so the arm order is not a lever (c2 normalises it before the packer). The bullet above "No statement in 360 differs" now reads: the
  only 360 differences found (arm order, findit, nested ifs, split Create statement) are all byte-identical on today's base.
- (wfE10-16) Verified by cmp of the q outputs: Dd1 / Ds1 / Do1 / F1 / N1 / S1 / S2 / V1 / E2 / R1..R3 are byte-identical to B0. The A1
  packer list equals pkwB0 position by position in size and weight (139 items; only the symbol numbers shift).
- (wfE10-16) Withdrawn: the suggestion to replace old_it with erase(it++) -- the 360 has `++` and erase on separate lines (645/646,
  884/885), which supports the existing old_it form. The `done` flag is now third-source evidenced (360: flag cleared with the GetSubDef
  line 836, set on 851, tested alone on 855, then `if (effects.Get())` on 857); only its name stays FITTED.
- (wfE10-16) NAME_FIXES.tsv: 0x007940F0 landed as CTCDivineWrath::PlaySoundW(unsigned long, unsigned long); both PDBs spell
  (const CCharString&, bool) private for CTCDivineWrath and CTCUnholyPower (ICF-folded body; the row calls the CTCUnholyPower name).
