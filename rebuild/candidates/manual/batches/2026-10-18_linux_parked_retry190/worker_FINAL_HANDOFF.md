# retry202H final handoff

All four original202 rows are worker READY. None is represented as root accepted at this freeze. Root confirmed reservation in ROOT_RESERVATION.json before any grade; independent review and landing remain pending. No parked/skipped/current DIFFER rows and no new section started.

## History, immutable artifacts and budgets

Selection evidence covers all40 original202 rows and all7 untouched alternatives: ALL_ORIGINAL202_AUDIT.json, ROOT_HISTORY_SEARCH.json, HISTORY_AUDIT.tsv, SELECTION.md, ROOT_RESERVATION.json. Proposed four only had original comment-only placeholders and identical pending202 archives; no later authored implementation, reservation or canonical source. Four original UNCERTAIN reservations per row remain preserved. None was reset or mislabeled as proven compiler attempts. Preparation script now refuses rerun after attempts to prevent accidental ledger resets.

Exactly12 new source grades and12 immutable source-grade snapshots:00A699D0=2,006FA3F0=2,00820790=3,008209C0=5. No precompiler infrastructure retries or hidden candidate compiler failures in this section. Every ledger source/hash/fullcommand/effectiveflags was frozen before invocation. All source/hash checks pass at freeze. No rows exhausted10; unused allowance is not permission to keep grading already exact READY rows.

| Address | Bytes | New grades | UNCERTAIN prior | Reserved + new | Remaining to10 | Status |
|---|---:|---:|---:|---:|---:|---|
|00a699d0|169|2|4|6|4|RELOCATION_MATCH; hostO2/noelide/nativePASS|
|006fa3f0|236|2|4|6|4|RELOCATION_MATCH; hostO2/noelide/nativePASS|
|00820790|161|3|4|7|3|RELOCATION_MATCH; hostO2/noelide/nativePASS|
|008209c0|193|5|4|9|1|RELOCATION_MATCH; hostO2/noelide/nativePASS|

All compiler grades use baseline /O2 /Oy /W3. Source006FA3F0 auto_inline(off) protects genuine out-of-line native LowerBound, then auto_inline(on) restores candidate caller compilation. It is not an opaque fabricated dependency. Fixtures retain actual expanded production bodies. Brush fixture replaces only the external allocation/free import names to observe real malloc/free; no loop or branch logic is duplicated. Complete commands and frozen fixtures reside in each fixture snapshot directory. Native offsets/PMF4 asserts retained; host uses full pointer/PMF widths.

## 00a699d0 CManagedTextureData::CreateVolumeTexture

Source `src/00a699d0.cpp`, SHA256 `0076c57dbec5927cf5c59ec8317d1ea6303f15b3f720698e27ad878d64e796fc`.

Expanded fixture `test/00a699d0.cpp`, SHA256 `333a749ae0961cf1ba43d908b40ff60b57f45059871c6e708ded9cf1c589ff4d`.

Exact parity source `snapshots/00a699d0_02.cpp`; behavior snapshot `snapshots/00a699d0_fixture_texture2`. O2/noelide/native logs all PASS. Audit `evidence/00a699d0_relocation_audit.json`; emitted native COFF and full objdump relocations retained.

Actual CManagedTextureData/CTexture/CPixelFormat headers retained and expanded. No CVolumeTexture/CBaseClass headers exist including quarantine. Genuine one-slot polymorphic base/derived constructors instantiated by native-sized scalar new16 and fixture; actual emitted destructor signatures/vtables independently audited. ByteLength remains uninitialised as native and never read by fixture. Exact production method executes; external rounding/volumeInitialise/countReset dependencies are fixture stubs observing inputs/constructor fields/replacement. Fixture covers six successful allocations, true/falseInitialise, rounded dimensions and ushorttruncation, ignored callerlevels, depth, formatidentity, resetoldobject through callback, final virtualdelete. Native method dereferences newly reset volume unconditionally: allocator and countblock success are genuine external preconditions; no OOM promise. Fixture countReset is a dependency stub, not a claim to reproduce all nativeResetfailure semantics.

