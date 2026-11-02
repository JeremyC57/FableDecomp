# Hand chunk 699 (CEnginePrimitiveRenderer2D .. CTCInventoryItem): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH under q.py and the read-only vland check (no RELOC_IDENTITY, no DIFFER); tests PASS
under g++ and the lane's VC7.1. Extents verified from the int3 padding (all ten oracle lengths are right). Eight of the ten
catalog labels were wrong (the propagated "CTCInventoryItem::OnDie").

## What matched (10)
- **CEnginePrimitiveRenderer2D::AddPrimitiveManager 0x00B4ABB0** (94): the landed 3D twin 0x00B33AE0 source with the 2D
  class (PrimitiveManagers +0x0C). Name: Ego_r 0x009EB610 / FableWin 0x02D9D980 AAE spelling; 5 callers all in the next
  function 0x00B4AC10 (renderer ctor, vtable 0x012A2308), Ego_r order ~dtor / AddPrimitiveManager / ctor.
- **CTCInventoryItem::GetInventoryOwner 0x005D8E90** (43): `PInventory ? PInventory->ParentThing : uid > 0 ?
  GetThingManager().PeekPThingWithUID(uid) : 0`. Name: Ego_r donor run GetBoughtPrice / GetInventoryOwner / OnDie =
  retail 0x005D8E70 / 0x005D8E90 / 0x005D8EC0; QBE + PBVCThing forces the const GetThingManager 0x00686AF0 and the const
  PeekPThingWithUID 0x0051ED60.
- **CTCInventoryItem::OnDie 0x005D8EC0** (34): vtable 0x01240004 slot 13 (CTCBase order; CTCBase's own slot 13 is the empty
  0x006868E0). `RemoveFromInventory(); CTCBase::OnDie();` with RemoveFromInventory (0x005D8E20) a plain same-TU member.
- **CFireHeartPatternDef::~CFireHeartPatternDef 0x00D34F80** (33): implicit; ??_G 0x00D34F60 of vtable 0x012CCC50 (the
  landed Copy/Transfer/ctor class) + the landed Resize 0x00F33D90's temporary; Ego_r 0x00D47AC0 / FableWin 0x03B5FF90.
  engine/CDefClassBase.h retail opt-in gives the CDefPointeeBase store + jmp ~CResource.
- **CHandleBasedMemoryPool::~CHandleBasedMemoryPool 0x00A02DB0** (52): user `{ Uninitialise(); }`; RTTI vtable 0x0129C964
  stored first; Ego_r 0x00B6DFA0 / FableWin 0x0319A080; members re-laid with the 12-byte STLport CArray.
- **CVtable_012bf0c8::~CVtable_012bf0c8 0x00C775A0** (54, libpk, descriptive class name kept from the landed reverb rows):
  user `{ ReleaseBlock(); }` with ReleaseBlock (0x00C77470) a plain same-TU member that VC7.1 inlines.
- **C3DMeshFileClothChunk::~C3DMeshFileClothChunk 0x00A9D3C0** (43): implicit; ??_G 0x00AC69D0 of RTTI vtable 0x0129F008;
  FableWin 0x033D4200 (after ??1C3DMeshFilePrimitiveChunk = retail 0x00A9D3A0). The XUVTangent 0x00AA7320 model.
- **NParticleEngine::CPSCSingleSprite::~CPSCSingleSprite 0x00B0A160** (41): implicit; ??_G 0x00B0A140 of RTTI vtable
  0x0129FB84; Ego_r 0x00ACB250 / FableWin 0x02F37EC0. Real STLport list in CEnginePrimitiveSpriteTrail: VC7.1 keeps
  ~_List_base out of line (= 0x00B09E40, cross-graded) and ~CPSCSplineBase out of line.
- **NScript::CV_FishingCompetitionScript::~CV_FishingCompetitionScript 0x00E81F30** (35): implicit; ??_G 0x00E81F10 of the
  vtable 0x012EA478 the landed ctor 0x00E7E110 stores; FableWin 0x039F1040.
