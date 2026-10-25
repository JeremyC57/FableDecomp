# Hand chunk 396 (std::sort<CThing**> .. std::vector<vector<CCard*> >::insert): 36 of 40 byte-exact

All 36 matched rows grade RELOCATION_MATCH. Each has a test that passes under both g++ (tall.sh) and the VC7.1 lane (tall.py).

## What matched (36)

### STLport algorithms and containers (22)
- **0x007A4260 `sort<CThing**, CThingCompare_NearestToPos>`** -- `if (first != last) { __introsort_loop(.., __lg(n)*2, comp); __final_insertion_sort(..); }` under /Gr.
  - Shape: the 12-byte comparator is copied into the argument area, and its words are re-read from the parameter slot. That happens only when C3DVector (pack 1) has the engine's memberwise user copy constructor and the comparator has none; with a POD C3DVector, VC caches the words in ebx/ebp.
- **0x007ACEA0 `vector<CCharString>::vector(const vector&)`** -- the `_Vector_base(n, a)` / `_STLP_alloc_proxy` model of 0x006F0810, with an out-of-line element copy constructor.
- **0x007AD180 `_Rb_tree<CCharString, pair<const CCharString,bool>>::erase(first, last)`** and **0x007AE0E0 `_Rb_tree<CCharString, pair<const CCharString, vector<CCharString>>>::_M_erase`**
  - Shape: STLport bodies with an inline `_M_increment`. The tree is a local class.
- **0x007AFA10 `__copy`**, **0x007AFA50 `__copy_backward`** (CIntelligentPointer<CThingAICreature>) and **0x007B84C0 `__copy_backward`** (CIntelligentPointer<CThingObject>, vtable 0x0125A858)
  - Shape: random-access loops over the intelligent-pointer assignment. They mirror the landed 0x004AA610.
- **0x007B4B00 `deque<CCrimeWaitingForBribe>::clear`**, **0x007B4F70 `deque::_M_push_back_aux_v`** and **0x007B5270 `back_insert_iterator<deque>::operator=`**
  - Layout: start +0, finish +0x10, map +0x20, map_size +0x24; 128-byte buffers.
  - Callees: `_M_reallocate_map` 0x007B4980, then malloc(0x80).
- **0x007B4F20 `back_insert_iterator<vector<CCrimeWaitingForBribe>>::operator=`** and **0x007CA350 `vector<CDefPointer<const CDefClassBase>>::push_back`**
  - Shape: the empty `__false_type` tag is homed on the value's live argument slot. VC7.1 does that only with a same-TU `__declspec(noinline)` `_M_insert_overflow` stand-in that reads x and leaves the tag parameter unnamed.
- **0x007B5030 `__copy(deque range -> back_inserter(vector))`**, **0x007B51B0 `__copy_aux`** and **0x007B5400 `__copy(const T* -> back_inserter(deque))`**
  - Calling convention: /Gr __fastcall with the hidden back_insert_iterator result in ecx (it has a user ctor). When the first fastcall-eligible argument after the hidden pointer is the tag, the tag reference rides in edx.
  - The deque iterator needs a memberwise user copy constructor.
- **0x007CE1F0 `__copy<CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing>>>, int>`** (first grade)
  - Shape: `*result = *first` converts through a temporary CIntelligentPointer<CThing> and inlines vector::push_back. The overflow tag gets its own frame byte, so no stand-in is needed. The intelligent pointer is pack(1) (the GiveItems model, `and esp,-8` frame).
- **0x007D22F0 `__uninitialized_copy<const CShoveHaPennyRegionDef*>`** (first grade) -- placement-new null test and the implicit copy constructor (out-of-line CDefClassBase copy 0x0046644C, vptr 0x0124383C, two floats).
- **0x007D78A0 `vector<CCard*>::insert(pos, x)`** and **0x007DC750 `vector<long>::insert(pos, x)`** (first grade each)
  - Shape: STLport 4.5 POD insert, with `__copy_trivial_backward` as memmove and `_M_insert_overflow(pos, x, __true_type(), 1UL)` (default `atend = false`). The tag is homed on x's slot without a stand-in.
