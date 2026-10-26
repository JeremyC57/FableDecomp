# namepass: one retail address, one name (2026-10-26)

Worktree `$N/repo` (HEAD 75912f3fc); no git. Tools in `$N/tools` (rg_run.sh / sg_run.sh = recheck/mkpayload + vland
--recheck-landed + pairs.py; apply.py; bindupd.py; cc.py; rx.py), generators in `$N/gen`, staged rows in `$N/stage/<phase>`,
regrade logs in `$N/rg/<tag>`. Every applied row was graded from its staged source/test first (MATCH/RELOCATION_MATCH +
test PASS), its `// Leaf:` checked against the object's defined symbols, and its relocation pairs checked for the
canonical callee symbol.

## Phase F1 -- CCharString(const char*, long) 0x0099EBF0 via `FableCharStringResult_X::build` (463 rows)
- Callee: `??0CCharString@@QAE@PBDJ@Z` (both PDB name sets; the only `const char*` constructor). 463 rows called it as
  the invented `?build@FableCharStringResult_<addr>@@QAEPAXPBDH@Z` (one spelling per row).
- The rows themselves are static `__fastcall` CCharString-by-value name functions; rewritten as such (hidden result in ecx):
  - 270 `X::StatGetClassTypeName` (catalog names were right; literal = the class name; PDB `?StatGetClassTypeName@X@@SI?AVCCharString@@XZ`).
  - 191 `CAIStateGroup_X::GetName` (catalog `_global::MakeCCharString_SG_*`): each is called once, by the static
    initialiser that registers it with an Alloc factory pushed just before the call (`push Alloc; push ecx; mov ecx, esp;
    call GetName`); the Alloc's new object (or the constructor it tail-calls) stores the class's RTTI vtable at this+0.
    PDB `?GetName@CAIStateGroup_X@@SI?AVCCharString@@XZ` and `?Alloc@CAIStateGroup_X@@SIPAVCAIStateGroupBase@@XZ` both
    exist for every class; 180/191 literals spell the class name, the other 11 are near-spellings
    (SG_MINION_GO_TO_BED = CAIStateGroup_GoToBed, SG_CREATURE_RANGED_COMBAT_TWO = ...RangedCombat2, ...).
  - 0x006922A0 = `NCreatureAction::CActionEventAddObject::GetHook() const` ("CARRYTHINGADD"; RTTI vtable 0x0125C178 slot 2;
    GetHook is the only CCharString-returning virtual of the CActionEvent* classes in the PDB name sets).
- Classes with an engine header use the layout-rename include pattern (144 rows); the rest declare the class locally
  (static member only). Tests construct the result in place and check text/length/identity with a stub constructor.
- Graded: 463/463 RELOCATION_MATCH + PASS (rg/f1). Renamed rows: 192 (RENAMES.tsv). Bindings: 463 dropped
  (the `build` spellings), canonical already recorded.

## Phase F2 -- CTCBase accessor run 0x006869D0..0x00686DB0 (338 rows)
- Names (Ego_r CTCBase member list, same order as the retail run; bodies all read 0x013B8A1C = CThingManager*):
  0x6869D0 `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ`, 0x686A70 `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`,
  0x686A80 `?PeekWorld@CTCBase@@IBEABVCWorld@@XZ`, 0x686AE0/0x686AF0 `?GetThingManager@CTCBase@@IAE/IBE...` (non-const
  then const), 0x686D20 PeekWorldMap, 0x686D30 GetWorldMap, 0x686D40 GetPlayerManager, 0x686D50 PeekPlayerManager,
  0x686D60 PeekComponent, 0x686D70 PeekSampleBank, 0x686D80/0x686D90 Get/PeekThingSearchTools (not landed),
  0x686DA0 GetPWorldSeed, 0x686DB0 GetPLocalSeed. All protected (`I`): only CTCBase-derived code calls them.
- Own rows: 0x686AE0/0x686AF0 rewritten and renamed `_global::GetGlobal_013b8a1c_*` ("GFGetPreMainMemoryUsed" Leaf, a
  propagated label) -> `CTCBase::GetThingManager`; 0x686A70 rewritten from a free `GetWorld()` returning
  `CViewBase::m_pDisplayEngine` to `CWorld& CTCBase::GetWorld() const`. Other run rows already carried the PDB Leaf.
- Callers (gen/hier.py, "base injection"): the caller's class gets its real RTTI primary-base chain down to CTCBase
  (`class CTCWife : public CTCBase`, `CTCPhysicsStandard : CTCPhysicsBase : CTCBase`, namespaced bases such as
  `NInventory::CTCInventoryBase` in their namespace); CTCBase carries the canonical protected declarations; the stand-in
  declaration on the derived class is removed; calls use the canonical name (pointer stand-ins become references:
  `GetWorld()->X` -> `GetWorld().X`, other uses `&GetWorld()`; const/non-const overload pairs resolved by the constness of
  the calling member). Layout: a stand-in that carried the vptr as `virtual v00..` moves those slot declarations into
  CTCBase (same order; the class re-declares only virtuals it defines) because VC7.1 puts an empty base of a polymorphic
  class at +4; a stand-in with a `void* __vftable` member keeps it and gets an empty CTCBase at +0 (EBO). A caller
  deriving from a generated engine header gets the D34 layout-rename (`#define CTCBase CTCBaseLayout` ... `class CTCBase :
  public CTCBaseLayout`; deeper: `class CTCPhysicsStandard : public CTCPhysicsBase, public CTCPhysicsStandardLayout`,
  first empty base at +0, verified). Stand-in return types renamed to the real class (CDefinitionHolder/CWorld_Calls ->
  CGameDefinitionManager/CWorld) and class keys fixed to PDB (V); a real base kept (`class CGameDefinitionManager : public
  CDefinitionManager {}` where the stand-in used the base). Callee constness fallout: a callee declared non-const is made
  const only when every PDB name of it is const (famfix.py); locals receiving const results become `const T*`.
- Regraded 422 staged rows (rg/f2u, f2w, f2z, f2own*): applied 335 callers + 3 own rows, all RELOCATION_MATCH + PASS with the
  canonical symbols only; 9 more graded good but rejected (they would introduce a brand-new spelling for another callee,
  mostly the non-template `CDefinitionManager::GetDef` const) and one DIFFER (0x0075C530) not applied.
- Bindings: 223 dropped (BINDINGS_DROP.tsv, phase F2), 37 added (canonical symbols new to the table).
- Kept (129 recorder rows still binding a CTCBase-run target under another spelling; gen/f2_kept.tsv, reason per row):
  45 const cascade into callees whose retail name is unproven or a template (`CGameDefinitionManager::GetDef/
  GetPDefFromGlobalIndex/GetDefGlobalIndexFromName`, real owner CDefinitionManager, GetDef a member template;
  `CThingManager::GetThingFromUID` = PDB PeekPThingWithUID const...), 19 rejected after grading (new spelling for another
  callee), 20 free-function stand-ins for the accessor (`_GetGlobal`, `helper_686a80` ...: the row itself needs re-modelling
  as a member), and ~45 shape cases (constructor/destructor in a class whose vptr would move, inline virtual bodies,
  call through `(expr)->`, protected access from a non-derived helper).
