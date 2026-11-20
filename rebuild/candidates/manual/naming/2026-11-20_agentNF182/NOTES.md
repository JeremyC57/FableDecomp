# NF182 notes (round 10, ctor/dtor slice 2: CI..CZ + PK)

## Rule slips
- One early Bash call exited non-zero (a `cat` of a catalog TestSource path that does not exist, 0x004D62D1). No effect on files.

## Per entry
- q3969 0x007E3420: self row right (it models CTCFlashOnHit locally although engine/CTCFlashOnHit.h exists: pre-existing debt, untouched).
  Binder ??_G 0x004D62D1 re-authored onto engine/CTCFlashOnHit.h with the Account B 0x00BB1740 anonymous-namespace scaffolding
  (`// Fidelity: scaffolding, not retail`); new self leaf ??_GCTCFlashOnHit@@UAEPAXI@Z (RENAMES s1; no landed row binds the old leaf).
  Pick-any check: the object newly defines the COMDATs ??0CTCFlashOnHit@@QAE@AAVCThing@@@Z (q.py vs 0x004D6292: RELOCATION_MATCH,
  byte-identical to the owner), ??_7CTCFlashOnHit@@6B@ (data) and the ??_G itself. VC7.1 never expanded the inline constructor (stack
  object, `new`, with and without the size pragma: always emitted), like retail's own Alloc 0x004D94AB which calls 0x004D6292.
  RESOLVED: 0x004D6292 (the ctor owner, which defined it strongly from a local model: the nf140 case) is ported in the same hand-back
  with the same opt-in + scaffolding, so both objects now emit identical pick-any COMDATs (ctor, vtable, ??_G); its HEAD ??_G copy had a
  different body (implicit dtor). The NF20 precedent pair 0x004DC1D8 / 0x004DAB44 still has the strong-vs-COMDAT shape at HEAD (debt).
- q4021 0x006A5D50: binder ??_G 0x005297D0 respelled IN PLACE (its existing local CThingPhysical model: `Destroy()` stand-in ->
  `virtual ~CThingPhysical()` called by qualified name). A header port is not possible as a single row: the ??_G is emitted only by a
  TU that compiles a CThingPhysical constructor / destructor, and retail's TU-mate is the out-of-line constructor 0x005296B0 (own row).
  Left: its self leaf stand-in and ?FreeSelf@CThingPhysical@@QAEXXZ for 0x004C73B0 (PDB ??3CThing@@KIXPAX@Z, Ego_r 0x004C9050 +
  FableWin 0x01CF0E00; 0x004C73B0 has 9 stand-in spellings across landed rows -- a separate family, NAME_FIXES).
- q4048 / q4050 / q4051 / q4101: the ??_G binders 0x007CF390 / 0x007ADA80 / 0x00A628B0 / 0x00670C50 respelled IN PLACE (same recipe
  as q4021, script work/dtorfix.py). Each class's constructor is out of line with its own row (0x007CF110, 0x007ABB80, 0x00A62120,
  0x00670050), so no single-row ??_G header port exists. The old stand-in callee spellings (?Dtor@X / X_dtor fastcall) had no other
  binder (REFS: one referencer each). q4101's list of seven other ??_G callers concerns other classes' destructors (other targets).
- q4284 / q4546 (CWideString dtor / ctor, 920 / 331 referencing rows): open (not started), census only (CENSUS_CWideString.txt,
  method: REFS rows x source grep of each stand-in token, the nf120 approach; 520 / 150 rows define a global-namespace local
  `class CWideString` and bind the PDB spelling already). The wrong binders are the single-row stand-ins listed in APPLIED plus the
  namespaced NGameText / N006bebf0 / N009ac5d0 / AFileSupport / ExternalRetry_00999620 local models, which are ports onto fable_string.h
  (not in-place respells: nf95b). Which NGameText rows really bind the namespaced dtor needs the gate.py exe scan + compile.
- q4302 0x00A04410: 0x009E45B0 ported onto engine/CMaintainedEvent.h (new opt-in, declaration of the ctor); 0x004473A0 respelled in
  place (its local CSavedInputEvent model keeps its token; member stand-in -> shared CInputEvent). 0x004473A0 newly defines the implicit
  ??1CSavedInputEvent@@QAE@XZ (no address owns it: retail inlines it) -- flagged for the coordinator's symbol check.
- q4551: done-already (reloc_bindings witness 0x00BC7E90 for ??0CVertexShaderNativeView is stale; HEAD binds the PDB spelling).
- q4554 0x00AC1EC0: 0x00BF3290 base onto engine/CIndexBuffer.h; the class is really CEngineDynamic3DResource<CIndexBuffer,3> (RTTI); its
  self leaf ??0CEngineDynamic3DResource@@QAE@XZ stays a non-template stand-in (self rename not attempted: one source, RTTI only).
- q4630 0x00CBE09A: leaf-only self rename (PAV -> AAV, two PDBs) on a referenced row: every referencer already binds AAV (the shared
  header's inline ctor; 0x00CBF6DC checked by compile). Its test still models the class standalone (pre-existing shape; rewritten on the
  header it would need the 917-slot CGameScriptInterfaceBase): only the parameter changed to a reference.