- **0x007D9B40 `random_shuffle<KnickKnack*>`**
  - Shape: STLport loop, `rand() % ((i - first) + 1)`, value-type swap.
  - Needs the memberwise C3DVector copy constructor. With POD C3DVector, VC strength-reduces differently: no `-4 - first` spill, and the temporary's words are cached the other way (163v159).
- **0x007DC500 `__uninitialized_copy<const vector<CCard*>*>`** and **0x007DC580 `__uninitialized_fill_n<vector<CCard*>*, unsigned, vector<CCard*>>`** (first grade each)
  - Shape: the inline STLport vector copy constructor (`_Vector_base(n, a)`, allocate = malloc or null) with the trivial `__uninitialized_copy` (`__copy_trivial` memmove).
- **0x007DCB10 `vector<vector<CCard*>>::insert(pos, x)`** (first grade)
  - Shape: the non-POD insert. The element copy constructor (0x007DC350) and operator= (0x007DC3F0) are out-of-line declarations; the copy's destructor is the inline `if (start) free(start)`.
  - Calls `__copy_backward_ptrs` -> fastcall `__copy_backward(.., random_access_iterator_tag(), (int*)0)` at 0x007DC600. Both empty tags are homed on x's slot.

### Serialisation (3)
- **0x007AD270 `GFSerialiseMapBinaryOut<bool, CCharString>`** -- `engine/CDataOutputStream.h`; inline `_M_increment`. The key traits' DefaultVal is the `push ecx` word.
- **0x007B3B80 `GFSerialiseVectorBinaryOut<CCrimeWaitingForBribe>`** -- the operator[] is written as `*(begin()+n)`.
- **0x007B5370 `GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe>`** -- a same-TU noinline `CPersistTraits<T>::TransferBinaryIn` body lets the traits share the dead count slot.

### Components (8)
- **0x007A44C0 `CTCWillEnflame::CancelMove`** -- out-pointer GetInterface(0xA7). Only ability 3 is cleared; then RemoveTC(0x95).
- **0x007AA4D0 `CTCAssassinRush::FinalCleanup`** -- the alive test goes through an inline `bool IsDead() const { return (flags & 1) != 0; }`, which keeps 1 in ebx.
- **0x007C2800 `CFilter_Bind3<CThingFilter_IsNot, (anon)::CThingFilter_ShotsToCheckForCollisionWith, CThingFilter_SolidToThing>::operator()<CThing>`** (first grade)
  - Shape: `(*F1)(t) && (*F2)(t) && (*F3)(t)` with the stateless shots filter `__forceinline`. It reads ThingTypeFlags and PhysicsTC into locals before `NCreatureMode::IsToEvadeAllHits`.
  - The same body is inlined in GetAllThingsInRoughArea 0x007C2D10.
- **0x007C3EB0 `CTCHealLife::GetLevelEffectName`** (first grade) -- the named CCharString is LEVEL1 by default.
  - The switch was checked by hand against the retail pushes: 1 -> LEVEL2, 2 -> LEVEL3, 3 -> LEVEL4.
  - The result is copied out (no NRVO).
- **0x007C53F0 `CTCGhostSword::IsThingGhostSword`** (static __fastcall)
  - The name must be a named local in its own block. A temporary in the call passes the ctor's eax instead of re-taking the address (72v76).
- **0x007CAB30 `CTCHeroReceiveItems::GiveItem`** (first grade) -- the one-item form of the landed GiveItems 0x007CA9E0.
- **0x007CABD0 `CTCHeroReceiveItems::AddItemToInventory`**
  - Shape: both lookups `__forceinline` (the item uses the plain GetInterface; the inventory uses the range-checked `bitset<274>::test` form with puts+abort). Kill(false) is written in both else arms; that gives the shrink-wrapped prologue (203v217 -> match).
- **0x007CB850 `CTCHeroReceiveItems::SetItemAsPickedUp`** (first grade)
  - Shape: `if (find(begin, end, item) == end) push_back(item)`. Both tags (find's random_access_iterator_tag and the overflow's __true_type) are homed on the live `item` slot through same-TU noinline stand-ins.

### Free functions (2)
- **0x007D7A90 `GetHaltonSequenceNumber(int, int)`** (first grade) -- `float invBase = 1.0f / base` (fst) and a running frac on the x87 stack.
- **0x007D7AE0 `GetNextHaltonSequenceNumber(float&, int)`** (first grade) -- `double h = 1.0; while (h >= 1 - value) h *= 1.0 / base; value += (base + 1.0) * h - 1.0;`.

