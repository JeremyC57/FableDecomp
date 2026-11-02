# Chunk 700 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 700) A def class's implicit dtor expands its whole implicit base chain when no class between it and CDefClassBase
  declares a destructor: ~CThingCreatureDef 0x00671100 = member vector, CThingBaseDef's CSmallVector Clear (`delete[]`, two
  zero stores), CParentDefClassBase's CVectorMap free, CDefPointeeBase store, `jmp ~CResource`. Model CParentDefClassBase
  WITHOUT the header's out-of-line dtor declaration (0x00430300 is itself the implicit one: no 0x01230CBC store).
- (chunk 700) `Member = p;` on a CIntelligentPointer<T> member is the 71-byte `sub esp,8` setter shape (base ctor call,
  vtable store after the argument load, SetPItem, GetPItem on the temp, SetPItem on the member, base dtor): PDB
  CIntelligentPointer(T*) + operator=(const CIntelligentPointer&) inline, no derived dtor declared. Byte-identical setters at
  different member offsets are different functions (0x007F5810 +0x0D, 0x008ABB50 +0x1D, 0x008D73E0 +0x7D); a reference
  parameter (SetTarget(CThingCreatureBase&)) compiles identically to the pointer one, so take the parameter form from the PDB.
- (chunk 700) CIntelligentPointer is 1-aligned in retail (odd member offsets +0x0D / +0x7D with 4-aligned members after
  them): put `#pragma pack(1)` on the pointer classes, not on the owning class.
- (chunk 700) A row with no callers is named from the FableWin.pdb neighbour pair (ctor/??_G then ??1) mirrored by the retail
  neighbour (0x009B5D50 after the landed ctor 0x009B5D30; 0x0085AD00 next to its own ??_G 0x0085AE90).
- (chunk 700) A destructor that stores its vtable and calls a member first (0x00AAC4F0) is a user `{ Member(); }`; an RTTI
  intermediate base with no store before the base tail (CASoundEmitter) declares no destructor.

Review at landing:
- Four rows (0x00671100, 0x00B18780, 0x0085AD00, 0x009B5D50) carry the ungraded `DestroyOne` emission helper (`p->T::~T()`),
  the accepted chunk 656/657/696-699 pattern for out-of-line implicit `??1`.
- Respell in the same landing (NAME_FIXES.tsv): the landed 0x008C4990 (SetTarget binding), 0x008A04C0 (SetRangedTarget
  binding), the landed 0x00AAC2E0 row's own name (CAnimQueueOwner00aac2e0::StopAllQueuedAnims -> CSoundEmitterLH member, slot
  10) and its RemoveInactiveEntries / clear bindings, the landed 0x00AACEA0 / 0x00AACD80 bindings of 0x00AACC60, and the many
  invented CBaseIntelligentPointer spellings.
- 0x00AAC2E0 is a virtual (slot 10): its final spelling should be UAE once CSoundEmitterLH's vtable is modelled; this chunk uses
  the existing ?Shutdown@CSoundEmitterLH@@QAEXXZ binding (descriptive name) to keep one spelling per address.
- The landed 0x004F9CFC "CTCInventoryItem::OnDie" (/Os, frees +0x3C then calls ~CParentDefClassBase 0x00430300; callers
  0x004F9CD0 and 0x004F9F3B in the CCardPositionsDef vector helpers) is another def-class destructor with a propagated label
  (not proven here: probably ~CCardPositionsDef).
- CSoundEmitterLH landed sources model the class as CBaseClass-derived with stand-in containers (CRetailList_00aac530,
  CEmitterArray); RTTI says CSoundEmitterLH : CASoundEmitter : CBaseClass with an STLport list at +0x14.
- No row needed more than two probes; no flags, no retail_asm, no volatile.