- q4568 rejected (address placeholder name); q4571 rejected + deferred (GameCODA, NF175); q4620 / q4621 open (not started; census: 0x00DDD030 needs a re-author on the shared script-object headers).
- q4891 / q4892 (libpk CSoundEvent): CSoundEvent HAS a shared header (libpk/LHAudioEventPosition.h, 0 landed includers), so local
  stand-ins cannot be renamed to it. 0x00C239B0 ported in a draft (work/00c239b0.cpp, RELOCATION_MATCH) and HELD. DECISION NEEDED: the
  draft moves 0x00C22840's binder from ?InitEntry@CEventSubscription_00c239b0@@QAEXPAUSEntry_00c239b0@@@Z to a new stand-in spelling
  (...PAVCSoundEvent@@@Z); 0x00C22840's self row is PKCFrallocStatic<CUnprovenFrallocElement_00C22840>::Return (a placeholder). Either
  accept the stand-in-to-stand-in move (count unchanged) or settle 0x00C22840 first (the class is really PKCFrallocStatic<CSoundEvent>,
  per the libpk source path string in 0x00C239B0). 0x00C21BA0 defines ??0CScriptModuleBase@@QAE@XZ, which 0x005ED190 binds for
  0x005ED070: a one-name-two-addresses conflict to queue.

## Flags lines / funclets / descriptive names
- No `// Flags:` line added or changed. No changed row is included by an EH funclet row (grep of rebuild/src/compiled for each changed
  row's file name); no self rename moved a parent file that funclets include.
- No new descriptive name was introduced. RENAMES s1 0x004D62D1 is a compiler-generated ??_G; 0x00CBE09A is a PDB spelling.

## Rule slips
- Two Bash calls exited non-zero (a `cat` of a missing file as the last command). No effect on files.

## Not started (APPLIED `open`; handed back for context)
q4900 q4902 q5031 q5057 q5081 q5189 q5192 q5193 q5209 q6161 q6196 q6204 q6224 q6930 q6932 q11796.
Census notes for the next pass: q5081 0x004A6AB0 binder 0x004AE8D0 is a ??_G `Dtor()` stand-in row (the q4021 in-place recipe applies);
q5209 binder 0x00723760 binds the C3DVector form; q6930 parent 0x00CB1890 + funclet 0x012173E0 spell CPatchNodeList_00cb1890;
q11796 rows left 0x009E45D0 (lock now released) and 0x009F61C0 (identity-blocked). q6224: NF123 already spent 2 forms on 0x00DFDE30.

## Decisions for the coordinator
1. 0x00C22840 InitEntry spelling (q4891 draft 0x00C239B0, held): see the q4891 / q4892 paragraph.
2. ??0CScriptModuleBase@@QAE@XZ is defined by 0x00C21BA0 (self, no Leaf line) and bound for 0x005ED070 by 0x005ED190: one name, two
   addresses (queue it; 0x00C21BA0 is really CSoundEvent's ctor).
3. 0x004473A0 newly defines the implicit ??1CSavedInputEvent@@QAE@XZ of its (HEAD) local model of a header class: no address owns it,
   no landed includer of engine/CSavedInputEvent.h emits it, and it is referenced only by its own .debug$F. Land, or hold 0x004473A0.
4. q4568: the claim is rejected, but the three-spelling split on 0x00C1EB10 (self ?Op@C@@QAEXXZ; 0x00C1D8B0 the placeholder;
   0x00C1E180 ??1CUIComponentBase_00c1e180) remains open.

## Pick-any checks (new or changed COMDAT copies of functions with their own row)
- 0x004D62D1 / 0x004D6292: ??0CTCFlashOnHit@@QAE@AAVCThing@@@Z RELOCATION_MATCH vs 0x004D6292; ??_GCTCFlashOnHit@@UAEPAXI@Z RELOCATION_MATCH
  vs 0x004D62D1 (both objects).
- 0x009E45B0: ??_GCMaintainedEvent@@UAEPAXI@Z RELOCATION_MATCH vs 0x009E45F0 (HEAD copy existed; body now runs ~CInputEvent like retail).
- 0x004473A0: ??0CSavedInputEvent@@QAE@XZ RELOCATION_MATCH vs 0x00447BB0 (HEAD copy); ??_GCGamePlayerInterface@@UAEPAXI@Z RELOCATION_MATCH
  vs 0x00449420 (HEAD copy differed: it expanded the implicit dtor; the row now declares ~CGamePlayerInterface, Ego_r + FableWin
  ??1CGamePlayerInterface@@UAE@XZ).
- 0x00BF3290: ??_GCIndexBuffer@@UAEPAXI@Z RELOCATION_MATCH vs 0x00AC2070; ??_GCMovableResource@@UAEPAXI@Z RELOCATION_MATCH vs 0x009D5110.
- 0x00CBE09A: its own leaf (pick-any, as every includer emits it); header-inline ??1CWideScreenMagicPauseEntities has no row.
- 0x00B260B0, 0x005297D0, 0x007CF390, 0x007ADA80, 0x00A628B0, 0x00670C50: defined-symbol set identical to the HEAD object (nm).