## Parked for Fable 5.1 (4)
- 0x007AF940 std::__uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*> DIFFER(62v62): register tie: retail keeps x in edi and n in ebx, ours swaps them (same instructions otherwise) | tried: plain placement new, `_Construct`, a count local, a const-ref local (4 grades) | next: compare with 0x007DC580 (same STLport loop, matched first grade over a vector element) -- the intelligent-pointer copy (base ctor + vtable + GetPItem/SetPItem calls) may need the 1-aligned GiveItems model; else lane gap: register allocation
- 0x007B8530 CTCTavernTable::SetSeatState DIFFER(57v57): `mov edx,[ecx+34h] ; mov eax,[ecx+38h] ; sub eax,edx ; mov edx,[esp+4]` vs retail `mov eax,.. ; mov edx,.. ; sub edx,eax ; mov eax,[esp+4]` (seat/size register swap; p = seat + begin in retail) | tried: `size() > seat`, `seat < size()`, `*(begin() + (int)n)` (3 grades, scheduling-only) | next: an index-first operator[] (`n + begin()`) or the bool-vector type of the real header; possibly a lane gap: register allocation
- 0x007B8AA0 CTCTavernTable::FreeSeat DIFFER(112v111): retail pushes esi in the prologue and keeps the seat start in esi across the size check (`mov esi,[edi+34h] .. mov ecx,esi ; add eax,ecx`); ours shrink-wraps `push esi` into the GetInterface hit block and re-reads `[edi+34h]` | tried: inline seat-free, STLport `end() - begin()` size, auto-inlined `SetSeatState(seat, false)`, explicit `bool* seats = begin()` local (4 grades) | next: match SetSeatState first (same TU; FreeSeat probably inlines it) and retry with that operator[]
- 0x007DB5C0 CTCTavernGameCardBlackjack::Split DIFFER(237v224): retail has no destructor for the empty hand passed to `PlayersCards.insert` (no reload/free after the call, `test ecx,ecx` on the index, immediate zero stores); ours always frees the temporary, which costs an extra ebx push and keeps `&Bets` live | tried: temporary `vector<CCard*>()`, const local, block-scoped const local, countdown loop + local hand pointers + STLport `__copy_trivial` erase (4 grades) | next: the `insert(iterator)` one-argument overload or a vector whose destructor the TU cannot see (out-of-line `~vector` would show a call, so look for a by-value/static empty hand); the rest (inline push_back from the next hand, erase via memmove, Bets.insert, ReorderCards) is already in place

## Skipped
None. No row contains a second function body.

