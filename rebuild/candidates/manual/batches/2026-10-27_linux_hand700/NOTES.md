# Hand chunk 700 (CThingCreatureDef .. CModeScorpionKingVulnerable): 10 of 10 byte-exact

All ten NA043 rows grade RELOCATION_MATCH under q.py (QTAG=c700) and the read-only vland check; every test PASSes under
g++ and the lane's VC7.1; calls.py: every call target is retail's (free 0x00BFEA14, operator delete[] 0x00BFEB1C, the
PDB-spelled base destructors / CBaseIntelligentPointer members). Extents verified from the int3 padding: all ten oracle
lengths are right (0x00B18780 and 0x009B5D50 end with their tail `jmp` at 27 bytes, then 5 int3 and the next function at
+0x20 -- ordinary alignment, not a second body in the row). All ten catalog labels were wrong (propagated).
No flags, no pragmas beyond the retail packing, no retail_asm, no volatile.

## What matched (10)
- **CThingCreatureDef::~CThingCreatureDef 0x00671100** (75, implicit): NavigatorTypes (STLport vector, +0xC8), then the
  implicit CThingBaseDef (Components = CSmallVector<CEntry,8>: PDB `~CSmallVector() { Clear(); }`, protected Clear =
  `delete[] Start; Start = 0; Size = 0;` -> operator delete[] 0x00BFEB1C and the two zero stores) and the implicit
  CParentDefClassBase (SubDefInfoMap CVectorMap free at +0x29), CDefClassBase's dead store dropped, CDefPointeeBase store,
  `jmp ~CResource`. Lesson: a def class's whole implicit chain expands in one ??1 when no class below CDefClassBase declares
  a destructor (local CParentDefClassBase without the header's out-of-line `virtual ~CParentDefClassBase();`).
- **NParticleEngine::CPSCAttractor::~CPSCAttractor 0x00B18780** (27, implicit): the chunk 699 CPSC chain model with
  AttractorUserPointsArray (CArray<C3DVector>) at +0x24; tail ~CBaseClassNonCopyable.
- **CGameMap::~CGameMap 0x006B7570** (33, user `{}`): vtable store, Cells (vector<CGameMapCell>) free, tail
  ~CBaseClassNonCopyable.
- **CCombatActionBase::CStrikeEvent::~CStrikeEvent 0x0085AD00** (36, implicit, QAE): the landed ??_G 0x0085AE90 members with
  the real STLport vector and a CIntelligentPointer<const CThing> template (no own dtor: base dtor call).
- **CMirroredTextureHighlightsChunk::~CMirroredTextureHighlightsChunk 0x00A9D330** (33, user `{}` out of line): the landed
  ??_G 0x00A9D360 model.
- **CSoundEmitterLH::~CSoundEmitterLH 0x00AAC4F0** (49, user `{ Shutdown(); }`): real STLport list<CCountedPointer<
  CActiveSound> > (`~list() {}`, inline ~_List_base with out-of-line clear() 0x00AACC60 and the null-tested sentinel free via
  edi); CASoundEmitter declares no destructor, so the tail is ~CBaseClass.
- **CDefinitionManager::CDefClassInfo::~CDefClassInfo 0x009B5D50** (27, implicit): GlobalIndices (CArray<unsigned long>,
  +0x08) free, tail ~CCharString on Name.
- **CTCTargetingBase::SetPNearestInterestingThing 0x007F5810**, **CTCTargetingBase::SetPScriptOverrideRangedTarget
  0x008ABB50**, **NCreatureMode::CModeScorpionKingVulnerable::SetTarget 0x008D73E0** (71 each, `ret 4`): `Member = thing;`
  (resp. `PTarget = &target;`) through the PDB's CIntelligentPointer<T>(T*) and operator=(const CIntelligentPointer&) inline
  (base ctor, vtable store, SetPItem; member.SetPItem(temp.GetPItem()); temp's base dtor). Three different functions with
  byte-identical shapes: +0x0D / +0x1D / +0x7D, vtables 0x01238C6C (CIntelligentPointer<CThing>) and 0x0123E7D8
  (CIntelligentPointer<CThingCreatureBase>).

## Names and evidence (all proven from two sources unless marked)
| row | symbol | evidence |
|---|---|---|
| 0x00671100 | ??1CThingCreatureDef@@UAE@XZ | ??_G 0x006710E0 slot 0 of RTTI vtable 0x0125B2DC; GetSizeofClass 0x006710D0 = 0x14C = Ego_r 0x154 - 8; Ego_r + FableWin ??1; member offsets |
| 0x00B18780 | ??1CPSCAttractor@NParticleEngine@@UAE@XZ | ??_G 0x00B18760 slot 0 of RTTI vtable 0x012A04AC; Ego_r + FableWin ??1; AttractorUserPointsArray +0x24 (landed ctor 0x00B18020) |
| 0x006B7570 | ??1CGameMap@@UAE@XZ | stores RTTI vtable 0x0125D558 first, ??_G 0x006B81A0 inlines the body; Ego_r 0x006F3890 / FableWin 0x0249FF30 in the class TU |
| 0x0085AD00 | ??1CStrikeEvent@CCombatActionBase@@QAE@XZ | no callers; FableWin ??_G 0x0282D400 then ??1 0x0282D440 (QAE); retail ??_G 0x0085AE90 expands the same members; Ego_r layout; CStrikeEvent ctor 0x0085ADB0 next door |
| 0x00A9D330 | ??1CMirroredTextureHighlightsChunk@@UAE@XZ | no callers; stores RTTI vtable 0x0129E0E0 first; ??_G 0x00A9D360 (landed) inlines it; FableWin 0x033D3F70 between ctor and GetChunkDescription |
| 0x00AAC4F0 | ??1CSoundEmitterLH@@UAE@XZ | no callers; stores RTTI vtable 0x0129E6EC first; ??_G 0x00AACEA0 (landed) expands it; landed clear() 0x00AACC60 names it (class absent from both PDBs: RTTI name) |
| 0x009B5D50 | ??1CDefClassInfo@CDefinitionManager@@QAE@XZ | no callers; FableWin ??0 0x0305A7B0 then ??1 0x0305A820 = retail landed ctor 0x009B5D30 then this row; Ego_r layout {Name, AllocFunc, GlobalIndices} |
| 0x007F5810 | ?SetPNearestInterestingThing@CTCTargetingBase@@IAEXPAVCThing@@@Z | Ego_r donor run Set/Get(+0x50)/SetPScriptOverrideInterestingThing = retail 0x007F5810/0x007F5860/0x007F5890 (landed); FableWin 0x02533F80; callers UpdateNearestInterestingThing (base and player override) |
| 0x008ABB50 | ?SetPScriptOverrideRangedTarget@CTCTargetingBase@@QAEXPAVCThing@@@Z | Ego_r 0x008E9A40 + CTCTargetingAI::SetScriptTargetOverride +0x50 = retail 0x008ABB50 / 0x008ABBA0 (+0x40 PScriptTargetOverride, vtable 0x0123E7D8); FableWin after EntitySetRangedTarget; callers EntitySet/ClearRangedTarget |
| 0x008D73E0 | ?SetTarget@CModeScorpionKingVulnerable@NCreatureMode@@QAEXAAVCThingCreatureBase@@@Z | Ego_r 0x00818760 + FableWin 0x0284D510; PTarget CIntelligentPointer<CThingCreatureBase> +0x7D (Ego_r, landed ctor 0x008D8120); sole caller ScorpionPrepareTailPlunge::FinishAction 0x008C4990 |

Descriptive (no PDB): CSoundEmitterLH::Shutdown (0x00AAC2E0, vtable slot 10; kept in the spelling the landed ??_G binds) and
the CSoundEmitterLH member names m_pManager / m_position / m_sounds / m_active / m_paused / m_finished / m_number (the landed
ctor 0x00AAC530's). Searched: Ego_r / FableWin name sets (CSoundEmitterLH / CASoundEmitter absent), RTTI (class names only).
Parameter names of the setters are descriptive.

Callee names (all checked by address with calls.py): free 0x00BFEA14, operator delete[] 0x00BFEB1C (??_V, declared so `delete[]`
calls the import), ~CResource 0x009FC550, ~CBaseClassNonCopyable 0x0099A300 (MAE), ~CBaseClass 0x0099A2E0, ~CChunkedFileChunk
0x00AA75C0, ~CCharString 0x0099EAE0, CBaseIntelligentPointer ctor / GetPItem / SetPItem / ~ 0x00A01B10 / 0x00A01B50 /
0x00A01B90 / 0x00A01C10 (Ego_r spellings), _List_base<CCountedPointer<CActiveSound>>::clear 0x00AACC60 (its landed leaf).

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
All ten catalog labels were propagated: the seven "CTCInventoryItem::OnDie" rows are seven different classes' destructors,
the three "CVillageTaskCarryCrate::AssignCreatureToTask" rows are two CTCTargetingBase setters and
CModeScorpionKingVulnerable::SetTarget. Landed spellings proven wrong (NAME_FIXES.tsv, 22 rows): the landed 0x008C4990
binds 0x008D73E0 as ?SetTarget@CModeScorpionKingVulnerable@@QAEXPAVCThing@@@Z (no namespace, pointer); the landed 0x008A04C0
binds 0x008ABB50 as ?SetRangedTarget@CTCTargeting_Retail@@...; the landed 0x00AAC2E0 row is CSoundEmitterLH vtable slot 10,
not "CAnimQueueOwner00aac2e0::StopAllQueuedAnims"; 0x00AACC60 is bound as ?clear@CRetailEntryList / ?DestroyElements@
CEmitterArray / ?CActiveFile_PostReadStep in three landed rows; 0x00AABCA0 as ?RemoveInactiveEntries@CAnimQueueOwner00aac2e0;
and 21-56 invented spellings each for the CBaseIntelligentPointer members 0x00A01B10/50/90 and 0x00A01C10, plus the stray
bindings of ~CBaseClassNonCopyable / ~CBaseClass / ~CChunkedFileChunk / ~CCharString / ~CResource.
No new spelling collides with a landed row's spelling at another address.

## Header debt
- engine/CParentDefClassBase.h declares `virtual ~CParentDefClassBase();` out of line (0x00430300); retail's destructor is
  implicit (0x00430300 stores no 0x01230CBC vtable) and is expanded inline into derived implicit dtors (0x00671100): local copy
  of the class without the destructor declaration.
- engine/CModeStandBase.h: flattened vptr, `struct` key (retail class); used as the base of the local
  CModeScorpionKingVulnerable model (members from the landed ctor 0x008D8120).
- engine/CTCBase.h: flattened vptr; used as the base of the local CTCTargetingBase (no header).
- engine/CMirroredTextureHighlightsChunk.h: flattened, no base class; local model over a local CChunkedFileChunk (as the landed
  ??_G 0x00A9D360).
- No headers for CThingBaseDef, CThingCreatureDef, CSmallVector, CThingComponentSet, CVectorMap, CDefString, CEngineGraphic,
  CGameMap, CPSCAttractor/CPSCBase, CStrikeEvent, CDefClassInfo, CSoundEmitterLH/CASoundEmitter, CBaseIntelligentPointer /
  CIntelligentPointer<T>, CTCTargetingBase, CModeScorpionKingVulnerable (local models; STLport as real templates).
- Retail packing kept (not a device): CBaseIntelligentPointer / CIntelligentPointer<T> are 1-aligned (Ego_r: +0x0D / +0x7D
  members, with 4-aligned members after them), CDefClassBase-derived defs through CThingBaseDef are pack(1) (Components +0x39),
  CEngineGraphic is 14 bytes 1-aligned (Ego_r EyeGraphic +0x116 = Graphic + 14), CThingComponentSet::CEntry is 9 bytes.
- engine/CDefinitionManager.h: flattened layout with no nested CDefClassInfo, so the row's `this` (the nested class) cannot
  come from it; local CDefinitionManager { class CDefClassInfo } as in the landed ctor 0x009B5D30 (vland
  SHARED_HEADER_AVAILABLE warning on 0x009B5D50 is this).

## vland (read-only) result
10 RELOCATION_MATCH, behaviour PASS, no RELOC_IDENTITY, no DIFFER. Two warnings: SHARED_HEADER_AVAILABLE CDefinitionManager
(0x009B5D50, see header debt) and RELOC_NAME ?Shutdown@CSoundEmitterLH@@QAEXXZ -> 0x00AAC2E0 labelled
'CSoundEmitterLH::vfunc_10' (catalog placeholder; the spelling is the one the landed ??_G 0x00AACEA0 already binds --
coordinator decision: keep the descriptive Shutdown, or rename the slot-10 member once its purpose is settled).
