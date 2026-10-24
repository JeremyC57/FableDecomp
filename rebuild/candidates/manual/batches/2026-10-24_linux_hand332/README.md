# Hand chunk 332 (CPersistTraits<double> .. CReplaceSerialise<CFileFormatEditMapBrushHeader>): 38 of 40 byte-exact

All 38 src/ rows grade MATCH/RELOCATION_MATCH (gall.sh) and their tests print `<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 cl (tall.py). No `__asm`/`naked`/`_emit`/`volatile` anywhere.

## What matched (38)
- **CPersistTraits<T>::TransferBinaryIn scalar reads** (004aa360 double, 0040efb0 float, 004cbc70 short, 00405940 / 004cbd50 uchar, 0044b890 u64): the inlined CMemoryDataInputStream::Read model of the 31 landed twins, now over `engine/CMemoryDataInputStream.h` through a methods view (`(unsigned long)position + size` gives retail's xor-eax high dword). Shape lessons: double/short/uchar persist a 4-byte **long** (`fild dword`, word/byte narrowing on the store); float is read as a float (fld/fstp tails); u64 gets its own 8-byte frame.
- **CDefString::TransferBinaryIn** (0044fc00, catalog said CPersistTraits<float>): `this` is the destination; FableWin.pdb names it. Uses `engine/CDataInputStream.h` (StreamPos/CurrentSourcePtr/SourceChunkBytesRemaining).
- **CEngineWaterRenderer::LoadNonPatchData** (00b6daf0): reads a block size from the header stream, then CWaterSeaStaticMapInfoBlock::Load on an 8-byte local; `this` and the data stream are unused.
- **CPersistTraits<float>::TransferIn** (0040f7e0): symbol-or-float text read; needed an inline `GetSymbolMap()` accessor so the map pointer loads before the argument pushes.
- **TransferOut wrappers** (005cc6c0 long, 005d8185 EInputKey /Os ebp frame): copy into the dead value slot, call the __fastcall TransferOutLong 0x9E2560.
- **pair traits** (00755a30 <long,long>, 006c78f0 <long,COpinionDeed*>, 006d3880 <CCharString,long> TransferOut; 006d3b90 <CCharString,long> TransferBinaryIn): both element traits declared first; inline element traits drop their DefaultVal, out-of-line ones keep it (the zero store / the CCharString ctor+dtor). The binary-in pair builds NO element traits temporaries (free element readers).
- **CVectorMap<long,long>::Add** (005f9e36, 005f9eea; catalog said TransferOut): pair temporary + push_back + Dirty, `optimize("y",off)` + `("s",on)`.
- **CPreAllocVectorMap Lower/UpperBound** (00a316b0/00a31700 C3DHelperPoint, 00a31790/00a317e0 C3DDummyObject): signed `long len` walk; for the 0x14-byte element `len` must be declared before `first`.
- **std::__median with a __fastcall function-pointer comparator** (009322d0 by-value pointer elements, 00a45a10 / 00ad0750 by-reference; catalog said CPredicate_And::operator()).
- **_Rb_tree<long,...>::_M_create_node** (00be0ac0; catalog said Clone): malloc + placement new of {long, {long,bool,long,intrusive ptr}}.
- **CPredicate_And<CPredicate_And<CPredicate_And<CIsThingAlive,CIsThingAlive>,CIsThingAlive>,CIsThingAlive>::??_G** (00ef6e40): nested operands by value (vptrs at +0/+4/+8); user dtors on And and on the CPredicate base give the merged base-vtable stores.
- **CPhysicsMeshInfo::CMeshData::UnloadResource** (00a24fc0, slot 2): counted-pointer `GetRefCount()` = `Data ? Data->RefCount : 0` gives the `mov eax,[eax+4] ; cmp eax,1`; Release reads Data into a local.
- **CPriorityQueue<CAStarState*>::UpdateElement** (00a21c30): `element == *it` and return inside the loop; inline push_heap -> __push_heap(first, (last-first)-1, 0, *(last-1), comp).
- **CBaseSerialise<CFileFormatBrushHeader,1>::ReadFromFile** (008269a0): ReadVar overloads (explicit member-template specialisation declarations ICE the compiler).
- **CPersistTraits<list<CReactionMatch*>>::TransferBinaryOut** (00678250): inline size() walk, virtual GetMatchType/TransferBinaryOut.
- **NCameraModes::CPhysicsLeaf ctors** (00850580 GotoOutsideSimple/Cartesian, 0085c950 really Rigid/Rigid): CSubComponentBase(owner, manager) stores, scoped_ptr reset = `old = ptr; ptr = p; delete old;`.
- **GFSerialiseMapBinaryIn** (0067c020, 0067c110, 0067c200 COpinionReactionManagerDef maps; 006dba50 map<long,CDefString>; 004e9156 map<EObjectAugmentationType,CWeaponTrailGraphicSet>, all catalogued as `CPersistTraits<list<COpinionTransientOffset>>::TransferBinaryIn`): `__fastcall(stream, map)`, inline clear, count through an inlined long traits, value-initialised pair; same-TU noinline stand-ins for the element traits let the empty traits temporaries sit on the pair's first word. 004e9156 is /Os with function-scope traits.
- **CPersistTraits<list<COpinionTransientOffset>>::TransferBinaryIn** (00479369): reading the count through an inlined CPersistTraits<long>::TransferBinaryIn homes it in the stream slot.

## Parked for Fable 5.1 (2)
- 0051a240 CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ReadFromFile DIFFER(164v175): retail keeps a `push ecx` frame word that gets `movzx eax,al ; mov [esp+14h],eax` (the zero-extended version) on the old-format path and is never read, and compares through al (`mov al,[v] ; cmp al,4`) | tried: plain if/else, a struct local holding the version, a switch on the version | next: find the construct that leaves the dead int store (an inlined callee taking the version by reference/value, e.g. an old-format factory or a debug trace), or call it lane gap: dead-store frame word.
- 00632830 GFSerialiseMapBinaryIn<long,NUISystem::ETableSprites> DIFFER(196v196): every frame displacement 4 low (retail leaves esp+10h unused; pair at +14h/+18h, traits word at +1Ch; ours pair +10h/+14h, traits +18h) | tried: per-call traits temporaries, function-scope key/value traits (both orders), plus same-TU traits stand-ins (frame shrank to 0x18) | next: an unused 4-byte function-scope object declared first (e.g. the key traits as a separate never-used temporary) or the CUIDef TU's out-of-line clear() stand-in.

## Skipped
- none. No row bounds looked wrong: every row ends in its own ret, trailing bytes are int3 padding.

## Label corrections
- 0044fc00 `CPersistTraits<float>::TransferBinaryIn` -> CDefString::TransferBinaryIn(CDataInputStream&) (FableWin.pdb, callers on CDefString members).
- 00b6daf0 `CPersistTraits<float>::TransferBinaryIn` -> CEngineWaterRenderer::LoadNonPatchData (caller CEngineWorldMap::LoadWaterData 0x00B41FA0).
- 005f9e36, 005f9eea `CPersistTraits<long>::TransferOut` -> CVectorMap<K,V>::Add (8-byte pairs; push_back 0x5F9C4F / 0x5947B6).
- 005d8185 `CPersistTraits<long>::TransferOut` -> CPersistTraits<EInputKey>::TransferOut (same /Os unit as the landed EInputKey TransferIn 0x005D8167).
- 006c78f0 `CPersistTraits<pair<long,long>>` -> pair<long,COpinionDeed*> (second element via CPersistTraits<COpinionDeed*>::TransferOut 0x6C7960).
- 004e9156, 00632830, 0067c020, 0067c110, 0067c200, 006dba50 `CPersistTraits<list<COpinionTransientOffset>>::TransferBinaryIn` -> GFSerialiseMapBinaryIn<V,K,less<K>> instantiations (see STATUS.tsv notes for V/K).
- 0085c950 -> CPhysicsLeaf<CPhysicsTargetModelGoTo,CPhysicsForceModelRigid,CPhysicsCoordRigid> (vtables 0x1276AC8 / 0x1276AC0).
- 00a24fc0 `vfunc_2` -> UnloadResource (FableWin.pdb).
- 00be0ac0 `CPredicate_And<...>::Clone` -> _Rb_tree<long,...>::_M_create_node.
- 009322d0, 00a45a10, 00ad0750 `CPredicate_And<...>::operator()` -> std::__median (function-pointer comparator).
- 00ef6e40 `vector deleting destructor` -> scalar deleting destructor of a 3-level (not 5-level) CPredicate_And of four CIsThingAlive.
- 008269a0 `CReplaceSerialise<CFileFormatEditMapBrushHeader,...>::ReadFromFile` -> CBaseSerialise<T,1>::ReadFromFile (bool; caller CWorldMap::LoadScriptBrushesFromFile; T = brush header, possibly ICF-folded).
- 0051a240 -> the CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3> instantiation (25-byte scratch record, version 4).
- Callee labels seen wrong: 0x00450855 is list::clear (not CGuiControlTreePane::SortTreeRecursively); 0x006815F0 is COpinionTransientOffset() (not CVsyncCallbackTimerInfo); 0x00A206F0 is __push_heap<CAStarState**,...> (not <float*>); 0x00850360 is ~CPhysicsForceModelBase (not ~CPatchTesselationEdgeStrip); 0x004DB9EC / 0x00630B20 are map::clear (not OnKill).