## 006fa3f0 NAnimationEvents::CManager::AddEventToAnim

Source `src/006fa3f0.cpp`, SHA256 `65882b275de9c16e1672867aa3daf0d93c41de807a977b43665fa203f2a463db`.

Expanded fixture `test/006fa3f0.cpp`, SHA256 `e617395d3bb232944538c55a4bf4273f11ec6a9774f11bf5159dda441d54ac67`.

Exact parity source `snapshots/006fa3f0_02.cpp`; behavior snapshot `snapshots/006fa3f0_fixture_events1`. O2/noelide/native logs all PASS. Audit `evidence/006fa3f0_relocation_audit.json`; emitted native COFF and full objdump relocations retained.

Actual sharedCManager header retained; NAnimationEvents manager/Event/vector/map template headers absent inclquarantine. Explicit native data projection backed byPDB with nativeoffset/size assertions. Genuine owning EventVector constructors/copypair/destructors execute, nativearraydelete[] independentaudit; full emitted constructor/dependency signatures saved. Exact production method and genuine LowerBound82byte implementation execute, no duplicate fixtureLowerBound. ExternalAllocate/Copy/resize/PushBack/sort are dependencydoubles preserving ownedeventarrays; not full allocator/engine algorithms. Fixture12 keyinsert/existingcases plus emptymap/alreadydirtyexisting: unsignedkeys0/1/7/80000000/FFFFFFFE/FFFFFFFF, before/middle/end insertion, NaNTimebits/bitfields, originaleventsretained, resizecallbackmodifiesinput before candidatecopy, temporaryarraydeletecallbackcan clearDirty so sortskip honored; genuine deepcopies/arraydeleteeveryallocation/noelide. Nativeexternalresize defaultfill event uninitialised bydesign and immediately overwritten; fixture deliberately does not inspect uninitialisedfillpayload. Successful dependent allocations and valid sourceevent acrossresize are actual preconditions. No virtualhierarchy or externalengineconstructor claim; method has no virtualcalls.

## 00820790 CMap::ScriptedMapBrushAddThings

Source `src/00820790.cpp`, SHA256 `8abb21a8e19f0209f04a1f49849af9c0e6611ff4aed9abf8340191a89486ac9a`.

Expanded fixture `test/00820790.cpp`, SHA256 `d719366c398f905a34e2b49dbea066b507e7690b7316e57bee4abe7d399e8240`.

Exact parity source `snapshots/00820790_03.cpp`; behavior snapshot `snapshots/00820790_fixture_brush3`. O2/noelide/native logs all PASS. Audit `evidence/00820790_relocation_audit.json`; emitted native COFF and full objdump relocations retained.

Actual quarantined CMap/CThing plus CWorld/CWorldMap/CScriptedMapBrush headers retained/expanded; native mapWorld20 vs donor2C. ScriptedMapBrushAddThings identity donor67B630 exactconstbrushref. Native linkedUID64list sentinel16 and eachnode owned with CRTmalloc/free, exact constructor/clear/destructor execute; allocation-success precondition nativeunchecked. External brush/lookup/activation methods stubbed; ten fixtures(0..4UIDs x callbackWorldswitch), full64bit IDs includingFFFFFFFFFFFFFFFF/8000000100000002, missingthings/live/deadreturned all activated, manager capturedonceevenempty, allnodes/sentinel freedonce and sentinelresetbeforefree. Getter uses typed nativePMF4/fullhost; actual engine constructors/full hierarchy not claimed. Original CThingcallee label independently correctedSetAsActivated, notGameStatePersisted; allnativeglobals/relocs allocatoridentities audited.

## 008209c0 CMap::ScriptedMapBrushRemoveThings

Source `src/008209c0.cpp`, SHA256 `9ace593110f9bc7205c5284286443d81d295d763cca26716e7f29de3a9e55096`.

Expanded fixture `test/008209c0.cpp`, SHA256 `c3113427c4a4e03360e653394ea4868b795c25ef7bdd9b6dbebfe5c8814b276a`.

