# Chunk 750 notes (Linux Wine lane, cl 13.10.3077)

14 / 14 rows matched (RELOCATION_MATCH + behaviour PASS), read-only vland: `WINS: 14` (vland_result.txt), no
SHARED_HEADER / NOT_GENUINE / RELOC_IDENTITY rejections. No asm, no helper, no pragma, no `volatile` in any row.

## Results

| address | name (exact symbol) | grade | attempts | identity evidence |
|---|---|---|---|---|
| 0x00558FD0 | NUISystem::CDraggableInto::~CDraggableInto `??1CDraggableInto@NUISystem@@UAE@XZ` | RELOCATION_MATCH | 1 | stores the three CDraggableInto vtables 0x0124B2D4/+4 0x0124B2B0/+0x18 0x0124B2A8; Ego_r + FableWin `??1`; same body inlined in the landed ??_G 0x00559040 |
| 0x00559020 | [thunk] `??_ECDraggableInto@NUISystem@@W3AEPAXI@Z` (adjustor{4}) | RELOCATION_MATCH | 1 | slot 0 of 0x0124B2B0 (the catalog's vfunc_0); `sub ecx,4 ; jmp 0x00559040` |
| 0x00559030 | [thunk] `??_ECDraggableInto@NUISystem@@WBI@AEPAXI@Z` (adjustor{24}) | RELOCATION_MATCH | 1 | slot 0 of 0x0124B2A8; `sub ecx,18h ; jmp 0x00559040` |
| 0x0077B290 | `??1CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAE@XZ` | RELOCATION_MATCH | 1 | vtables 0x0126ACDC / 0x0126ACC0 |
| 0x0077B9F0 | `??1CTCGraphicAppearanceStaticMesh@NGraphicAppearance@@UAE@XZ` | RELOCATION_MATCH | 1 | vtables 0x0126AD6C / 0x0126AD50 (catalog label "~CTCGraphicAppearanceAnimatedMesh" propagated) |
| 0x0077BE80 | `??1CTCGraphicAppearanceStaticMeshFadeOutOnProximity@NGraphicAppearance@@UAE@XZ` | RELOCATION_MATCH | 1 | vtables 0x0126AE04 / 0x0126ADEC (propagated label) |
| 0x0077C440 | `??1CTCGraphicAppearanceSprite@NGraphicAppearance@@UAE@XZ` | RELOCATION_MATCH | 1 | vtables 0x0126AE94 / 0x0126AE78 (propagated label) |
| 0x008FFBEE | `??0CAIStateGroup_KidPlayHopscotch@@QAE@XZ` | RELOCATION_MATCH | 1 | own vtable 0x0128BB90; factory 0x00902448 `new(0x68)` = Ego_r size; member CIntelligentPointer<CThing> +0x15 |
| 0x008FFD84 | `??0CAIStateGroup_WorkSawingWood@@QAE@XZ` | RELOCATION_MATCH | 1 | own vtable 0x0128BCBC; factory 0x009024D2 `new(0x48)`; CIntelligentPointer<CThingObject> (vtable 0x0125A858) +0x15 |
| 0x008FFDCE | `??0CAIStateGroup_WorkHammerAnvil@@QAE@XZ` | RELOCATION_MATCH | 1 | own vtable 0x0128BCF8; factory 0x009024E8 `new(0x4C)` |
| 0x00C207F0 | CSoundEvent::~CSoundEvent `??1CSoundEvent@@QAE@XZ` | RELOCATION_MATCH | 2 | eh vector ctor iterator 0x00C23A10 (ctor 0x00C21BA0, size 0xB4) and vector deleting dtor 0x00C22C30 |
| 0x00C20810 | `?StopListening@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z` | RELOCATION_MATCH | 1 | bound spelling (caller ResetLevel 0x00C20DF0); clears +0xB0, Remove from +0x1F4 |
| 0x00C21040 | `?Describe@CSoundEvent@@QAE?AVCString@@PAUCSoundEventManager@@@Z` | RELOCATION_MATCH | 3 | bound spelling (TraceEvents 0x00C12720); strings; jump-table case/string pairing checked by hand |
| 0x00C21780 | `?Play@CSoundEvent@@QAEXPAUCSoundEventManager@@@Z` | RELOCATION_MATCH | 8 | bound by Update 0x00C21A50 |

Row-bound checks: the three 36-byte "CAIStateGroupBehaviourBase" rows at odd addresses are real functions (constructors) of the
AI state-group TU, which retail built without function alignment (`pop ecx` cleanups, bodies packed back to back
0x008FFBA4 / 0x008FFBC8 / 0x008FFBEE / 0x008FFC12 ...); bounds are right (each ends at its `ret`). The CDraggableInto
"vfunc_0" rows are compiler adjustor thunks. Describe's 684 bytes = body to `ret 8` + `mov edi,edi` + the 5-entry jump table.
The 55-byte appearance destructors and the CDraggableInto destructor have no callers (their ??_G expand the same body).

## Header overlay (hdr/engine/), hdrcheck2: default=same for both
- `engine/NGraphicAppearance_CTCGraphicAppearance.h`: `FABLE_RETAIL_CTCGRAPHICAPPEARANCE` adds the declaration
  `void UnregisterComponent(CAGraphicAppearanceComponent&)` 0x004C0000 (Ego_r / FableWin, the bound spelling) and the forward
  declaration of CAGraphicAppearanceComponent. Declaration only.
- `engine/CString.h` (libpk CString): `FABLE_RETAIL_CSTRING_BUILDERS` adds the ctor(const char*) 0x00C0BFE0, copy ctor 0x00C0DAB0,
  dtor 0x00C0BA40 and the `__fastcall` builders operator<<(const char*) 0x00C0DEC0 / (unsigned long) 0x00C0DF10 / (long) 0x00C0DF00 /
  (double) 0x00C0DED0. Declarations only.
- Landed includers rechecked with `vland --recheck-landed` on the mirror (SIBLINGS.tsv): 0x006AA2E0 and 0x00C0D1B0, both
  RELOCATION_MATCH PASS.

## Header debt
- engine/CHoverable.h, engine/CAIStateGroup_WorkSawingWood.h, engine/CAIStateGroup_WorkHammerAnvil.h are flattened generated
  PODs (vptr as a data member): no polymorphic retail branch, so the CDraggableInto / CAIStateGroup_* classes are local models (as
  the landed 0x00559040 / 0x008FFBA4 siblings do). The CAIStateGroup rows use engine/CAIStateGroupBase.h
  (FABLE_RETAIL_CAISTATEGROUPBASE) and engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER, packed).