- Header debt: the injected local `class CTCBase` (no base clause) triggers SHARED_HEADER_AVAILABLE where engine/CTCBase.h
  exists but models the vptr as data (unusable as a polymorphic base); 508 sources already declared a local CTCBase.

## F3 — CEntityScriptBase::SetPredicate 0xF35B10 / IsActiveThreadTerminating 0xF35B30 (10:00-11:00)
- Names: Ego_r `?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z`,
  `?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ` + the landed 0xF35B10 row + bytes (slot 3 GetParentScript then
  `jmp` CScriptBase::SetPredicate 0xCB7920 / CScriptBase::IsActiveThreadTerminating 0xCB7940). 196 spellings before
  (IsDone/IsAborted/IsFinished/... on every entity class, free `@helper@4` forms).
- Derivation: entity scripts have no RTTI; `CHAIN` accepts X when Ego_r has `?Alloc@X@...` returning
  CCountedPointer<CEntityScriptBase> (594 classes) or when the stand-in has Game +0x04 / Me +0x08.
- Filters: NScript::CIsThingAlive / CIsThingConscious derive from NScript::CPredicateBase (Ego_r Clone returns
  CPredicateBase*); CPredicateBase is polymorphic, abstract, 4 bytes (struct_layouts_egor) and modelled
  `class __declspec(novtable)`: with a plain polymorphic base, a filter ctor written under `#pragma inline_depth(0)`
  (to keep the CScriptThing copy 0x004ABE90 out of line) CALLS the base ctor; novtable makes it vanish (7 rows
  regressed DIFFER before, all MATCH after). The 0x00D9C4A0 row already modelled it novtable with pure Clone.
- Scope: Ego_r quest-script classes are `NScript::CQ_x::CTC_y` but CEntityScriptBase is global; stand-ins that had put
  CEntityScriptBase inside `namespace NScript` are moved out (10 rows); `struct CPredicateBase` -> class (key V).
- 0x00F35B30 own row: was a junk free `IsActiveThreadTerminating(CScriptBase*)` + TrollWhack `Initialise` body (PassPattern
  TROLLWHACK_INIT_OK); rewritten as the member (twin of SetPredicate) with a behaviour test -> RELOCATION_MATCH PASS.
- Applied: f3b 243 + f3c 18 rows; 00EF5CB0 still NONCANON (left). Bindings: drop 247+5, add 17.

## N1 — CThing::PeekDefinitionManager 0x4C7990, GFGetSystemManager 0x9A4EC0
- n1a 21 rows, n1b 2 rows (0x0073A9F0, 0x008CB6E0). Kept: FABLE_FASTCALL `GFGetSystemManager` stand-ins returning
  local types (CSystemManagerNative/...): need CSystemManager modelling; CThingNative/ThingMethods non-RTTI stand-ins;
  GetDef template const cascade (C2662/C2663 on member templates `??$GetDef@`, no non-template name exists).

## OWNER 0x993B10 = CDataStream::~CDataStream (done before F3; see F2 notes) and 0x99A300 = CBaseClassNonCopyable dtor (label).

## G1 — generic PDB-proven targets (canon2: PDB spelling agreeing with caller binding or row Leaf, 796 targets)
- famdrv g1specs over the base-graded rows -> 938 staged, 594 compiled (famfix), 573 graded (21 overlapping F3/f2clean
  rows deferred to redo), 429 accepted, 428 applied (0x00E386E0 overlapped f3c). Rejects: 25 DIFFER + 9 LINK_FAIL/IDENTITY
  (kept, bytes over names), 49 NEWSPELLING (the owner-alias rename made a non-retail spelling), 60 NONCANON (planned
  target still off: return-type stand-ins such as GetPItem -> CThing*, F4 below).
- famcheck now takes `--plan`: other spec targets a row did not plan to fix may keep a pre-existing table spelling.

## F4 — CBaseIntelligentPointer / CIntelligentPointer<T> (vtable 0x0129C95C; 0xA01B10/0xA01B50/0xA01B90/0xA01C10)
- Names: Ego_r `??0CBaseIntelligentPointer@@QAE@XZ`, `??1...@@UAE@XZ`, `?GetPItem@...@@QBEPAVCBaseObject@@XZ`,
  `?SetPItem@...@@QAEXPAVCBaseObject@@@Z` (8 bytes: vptr + CBaseObjectPointer* PData) + bytes (ctor 0xA01B10 stores
  0x0129C95C). The 33 CIntelligentPointer<T> vtables are named from RTTI (gen/ip_vtables.txt: 0x1238C6C CThing,
  0x1237F1C const CThing, 0x1231C4C CThingPlayerCreature, 0x123E7D8 CThingCreatureBase, ...).
- gen/f4.py: canonical base text; derived stand-ins -> `template <class T> class CIntelligentPointer : public
  CBaseIntelligentPointer` with typed inline wrappers `T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }`
  (byte-identical); DIRECT mode retypes base-typed uses; explicit `x.~CBaseIntelligentPointer()` is qualified
  (`x.CBaseIntelligentPointer::~CBaseIntelligentPointer()`), else a virtual call (14 DIFFER before the fix).
- Applied: f4b 170, f4c 171, f4d 14. Bails: 121 rows with no class (free fastcall IP_Get/DestructMember stand-ins), 54 with
  an unknown pointee, 22 with differing derived bodies, base data members used directly.
- ~150 sources call CBaseObject (vtable 0x0129A7D8, ctor 0x99A380) "CBaseIntelligentPointer": that is F5.

## GV1 — global 0x0143E90C = Ego_r `pGlobalScriptDef` (9 rows renamed `extern CScriptDef* pGlobalScriptDef`; gen/gvar.py).
  Retail's object is a dynamic-init CDefPointer<const CScriptDef>; the pointer spelling is the first-word simplification
  the table already used. GFLoadScriptDef 0x00CBF647 keeps `extern CDefPointer g_scriptDef` (its own row models the object).

## O1 — owner member names (gen/o1specs.py; 13 rows): 0x4C7A10 CThing::PeekPlayer (QBE, calls the const GetPlayer
  0x449910; 0x4C7A30 is CThing::GetPlayer over the non-const 0x4498C0), 0xA14040 C3DVector::RotateXY, 0xA14510
  C2DVector::AccurateNormalise (fsqrt) vs 0xA14540 C2DVector::Normalise (table approximation; Ego_r has both),
  0x9CC410 NGameText::CDataBank::LookupValue (descriptive; CBankFile::FindIndexBySymbolCRC is a candidate, unproven).
## CR1 — gen/clsren.py: a stand-in class whose every bound member is one real class's canonical member is renamed to
  it (7 rows); real classes are never renamed (inheritance, not a rename).
