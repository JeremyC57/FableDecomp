# Hand chunk 335 (ForEachIf .. GetAllLeafThings): 38 of 40 byte-exact

All 38 sources in `src/` grade MATCH (9) or RELOCATION_MATCH (29) in `grades.txt`. Every test in `test/` prints PASS under g++ (`tall.sh`) and under VC7.1 in Wine (`tall.py`). `finish.py` wrote `payload.json` and `oracle.tsv`.

## What matched (38)

- **STLport `__uninitialized_fill_n` (13 rows, all labelled "GFExplicitNew<N3DPrimitive::CAnimatedBlock>")**
  - Rows: 00513a00, 00543870, 00720310, 00769000, 0076f7d0, 00776bc0, 0077e2e0, 0088ac20, 009e62e0, 009f6c90, 00af2c00, 00b0f6b0, 00b28f60.
  - What it does: copy-constructs n copies of x into raw storage and returns first + n (`__fastcall`, `ret 8`).
  - Shape lesson: the element needs a memberwise **user** copy constructor, and it must hold a member record that has its **own** user copy constructor. That record is a 3-long record, or a colour, a string pair or a polymorphic member. With both in place, VC7.1 keeps the second induction register (`lea esi,[eax+N]`) that points into the record.
  - Per-row details:
    - 00513a00: really is a 13-byte packed element (stride 0x0d).
    - 00720310: copies its colours through an inline `Set(r,g,b,a)` member.
    - 009f6c90: its 16-byte member is a constructor-less POD (implicit block copy).
    - 00b0f6b0: its polymorphic member stores the vtable inside the copy constructor.
- **GFExplicitNew<N3DPrimitive::CStaticBlock> (00a94150)**
  - The label is genuine; the caller is C3DPrimitive2::LoadBinary.
  - Loop shape: `for (i < n) new (&p[i]) T;`.
  - Model: CBlockBase holds zeroed start/count/flag bytes; CStaticBlock zeroes its fixups and textures and sets MaterialIndex to -1.
- **GFSerialiseMapBinaryIn / GFSerialiseMultiMapBinaryIn (9 rows)**
  - Rows: 004ec66c, 00469381, 0045df5e, 0045ec72, 0045e37f, 0043151d, 00463732, 00472896, 00475aa6.
  - What it does: clears the map, reads the count, then reads a key and a value and inserts the pair, `count` times.
  - Shape lessons:
    - The TU uses `#pragma optimize("y", off)` + `("s", on)`.
    - The key and value traits are empty objects declared at function scope.
    - Each out-of-line traits call gets a same-TU `auto_inline(off)` + `__declspec(noinline)` stand-in, so VC7.1 homes the empty object in retail's live slot.
    - The key traits of 0045df5e and 0045ec72 carry a value-initialised DefaultVal (`and [ebp-0Ch],0`).
    - The string traits call the stream's slot 6 `ReadNullTerminatedString` directly through a vtable view. An inline wrapper adds a `lea`.
    - The out-of-line `pair()` of 00469381 is declared but not defined.
  - Model: `engine/CMemoryDataInputStream.h` plus a methods-only reader adapter.
- **GFSerialiseVectorBinaryIn / Out (6 rows)**
  - Rows: 005c2ad8, 006194a0, 005c12a1, 00614620, 006c7ba0, 006d3a30.
  - Shape lessons:
    - The in direction is `resize(count)` followed by a traits read into `*(begin()+i)`. The traits object holds a DefaultVal.
    - The CUIMiniMapMarker rows are speed TUs. The marker constructor is `Region(0), UIDefName("", -1)` plus body zeroes.
    - The pair<long, COpinionDeed*> writer uses function-scope pair traits and a **named** inner `secondTraits` local.
    - The CNamedLong traits builds its DefaultVal by implicit conversion (`DefaultVal("", 0)`).
  - Model: `engine/CDataOutputStream.h` / `engine/CMemoryDataInputStream.h` with adapters.
- **GFWriteVectorMapMemDump (0086b210)**
  - Writes the count, the byte size, the raw entries (or `this` when the map is empty) and the Dirty flag.
  - Write is the stream's slot 4, reached through a vtable view.
- **std::remove_copy_if<C3DVector*, C3DVector*, CIsOutsideRadius> (008c2ed0)**
  - The length goes through an inline `float sqrtf(float)` over the sqrt intrinsic.
  - The predicate uses a named `offset` local and a memberwise user copy constructor.
- **std::map<unsigned long, CAllocationNode*>::insert (0042d1a3)**
  - Built under `optimize("s", on)` as an out-of-class definition plus an explicit instantiation.
  - The pair has user constructors.
- **CEnginePrimitiveRenderer::AddRepeatedStaticMesh (00b32f70)**
  - Forwards to `RepeatedStaticMeshManager->CreatePrimitive(desc)`.
  - The handle has a user copy constructor and destructor.
  - Uses a local model, because the header puts the manager 4 bytes high.
- **SRecord copy constructor (009922f0)**
  - Copies two CCountedPointer members (count bump guarded by `if (Info)`) and four CCharString members.
  - The constructor is defined out of class.
- **CThingSearchTools::GetAllLeafThings (4 rows)**
  - Rows: 006186f0, 00618790, 00964ef0, 006ae830.
  - Shape lessons:
    - The vector instances call `push_back((TThing*)thing)` with an rvalue, so the value lands in the dead container argument slot.
    - A same-TU noinline `_M_insert_overflow` stand-in plus a `__true_type` with a user constructor homes the tag in the filter slot.
    - The list instance (006ae830) is `insert(end(), x)` with `_M_create_node` (malloc 0x0c) and a copy-constructed iterator. This is the landed 0x004ACF90 model.
    - `CFilter_Bind2` holds two filter pointers, and its `operator()` is inline.