- Appearance rows: engine/CThing.h (FABLE_RETAIL_CTHING: IsDead / GetTCGraphicAppearance) + engine/CTCBase.h
  (FABLE_RETAIL_CTCBASE); CAGraphicAppearanceComponent and the four component classes have no header (local, the landed
  0x007A66F0 model).
- libpk CSoundEvent / CSoundEventManager / CStreamBuffer / CSoundSample / CSampleRef have no shared header; every landed row of
  the family models them locally (descriptive names, no PDB/RTTI).

## Callee names
- CheckState 0x00C20720 `?CheckState@CSoundEvent@@QAEXJ@Z` (family spelling; same-TU stand-in body as in WaitForData 0x00C20850).
- Bound spellings reused: Stop 0x00C21620, WaitForData 0x00C20850, WaitForMemory 0x00C20880, AllocateHandle 0x00C22100,
  GetPhysicalPlayer 0x00C10C90 (`void*` result, cast to CPhysicalPlayer* at the two use sites), ??0/??1CStreamBuffer
  0x00C262A0 / 0x00C26400, FinishLoading 0x00C26090 (own landed leaf), GetRecord 0x00C34600, the CString set, ~CTCBase,
  UnregisterComponent, ??0CAIStateGroupBase, ??0CBaseIntelligentPointer, ~CHoverable, clear 0x005356A0, free, delete.