## OWN4 — 0x9038D0 / 0x904030 are virtual members of CScriptGameResourceObjectScriptedThing (vtable 0x0128D86C slots 12/7):
  rows rewritten with the real `UBE`/`UAE` members of a local polymorphic class (engine header models the vptr as
  data); callee 0x905180 = `NTCScriptedControl::CActionFollowCreature(CThing&, float, bool)`.

## F5 — CBaseObject special members (0x99A380 ctor, 0x99A390 copy, 0x99A3B0 operator=, 0x99A430 dtor)
- Names: Ego_r 0x00B01FD0.. and FableWin.pdb both: `??0CBaseObject@@IAE@XZ`, `??0CBaseObject@@IAE@ABV0@@Z`,
  `??4CBaseObject@@IAEAAV0@ABV0@@Z`, `??1CBaseObject@@MAE@XZ` (protected; protected virtual dtor), same order as retail.
- gen/f5.py: the stand-in (N_0099a380::CBaseIntelligentPointer, CVillageTask, CBase_0099a430, public CBaseObject...)
  becomes CBaseObject with those members in a trailing `protected:` block and `virtual ~CBaseObject()` replacing the
  placeholder virtual / vptr word. 33 staged, 30 compiled, 30 RELOCATION_MATCH PASS, 30 applied.
- Left: 2 free stand-ins (CVillageTaskBase_Destruct, CScriptConversation_dtor), CMember_0099a430 (no vptr in the row),
  3 rows whose protected dtor is called from a non-derived helper (C2248).

## OQ1 — CDefClassBase::operator= 0x431F10 (41 table spellings: Copy/BaseCopy/base_clone/Duplicate/InheritFrom/free fastcalls)
- Name: Ego_r 0x0043CE00 + FableWin `??4CDefClassBase@@QAEAAV0@ABV0@@Z` + the own row's Leaf. The callers are the DECLARE_DEF
  `X::Copy(const CDefClassBase*)` bodies (Ego_r `?Copy@CSmashableDef@@UAEXPBVCDefClassBase@@@Z` + `??4CSmashableDef@@...`):
  retail inlines the def's implicit operator=, which calls the base operator= first.
- gen/opeq.py: stand-in S::M(const X*) -> `((CDefClassBase*)obj)->operator=(*(const CDefClassBase*)src)` (cast object),
  or S renamed CDefClassBase when S declares only M (+ data) and is not a real class (`S::M(o)` -> `CDefClassBase::operator=`);
  free fastcall F(self, edx, src) / stdcall F(src) riding ecx -> the member call. Calls are only rewritten on objects typed S
  (cast, declared variable, `this`); other classes' same-named methods (sub-def Copy at 0x46078C...) are left alone.
- gen/tfix_opeq.py: 11 behaviour tests mocked the old stand-in callee; their mock becomes CDefClassBase::operator=.
- Applied 59 + 11 rows. Table spellings 41 -> 16 (the rest: rows not yet regraded hold stale spellings; rebuilt at the end).

## OWN5 — own rows of the CBaseClass / CBaseObject / CBaseIntelligentPointer TU (13 rows; all graded, all applied)
- 0x99A2E0 ??1CBaseClass@@UAE@XZ (was free FableRestoreCBaseVTable_0099A2E0; new behaviour test), 0x99A390 IAE copy,
  0x99A3B0 IAE operator= (was a placeholder `C0099a3b0::M`, path CFourierAnalysis_* renamed), 0x99A3C0 QAE
  ClearIntelligentPointers (was CBaseObject_Methods), 0x99A400 UBE Validate (was free), 0x99A430 MAE dtor, 0x99A4D0 ??_G MAE
  (protected ctor/dtor, friend factory); 0xA01B10/B20/B50/B90/C10/C50 with Ego_r's members: ClearData is private const
  (`ABE`), GetPItem const (`QBE`), PData mutable; the dtor UAE. Every own row now has an exact `// Leaf:`.
