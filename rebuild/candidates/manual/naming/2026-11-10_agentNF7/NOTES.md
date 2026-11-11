# NF7 notes (agentNF7, 2026-11-10): the large binder groups

Hand-back:
- `rows/`: 132 row sources and 26 tests.
- `STATUS.tsv`: one line per target, with the rows changed, each row's grade, and the binders left.
- `RENAMES.tsv` and `NAME_FIXES.tsv`: 0x01228CA0.
- `hdr/`: one opt-in, see `hdr/README.md`.
- `work/`: scripts and logs.

How every row was checked:
- All 132 grade RELOCATION_MATCH with `FABLE_HDR_OVERLAY=$S/w22/agentNF7/hdr`, one compile at a time.
- Each target binding is OK, with no HARD reloc_identity hit and no new BAD binding against the NF6 scan baseline.
- Each behaviour test passes under g++ -std=c++11 (64-bit) and VC7.1 /Od, or fails the same way on the landed source (PREFAIL, listed below).

No BUSY address and nothing of agent1022 was touched.

## Method
I reused the NF6 tool chain, copied to `work/` and pointed at agentNF7:
- `apply.py` is the edit, grade, keep-or-revert loop.
- `chk.py`, `scan.py`, `find_binders.py` and `loc.py` are unchanged in function.

`apply.py` changes:
- The PREFAIL comparison normalises paths and line numbers.
- `NOTESTEDIT=1` skips test edits.
- A changed BAD spelling of a target that was already BAD is reported as BADCHANGED instead of rejected.

Binders came from the NF6 compile scan (`$S/w22/agentNF6/work/scan.out`), cross-checked against a retail-byte scan (`work/binders.txt`):
- For the small and mid targets the scan is near-complete: PClass 150 of 151, GetDef 80 of 84, GetSubDef 44 of 45.
- For the big shared symbols the "left" lists in STATUS.tsv are lower bounds, as in NF6.

## Per group (rows changed / BAD binders left)

### 0x013B89FC `CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass`: 71 changed, 1 left
- **The fix.** The private stand-in global (`extern T* g_x;`) is replaced by `#include "engine/CTBaseSingletonNoDefaultAlloc.h"`.
  - Uses become `CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get()`, the shared public inline `Get()` over the private `PClass`.
  - Where the row keeps a private layout struct (`QuestManagerNative`, `CQuestMgr`, ...), the reference is `reinterpret_cast<T*>(&...::Get())`. Those layouts are still row debt (see below).
  - Retail's own spelling is `CQuestManager::Get()`. It needs the FABLE_RETAIL_CQUESTMANAGER branch, which most rows do not use.
- **Rows with special edits:**
  - 0x004A6550 PostInit uses the new opt-in `FABLE_RETAIL_SINGLETON_SET`: FableWin `?Set@...@@SIXPAVCQuestManager@@@Z`, body `PClass.Reset`.
  - 0x005FD410 had a local template with a public PClass (`2V`).
  - 0x004FEEC0: the local CCountedPointer was replaced by engine/CCountedPointer.h (`Clear` became `ReleaseData`), and CQuestManager is now class-keyed.
  - 0x008D1890 / 1B80 / 1DD0 / 21B0: a duplicate local CCPPointerInfo was replaced by engine/CCountedPointer.h.
- **0x01228CA0 self:** the row was "Release_013b89fc" over a `CCountedPointerStatic` stand-in. It is the
  ``dynamic atexit destructor for 'CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass'`` (Ego_r.pdb name set plus the body).
  - Rewritten as the static member's definition plus an explicit instantiation; VC7.1 emits the destructor as `_$E2`, which is the row's `// Leaf:`.
  - RELOCATION_MATCH, with a new portable test. See RENAMES.tsv and NAME_FIXES.tsv.
  - Coordinator check: line 1 has `::` with no `(`, so look at finish.py's line-1 parse.
  - The row says "one of 25 bodies": the other PClass atexit destructors are likely the same shape.
- **Test edits.** Where a test links the row object and defined the old global, it now models the private static with a test-only `TestPClass()` accessor and an explicit instantiation.
- **Left:** 0x00892D80. Its local `CCountedPointer` is a different (def-pointer) model, which clashes with engine/CCountedPointer.h. It needs a real rewrite.

### 0x0042AF3C `CDefinitionManager::GetDef<CThingBaseDef>`: 12 changed, 55 left
- **Changed:** 12 rows that already use the shared CDefinitionManager template but typed the out handle `CDefPointer<const CDefClassBase>`. The handle is retyped `CDefPointer<const CThingBaseDef>` (engine/CThingBaseDef.h).
  - The rows: 005B48A2 005B76D1 005B8743 005C47D5 005C4F42 005C574E 005C5F11 005C66B2 005C8176 005C8459 005C9A95 005CACB9.