Exact parity source `snapshots/008209c0_05.cpp`; behavior snapshot `snapshots/008209c0_fixture_brush4`. O2/noelide/native logs all PASS. Audit `evidence/008209c0_relocation_audit.json`; emitted native COFF and full objdump relocations retained.

Actual quarantined CMap/CThing plus CWorld/CWorldMap/CScriptedMapBrush headers retained and expanded. Native projections mapWorld20/WorldPos1D24/ThingDead91bit0 PDBbacked; allPMFs4 native/fullhost stride asserted. Exact UIDList ctor/iteration/clear/dtor execute, importedmalloc/free tracked once pernode/sentinel and reset-beforefree. Ten fixtures0..4UIDsxcallbackWorldswitch: high64UIDs, nullthings, deadskip, Kill(false)onlylive, GetThingManagerreloaded perUID aftercallbackswitchesWorld, PeekWorldMap/GetMapNumberAt evenemptylist with actualWorldPosreference. Actualengineconstructors/fullhierarchy/dependencyalgorithms not claimed; doubles genuine own constructors/signatures. UIDlist allocation success externalprecondition originalunchecked. No duplicate candidate logic in tests; only externalimports/helpers stubbed. Grades1..3 same-length scheduling issue, grade4 namedUIDlocal extra register topology, grade5 naturalGetWorld().GetPThingWithUID() nestedinlinegetter matches exactly.

## Recovered original function and variable names

00A699D0 original CManagedTextureData::CreateVolumeTexture(ulong,ulong,ulong,long,constCPixelFormat&) from Ego_rPDBB7D0E0. Actual shared CManagedTextureData header owns Width/Height/VolumeTexture/Allocated names and exact native offsets1C/1E/28/48. Actual CTexture and CPixelFormat headers retained. CTexture::GetNextValidDimensions(constC2DExtentsI&,constCPixelFormat&) fastcall has PDBB6A3C0; CVolumeTexture::Initialise(constC3DExtentsI&,long,constCPixelFormat&,ulong,EVolumePool,bool) PDBBE9830. Native signature consumes six arguments: dims,1,format,0,1,false. Caller levels argument is unused. PDB Volume fields PD3DVolumeTexture4/ByteLength8/PDataC, native ctor clears4/C and leaves8 uninitialised; tests never read8. CBaseClass identity PDBB01F20; native99A2D0 installs1231710. Genuine derivedVolumeVT129DC18/one destructor slotA69590; fixture actual base/derived emitted vtables each exactlyone slot. Enumtype EVolumePool is proven, VolumePoolManaged enumerator spelling is descriptive; only numeric1 established. Source uses native counted-pointer reset projection atA6AE60, dependency stub does not claim complete native OOM behavior.

006FA3F0 original NAnimationEvents::CManager::AddEventToAnim(ulong,constCEvent&) from PDB71F700. PDB exact CEvent bitfields Index30/ExternallyImported1/StopEvent1 and Timefloat4; SmallVector Start0/Size4; manager AnimToExtraEventsMap and DifferentToSavedCopy names, native2C/3C vsdonor34/48. EventVector/EventPair/EventMap/SortEventPairs are readable native-template projection aliases for CSmallVector<CEvent,32>,std::pair<ulong,...>,CVectorMap<ulong,...>,std::sort; no claim those aliases were original source spelling. NativeUnsignedLowerBound6FAF20 is fully reproduced82bytes unmasked, not replaced by fixture logic. Allocate6FAFE0(PDB720690),Copy6FB5C0(PDB7206F0),resize6FADE0(PDB720540),PushBack6FCD90,fastcallSort6FD380 signatures audited. All8 direct relocations audited; both temporary destructors call true CRTarraydeleteBFEB1C and source declares operatordelete[] explicitly. Actual source constructs temporary vector, owns pair deepcopies, destroys temporaryarrays before conditionalSort; arraydelete callback can clearDirty and skipSort, verified.