- Binding fixed: `??_7CBaseIntelligentPointer@@6B@` was bound to 0x0129A7D8 (CBaseObject's vtable) -> 0x0129C95C.

## D1 — virtual base destructors (gen/dv.py)
- Targets: canon2 `??1C@@UAE@XZ` + three decided here: 0x62E900 ~CCombatActionBase (catalog + PDB UAE; implicit dtor that
  destroys the CCombatActionBase members and tail-jumps 0x693EF0), 0x693EF0 ~CCreatureActionBase (stores vtable 0x01259D24,
  RTTI .?AVCCreatureActionBase@@ + PDB UAE), 0x8FD288 ~CAIStateGroupBase (catalog + PDB UAE; implicit: CCharString +0x0C).
- A row binding `??1C@@QAE@XZ` (plain ~C()) or `??1CBase_x/CMember_x@@QAE@XZ` gets `virtual ~C()`; the class must already be
  polymorphic in the row, or its leading `unsigned char _pad[N]` gives its first dword to the vptr ([N - 4]); explicit
  calls are qualified. 451 staged, 403 MATCH, 402 accepted + applied; ~47 DIFFER kept (a derived user-declared dtor of a
  now-polymorphic class stores its vptr: retail's derived dtor is implicit there).
- Not done: 102 rows whose stand-in is a real class used as the base's name (CPhysicsMeshInfo for ~CCreatureActionBase...).

## ITA — vtables 0x012C2FE8 / 0x012C32F0: slot 0 of 0x012C2FE8 is `IsAlive() && !IsUnconscious()` (CScriptThing slots
  75/61) = NScript::CIsThingConscious; 0x012C32F0's is `IsAlive()` = NScript::CIsThingAlive (Ego_r has both, NScript).
  3 rows (0xDF4610, 0xDF4BE0, 0xEF3F00) renamed their 0x012C2FE8 filter to NScript::CIsThingConscious. 0xEFE700 keeps
  CIsThingAliveEndGame (its module-local out-of-line dtor 0xEF7AB0 would collide).
- IS_THING_ALIVE (0x40DCE0 / 0x43DBF0 ...): identical bodies of the function template `IS_THING_ALIVE<T>(const T&)` (Ego_r
  has ~30 instantiations); the linker folded instantiations, so one retail address legitimately carries several
  `??$IS_THING_ALIVE@...` names (same for CDefClassBase::GetSubDef<T> 0x669580, 151 Ego_r instantiations). Left as is.

## F4 second sweep (f4e/f4f/f4g): over the 24.8k rows graded so far: 1057 staged, 737 compiled, 716 MATCH, 644 applied;
  f4.py now emits `reinterpret_cast<T*>` wrappers (C-style cast to an incomplete CBaseObject is C2027), `public:` before
  them, `> >`, and retypes helper parameters that dereference the item (`IS_THING_ALIVE(const CBaseIntelligentPointer&)`
  -> CIntelligentPointer<T>): +29 +33 rows.

## FM1 — free fastcall stand-ins for a member callee (gen/fm.py; 985 + 26 rows)
A row reaching a proven member C::m (g1specs / F4 / D1 virtual base destructors) through a free `__fastcall F(self[, edx], ...)`
(same bytes: ecx = this, dummy edx) calls `((C*)(p))->m(...)`, or `((C*)(p))->C::~C()` for a destructor target (a direct
call, as before). 22 rows not applied: CUserProfileManager::ApplyValues 0x407370 is `IAE` (protected) in Ego_r and the
callers are free stand-ins, so the PDB spelling cannot be emitted from them without inventing a member; 1 row (0x900F43)
binds CCharString::Destroy to 0x00A01C10 (pre-existing). Tests: gen/tfix_fm.py turns the mock of F into the member's
definition; a mock destructor's class is `__declspec(novtable)` so it does not overwrite the vptr the row under test
stored (0x46642A failed on exactly that).

## CRT1 / CRT3 — MSVCR71 imports under their CRT names (gen/crt.py)
`_free`, `_malloc`, `_memmove`, `??2@YAPAXI@Z`, `??3@YAXPAX@Z`, `??_V@YAXPAX@Z` (the import thunks 0xBFEA14 / 0xBFEA0E /
0xBFEAE6 / 0xBFEA1A / 0xBFE9BC / 0xBFEB1C; the catalog names them MSVCR71.DLL::x). 465 + 15 rows (CRT1). Bug: the
"declaration already present" test matched the rewritten `free(p);` call, so 358 rows lost their declaration (C3861);
fixed and re-run over every graded row as CRT3. Tests mocking the stand-in get a replacement global operator new/delete
(gen/tfix_crt.py); a mock of free/malloc cannot replace the static CRT and is reported instead. 26 rows bail: the
stand-in returns a value the caller uses where the CRT function returns void (cannot be expressed without a cast that
changes nothing but hides the type).

## ACC1 — retail access / virtual in the class model (gen/access.py; 11 rows) + 4 Leaf lines
Rows of the keypass leafscan list touched by this pass whose emitted symbol differed from the retail Leaf only in the
access/virtual letter (InitClosed AAE, GetWorldNames AAE, IsNightTime AAE, PlayLoopAnim / UpdateFlight IAE, FinishAction /
FrameUpdate / OnCreate / ExtinguishFiresOnThing / SetThingPersistent U, PostInit MAE): the declaration moved under the
retail access label (virtual only where the modelled class was already polymorphic). All RELOCATION_MATCH + PASS, leaves
now name the emitted symbol. Leaf lines corrected: 0x4D95EB `??1CTCObstacle@@UAE@XZ` and 0x550C60
`?Initialise@CTable@NUISystem@@UAEXXZ` (PDB; D1/G1 made them virtual); 0xD91320 / 0xE64DF0 `Main` restored to private
(F3's base injection had hoisted it to public). Not fixed (stand-in class names, out of scope): 0x904540, 0x851890
(`*_Methods` stand-ins), 0x5E4740 (last parameter type), 0xD9EE20 (nested class).

## IP1 — CIntelligentPointer<T> vtables bound to another T (gen/ipfix.py; 11 rows)
33 RTTI-named vtables (gen/ip_vtables.txt). Rows whose `??_7?$CIntelligentPointer@<T>@@6B@` relocation hit the vtable RTTI
names for U != T now instantiate U (0x5EA9B0, 0x739550, 0x7BA550, 0x8367B0, 0x95E260, 0x771660: <const CThing>;
0x6AC140, 0x970AB0: <CThingCreatureBase>; 0x6C5310: <CThing>; 0x89E540: <CThingPlayerCreature>; 0x981350: speaker
<CThingCreatureBase> / target <const CThing>, swapped, which Ego_r's AIAction_SpeakAISpeechToTarget signature confirms).
0x7C9180 kept (DIFFER when its const and non-const instantiations are merged).

## OWN6 / OWN7 / OWN8 — family pass 3's wrong vtable bindings
- 0x7E2760 = CTCFineDialogue::~CTCFineDialogue (stores 0x01270274 = RTTI CTCFineDialogue; was modelled as
  CInputProcessManager with g_vtbl_CInputProcessManager -> 0x01270274). Body `{ RemoveFromGlobalTCs(); }`, base ~CTCBase.
- 0x56E2D0 = NCameraModes::CCameraPointModeBase::~CCameraPointModeBase (`MAE`, Ego_r; stores 0x0124DDDC whose RTTI is
  .?AVCCameraPointModeBase@NCameraModes@@; was `??_7CInputProcessManager@@6B@` -> 0x0124DDDC). Base ??1CBase@NCameraModes@@UAE@XZ.
- 0xC33120 stores 0x012B4F74 (no RTTI; slot 0 = its deleting dtor 0x00C33170; the landed ctor 0x00C3C3B0 stores it):
  CVtable_012b4f74::~CVtable_012b4f74, base ~CVtable_012b5860 0x00C3C5C0 (virtual: 0x012B5860 slot 0 = 0x00C3C600 calls
  it). CScriptDefinitionManager's own constructor 0x00CD3F00 stores 0x012C2648, so `??_7CScriptDefinitionManager@@6B@`
  -> 0x012B4F74 was wrong. Same class cluster unified: 0xC3C450 CVtable_012b4f74::Close (was CVtable_012B4F74 /
  Shutdown / CSoundBufferBase_00c3c7a0::Close), 0xC3C720 CVtable_012b4f84::Close (slot 1 override; was Shutdown),
  0xC3C7A0 CVtable_012b4f84::SetupEventNotification (was CSoundBuffer_00c3c7a0).
- 0xA54FA0 / 0xA13800 (C2DLineF) and 0x456B7D: see TR1 / remaining.

## TR1 — CPersistContext one-argument transfer helpers (gen/tr.py; 59 + 24 rows) — DESCRIPTIVE
0x431061 (26 spellings), 0x431102 (24), 0x43314A (18), 0x431020, 0x45228F, 0x451E11, 0x431143, 0x458F2B, 0x4595A3,
0x456B7D: one name each, the catalog-wide majority `void CPersistContext::Transfer(T&)` (float, long, bool, unsigned long,
CDefString, CEngineGraphic, CCharString, CFloatRange, CRGBColour, EOpinionDeedType). Ego_r's CPersistContext::Transfer<T>
are templates taking a name string (`??$Transfer@M@CPersistContext@@QAEXPBDAAM@Z`); these helpers take one reference and
have no PDB name, so the name is descriptive. 0x456B7D's placeholder `TransferOpinionDeed(int&)` is now
Transfer(EOpinionDeedType&) (Ego_r CExpressionDef::OpinionDeedType / CKickableDef fields are EOpinionDeedType).
Arguments keep their address: `*(T*)(p)` for a pointer parameter, `*(T*)&(x)` for a reference of another type.

## TR2 — the TR1 name becomes the member template (gen/tr2.py; 146 rows + own10 6 rows)
Superseding TR1's non-template spelling: the landed own rows 0x431061 / 0x431102 / 0x431020 already define
`??$Transfer@M@CPersistContext@@QAEXAAM@Z` (explicit specialisations of a member template), so the one name for every TR
target is `template <class T> void CPersistContext::Transfer(T& value)` specialised for T (Ego_r's Transfer<T> are
member templates too). Callers declare the template once instead of the non-template overload; calls are unchanged (T is
deduced). OWN10 re-landed the other own rows as the specialisations: 0x43314A <bool>, 0x458F2B <CFloatRange>, 0x4595A3
<CRGBColour>, 0x45228F <CDefString>, 0x451E11 <CEngineGraphic>, 0x456B7D <EOpinionDeedType> (the last three were
`CEngineLightingManager_UpdateShadowScene` free functions). 0x431143 <CCharString> is not landed. Tests declare the
specialisation right after the class (C2908 otherwise); gen/tr2_tfix.py reverted 3 tests whose fixer over-converted
types that keep a non-template overload (EReverbEnvironmentType, CLongVector, the CWeaponDef enums). 16 rows bail (no
non-template TR declaration: their CPersistContext spells the helper differently, e.g. `TransferBinary(float*)`,
`Transfer(CCharStringData*&)`); listed under Remaining.

## OWN9 — 0xA54FA0 = std::vector<CQuaternion>::_M_insert_overflow
Caller 0xA4EFC0 inserts identity quaternions (IsIdentity, agent952), CQuaternion {X,Y,Z,W}; the C2DLineF helpers the
old model pointed at are 0xA13490 / 0xA134D0 (bindings recorded at 0xA13800, BINDINGS_DROP.tsv ADD lines). C2DLineF has
a user copy constructor in Ego_r, which the old trivially-copied model contradicted.

## OWN11 — 0x8FD288 = CAIStateGroupBase::~CAIStateGroupBase (implicit UAE dtor)
Destroys the CCharString Name at +0x0C (0x99EAE0) then tail-jumps ~CBaseClass 0x99A2E0. Modelled as an empty user dtor
on a `__declspec(novtable)` class (no vptr store, like retail's implicit one). Was `FableDestVal_008fd288`.

## DEF1 / OWN12 — the def Transfer cluster (gen/def1.py; 15 rows) + four enum/byte transfer helpers
TR1/TR2 bailed on 12 def `Transfer(CPersistContext&)` rows whose archive stand-in was not called CPersistContext
(CXfer / CArchive / CSerial / CTransferArc / CTransferSink / CSerializer / CTransferObject / CTransferContext, or
CPersistContext::TransferValue / TransferBinary / Transfer(X*)). Each row now models its def from Ego_r.pdb (retail
offset = PDB less 4 for CThingBaseDef-derived defs; equal for CDefClassBase-derived ones), declares the member template
and passes the member by reference; CThingBaseDef-derived rows inherit from CThingBaseDef and call
`CThingBaseDef::Transfer(context)` (0x676BE0: 7 spellings -> 1). The own rows are now the virtual
`?Transfer@<Def>@@UAEXAAVCPersistContext@@@Z` the PDB names (were free `__fastcall` stand-ins or `QAE` with the stand-in
archive type), except 0x452B04 (engine/CHairCardDef.h declares no members, so the free form stays) and 0x452398 (header
class; its `CHeroMarriageDef_Transfer` derived-methods struct stays). 0x434290's member types were picked per callee by
the old model (long* -> 0x431020 ...): they now follow the PDB (Antialiasing unsigned long, Resolution*/BitDepth long,
the eight detail floats) and the helpers line up with their own rows' types. Further names fixed on the way (2 sources each):
- 0x455F3C = CPersistContext::TransferSubComponent<CTCNoiseDef>(const char*, CTCNoiseDef&) (Ego_r symbol of that name +
  0x455F1A passes "Noise" @ 0x12338F0 and &Noise, Ego_r CThingNoiseDef::Noise is a CTCNoiseDef at that offset).
- 0x5DEDC0 = CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess>::SortAndTrim (its own landed name; callers
  0x473504 / 0x4ED459 called it CAnimationSet::EnsureSorted / SubObject::Finalise on Animation.Anims at +0).
- 0x47367A Transfer<CAnimationSet> (already landed under the template name) for 0x4ED459's `Persist(SubObject*)`.
- OWN12 own rows re-landed as specialisations (were "CEngineLightingManager::UpdateShadowScene" / PersistTransfer_x):
  0x454E5B Transfer<ESwitchTriggerType> (CThingMarkerDef +0x80 / CThingSwitchDef +0x70 = PDB TriggerType; Ego_r
  instantiates Transfer<ESwitchTriggerType>), 0x455008 Transfer<EPSwitchTriggerType> (CThingPhysicalSwitchDef +0x70),
  0x473593 Transfer<unsigned char> (CAppearanceDef OutlineEffectAlphaRef/Bias), 0x4735D6 Transfer<ELightingChannel>
  (CAppearanceDef LightingChannel; Ego_r Transfer<ELightingChannel>). New tests for the rewritten rows use a recording
  harness (order, helper kind, member address).

## CRT4 — the 12 CRT3 rows famfix rejected (stage/crt4)
crt.py wrote `((" " T)(call))` for a stand-in whose declared return type differed from the CRT's (5 rows: plain casts now),
`malloc(void*)` (2 rows: `(unsigned int)` cast, same bytes) and called `operator delete[]` without declaring it (5 rows:
VC7.1 C3861; the declaration is added). 12/12 RELOCATION_MATCH + PASS; 0x462F50's test mocked the stand-in allocator
and now checks the real malloc'd block (a static-CRT malloc cannot be replaced).

## BS1 — CBasicString<char>::operator==(const char*) const 0x4115A0 (gen/bs.py; 63 rows)
FableWin `??8?$CBasicString@D@@QBE_NPBD@Z` / `??0?$CBasicString@D@@QAE@PBD@Z`; the rows modelled CCharStringData's
member as a non-template `CBasicStringChar` / `BasicString`. It is now `template <class T> class CBasicString` used as
CBasicString<char> (same members, constructors keep the injected name, same-TU stand-in bodies become template member
definitions). 63 applied (61 + 0x9AF1E0 + 0x99E960; 0x99E960's constructor 0x9A0590 now spells
`??0?$CBasicString@D@@QAE@PBD@Z`, FableWin's name). Not applied: 0x859040 (its anonymous namespace hash changes with the
file, NEWSPELLING), 0x4AFE20 / 0x6238A0 (their same-TU stand-in body of 0x4115A0 gets inlined once it is a template
member: DIFFER 356v345 / 247v225, current sources kept).

## CAM1 / CAM2 — NCameraModes::CBase::~CBase 0x56E0E0 (RTTI base of every camera mode)
0x56E2F0 named the base `CCameraModeBase`, 0x69DE00 `CBase_0056e0e0` (non-virtual, outside the namespace); both now
derive from `NCameraModes::CBase` with its virtual dtor (RTTI: CCameraPointModeBase / CStringAndCageBase :
NCameraModes::CBase; Ego_r ??1CBase@NCameraModes@@UAE@XZ). 0x69DE00's vtable relocation now spells
`??_7CStringAndCageBase@NCameraModes@@6B@` (RTTI-correct; replaces the old spelling 1:1) and its Leaf is the `??_G`.
CAM2: the free `__fastcall` composite deleting destructors 0x69E700 / 0x873490 / 0x874E70 call the base as
`((NCameraModes::CBase*)p)->NCameraModes::CBase::~CBase()` (was `CScriptedControl_Destruct` / `CZTargetNew_Destruct` /
`CPCMouseLook_Destruct`; 0x873490's `OperatorDelete` stand-in is operator delete).

## Re-validation of $S/names/callees (tools/callees_check.py -> CALLEES_REVALIDATE.tsv)
Of the 995 targets this pass named, 619 agree with callees/CANON.tsv, 136 differ, 240 are not in CANON. Of the 136,
110 are CANON `descriptive` / `landed` labels (mostly propagated labels the pass replaced with the PDB name: e.g.
CPersistContext::TransferULong -> Transfer<unsigned long>, CEngineLightingManager::UpdateShadowScene -> Transfer<E...>).
The 26 with stronger CANON confidence: 9 `rtti` (CANON took the vtable's most-base class or a `vfunc_N` slot name; the
pass has the PDB name of the override, or the dtor's own vtable), 13 `callers` (CANON's majority caller spelling was a
stand-in; the pass used PDB + Leaf / binding), 4 `pdb`: CThing / CTCBase Peek* vs Get* (the F2 PDB-order accessor run
decides: the const Peek and the non-const Get are adjacent bodies) and 0x9E1DA0 -- CANON's GFVXToAX (chunk294) is wrong:
the body is VirtualWidth * x / width = actual->virtual = GFAXToVX, and the landed 0x9E1E10 is GFVXToAX (Ego_r order
GFAXToVX before GFVXToAX). CANON.tsv should take the namepass names for these 136 rows.

## NG1 — NGameText::CDataBank::GetPTextEntry 0x9C9070 / NGameText::CTextEntry::GetText 0x9C7A40 (gen/ng.py; 24 rows)
FableWin/Ego_d: `?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z`;
0x9C7A40 returns the CWideString at +0x28 = Ego_r CTextEntry::Text (CGroupEntry has no text), so it is
`?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ`. Callers modelled the entry as `CGroupEntry` / `GetPGroupEntry`,
outside the namespace. ng.py renames, keys struct -> class (bases public), wraps in `namespace NGameText` (+ using-decl),
and the pointer holds `const CTextEntry`. That made the row-local release helpers const-incorrect (C2662, 18 rows);
gen/ngconst.py makes them const as retail's are (PDB `?Release@CIVCountedPointeeBase@@QBEJXZ`, `?Delete@...@@UBEXXZ`:
counter `mutable`, DecRef / Release / ReleaseTemporary / Delete const; no code change). gen/ng2fix.py: 7 rows famcheck
rejected get hand recipes toward PDB spellings (row-local `CIVTextPointer` / non-template `CIVCountedPointer` /
`CCountedPointer<CTextEntry>` -> the template; GetTextEntryFromGroup `const CTextEntry* (..., bool&, long*, bool) const`;
GetTextInfo `const NGameText::CDataBankTextEntry*`; LookupValue const). Applied ng1 1 + ng1c 16 + ng2 7 = 24, all
RELOCATION_MATCH PASS; bindings -9/+3. Not attempted (bails): 0x47EEE4 (other accessor names), 0x639BA0 / 0x66BA80 /
0x892850 (define both entry classes), 0x65C781 (no local CTextEntry); 0x594FDB / 0x891940 / 0x891A00 were claimed by
sweep2.
  NG3/NG4 (gen/ng3fix.py hand recipes + ng.py rerun): 0x639BA0 / 0x892850 (both entry classes modelled) / 0x66BA80
  (text entry aliased as `CGroupEntry : CTextEntry {}`) and 0x594FDB / 0x891940 / 0x891A00 (sweep2-claimed): applied 6.
  CDataBank now derives from `CBankFile` where GetEntryDataType 0x9CC210 is called (FableWin
  `?GetEntryDataType@CBankFile@@QBEKK@Z` -- callers reach it through the base, as retail), GetPGroupEntry 0x9C8890 returns
  `CIVCountedPointer<const NGameText::CGroupEntry>` (PDB). 0x594FDB's `GetPGroup`/`CGroup` and `GetTagFromCRC` became
  GetPGroupEntry/CGroupEntry and LookupValue (0x9CC410's one descriptive name). Remaining: 0x47EEE4 (CDefPtr / CTempPtr
  model with GetBankHandle/GetEntry/GetGroup names: a bespoke rewrite of four callees, not attempted), 0x65C781 (its
  0x9CC410 call takes a `const char*` -- the model of that callee is unresolved, so the bank class keeps its name).

## GTS1 / GTS2 — NGameText::CDataBank::GetTextBySymbol 0x9C95E0 (gen/gts.py; 36 + 3 + own row)
FableWin `?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z` (const member, returns
`const CWideString`). 9 caller spellings (global CDataBank, QAE, `?AV` return, CDataBank_Retail, `Sub::GetText`,
`SaveSub::Format`) -> 1. gts.py moves the local CDataBank into NGameText and rewrites the declaration (36 rows); gts2 hand
recipes: 0x406A80 (`SaveSub::Format(CWideString*)` was the bank call; its "CWideString" temp is really
CCharString(const char*, long) 0x99EBF0 / ~CCharString 0x99EAE0 and the fallback is CWideString(const unsigned short*)
0x99B6B0 -- all three now bind their canonical names), 0x891340 (CDataBank_Retail), 0x8913A0 (`Sub::GetText`), and the
own row 0x9C95E0 (`const CWideString` return; CDataBank : CBankFile so GetEntryDataType binds CBankFile's name). All
RELOCATION_MATCH PASS.

## SB1..SB3 — CScriptBase is global (gen/sb.py; 86 + 20 + 17 rows)
FableWin: `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` 0xCB7940, `?AddSpawnedFunction@CScriptBase@@QAEXPAVCSpawned
FuncBase@@ABVCCharString@@@Z` 0xCB7E50, `?AddEntityScriptBinding@CScriptBase@@QAEXPAVCEntityScriptBindingBase@@@Z`
0xCB8230, `?PostAddScriptedEntities@CScriptBase@@QAEXXZ` 0xCB8930, `?SetPredicate@CScriptBase@@QAEXABVCPredicateBase@
NScript@@@Z` 0xCB7920 -- CScriptBase, CSpawnedFuncBase, CEntityScriptBindingBase are global classes; the quest scripts
and CPredicateBase are NScript. 106 rows had put CScriptBase inside `namespace NScript` (the NScript sweep over-applied
the namespace to the base): sb.py closes and reopens the namespace around the base (and its parameter classes), keys
them `class`, qualifies NScript names the moved body uses. sb2: CEntityScriptBindingBase `struct` -> `class` (PDB `V`);
sb3: rows whose base was already global but whose parameter classes were `struct` (CEntityScriptBindingBase,
NScript::CPredicateBase). Rows that sweep2's g1 had rejected were staged from their g1 version so both fixes land.
Before: 95 / 30 / 20 / 20 caller rows spelled 0xCB7940 / 0xCB7E50 / 0xCB8230 / 0xCB8930 as `CScriptBase@NScript`. After:
0xCB7940 306 of 332 caller rows canonical, 0xCB7E50 224/242, 0xCB8230 and 0xCB8930 32/32, 0xCB7920 7/11.
Remaining (bespoke, sources kept): 26 rows call IsActiveThreadTerminating / Add(spawned func) as a member of the
derived script class (`CHeroBoastsScript::IsActiveThreadTerminating() const`, `CQ_AmbushTradersScript::Add(CSpawnedFunc
Ambush*)`, `CScriptBase_Retail`, `@Q_Sunny_cb7940@4`), 0xE0E2E0 (`Add`), and 4 rows (0xEEA460 / 0xEEA7C0 / 0xEEB7B0 /
0xEEC890) whose SetPredicate takes their local non-polymorphic `CIsThingConscious&` (making it the PDB
`NScript::CIsThingConscious : CPredicateBase` changes the predicate's dtor/vtable model -- not attempted).

## S2 — second family sweep over rows the first runs never saw (tools/need.py + tools/sweep2.sh)
Disjoint claims (bs1 rows seeded), per family: candidates / staged / compiled / graded MATCH / applied:
crt 200/200/200/200/200, fm 3567/77/59/59/58 (3490 candidates are free/static callees fm.py does not handle: "target is
static" / "target is free" / "no this parameter"), dv 459/121/121/121/118, f5 170/150/150/150/150, f4 107/79/64/63/59,
g1 733/481/320/318/234 (86 NONCANON: CScriptBase-in-NScript rows -> SB1), f3 5/3/3/3/3, o1 36/29/27/27/24, n1 2/0,
ctc 2/0. Applied 846 rows; bindings updated per tag.
  NG5 / OWN13: 0x47EEE4 (CDefPtr / CTempPtr model: GetEntry / GetGroup / GetBankHandle -> GetPTextEntry / GetPGroupEntry /
  GetText on NGameText::CDataBank : CBankFile, one `CIVCountedPointer` template), 0x4B1BA0 (GetTextInfo returns const),
  0x6E2910 (GetTextEntryFromGroup `(..., bool&, long*, bool) const`) applied; own row 0x9C7D00 renamed
  CDataBank::GetTextInfo -> NGameText::CDataBank::GetTextInfo (FableWin `?GetTextInfo@CDataBank@NGameText@@QBEPBV
  CDataBankTextEntry@2@K@Z`; source/test paths NGameTextCDataBank_GetTextInfo_009c7d00). NGameText bank targets
  0x9C9070 / 0x9C7A40 / 0x9C8890 / 0x9C95E0 / 0x9C7D00 / 0x9C9280 / 0x9CC210 now have one spelling each over all good
  rows except 0x65C781 (above).

## CRT3 / CRT5 — collected (gen/crt.py)
crt3 (932 staged, 863 PASS + 69 test FAIL): 380 of its rows were still byte-identical to the baseline source and were
applied from crt3 (379 + 1 test-fixed). The other 493 had been changed by later phases after crt3 was staged, so they
were restaged from the current source as crt5 (492 staged, 483 PASS + 9 test-fixed by gen/tfix_crt.py, all applied).
59 crt3 rows are NOT applied: their behaviour tests mock the `free` stand-in, and the static CRT's free cannot be
replaced in the test link (tfix_crt reports them; a hand test rewrite is needed).

## FINAL COUNTS (2026-10-26 ~21:30)
- Rows changed by the pass: 6,644 (finalcheck: every one proven by a MATCH / RELOCATION_MATCH + PASS grade of its
  current source; 0 left to regrade). Catalog renames (own rows): 218 (RENAMES.tsv). Changed-file log: CHANGED_FILES.txt.
- Bindings table (bindupd-maintained, keeps first-recorder rows): 2,291 rows dropped with reasons (BINDINGS_DROP.tsv),
  1,336 added (BINDINGS_ADD.tsv). Multi-spelling targets (same counting for both): original table 4,581 targets /
  17,262 spellings -> 4,552 / 15,740. (The coordinator's 4,597 / 17,326 counts symbol/target pairs with address case.)
- Row-level census (tools/paircount.py; 45,072 rows good both before and after; BEFORE = baseline regrade, AFTER =
  finalcheck proven pairs): multi-spelling targets 5,240 -> 5,085; spellings on them 20,557 -> 17,652; row-pairs that
  bind a target under a minority spelling 28,506 -> 22,570.
- Diagnostic rebuild from proven pairs (tools/rebuild_bindings.py final --from final_pairs.tsv final_status.tsv;
  $N/final/): 36,566 symbols, 24,020 targets, 5,069 multi-spelling targets / 17,615 spellings, 310 symbol conflicts
  (CONFLICTS.tsv), 1,635 rows without a baseline grade (chunks 113-117, below). Not applied to the table.
- Labels: ghidra_out/labels_namepass_2026-10-26.tsv (16 names, two evidence sources each).
- Leaf: tools/leafwarn.py over every tag of this session reports no new `// Leaf:` mismatch.

## REMAINING — not resolved in this pass (each row keeps its current byte-matching source)
Multi-spelling targets left, highest fan-out first, with the reason (row-level counts: final_pairs.tsv):
- 0xA01B50 CBaseIntelligentPointer::GetPItem (PDB `QBEPAVCBaseObject@@XZ`): ~340 rows declare `CThing* GetPItem() const`
  (and 100+ other stand-ins). F4 rewrote the rows whose T* cast is a no-op; for the rest the PDB return type needs a
  static_cast<T*> through T's real base chain, which changes codegen where CBaseObject is not at offset 0 -- per-row
  hierarchy work, not attempted. Same family: 0xA01C10 dtor (CMember_00a01c10 / CBaseIntelligentPointerCtor stand-ins),
  0xA01B90 SetPItem, CIntelligentPointer<T> vtables 0x1238C6C / 0x1237F1C (rows modelling the template as a plain class).
- 0xBFE9BC operator delete / 0xBFEA14 free / 0xBFEB1C delete[]: the CRT family. CRT1/CRT3/CRT4/CRT5/S2 rewrote about 1,560 rows; left:
  rows crt.py bails ("void result used", same-TU stand-in bodies), and 59 rows (crt3 test FAIL) whose behaviour test mocks
  the `free` stand-in -- a static-CRT free cannot be replaced in the test link, so those tests need a hand rewrite.
- 0x40F020 / 0x4365B0 CInterfaceMap::LowerBound (QAE vs QBE overloads, ~50 spellings each): the two bodies are the
  const and non-const instantiations; rows mix them up. Needs per-row const analysis of the GetInterface caller chain.
- 0x99A300 (101 spellings; label CBaseClassNonCopyable::~CBaseClassNonCopyable): every landed class hierarchy declares its
  own `CBase@N_0099a300`-style base. Owner rule: a dtor's identity is the vtable it stores first -- this body stores none
  (inline empty dtor), so the per-row base class is each row's own; unifying needs the RTTI base chain per row (D1/hier
  handled 402 virtual cases).
- 0x487DC0 CPlayer::GetHeroThing (72), 0x49D870 CWorld::GetFrame (61), 0x449970 CPlayerManager::GetMainPlayer (48),
  0x1375550 g_framesPerSecond (48), 0x13B86A0 g_pMainGameComponent (57): G1 covered rows whose stand-in class could be
  renamed; the rest are free fastcall / global-object stand-ins (`GetHero_487dc0`, `CWorld_Retail`) that fm.py classifies
  "target is static/free" (3,490 sweep2 fm candidates).
- 0xCB7940 / 0xCB7E50 (26 + 18 rows): derived-script-class stand-ins (`CHeroBoastsScript::IsActiveThreadTerminating() const`,
  `CQ_AmbushTradersScript::Add(CSpawnedFuncAmbush*)`, `CScriptBase_Retail`, `@Q_Sunny_cb7940@4`); 0xCB7920 4 rows
  (0xEEA460 / 0xEEA7C0 / 0xEEB7B0 / 0xEEC890: local non-polymorphic CIsThingConscious& parameter).
- 0x65C781: its 0x9CC410 call passes a `const char*` (model of that callee unresolved), so its CTextBank keeps its name.
- 0x9CC410 itself keeps the descriptive NGameText::CDataBank::LookupValue (forge label says CVectorMap::LookupValue; the
  shared lookup is called by CDataBank / CLipSyncDataBank / CSpeechMap / CBankFileAsync rows -- an unproven ICF fold).
- 0x8FD288 callers 0x8FD2CA / 0x90010E / 0x900F43 / 0x8FD544 / 0x8FD769 (pair<..., CSymbolInfo> first-member destroy
  through an ICF-folded body; the class is unproven).
- 0x4115A0 rows 0x4FE630 / 0x6D41F0 (namespaced stand-in TUs), 0xA63EE0 (StringDataMethods stand-in), 0x4AFE20 /
  0x6238A0 (DIFFER once the stand-in becomes a template member), 0x859040 (anonymous-namespace hash -> NEWSPELLING).
- 0x6637D0 (anonymous namespace), GetSubDef 0x669580 / IS_THING_ALIVE (ICF templates: one body, many T), 0x407370
  (protected CUserProfileManager::ApplyValues called from free stand-ins: 22 rows), 0x7C9180 (DIFFER).
- CScriptDefinitionManager vtable 0x12C2648 (no binding records it), Leaf issues 0x904540 / 0x851890 / 0x5E4740 / 0xD9EE20,
  g1e clashes (`_s_Default` 0x1238198 vs 0x123817C), 26 crt rows "void result used", D1 real-class stand-ins,
  227 CDataInputStream rows (private-member bails), CHairCardDef / CHeroMarriageDef own symbols (header classes declare
  no members, so the free form stays).
- Treepass KEPT (556) / OS_FAMILY (180): bespoke per-caller STLport recipes, not attempted.
- Baseline regrade chunks 113-117 (1,636 rows: 0x0121xxxx Unwind funclets and 0x0122xxxx data): every compile in them
  timed out (200 s per attempt, several attempts per row), so the run was stopped; those rows have no baseline pairs, were
  never candidates for any family, and their table rows are unchanged.
- bindupd CLASH lines (symbol already recorded at another target): generic stand-in names reused across rows
  (`??_7CBase@@6B@`, `??_7CSpawnedFunc1@@6B@`, `??BCDefString@@QBE?AVCCharString@@XZ`, `_$E2`), first recorder kept.

## PROPOSALS (for the coordinator to commit; not written into the repo docs by this pass)
GOTCHAS.md one-liners:
- One address, one name: before inventing a caller-side stand-in, look the callee up in the PDB name sets -- the namespace
  (`NGameText::CDataBank`, NOT `NScript::CScriptBase`), the class-key, const-ness (`QBE`) and a const-qualified return
  (`?B`) are all part of the symbol, and `const T` pointees (`CIVCountedPointer<const X>`) need the pointee's release
  helpers const + `mutable` counter (retail's are: `?Release@CIVCountedPointeeBase@@QBEJXZ`).
- VC7.1: declare an explicit specialisation before its first use (C2908); a member-template specialisation that is
  declared but not defined in the TU and then called makes cl ICE (C1001) -- leave it undeclared, the call links to the
  other object's specialisation (same mangling).
- `struct` -> `class` in a row model needs `public:` AND `public` bases (a class's bases default to private).
- A `// comment` on a one-line `namespace X { ... }` swallows the closing `}` and cl.exe hangs instead of erroring.
- A test mock that stands in for a base destructor needs `__declspec(novtable)` or it re-stores its own vptr over the
  row's.
- Wine: a test exe can spawn `explorer.exe /desktop` that inherits vland's stdout pipe, so vland waits forever on a zombie
  test; kill that explorer PID (check its FABLE_ROOT env is yours).
- The lane's recheck copy rewrites `__thiscall` to `__fastcall` inside comments; compare graded copies to repo sources
  modulo comments.
FINDINGS_LOG.md paragraph:
2026-10-26 one-address-one-name pass (namepass). Every retail address should be called by one name catalog-wide: the
PDB name where both name sets prove it, else one descriptive name, reached the way retail reaches it (inheritance,
base-dtor chaining, member templates). Families (CCharString build, CTCBase accessor run, CScriptBase SetPredicate /
IsActiveThreadTerminating / AddSpawnedFunction (global class, not NScript), CBaseIntelligentPointer / CIntelligentPointer<T>,
CBaseObject special members, CDefClassBase::operator=, virtual base dtors, free fastcall stand-ins -> member calls,
MSVCR71 imports under CRT names, CPersistContext::Transfer<T> member template and the def Transfer cluster,
CBasicString<char>, NCameraModes::CBase, the NGameText::CDataBank bank API) were rewritten by generators in
`$S/w22/namepass/gen`, every row regraded (`vland --recheck-landed`) and kept only at MATCH / RELOCATION_MATCH + test PASS
with the canonical callee symbol in its relocation pairs. Own rows renamed: see RENAMES.tsv. Bindings table updated per
phase (BINDINGS_DROP.tsv with reasons). Unresolved items are listed in NOTES.md REMAINING (largest:
CBaseIntelligentPointer::GetPItem return type, CRT free mocks in tests, CInterfaceMap::LowerBound const overloads).
