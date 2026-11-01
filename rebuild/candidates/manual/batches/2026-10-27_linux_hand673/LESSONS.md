# Chunk 673 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 673) An STLport `vector(n, value)` static initialised from a string literal (`_$E1` of
  CConversationAttitude::GeneralAttitudeToAnimName 0x0121A460) must pass the literal itself (`v(18, "")`): the implicit
  CCharString(const char*, long = -1) conversion makes VC7.1 rematerialise `lea edx,[esp+8]` per element as retail does, while an
  explicit `CCharString("")` temporary binds the reference to the ctor's returned this and holds it in ebx (`push ebx`, 139v139).
- 2026-11-01 (chunk 673) Five quest-script implicit dtors matched first probe from the chunk 665/671 template; the Ego_r offset
  rule is "minus 0x14 for CScriptBase, minus 4 more after every Dinkumware vector/map member" (CQ_ArenaScript: Rounds at Ego_r
  0xAC -> retail 0x98, later members -0x18), and the Alloc's `push size` confirms the total (0xE4 / 0x178 / 0x11C / 0xC8 / 0x8C).
- 2026-11-01 (chunk 673) A Dinkumware `vector<bool>` (Ego_r 20 bytes) is retail's 12-byte generic STLport vector (one byte per
  element, freed with no element loop) inside quest scripts too (CV_TalentlessBardScript::SongThingAvailable, 0x00EDAD10).
- 2026-11-01 (chunk 673) A user dtor that stores a secondary CDeviceResetCallback vtable and starts with
  `GFGetSystemManager()->[+60h]->RemoveDeviceResetCallback(this+4)` is `GFGetSystemManager()->GetDisplayManager()->
  RemoveDeviceResetCallback(this);` (no null test on the base conversion inside a dtor) (0x00408990).
- 2026-11-01 (chunk 673) A retail deque member's dtor stays an out-of-line call with the landed STLport ~deque text in the TU
  (by-value iterator copies through the declared __destroy_aux); VC7.1 does not inline it into the owner's dtor (0x00408990).
- 2026-11-01 (chunk 673) A DirectShow DECLARE_IUNKNOWN forwarder with `mov eax,[eax+0Ch]` (positive offset) has `this` at the
  first base: `class X : public IFoo, public CMediaPosition` puts CUnknown at +8 with m_pUnknown +4 inside it (DX9 combase.h order
  m_pUnknown then m_cRef). The CPosPassThru QI 0x00CAFC40 matched first probe written out of class over the landed model.
- 2026-11-01 (chunk 673) Destructor labels in this band: 5 of 8 right; the three wrong ones (LTextSymbol, CActionTalkedTo,
  CTCTavernGame) were each settled in one step by the slot-0 ??_G's vtable (RTTI or ctor -> Alloc -> registration string).

Review at landing:
- NAME_FIXES.tsv (28 rows): three wrong own-row labels; all eight slot-0 ??_G rows landed as stand-ins (`ScalarDeletingDestructor`
  members, free `__fastcall` `*_scalar_deleting_destructor`, and ONE spelling `@CLandscapeBackgroundPatch_vector_deleting_destructor@12`
  shared by five quest ??_G addresses) and their `Dtor()` / `@*_dtor_<addr>@4` callee spellings; the five quest ctors landed with
  `CGameScriptInterface` / `void*` parameters (PDB: `CScriptDataBase*, CGameScriptInterfaceBase&`; three also declare a user dtor
  that retail does not have); the two landed Allocs `_global::Create_CClass_*`; callees 0x00A5C600 (ClearMemoryBlocks is private,
  AAE), 0x009BF5A0 (landed CEngine::Uninitialise 0x00B25D80 calls it as CRenderManager::RemoveDeviceResetCallback on
  g_pRenderManager -- check whether that global is really the display manager when respelling), 0x0099A410 (catalog label); the two
  gap-scan placeholders.
- vland WARN RELOC_NAME 0x0099A300 (`??1CBaseClassNonCopyable@@MAE@XZ` labelled CBaseClass::~CBaseClass) is the catalog fix raised by
  chunks 662-671; WARN RELOC_NAME 0x0099A410 is NAME_FIXES row above.
- vland WARN RELOC_CONFLICT on 0x0121A460: `_$E2 -> 0x01228D00 here, 0x01228930 in landed 0x01219F00` -- `_$E1`/`_$E2` are TU-local
  static names VC7.1 reuses in every TU (the landed 0x01228D00 is itself `_$E2`); not a source error.
- TU emitters (empty-bodied PDB ctors, not retail bodies; only the dtor is graded): CEnginePrimitiveManagerParticleMeshGroup and the
  five quest scripts. 0x00408990 / 0x00B261D0 are user dtors and emit through their own definitions.
- Landed rows with poor models met on the way: 0x00B38590 ~CEnginePrimitiveManagerOcclusionBase (address-suffixed stand-in members,
  novtable struct), the eight ??_G stand-ins and five quest ctors above.
- Unlisted functions next to 0x00CAFC40 (CPosPassThru AddRef 0x00CAFC50, Release 0x00CAFC60, adjustor thunks 0x00CAFC80 / 0x00CAFC90,
  stub 0x00CAFC70) are not in the catalog.
- Every row matched within 2 probes; none needed more than 4 grades.