- Respelled (NAME_FIXES.tsv; proven type identity): PlayPhysical / WaitForPhysicalPlayer take CStreamBuffer* (was CSoundChannel*);
  CStreamBuffer::Attach / Release take CSoundEventManager* (was CPanel* / CPooledResourceManager*); CSampleRef::GetSample returns
  CSoundSample* (was long; REVIEW, its callers pass it as long to CVirtualPlayer::Start); ??1CSampleRef 0x00C25CA0 (was
  ??1CKey_00c119d0); ??1CSoundEvent / ??0CSoundEvent / ??_7CSoundEvent (were SEntry_* / CScriptModuleBase).
- Descriptive (no PDB covers libpk): CSoundSample::GetSize 0x00C1EA80 (header size + data size + [+8]), CSoundSample::LoadData
  0x00C1EA60 (`this->vslot2(buffer->GetData(manager))`), CSoundSample virtual slots 1 StartStreaming / 3 IsStreamed, the manager's
  cached-buffer queue (+0x284) slot 17 Find, CString::operator<<(double) 0x00C0DED0 (signature from `fld qword [esp+4]` / `ret 8`).
  CSoundEvent member names: m_index +0x0C, m_priority +0x14, m_idEmitter +0x24, m_listening +0xB0 (the describer prints
  "Emitter ID = " / "Not in behaviour").

## How Play 0x00C21780 matched (8 grades)
1. inline helper for the two "physical player or wait" tails: the second copy is not inlined (DIFFER 533v622).
2. tails written out, one `buffer` variable for the cached and the new buffer: right size, register rotation
   (this/ref/buffer = ebx/edi/ebp vs retail ebp/ebx/edi) plus two argument-order diffs.
3-5. GetSample fetched into a local before AllocateHandle / Attach: retail calls it before pushing m_priority / the handle
   (fixes the order); the register rotation stays. ~300 probe compiles (inline accessor layers, const&, getters, flags,
   nested if/else, same/separate buffer variable, CheckState stand-in on/off) never moved it.
6. Each failure exit spelled out in full (`FinishLoading(0); Release; delete; Stop; return;` in the streamed branch and
   `status = 0; Release; delete; Stop; return;` in the loaded branch), each play exit spelled out: VC7.1 cross-jumps the
   identical tails into retail's single release block, and the unmerged source copies weight the buffer above the sample
   reference and `this`: RELOCATION_MATCH. (A probe with extra duplicated Release calls first showed the weight lever.)

## GOTCHAS one-liners
- VC7.1 adjustor thunks for a virtual destructor are `??_E<Class>@<ns>@@W<adj>AEPAXI@Z` (`W3` = 4, `WBI@` = 0x18) jumping to the
  weak `??_E` alias of the `??_G`: emit them by defining the class's virtual dtor out of line (vtables in the TU); grade with the
  exact ??_E...W.. leaf (0x00559020 / 0x00559030).
- A register rotation among three callee-saved variables that no accessor/temporary spelling moves can be the source having two
  identical exit sequences that VC7.1 cross-jumps into one block: the unmerged copies count in the register weights. Spell each
  exit out in full (0x00C21780 CSoundEvent::Play).
- When retail calls a getter before pushing the plainer arguments of the call it feeds (`call Get ; push [this+14h] ; mov ecx,eax ;
  call Size`), the getter result was a named local in the source (0x00C21780).
- Purity grep: an identifier containing `_emit` (`m_emitterId`) makes vland reject a row as NOT_GENUINE; spell it `m_idEmitter`.
- The AI state-group TU 0x008FD000..0x00903xxx has no function alignment (odd start addresses are real functions); its
  CAIStateGroup_* constructors follow the 0x008FFBA4 model, with a `bool TookActionPoint` +0x14 before the packed pointer +0x15.