## Label corrections
| Address | Catalog label | Real identity |
|---|---|---|
| 0x007A4260 | MakeHeap_AndSort | `std::sort<CThing**, CThingCompare_NearestToPos>` |
| 0x007A44C0 | CTCEnflame_ResetState | `CTCWillEnflame::CancelMove` |
| 0x007AA4D0 | CTCInventory_OnDeactivate | `CTCAssassinRush::FinalCleanup` |
| 0x007ACEA0 | Vector_Copy | `vector<CCharString>` copy ctor |
| 0x007AD180 | LTextBinTree_EraseRange | `map<CCharString,bool>` tree `erase(first, last)` |
| 0x007AD270 | LTextBinTree_SerializeNode | `GFSerialiseMapBinaryOut<bool, CCharString>` |
| 0x007AE0E0 | List_DestroyRecursiveWithFree | `_Rb_tree::_M_erase` of `map<CCharString, vector<CCharString>>` |
| 0x007AF940 | _Fill_n<..> | `__uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*>` |
| 0x007AFA10, 0x007AFA50, 0x007B84C0 | CopyArray_* | STLport `__copy` / `__copy_backward` (intelligent pointers) |
| 0x007B4B00 | CharacterBuffer_Reset | `deque<CCrimeWaitingForBribe>::clear` |
| 0x007B4F20 | std_pair_InsertAt | `back_insert_iterator<vector<..>>::operator=` |
| 0x007B4F70 | StdVector_PushBack | `deque::_M_push_back_aux_v` |
| 0x007B5030 | Vector_InsertRange_8byte_Deque | `__copy` deque -> vector inserter |
| 0x007B51B0 | Fractal_InitDensityMap | `__copy_aux` |
| 0x007B5270 | std_pair_PushBack | `back_insert_iterator<deque<..>>::operator=` |
| 0x007B5370 | GFSerialiseVectorBinaryIn<CDefString> | `GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe>` |
| 0x007B5400 | Vector_InsertRange_8byte_StdVector | `__copy` pointer -> deque inserter |
| 0x007B8530 | BitArray_SetBit | `CTCTavernTable::SetSeatState` |
| 0x007B8AA0 | CSeatingArea_FreeSeat | `CTCTavernTable::FreeSeat` |
| 0x007C2800 | CThingFilter_Pred_IsNot_IsEvading | `CFilter_Bind3<CThingFilter_IsNot, (anon)::CThingFilter_ShotsToCheckForCollisionWith, CThingFilter_SolidToThing>::operator()<CThing>` |
| 0x007C3EB0 | CThing_GetHealingEffectName | `CTCHealLife::GetLevelEffectName` |
| 0x007C53F0 | IsThingGhostSword | `CTCGhostSword::IsThingGhostSword` (static __fastcall) |
| 0x007CA350 | vector_PushBack_CCountedPtr | `vector<CDefPointer<..>>::push_back` (CDefPointer, not CCountedPointer) |
| 0x007CAB30 | TCHeroReceiveItems_AddItemToInventory | `CTCHeroReceiveItems::GiveItem` |
| 0x007CABD0 | Entity_OnItemSpawned | `CTCHeroReceiveItems::AddItemToInventory` |
| 0x007CB850 | std_vector_Insert | `CTCHeroReceiveItems::SetItemAsPickedUp` |
| 0x007CE1F0 | Vector_InsertElements_SmartPtr | `__copy<CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing>>>, int>` |
| 0x007D22F0 | _Copy_backward<CBrainUpdateZone*,..> | `__uninitialized_copy<const CShoveHaPennyRegionDef*, ..>` |
| 0x007D78A0 | vector_PushBack_uint | `vector<CCard*>::insert(pos, x)` |
| 0x007D9B40 | std_random_shuffle_16byte | `random_shuffle<KnickKnack*>` |
| 0x007DB5C0 | CTCTavernGameCardBlackjack_DealCards | `CTCTavernGameCardBlackjack::Split` |
| 0x007DC500 | std_vector_DeepCopy | `__uninitialized_copy<const vector<CCard*>*, ..>` |
| 0x007DC580 | Vector_Allocate_Triplets | `__uninitialized_fill_n<vector<CCard*>*, unsigned, vector<CCard*>>` |
| 0x007DC750 | vector_PushBack_uint | `vector<long>::insert(pos, x)` |
| 0x007DCB10 | vector_PushBack_ComplexElement | `vector<vector<CCard*>>::insert(pos, x)` |

Callee and neighbour labels found wrong along the way:
| Address | Catalog label | Real identity |
|---|---|---|
| 0x007D23B0, 0x007D24F0 | "*_CBrainUpdateZone" | `vector<CShoveHaPennyRegionDef>` growth paths |
| 0x007DC350 | std::vector<uint>::vector | `vector<CCard*>` copy ctor |
| 0x007DC3F0 | Vector_Assign_FromOther | `vector<CCard*>::operator=` |
| 0x007DC600 | _Copy_backward<CSeaBodyDesc*,..> | `__copy_backward<vector<CCard*>*, ..>` |
| 0x007DC9A0 | Vector_InsertRange_ComplexElements | `vector<vector<CCard*>>::_M_insert_overflow` |
| 0x00440990 | std::vector::InsertMultipleAndGrow | `vector<long>::_M_insert_overflow` |
| 0x007B52B0 | GenerateNewFractal | STLport `copy()` deque -> back_inserter(vector) |
| 0x007B3760 | Save_TransferCriminalRecord | the CTCGuard bribe-record transfer |
| 0x00661640 | CThingFilter_IsNot::operator() | `CThingFilter_SolidToThing::operator()` (`thing != SourceThing`). This is the F3 call of the Bind3, so the IsNot name is at best a shared body. |
