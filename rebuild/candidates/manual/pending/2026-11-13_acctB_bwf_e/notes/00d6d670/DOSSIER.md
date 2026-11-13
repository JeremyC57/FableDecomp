# DOSSIER 0x00D6D670 (5786 B) -- NScript::CQ_HangingTreeEvilScript::Conclusion()

Scouted from: $W/dis_all.txt, my annotated listings $W/ann3.txt (every call / slot / string resolved; tool $W/tools/ann3.py)
and $W/compact.txt (calls/branches/member accesses only). $W/ann.txt / ann2.txt / xdis.txt are an earlier (pre-reset) pass;
ann2's `G:` / `ST:` slot tags are BOTH candidate names -- the right one depends on the receiver (see 3).

## 1. Retail shape
- `sub esp,0FCh; push ebx; push ebp; push esi; push edi` ... `pop edi/esi/ebp/ebx; add esp,0FCh; ret` (0x00D6ED09).
  thiscall, **plain `ret`** (no args), **no frame pointer** (ebp is a general register: &Prisoner.PImp, then &Executioner),
  **no SEH / no fs:0 / no EH states** even though it has many destructors -> this TU is the same as DoMission's (no /EHsc
  states emitted; like every landed sibling of this class -- they all build with default flags, `// Flags: none`).
- ebx = 0 for the whole body (`xor ebx,ebx` at entry); esi = this; edi = cached vtable / 0x0127094C (CSGRO vptr) / Game.
- No /GS cookie. No optimize("s") idioms seen (normal /O2: `push -1; push str; lea ecx; call CCharString`).
- Two locals are zero-initialised in the prologue region: a dword at body+0x48 (conditional-temporary flag word, bit0/bit1,
  used by the `||` expression at 0x00D6E6B5) and a bool at body+0x47 (`timeUp`, see OUTLINE B14). `body` = esp after the 4 pushes.

## 2. Identity (proven, 3 sources)
- Name: `NScript::CQ_HangingTreeEvilScript::Conclusion`, leaf `?Conclusion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ`
  (`void Conclusion();`, public, non-virtual, non-const).
  - Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00d89500) and FableWin.pdb (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv
    017ff404) both carry exactly this mangled name.
  - The only caller 0x00D6C37C is inside the landed DoMission 0x00D6BFA0
    (rebuild/src/compiled/00/d6/NScriptCQHangingTreeEvilScript_DoMission_00d6bfa0.cpp), which already declares
    `void Conclusion(); // 0x00D6D670` and calls it after the HTE_State loop.
  - Retail order matches the Ego_r method order: Introduction 0x00D6D080 -> Conclusion 0x00D6D670 -> Alloc 0x00D6ED10.
  - Content matches: outro / execution / escape cut-scenes CS_HANGINGTREE_EVIL_OUTRO / _EXECUTION / _ESCAPE.
- **Manifest label `_global::CQ_HangingTreeEvil_Execute` is WRONG** (made up).

