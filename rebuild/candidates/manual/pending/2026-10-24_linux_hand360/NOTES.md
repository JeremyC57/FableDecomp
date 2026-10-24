# Hand chunk 360 (_Push_heap<CDegradableInfo> .. _Push_heap<pair<float,EConversationAttitude>>): 39 of 40 byte-exact

All 40 rows are SGI/STLport heap helpers from the /Gr algorithm TUs (link-time `__fastcall`: first in
ecx, hole in edx, then top, the element by value and the comparator by value; `ret` = 4 + sizeof(T) +
sizeof(Compare)). One body fits 38 of them:

    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;

Each instantiation only needs the right element model (copy form) and comparator. Where a landed
sibling (`_Adjust_heap` / `__linear_insert` / `__unguarded_linear_insert` of the same instantiation)
exists, its model was reused unchanged. Grades: 22 MATCH + 17 RELOCATION_MATCH (gall.sh); all 39 tests
PASS under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (39)
- **POD records copied with `rep movsd`** (0x004CFEB0, 0x0067E650, 0x007583F0, 0x008170D0, 0x00819BC0,
  0x00A31410): a trivially copyable record with the key at +0; float keys give `fld ; fcomp ; test ah,5 ; jp`
  for `<` and `test ah,41h ; jne` for `>`; long keys `jge`, unsigned keys `jae`. 0x0067E650 is the real
  CDegradableInfo / CDegradableDef::CIsHealth1GTHealth2 (callers in CDegradableDef::Transfer);
  0x008170D0 is THitLocationIntersectionElement (32 bytes, PDB). The SHAPE lesson: SGI `hole > top`
  (Dinkumware's `_Top < _Hole` gives `cmp ebp,edi ; jge` -- the only diff of the first try).
- **CIntelligentPointer<CThing> + CDoNotUseBinaryPredicate** (0x00806990 `_Push_heap`, 0x00806910
  `__unguarded_linear_insert`): model of the landed 0x00807520; the coin flip must be written
  `if (Random() & 1) return false; return true;` to give `test al,1 ; mov [edi],eax ; jne`. The by-value
  pointer is destroyed by the callee (`call 0x00A01C10`).
- **12-byte records** CBlockInfo (0x00ADAAA0, Cost at +4), CMeshInfo with the PrimaryMesh bitfield
  (0x004C1F80 `a.PrimaryMesh`, 0x004C2000 `!a.PrimaryMesh`), pair<CSkeletalMorphs::CEntry,float> (0x0071ECF0,
  signed word compare): models of the landed siblings; three dword moves, the value word loaded once.
- **CWideString** (0x00497060): `_wcsicmp(a, b) < 0`, arguments evaluated right to left.
- **CTargetPoint / CThingCompare_BestForTargeting2** (0x0080A480): the 0x34-byte comparator by value with
  its out-of-line operator() (0x008098B0); CTargetPoint's `__forceinline` memberwise assignment from the
  landed 0x0080AA60.
- **Pointer/scalar heaps with propagated `float` labels** (0x005140A0 CThingBuilding*/CIsHouseLargerThan,
  0x0097ADF0 CThing*/CThingCompare_InDirectionOfAndNearestToHeuristicBitch (16-byte functor), 0x00932370
  CThing* with a `bool (__fastcall*)(CThing*, CThing*)`, 0x00776750 long/less<long>, 0x00A206F0
  CAStarState*/CAStarStateTotalGreaterThan, 0x00A586D0 float/less<float>): the comparator is named by its
  out-of-line operator() address; VC7.1 tail-duplicates the final drop into both exits on its own.
- **Function-pointer predicate on a struct** (0x00AD0920, QuadricOptimiserInternals::COptimisedVertex::CGroup):
  `mov ecx,&parent ; lea edx,&value ; call [esp+20h]` = `bool (__fastcall*)(const CGroup&, const CGroup&)`.
- **CCharString-keyed pairs** (0x00A6F1D0, 0x00A6F0F0, 0x009E67C0 CCompareString = out-of-line FastCompare;
  0x0077E730, 0x00CB64D0, 0x00AF2F10, 0x004B9540, 0x009B4940, 0x00A403F0 CKeyPairCompareLess = the inline
  CCharString::operator< of the landed 0x009B5C70): implicit pair assignment (CCharString::operator= calls
  plus dword moves, `rep movsd` for the 52-byte C3DDummyObject), the by-value pair destroyed at the end in
  reverse member order. Generated from one template (tools/genstr.py + tools/rows_str.py).
- **/Os TU** (0x004DD875 `__push_heap`, pair<CCharString,CTCBase::CRegisteredTCInfo>): `#pragma optimize("s", on)`
  with the pair's operator= declared out of line (0x004DA56A) gives the ebp frame, the `first` spill and `leave`.
- **pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>** (0x005E0940): the landed 0x005E07F0 model
  (inline intrusive operator= and destructor) matched first try.
- **pair<EHeroMorphType, CTextureMorphEntry>** (0x0071F710, 0x00819B10, byte twins): implicit 6-dword assignment.
- **pair<float, CCharString>** (0x004CFE10), **pair<float, EConversationAttitude>** (0x004CFD80), **pair<long, dword>**
  less (0x004CB640, 0x00680460) / greater (0x0088C7C0): the landed _Adjust_heap models.

## Parked for Fable 5.1 (1)
- 00807620 _global::_Push_heap<pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting> DIFFER(164v164): retail `call 0x7F9660 ; lea ecx,[esp+1Ch] ; push ecx ; fstp [esp+14h]` vs ours `call ; fstp [esp+10h] ; lea ecx,[esp+1Ch] ; push ecx` (the second GetCompareValueForPoint argument scheduled before the first result's store) | tried: `GetCV(a.first) > GetCV(b.first)` (landed 0x008074A0 form), named float locals, `/G7`, an inline `Value(p)` helper (flips the evaluation order: value's call first) | next: lane gap: x87 store scheduling (try the comparator's operator() `const` + `GetCompareValueForPoint` const, or the comparator copy built by a user copy ctor, once at landing)

## Skipped
- none (no row holds two functions; the trailing int3 runs of 0x00806910 / 0x004C2000 / 0x0080A480 /
  0x00807620 / 0x00A206F0 / ... are padding only).

## Label corrections
- 0x00806910: not `_Push_heap` -- `std::__unguarded_linear_insert<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate>` (called by the landed 0x00807320).
- 0x004CFEB0: `_Push_heap<CDegradableInfo, CIsHealth1GTHealth2>` label is propagated: a 28-byte float-keyed record sorted with `<` (caller chain 0x004D0660 -> 0x004D2570, reached from 0x004CEA2A / 0x004D264E / 0x004D26E5 -- labels there are propagated as well); the true CDegradableInfo instance is 0x0067E650.
- 0x007583F0 (44-byte long key, reached from the manifest-labelled AddWound 0x00757B30), 0x00819BC0 (40-byte long key), 0x00A31410 (56-byte unsigned key = pair<unsigned long, 52-byte POD>, reached from the manifest-labelled LoadBinary 0x00A2F2D0): CDegradableInfo labels propagated.
- 0x008170D0: `_Push_heap<(anon)::THitLocationIntersectionElement*, ..., THitLocationIntersectionElementPred>` (not CDegradableInfo).
- 0x005140A0: `_Push_heap<CThingBuilding**, ..., CIsHouseLargerThan>`; 0x0097ADF0: `_Push_heap<CThing**, ..., CThingCompare_InDirectionOfAndNearestToHeuristicBitch>`; 0x00932370: `_Push_heap<CThing**, ..., bool (__fastcall*)(CThing*, CThing*)>`; 0x00776750: `_Push_heap<long*, ..., less<long>>`; 0x00A206F0: `_Push_heap<CAStarState**, ..., CAStarStateTotalGreaterThan>` (all labelled `float*/less<float>`).
- 0x00A586D0: `_Push_heap<float*, ..., less<float>>` (labelled pair<float,CCharString>).
- 0x004CB640 / 0x00680460: pair<long, dword> with `<`; 0x0088C7C0: pair<long, dword> with `>` (all labelled pair<float,EConversationAttitude>; 0x004CFD80 is the real float one).
- 0x00930D90 (unlabelled): `bool __fastcall (CThing* a, CThing* b)` comparing `GetPosition().Z` (`a < b`), the predicate CAIStateGroup_MinionReactToCriminal::Update sorts with.
- 0x004C1F80: egor_pdb_names maps it to CTCGraphicAppearance::GetDummyObjectAssumingObjectAt -- a debug-build address collision; the bytes are the CMeshInfo heap.
- 0x00AD1DD0 (landed as `_Adjust_heap<CSpeechAnalyser::CMatchProbability>`): its callers are the QuadricOptimiser TU (COptimisedVertex::CopyTo 0x00ACC490), so the element is COptimisedVertex::CGroup (same 8-byte layout); the manifest label of 0x00AD0920 is right.