- **Left, real rewrite:**
  - 0x0041E6D3: GetDef is called at two addresses, 0x0042AF3C and 0x0042B144 (a second instantiation). Each call site's T has to be matched to its retail call; the blanket retype was HARD.
  - 0x005D9A10 / 0x005D9AA0 / 0x0072F170: a local CDefClassBase.
  - About 50 more rows: local CDefinitionManager / CDefPointer models with same-TU stand-in GetDef bodies and many wrong signatures (`QAE ... CDefPointer<CDefClassBase>*`, `CDefPointerSlot`, `CThingBaseDefPointer`, ...). Each needs the shared FABLE_RETAIL_CDEFINITIONMANAGER chain (CResourceList, DEF_CLASS_BASE, GETDEF_GLOBAL_BODY).

### 0x0042B0A2 `GetSubDef<CAppearanceDef>`: 0 changed, 38 left
- All 38 are local CThingBaseDef / CDefClassBase models with same-TU stand-in bodies (`FindDef_*` stand-in callees).
- Moving them needs the shared CDefClassBase plus FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, per row.
- Several rows call 0x0042B0A2 with T = CAnimatingObjectDef / CVillageStateDef / CMeleeTrapDef. If the queue claim holds (one address, one T), those rows' types are wrong too.

### 0x006869D0 / 0x004C7990 GetDefinitionManager / PeekDefinitionManager: 0 changed, 56 / 39 left
- 92 of the 95 binder rows model CTCBase and/or CThing locally, with a `CDefinitionManager*` return.
- The PDB signature returns `const CGameDefinitionManager&` (protected on CTCBase), so each call site changes (`->` to `.`, const GetDef). Real rewrites.

### CIntelligentPointer family (01238C6C, 01237F1C, 00A01B50, 00A01B90, 00A01C10, 00A01B10): 30 rows changed
| Target | Changed | Left |
|---|---|---|
| 01238C6C | 8 | 44 |
| 01237F1C | 5 | 24 |
| A01B50 | 18 | 20 |
| A01B90 | 2 | 19 |
| A01C10 | 0 | 13 |
| A01B10 | 0 | 9 |

- **Wrapper structs** (`struct CIntelligentPointerThing : CBaseIntelligentPointer {...}` and similar): replaced by the shared `CIntelligentPointer<T>`.
  - Opt-ins `_COPY`, `_ASSIGN` and `_CONVERT_MUTABLE`. The last is the FableWin converting ctor `??$?0VCThing@@@?$CIntelligentPointer@$$CBVCThing@@@@QAE@AAV?$CIntelligentPointer@VCThing@@@@@Z`.
  - Kept: 00741EE0 00915890 0092C8A0 009574D0 00958000 0090EEC0 0097EF60 00969CB0 (and 008BAE10 008BAF20 008C99C0 00917130 009473E0 from the base move below).
- **Local `CBaseIntelligentPointer` base** (GetPItem returning `CThing*` and similar): replaced by the shared header. Results are `reinterpret_cast` to the row's local type; SetPItem arguments are cast to `CBaseObject*`.
  - Kept 22 rows, including 006C0160 00773950 007BC8FC 007BE0F9 008663D0 0087EBD0 0087ED70 008BAE10 008BAF20 008C2550 008C2980 008C29F0 008C2A80 008C99C0 00924880 00924930 0093DFC0 00947A30 009647C0 0096EB40 (STATUS.tsv has the full per-target lists).
- **Left, and why:**
  - The shared `CIntelligentPointer<T>` does `static_cast<const CBaseObject*>(T*)`, so T must be a complete CBaseObject. Many rows have a forward-only or local CThing (C2440); including engine/CThing.h then collides with their local CCharString / CCharStringData.
  - Local CBaseIntelligentPointer layouts that size-assert (C2118).
  - Wrappers with a by-value `operator=` (DIFFER: 008B97E0 008B9970 008B9B00 008BA220 008BA7F0 008CD5A0 008CD730, about 42 bytes short, which looks like a temporary copy and dtor the shared const& ASSIGN does not make).
  - Wrapper names I could not type (CIntelligentPointerThing2, CIntelligentPointer_CThingCreatureBase, CThingPointer, ...).
  - Call sites passing a `CBaseIntelligentPointer` into PDB `CIntelligentPointer<const CThing>` parameters.
- **0x00A01C10 / 0x00A01B10 binders** mostly use invented classes and namespaces (`CBaseIntelligentPointerCtor`, `Maze*PointerBase`, `X_006c8780`).
- 0x0097EF60: AIAction_MoveToThingProximity and the row's own AIAction_ProjectileAttackAndStrafeToThingProximity are now protected, as both PDBs (IAE).
  - So 0x0097E380 binds the PDB spelling there.
  - The row's own symbol changes QAE to IAE. The name is unchanged, so there is no RENAMES entry.
- 0x0090EEC0 and 0x00969CB0 still bind 0x0097E380 BAD, but closer: the parameter is now the real `CIntelligentPointer<const CThing>`. The return type, access and parameters are still the local ones.