- **NCreatureMode::CModeSummonerSummon::~CModeSummonerSummon 0x008CC330** (50): implicit; ??_G 0x008CC310 of RTTI vtable
  0x01282BA0; Ego_r 0x00903940; the CModeJackOfBladesScream 0x00740540 model + STLport vector<C3DVector>.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
0x008CC330, 0x00E81F30, 0x00B0A160, 0x00A9D3C0, 0x00C775A0, 0x00A02DB0, 0x00D34F80 "CTCInventoryItem::OnDie" (propagated;
destructors of seven different classes, see above). Also proven wrong (NAME_FIXES.tsv): landed 0x00A9D3A0
"CTCInventoryItem::OnDie" = ~C3DMeshFilePrimitiveChunk; 0x00AA8490 binding ?OnDie@BaseItem = ~CChunkedFileSuperChunk; catalog
0x00B09E40 StdList_Clear = ~_List_base<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry>; landed 0x0051ED60 CStopWatch::GetTicks =
CThingManager::PeekPThingWithUID; 0x005BC499 binding COwner::Release = NInventory::CTCInventoryBase::RemoveItemFromInventory;
0x006868E0 binding vfunc_13 = CTCBase::OnDie; catalog 0x00C77470 ~CBuffer_00c77470 = ReleaseBlock (not a dtor).

## Names: proven vs descriptive
- Proven (PDB name set + vtable/RTTI or donor order or callers): all rows except 0x00C775A0.
- Descriptive: 0x00C775A0 `CVtable_012bf0c8` / `CDspObject` / `ReleaseBlock` / `CPKObject::operator delete` -- libpk has no
  PDB and no RTTI; the spellings are the landed reverb sources' (0x00C6A780, 0x00C774F0). Searched: RTTI (none), PDB name
  sets (libpk absent), strings (none referenced by the class).
- Callee names: g_pEnginePrimitiveTypeInfo 0x01436E84 (established landed spelling of 0x00B33AE0, no PDB data symbol);
  vector<CCountedPointer<CEnginePrimitiveManager> >::_M_insert_overflow 0x00B38BB0 (bound spelling).
- Symbol collision to note: the landed 0x00A9D3A0 row's ?OnDie@CTCInventoryItem@@QAEXXZ is a different mangled name from this
  chunk's ?OnDie@CTCInventoryItem@@UAEXXZ (0x005D8EC0), but both claim CTCInventoryItem::OnDie; respell 0x00A9D3A0 to
  ??1C3DMeshFilePrimitiveChunk@@UAE@XZ in the same landing.

## Header debt
- engine/CTCBase.h, engine/CTCInventoryItem.h: flattened vptr (local polymorphic CTCBase with the slot order of the landed
  0x00713700 row; CTCInventoryItem members at the header's offsets). engine/CDefPointer.h used for PDef.
- engine/CModeStandBase.h, engine/CHandleBasedMemoryPool.h, engine/C3DMeshFileClothChunk.h, CChunkedFileSuperChunk.h,
  CChunkedFileChunk.h, engine/CEnginePrimitiveRenderer2D.h, CEngineComponent.h, CEnginePrimitiveTypeInfo.h: flattened vptr /
  no member functions (local models; vland SHARED_HEADER_AVAILABLE warnings for CEngineComponent / CEnginePrimitiveTypeInfo
  are this). CHandleBasedMemoryPool.h already carries the retail 12-byte CArray offsets.
- engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (0x00CDD250): local copy
  without it (same as the landed 0x00D83350).
- No headers for CModeSummonerSummon, CV_FishingCompetitionScript, CPSCSingleSprite/CPSCSplineBase,
  CEnginePrimitiveSpriteTrail, CFireHeartPatternDef, CArray, C3DVector, CCharString (local models as in the landed siblings).