## 3. Callees (direct)
| addr | landed row | name / PDB leaf |
|---|---|---|
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long = -1) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString |
| 0x0099EFE0 | 00/99/CCharString_operator_0099efe0.cpp | CCharString::operator=(const char*) (ReasonForFailing = "...") |
| 0x00CB7940 | 00/cb/CScriptBase_IsActiveThreadTerminating_00cb7940.cpp | CScriptBase::IsActiveThreadTerminating (17 sites) |
| 0x00BFE9BC | global_operatordelete_00bfe9bc.cpp | operator delete (inline CCountedPointer release) |
| 0x00BFEA0E / 0x00BFEA14 | global_malloc / global_free | CRT malloc/free (inline STLport map header node, block 1 only) |
| 0x007044D0 | 00/70/CCountedPointerCScriptThing_ShareData_007044d0.cpp | CCountedPointer<CScriptThing>::ShareData (inline CScriptThing::operator= / PImp assign). Landed leaf spells the pointee `CUnprovenPointee_007044d0` -- placeholder, not PDB |
| 0x0099A2E0 | CBaseClass_CBaseClass_0099a2e0.cpp | ~CBaseClass (tail of inline ~CScriptThing on temporaries) |
| 0x004AA840 | CScriptThing_CScriptThing_004aa840.cpp | ~CScriptThing (out of line: only the 6 function-scope CScriptThing locals at exit) |
| 0x0099A380 / 0x0099A430 / 0x0099A3B0 | CBaseObject_* | CBaseObject() / ~CBaseObject / operator= (inline CSGRO / movie ctor, dtor, assign -- block 1 + function-scope handles) |
| 0x007E72A0 / 0x007E74D0 / 0x008ABD10 | CScriptGameResourceObjectScriptedThingBase_* | CSGRO ctor / dtor / operator= COMDATs (blocks 2-3 and the exit dtors) |
| 0x00CD23B9 / 0x00CD2770 | ..._IsValid_00cd23b9 / _Release_00cd2770 | CSGRO::IsValid() const / Release() (unaligned COMDATs, never inline) |
| 0x007E72F0 | ..._MoveToPosition_007e72f0 | CSGRO::MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool) -- called DIRECT (devirtualised: by-value local) |
| 0x007E7360 | ..._ClearCommands_007e7360 | CSGRO::ClearCommands -- landed Leaf is a bogus `?CScriptGameResourceObjectScriptedThingBase_ClearCommands@@YI...` free function; the PDB is a member (see FreedomFighter Main's declaration). Flag in NAME_FIXES if you prove it. |
| 0x006E7B60 / 0x006E7B80 | CScriptGameResourceObjectMovieBase_* | movie ctor / dtor COMDATs (blocks 2-3) |
| 0x006E7AB0 | ..._006e7ab0.cpp | CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer (inside the inline movie dtor, block 1) |
| 0x00CD3D2E | STLmap_operator_00cd3d2e.cpp | map<CCharString,CSGROScriptedThingBase>::operator[] (out of line always) |
| 0x00CDBF70 / 0x00CDBFB0 | std__Rb_tree..._00cdbf70.cpp / CRetailTreeerase00cd344a..._00cdbfb0.cpp | that map's ctor / dtor COMDATs (blocks 2-3; block 1 expands them) |
| 0x00CD344A | Rbtree_Merase_00cd344a.cpp | _Rb_tree::_M_erase (inline map dtor/clear, block 1) |
| 0x00CBFB7D | not landed | `?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@...@std@@PAV?$map@VCCharString@@_N...@3@PAV?$map@VCCharString@@V1@...@3@_N4@Z` (Ego_r) = `void __fastcall RunCutsceneMacro(const CCharString&, std::map<CCharString,CSGRO>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool)`; all 3 sites: (name, cast, 0, 0, false, true) |
| 0x00CBE2FF | global_IsDistanceBetweenThingsUnder_00cbe2ff.cpp | `bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float)` |

Interface slots (receiver `Game` = this+0x40, CGameScriptInterfaceBase/CGameScriptInterface vtable 0x01260F0C; names from
rebuild/include/engine/CGameScriptInterface.h): 7 NewScriptFrame (+0x1C), 8 StartScriptingEntity(const CScriptThing&, CSGRO&,
EScriptAIPriority) (+0x20, always priority 4), 70 GetHero (+0x118, returns CScriptThing&), 72 GetThingWithScriptName (+0x120),
74 GetRandomThingWithScriptName (+0x128), 91 CreateCreature(const CCharString& type, const C3DVector&, const CCharString& scriptName,
bool) (+0x16C), 92 CreateCreatureNearby(type, pos, float, scriptName, bool) (+0x170), 265 ModifyThingHealth(thing, float, bool)
(+0x424), 296 SetQuestCardObjective(4 x const CCharString&) (+0x4A0), 321 DisplayQuestInfo(bool) (+0x504), 325
AddQuestInfoBarHealth(thing, const CRGBColour&, const CCharString&, float) (+0x514), 338 RemoveQuestInfoElement(long) (+0x548),
350 MiniMapAddMarker(thing, const CCharString&) (+0x578), 364 AddNewConversation(thing, bool, bool) (+0x5B0), 366
AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) (+0x5B8), 370
StartMovieSequence(const CCharString&, CSGROMovieBase&) (+0x5C8), 371 FixMovieSequenceCamera(bool) (+0x5CC), 376 Pause(float)
(+0x5E0), 379 PauseAllNonScriptedEntities(bool) (+0x5EC), 451 GiveThingBestEnemyTarget(thing, target) (+0x70C), 597
EntitySetThingAsEnemyOfThing(a, b) (+0x954), 601 EntitySetInFaction(thing, const CCharString&) (+0x964), 622
EntitySetAsOpinionSource(thing, const CCharString&) (+0x9B8), 655 GetActiveQuestName() (+0xA3C, returns CCharString).
pDSTGame 0x0143E8F8 slot 90 GetTimer(long) (+0x168) = inline `CTimer::GetTime()` on MissionTimer (+0xA4).
CScriptThing slots (vtable 0x01238C8C): 6 GetPos (+0x18), 18 MsgIsKilledBy(const CCharString&) (+0x48), 21 MsgIsHitBy (+0x54),
75 IsAlive (+0x12C), 76 IsDead (+0x130). ann2's `G:` names for +0x12C/+0x130/+0x18/+0x48/+0x54 are wrong when the receiver is a
CScriptThing (Prisoner/Executioner/pimp); `ST:` names for +0x1C/+0x20/+0x118 are wrong (receiver is Game).