### 0x004365B0 / 0x0040F020 CVectorMap::LowerBound: 16 rows changed (14 / 6 per target), 31 / 23 left
- **The fix.** The local `struct CInterfaceMap { SEntry{type,component}; first; last; LowerBound(const long*) }` becomes `typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> > CInterfaceMap`. The typedef is landed precedent (0x00924930).
  - `->type` / `->component` become `first` / `second`; `->first` / `->last` become `begin()` / `end()`.
  - The key is `reinterpret_cast<const ETCInterfaceType&>` of the long. Retail passes the address of the same stack slot.
  - A const receiver is forced where the row binds only the const 0x004365B0.
  - Same-TU stand-in LowerBound bodies are dropped: the shared header defines the noinline template bodies.
  - The member padding after an 8-byte local map is widened to the 0x10-byte CVectorMap.
- **Kept rows:**
  - 004365B0: 0068EB20 006D7960 006E9F60 007B2D50 007ED560 007EE8F0 0091DB50 00930EE0 0093C830 0093DFC0 00942B30 00944730 0094C900 0094D020.
  - 0040F020: 00773950 007ED560 007EE8F0 0093C830 00944730 0094D020.
- **Left, and why:**
  - A local `std::bitset` stand-in that is an empty class, so the real 36-byte bitset breaks the row's layout (C2953 / C2955).
  - `CInterfaceMapB` and other differently named maps.
  - Maps with extra members (Find / LowerBound2 / LowerBoundInline).
  - Rows with no local map at all: the binding comes through a different model, often an invented namespace.

### 0x0062E900 ~CCombatActionBase (and 0x006644F0 SetCurrentAction): 6 rows changed
- 0x007DDDA0: the local CCreatureActionBase / CCombatActionBase (non-virtual dtor) replaced by engine/CCombatActionBase.h (FABLE_RETAIL_CCREATUREACTIONBASE).
- 0x0072C930 / 0x0072C9C0: the action class derived the flat generated header and declared its own dtor at 0x0062E900. It now derives the shared CCombatActionBase (destructor identity = the vtable stored first), padded to the generated header's size.
- 0x008A8E40 / 0x008A8EB0 / 0x008E08C0: the stand-in CCombatActionBase without a base replaced by the shared header, and SetCurrentAction respelled `(const CCreatureActionBase&)`. Both 0x006644F0 and 0x0062E900 bind OK.
- **Left (27 dtor binders, 21 of the 24 SetCurrentAction rows):** the stand-ins carry members (PCreature, PTarget, FinishAction, local CCreatureActionBase / CBaseIntelligentPointer) that need the FULL_MEMBERS / MELEE opt-ins. Some rows also clash with fable_string.h through their local CCharString (00667310, 007F04C0, 008DF510).

### Not reached (all need per-row rewrites, examined only)
- 0x0129AAF4 NullChar: every binder is a local CCharString model whose `GetString()` / `GetAsCharArray()` selects a private empty literal. The fix is shared fable_string.h plus `GetAsCharArray()`.
- 0x0099EAE0 / 0x0099EBF0 / 0x0099EC30: one-off stand-ins (thunks, `Str`, `InitGuard`, invented namespaces).
- 0x0097DB70 / 0x0097E200 / 0x008FCF40 / 0x0097E380: local AI-state-group models (`...Methods`, `CQueenNative`, `CAIStateGroupBaseA`). They need engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS / _AIACTIONS / _PERFORMACTION).
- 0x004C9D60 AddTC: eight different signatures in local CThing models.
- 0x009AD410: adapters and namespaces.
- 0x0042D9E0: `CBase_0042d9e0` / CPSCBase ctors.
- 0x00486024: needs std::vector.
- 0x0073A6E0: the nested CModeBase.

## PREFAIL tests (fail the same way on the landed source; not counted against the change)
004C7E50 004C7E70 006C0160 006E7410 006E7510 006E7530 007DDDA0 007ED560 008913F0 00891490 008914E0 00891510 00891520 00891540
008915B0 008915E0 008917F0 00891820 00891860 00891870 00891880 00892E80 00892EA0 00892EE0 00892F40 00892F50 00892F60 00892F70
00892F80 00893030 008CFF40 0091DB50 00930EE0.

The usual cause is that g++ cannot build the landed test (`__fastcall`, a test that needs the row object, or `_snprintf`). VC7.1 /Od passes, or prints the test's own non-standard OK line, as on the landed source.

## Row debt kept (pre-existing, noted)
- PClass rows that read the quest manager through a private layout struct now `reinterpret_cast` the real singleton's object. The layout structs themselves are row debt.
- Rows with a local CThing still `reinterpret_cast` the `CBaseObject*` from GetPItem to that local CThing. Local CThing / CTCBase / CDefPointer models stay where they were.