## Parked for Fable 5.1 (2)

- 0080e5d0 ForEachIf<CThing**,CThingFilter_IsThrownByExplosion,CApplyAdditionalExplosiveImpulseIfInsideRing> DIFFER(158v158): retail `mov eax,[eax+44h] ; mov ecx,edi ; sub ecx,eax ; sar ecx,3 ; test ecx,ecx ; mov esi,eax` vs ours `mov esi,[eax+44h] ; ... sub ecx,esi` (plus the loop-align pad `lea ebx,[ebx]` 6 bytes vs `lea esp,[esp]` 4) | tried: plain inline LowerBound/GetTC/filter (left out of line), __forceinline LowerBound, +GetTC, +filter operator() | next: STLport free `__lower_bound(first, last, val, less<long>(), (long*)0)` with `_Distance len = distance(first, last)` in its own variable (the length is taken from the loaded first before it is copied into the walking pointer)
- 004ad700 GetAllLeafThings<const CThingBuilding,vector<const CThingBuilding*>,CThingFilter_None> DIFFER(121v121): thing held in ecx in retail (`mov ecx,[edi+8]`, not-copy in dl, `lea ecx`/`lea edx` swapped at the overflow pushes) vs edx in ours (same instructions otherwise) | tried: named CThing* local, TThing* local, re-reading `*it`, ThingCast<T> temporary + filter by reference | next: lane gap: register naming (the same template with real filters, 006186f0, matches; perhaps a filter that is a real out-of-line-free class with a data member, or a different CThing scan accessor shape)

## Skipped

None.

## Label corrections

The labels below are propagated across byte-identical bodies, so they are wrong for these rows.

- 00513a00, 00543870, 00720310, 00769000, 0076f7d0, 00776bc0, 0077e2e0, 0088ac20, 009e62e0, 009f6c90, 00af2c00, 00b0f6b0, 00b28f60:
  - Catalog: "GFExplicitNew<N3DPrimitive::CAnimatedBlock>".
  - Really: STLport `__uninitialized_fill_n` for 13 different element types (the callers are vector fill/insert paths).
- 009922f0:
  - Catalog: "GFPredicateAnd<CIsThingAlive,CIsThingAlive>".
  - Really: the copy constructor of a 0x20 record {CCountedPointer A, B; CCharString S0..S3}.
- 0042d1a3:
  - Catalog: "GFPredicateAnd<...>".
  - Really: `std::map<unsigned long, CAllocationNode*>::insert`.
- 00b32f70:
  - Catalog: "GFPredicateAnd<...>".
  - Really: `CEnginePrimitiveRenderer::AddRepeatedStaticMesh`.
- 00469381:
  - Catalog: <CDefString, EObjectAugmentationType>.
  - Really: `map<long, CBlendedParticleEffectSet>`.
- 0045df5e:
  - Catalog: <bool, EOpinionAttitudeType>.
  - Really: key EOpinionReactionType.
- 0045e37f:
  - Catalog: <bool, EOpinionAttitudeType>.
  - Really: key EOpinionDeedType (COpinionDeedMaskDef::DeedEnabled).
- 0043151d:
  - Catalog: <pair<long,long>, CCharString>.
  - Really: `map<CCharString, long>`.
- 00463732:
  - Catalog: <pair<long,long>, CCharString>.
  - Really: `GFSerialiseMultiMapBinaryIn<long, CCharString>` (a multimap).
- 00472896:
  - Catalog: <pair<long,long>, CCharString>.
  - Really: `map<long, CCharString>`.
- 00475aa6:
  - Catalog: <pair<long,long>, CCharString>.
  - Really: `map<CCharString, EGameAction>`.
- 008c2ed0:
  - Catalog: "Geometry::FilterPointsWithinRadius".
  - Really: `std::remove_copy_if<C3DVector*, C3DVector*, CIsOutsideRadius>`, called by remove_if 0x008C34A0.
- 00964ef0:
  - Catalog: <CThingAICreature, ..., CFilter_Bind2<EnemyCreature, InMap>>.
  - Really: `GetAllLeafThings<CThingMarker, vector<CThingMarker*>, CThingFilter_MarkerHasProperty>`, with PeekTypeList(0x0b).
- 006ae830:
  - Catalog: <CThingBuilding, vector<CThingBuilding*>, None>.
  - Really: `GetAllLeafThings<CThingAICreature, list<CThingAICreature*>, CThingFilter_None>`.
- 004ad700:
  - Catalog filter: CThingFilter_HasTC.
  - Really: the body has no filter call, so CThingFilter_None.
- 006186f0:
  - Catalog first filter: CThingFilter_EnemyCreature.
  - Really: its callee 0x006A80A0 is the HasTC component-bitset test.
- Callee labels:
  - 0x006A80A0: labelled "Bitset_Test", really CThingFilter_HasTC::operator().
  - 0x0073A6E0: labelled "CPixelShader::CountReferences", really a physics-TC getter (`[[this+0Ch]]+8`).
  - 0x006A7550: labelled "_Cons_val<...>", really a thing filter's operator() (`edx=[this] ; ecx=thing ; call 0x0076D2F0`).
