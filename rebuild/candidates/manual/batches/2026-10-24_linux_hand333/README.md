# Hand chunk 333 (CReplaceSerialise<CFileFormatEditMapBrushHeader> .. CVectorMap<long>): 40 of 40 byte-exact

## What matched (40)
- **CReplaceSerialise<T,TOld>::ReadFromFile** (0x00826A00 EditMapBrushHeader v2, 0x008275B0 HeightMapCell v7, 0x0051CBB0 MapHeader v9):
  read length, peek version, seek back; current version read in place, older ones read into a `new unsigned char[GetMaxSize(version, 0)]`
  scratch block, ByteLength set, `ConvertToThisVersion` (static `__fastcall`, ecx = block), copied over `*this`, `delete[]`.
  Lesson: `GetMaxSize` is the static `__fastcall` recursive template `long maxSize = version == GetClassVersionNumber() ? size :
  TOld::GetMaxSize(version, size); return MAX(maxSize, sizeof(T));` -- it inlines level by level (MapHeader inlines the v9 and v8 levels
  and calls the v7 level 0x00519630; the brush header folds to the constant 0x49 through CBaseSerialise<Old,1>). Unsigned compares come
  from sizeof. Same model also closes chunk 332's parked 0x0051A240 (see Label corrections / LESSONS).
- **CSmallVector<T,N>** (12 rows: CCharString,8 resize/Clear/Copy; const CANavQuadTreeNode*,8 resize/push_back; CThingComponentSet::CEntry,8
  Copy/Allocate; NAnimationEvents::CEvent,32 erase/resize/push_back/Allocate/Copy): one shared model with the Ego_r members Start/Size
  (schar for N=8, int for N=32) and the FableWin.pdb member set (public resize(long,const T&)/push_back/erase, protected Clear/Allocate(unsigned
  long)/Copy). Lessons: `Min(n, GetSize())` with a by-value `long GetSize()` gives retail's spilled Size copy in the reference select;
  `delete[]` over CCharString inlines the cookie destroy loop; `new T[n]` over CEntry's inline ctor is the null-checked init loop, over CEvent's
  empty inline ctor a bare operator new[] (but resize still goes through `vector constructor iterator`); Clear must be declared-only in the
  CCharString resize TU (0x009D2110 is out of line); erase saves Size in a local before the delete[].
- **CVectorMap<CCharString,V,C>::LowerBound/UpperBound** (14 rows, entry strides 8/12/16/20/24): Dinkumware-style count/half loop. Inline
  CCharString compare rows need the key's data pointer copied into a local inside `if (count > 0) { K k = key; do {...} while (count > 0); }`
  and the count computed before `first = First`; CCompareString rows call CCharString::FastCompare 0x009E6530 with the key reference.
- **CVectorMap<int-key,V>::LowerBound/UpperBound** (EBodyLocation 44-byte, ETCInterfaceType 8-byte, long 16-byte; 5 rows): the landed
  0x00447C80 model verbatim (exact MATCH, no relocations).
- **CVectorMap<long,V>::GetKeyList** (0x0086D240): distinct sorted keys pushed into an STLport vector<long>; `bool first = true; long lastKey = 0;`
  plus the landed inline push_back (placement new, `_M_insert_overflow` 0x00440990 with the scalar tag in the dead argument slot).
- **CSpawnedFuncBase / CSpawnedFunc<T> / CSpawnedFunc1 / CSpawnedFunc2** (4 rows): base ctor (process base 0x00A44740(1, 0x40, 0.1f), member
  initialisers), the CSpawnedFunc<CV_ExposeMayorScript> ctor inlining it, and the SuspendableProcess bodies `(PClass->*Func)(Par...);
  StopRunning = true;` with by-value CCharString arguments built in their stack slots.
- **CTCCreatureModeManager::CModeBase::IsHoldWeaponWithBothHands** (0x0073A9F0): HasTC/GetTC (the landed CModeStrafe model) over
  CTCCarrying/CTCWeapon. Lesson: the def-slot index must be a named local (`long slot = ...GetDefGlobalIndexFromName("...")`) so the string
  temporary dies before the carry-slot call and shares the GetTC key's frame word (frame 8 -> 4).

## Parked for Fable 5.1 (0)

## Skipped
None. All 40 rows' bounds are correct (trailing int3 only).

## Label corrections
- 0x009D2660 `CSmallVector<CANavQuadTreeNode const*>::resize` -> `CSmallVector<CCharString,8>::resize` (CCharString ctor iterator / operator=, Clear 0x009D2110).
- 0x009D21D0 `CSmallVector<CThingComponentSet::CEntry>::Copy` -> `CSmallVector<CCharString,8>::Copy` (Allocate 0x009D2160, CCharString::operator=).
- 0x006FB110 `CSmallVector<CEvent>::Clear` -> `CSmallVector<NAnimationEvents::CEvent,32>::Allocate(unsigned long)` (Clear takes no argument).
- 0x00CDD450 `CSpawnedFunc<CExpression_FollowScript>` ctor -> `CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long)`.
- 0x00E95F10 -> `CSpawnedFunc<NScript::CV_ExposeMayorScript>::CSpawnedFunc(...)` (its only caller passes CV_ExposeMayorScript::DoMission 0x00E953E0).
- 0x0073A9F0 `CModeBase::vfunc_10` -> `CModeBase::IsHoldWeaponWithBothHands() const` (FableWin.pdb virtual, slot 10).
- Callee labels: 0x006A11F0 `CThing_Array_FindByID` is CTCCarrying::GetThingInCarrySlot(long); 0x00825D40 `CAFile_ReadRigidBody` and
  0x0051C890 `ResourceData_CopyIfDifferentBuffer` are CReplaceSerialise::ConvertToThisVersion; 0x00826560/0x00519630 `GetMaxSize` are the
  out-of-line CReplaceSerialise<Old6,Old5>/<Old7,Old6> levels.
- The CVectorMap<CCharString,...> value types in the manifest are propagated across byte-identical bodies (strides 8..24 under one label);
  the sources name the instance from the callers and keep the payload opaque.