00820790 original CMap::ScriptedMapBrushAddThings(constCScriptedMapBrush&) PDB67B630; GetThingsToAdd(list<ulong64>&)const PDB7355A0. List nodes native16 =Next0/Previous4/UID64at8. World member native20 corresponds donor2C; world virtualslot3=49E1B0 loadsPThingManager50 and PDB4A7360 names GetThingManager() returning mutable reference. Direct lookup51ED80 takes ulong64 byvalue/ret8 and looks through tree58 to node18, matching PDBGetPThingWithUID529000; propagated CStopWatch::GetTicks is false. Existing canonical4C7770 label SetAsGameStatePersisted is false: native writes91bit3, PDB CThing Dead/Activated byteA1 bits0/3 after native-10layout, and exact PDBSetAsActivated(bool) donor4C9330/fablewin1CF9A60. PDB GameStatePersisted is a different byteA2bit5. Only nativecaller8207E9 passestrue; companion8217EE passesfalse. Worker corrected candidate function name undergrade3 and notified root; canonical source untouched. Fields/methods activated, not persisted, are the actual behavior.

008209C0 original CMap::ScriptedMapBrushRemoveThings(constCScriptedMapBrush&) PDB67B900. GetThingsToDelete(list<ulong64>&)const PDB735500. CMapWorldPos native1D24 vs donor1D34 and CThingDead native91bit0 vs donorA1bit0 independently match native use. NativePeekWorldMap worldslot13=4AE8C0 exact constreference signature is fablewin1B34C80; WorldMapslot17=4FC080 is GetMapNumberAt(constC2DCoordI&)const PDB4CEAE0. Its result is discarded but call is preserved and tested even for an empty list. CThing::Kill(false)4C9B80 only for nonnull nondeadthings. GetWorld/GetPThingWithUID forwarding helper names are descriptive native projections, exact original inline spelling unestablished. Native-by-value64 UID evaluation and liveWorldgetter reread preserved; callbacks switchingWorld prove remove differs from cached add behavior.

## Source iteration history and lessons

00A699D0 grade1 167v169 used wrong inferred pool/bool operands and no named volume local; native pushes independently proved pool1/boolfalse, and named volume read before argument construction reproduces169 in grade2. All signatures/vtable/allocator identity audits happen in addition to masked matching.

00820790 grade1 163v161 stored Head before sentinel links, requiring reload. Native constructor must build sentinel via localh, linkh->Next/Previous, then assignHead; grade2 matched161. Grade2 still inherited incorrect canonicalSetAsGameStatePersisted label. Native flags/PDB review proved SetAsActivated; grade3 conservative rename/description correction matched and finalfixtures passed.

006FA3F0 grade1 234v236 used opaque external LowerBound. Defining the genuine native82byte binarysearch body under auto_inline(off) let compiler prove animationkey address did not escape, preserving keyEBP across owning constructors and producing native coldEBX/frame/temporaries in grade2. Do not blame scheduling before restoring real same-TU callees.

008209C0 grades1-3 were193v193 with UID argument/Worldgetter load order differences; grade4 named UIDlocal introduced extra callee-saved register topology while remaining193bytes. Grade5 natural nestedGetWorld().GetPThingWithUID() projection put UIDhigh/World/UIDlow in native order and matched193. No flagsweep, synthetic volatile or assembly. Prior best grade3 remains preserved as evidence/008209c0_previous_nearmatch_grade03.cpp and snapshots, not a current DIFFER row.

Fixture-only compile failures preserved: texture first host specialization-after-instantiation fixed by generic dependency definition; brush first host donor __int64 unrecognized fixed by standard fullwidthhost macro. Native fixture had already passed those runs. No candidate source-grade reset or omission. All later frozen fixtures execute literal production candidates, real owning copies or list destructors, plus explicit dependency stubs.

## Freeze and root review

Machine-readable ROW_INVENTORY.json separates workerREADY from rootaccepted; currently all4 await root and none accepted. ATTEMPTS.json +snapshots +logs preserve all12 sourcegrades. PACKAGE_SHA256.json covers all files except itself. Shared actual-header hashes captured in evidence/HEADERS_SHA256_at_freeze.json. No canonical/shared files or git state edited, no Wine kills/restarts, no subagents, no automatic sweeps, no assembly/injection/padding tricks. Root should independently verify native signatures, dependency identities, stored source/test hashes and all fixtures before landing. No next section begins before whole-section review.
