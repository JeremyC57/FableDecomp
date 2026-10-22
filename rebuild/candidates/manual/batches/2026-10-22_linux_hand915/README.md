# Hand chunk 915 (_global boundary rows: CThing helpers .. DirectShow CBasePin): 33 of 33 byte-exact

All rows grade MATCH / RELOCATION_MATCH with the lane default RTM cl 13.10.3077, no pragma or flag. Graded with
`$S/w22/agent915/q915.py` (chunk915 oracle first) and re-checked with the shared gall.sh / q.py (no shadowing hit).
Tests pass under g++ and VC7.1 (tall.sh / tall.py).

## What matched (33)
- Small accessors and constructors (13): 006d4cc0 one-bit uchar bitfield getter (+0x1C9; `and al,1`, no setne);
  006d4cd0 list empty(); 006d4d00 CIntelligentPointer<CThing> accessor (tail `jmp GetPItem`); 007b55b0 / 007eeb90 /
  00a0d260 member setters; 007b55c0 / 0083ce50 one-pointer null constructors with an ignored argument (`ret 4`);
  007eeb80 `return 0x4E`; 007f7930 `return *this` with an ignored argument; 00a0d280 3-dword zero constructor;
  009be380 `(float)b / a` (fild/fidiv, this unused); 00be5d10 fastcall `value * (1.0f/255.0f)`.
- 006d4ce0 IS_THING_DEAD<CThing*>: `template bool __fastcall IS_THING_DEAD(T const&)`, `if (p && !(flags & 1)) return false; return true;`.
- 007b5590: an in-place `CCharString("CActivateQuestDef", -1)` constructor (the string that opens GetSubDef<CActivateQuestDef>).
- 00800c90: `if (p) free(p)` (CRT free thunk 0xBFEA14).
- STLport algorithms with retail's __fastcall: 007b5530 __copy_aux and 007b5560 copy into a deque back_insert_iterator
  (the non-POD iterator result comes back through a hidden pointer in ecx, first in edx); the empty
  random_access_iterator_tag / __false_type need user constructors (no zero byte store) and the tag is then homed
  in the dead `result` slot. 007f7940 remove_copy over pointers. 00800c60 vector::erase(first,last) -> fastcall __copy
  0x8001F0 with the tag in the dead `first` slot.
- 0080aa60 __linear_insert<CTargetPoint*, CTargetPoint, CThingCompare_BestForTargeting2> (219 B, `ret 50h`): the
  real __copy_backward body out of line in the TU (auto_inline off) homes the tag in the LIVE `__val` slot;
  CTargetPoint / CIntelligentPointer copy + assignment `__forceinline` (else an out-of-line copy is called); a
  C3DVector user copy constructor gives the copy constructor's direct dword stores while the implicit C3DVector
  assignment keeps retail's `lea ecx,[esi+0Ch]` form in `*first = val`.
- List iterator family 0092b230..0092b2b0 (7): iterator ctor, operator++, begin()/end() through the hidden result
  (the iterator has a user ctor), __distance by const& (twins 0092b270/0092b290 given distinct element types so they
  do not share a symbol), size() = __distance(begin(), end()).
- PDB-named rows: 007f4f40 CTCStockItem::GetSellersBanter (CStockItemDef::SellersBanter +0x30; the landed OnCreate
  0x007F4E80 idiom: noinline GetSubDef stand-in, hand release without null test); 00a64480
  CMemoryAllocatorVariableSize::IsNodeBlack (int-returning, `mov eax,1`); 00a644a0 GFCheckBlackHeight (fastcall, self
  tail `jmp`: the recursion must sit in an inline helper or VC7.1 makes a loop); 00be5d20
  CLandscapeBackgroundPatch::RegisterForZSorting (-> CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long&,
  patch) 0x00B67E30 on 0x01436EA8); 00ca53f0 CBasePin::AddRef (DECLARE_IUNKNOWN `GetOwner()->AddRef()`, __stdcall,
  owner at [this-8], layout as the landed CBasePin ctor 0x00CAC070).

## Parked for Fable 5.1 (0)
(none)

## Skipped
(none) -- no row holds two functions.

## Label corrections
- 00ca53f0: manifest `CScriptThing::operator==` is propagated; it is the DirectShow pin's DECLARE_IUNKNOWN AddRef
  (slot 1 of the IPin vtable 0x012BF3C0, reached from IQualityControl through adjustor 0x00CA6180). Its vtable
  neighbours 0x00CA53C0 (manifest `CConsoleCommand<CNewFrontendGameComponent>::CallCommandFunction`) and 0x00CA5510
  (manifest `CScriptThing::operator==`) are the same delegation for QueryInterface and Release -- also wrong labels.
- 00a644a0 = GFCheckBlackHeight, 00a64480 = CMemoryAllocatorVariableSize::IsNodeBlack, 007f4f40 =
  CTCStockItem::GetSellersBanter, 00be5d20 = CLandscapeBackgroundPatch::RegisterForZSorting (Ego_r.pdb names).
- 0080aa60 `std_insert_WithComparison` = STLport `__linear_insert<CTargetPoint*,CTargetPoint,CThingCompare_BestForTargeting2>`;
  its callees 0x008076D0 (manifest `_Move_backward`) = `__copy_backward`, 0x0080A340 (manifest
  `NPlayerTargeting_InsertTargetPoint`) = `__unguarded_linear_insert`.
- 0x007B5400 (manifest `Vector_InsertRange_8byte_StdVector`) is the fastcall `__copy` into a
  back_insert_iterator<deque<8-byte T>> (hidden result in ecx, first in edx), not a vector insert-range.
- 0x00B67E30 (manifest `sub_00b67e30`) = CEngineLandscapeRenderer::RegisterSubPatchForZSorting.