## 4. Members (engine/CQ_HangingTreeEvilScript.h, retail = Ego_r - 0x14; all already named there)
Game +0x40 (CLevelScriptBase), MissionSucceeded bool +0x4C (escape path sets 1), MissionFailed bool +0x4D (sets 1 x3),
ExecutionerInterrupted bool +0x56, AudienceLeggit bool +0x57, ReasonForFailing CCharString +0x5C, TimerGUIIndex long +0x6C,
Executioner CScriptThing +0x74 (PImp +0x78), Prisoner CScriptThing +0x80 (PImp +0x84), MissionTimer CTimer +0xA4.
The header does NOT declare `Introduction()` / `Conclusion()` -- add `void Conclusion(); // 0x00D6D670` through a
`FABLE_RETAIL_*`-guarded overlay (overlay rule: full copy of current header + #ifdef additions), or define it out of class
after declaring it the way the header's other members are declared. Header macros used by siblings: FABLE_RETAIL_NSCRIPT,
FABLE_RETAIL_CLEVELSCRIPTBASE, FABLE_RETAIL_BASE_OBJECT, FABLE_RETAIL_CCHARSTRING (CHTE_FreedomFighter::Main 0x00D6A560).
DoMission 0x00D6BFA0 / CleanUp / SpawnParty model the class LOCALLY although the shared header exists -- do not copy that.
Other shared headers: engine/CScriptThing.h (FABLE_RETAIL_NSCRIPT; static NullCoord 0x0143E8E0 for inline GetPos;
FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT leaves operator= implicit so the TU expands it -> ShareData 0x007044D0 inline,
which is what retail does here at all 6 member assignments), engine/CScriptGameResourceObjectScriptedThingBase.h
(_OUTOFLINE_CTOR / _OUTOFLINE_DTOR / _OUTOFLINE_ASSIGN / _FORCEINLINE_ASSIGN), engine/CScriptGameResourceObjectMovieBase.h
(_OUTOFLINE_CTOR / _OUTOFLINE_DTOR / _FORCEINLINE / _FORCEINLINE_CTOR), engine/CBaseObject.h, engine/CTimer.h,
engine/C3DVector.h, engine/CRGBColour.h, engine/CGameScriptInterfaceBase.h, stlport/fable_containers.h.

## 5. Visibility / inline split (THE hard part)
Same source construct is expanded early in the body and called later -- the VC7.1 inline budget of a big (5.8 KB) body, the
same effect noted in CScriptGameResourceObjectMovieBase.h for CBH_GangDeputy2::Main 0x00D18DF0 ("VC7.1's own choice for the
implicit pair in that 3.3 KB body is the COMDAT call"):
- CSGRO ctor: INLINE at the 5 function-scope handles (0x00D6DEC6..: CBaseObject() + vptr 0x0127094C + zero +8/+C) and the
  block-1 hero handle (0x00D6DFF2); COMDAT 0x007E72A0 at blocks 2-3 (0x00D6E83F, 0x00D6EAF9).
- CSGRO operator=: INLINE in block 1 (CBaseObject::operator= 0x0099A3B0 + PImp share); COMDAT 0x008ABD10 in blocks 2-3.
- CSGRO dtor: INLINE for block-1 hero (0x00D6E3DD..0x00D6E412: PImp release, vptr 0x0126008C, ~CBaseObject); COMDAT
  0x007E74D0 in blocks 2-3 AND for the 5 function-scope handles at exit.
- map<CCharString,CSGRO>: ctor/dtor INLINE in block 1 (malloc(0x24) header, _M_erase/free), COMDAT 0x00CDBF70/0x00CDBFB0 in
  blocks 2-3. operator[] always COMDAT 0x00CD3D2E.
- Movie: INLINE ctor (0x00D6E2E4, vptr 0x01260EF4) + inline dtor (0x006E7AB0 + vptr 0x0126008C + ~CBaseObject) in block 1;
  COMDAT 0x006E7B60/0x006E7B80 in blocks 2-3.
- ~CScriptThing: inline (ShareData-less release + ~CBaseClass) on every TEMPORARY; COMDAT 0x004AA840 for the 6 named locals at exit.
=> First try: give the compiler visible implicit/inline bodies everywhere (no OUTOFLINE macros; STLport map with inline
ctor/dtor as in the landed PlayCutscene 0x00D848D0 / 0x00D7AA30 model or stlport/fable_containers.h) and let the budget split.
If the split lands elsewhere, measure with the c2 inline trace (agent2050 CHUNK_BRIEF Lessons) before forcing anything.
GetPos: on a NAMED by-value CScriptThing local -> inline `!PImp.Data ? NullCoord : PImp.Data->GetPos()` (0x00D6D822,
0x00D6DA08, 0x00D6DE5A, 0x00D6DE7D); on a TEMPORARY returned by GetThingWithScriptName (guards 0x00D6DB45/0x00D6DC09) and on
GetHero()'s reference (0x00D6DD05) -> virtual slot 6 through the object's own vptr. IsAlive on the named freedomFighter local
(0x00D6DCAA) inline: `PImp.Data && PImp.Data->IsAlive()`. IsDead / IsAlive / Msg* on the members Prisoner/Executioner: virtual
through the member's vptr (not inlined).

## 6. Siblings (same class, same TU neighbourhood)
- 00/d6/NScriptCQHangingTreeEvilScriptCHTEFreedomFighter_Main_00d6a560.cpp -- best template: shared CQ_HangingTreeEvilScript.h,
  defines FABLE_RETAIL_BASE_OBJECT / _CCHARSTRING / _CLEVELSCRIPTBASE / _NSCRIPT, includes fable_string.h, CCountedPointer.h,
  C3DVector.h, CSGRO header, CGameScriptInterfaceBase.h; `FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const
  { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }`; `FABLE_FORCEINLINE long CTimer::GetTime() const { return
  pDSTGame->GetTimer(TimerIndex); }`; `if (h.IsValid()) h.Release();`; EScriptAIPriority enum local.
- 00/d6/NScriptCQHangingTreeEvilScript_DoMission_00d6bfa0.cpp (the caller; terminate idiom `if (IsActiveThreadTerminating()) return;`).
- 00/d8/NScriptCQHeroSoulsMotherScript_PlayCutscene_00d848d0.cpp -- exact cut-scene idiom: `CSGRO hero; Game->StartScriptingEntity(
  Game->GetHero(), hero, 4); std::map<CCharString,CSGRO> cast; cast["HERO"] = hero; CSGROMovieBase movie;
  Game->StartMovieSequence("", movie); CGameScriptInterfaceBase* game = Game; game->PauseAllNonScriptedEntities(true);
  Game->FixMovieSequenceCamera(true); RunCutsceneMacro(name, cast, 0, &csargs, false, true); Game->FixMovieSequenceCamera(false);
  game->PauseAllNonScriptedEntities(false);` with an inline local STLport map model (malloc/free/_M_erase).
- 00/da/NScriptCQMinionCampScript_DoMission_00da0230.cpp -- the COMDAT form (CCutsceneCastMap ctor 0x00CDBF70 / dtor 0x00CDBFB0,
  CSGRO ctor/operator= declared out of line).
- 00/d6/NScriptCQHangingTreeEvilScript_SpawnEnemies_00d6bb80.cpp (`// Flags: none`, CRGBColour.h).
- No pragmas in any sibling of this class.
- Lua oracle not consulted (docs/scripts/AEON_LUA_PORTS.md -- check the HangingTreeEvil conclusion for statement order if a block reorders).
