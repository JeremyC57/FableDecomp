# 2026-11-10 landing ax -- chunk 1019 (3 of 14 near misses)

Source: `pending/2026-11-10_linux_hand1019/`. vland --land 3 RELOCATION_MATCH, tests PASS:
- 0x008365F0 std::for_each<const pair<unsigned long,CAnimationEntry>*, CDeadCreatureAnimPreloader> (/Gr; catalog
  "CTCInventoryAbilities_PreloadDeadAnimation" wrong). Lever: the def name read through a CDefString copy shifts VC7.1's rotating
  scratch register by one (strcmp zero in eax, reused for BeginLoading(false)).
- 0x00AE3920 CParticleProgram::RemapBoneIndices(map&, CArray<C3DGroup2>&), private static __fastcall. Lever: FABLE_RETAIL_STLP_MAP --
  the visible map::operator[] body moves the key store after the tree-root load (the call stays out of line, 0x00553E60).
- 0x006A3E00 std::__introsort_loop<pair<long,CIntelligentPointer<CThing> >*, ..., CKeyPairCompareLess> (/Gr; catalog "Heap_SiftDown"
  wrong). Lever: FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN for the pivot copy ties first/last at priority 256.
Headers (chunk 1017's opt-ins, merged onto HEAD; hdrcheck2 default=same, 7 files): fable_definition_table ASCHARARRAY, CDefString
GETASCHARARRAY, CMeshDataBank PEEKANIMATION, new C3DGroup2.h, CArray PUSHBACK_NOINLINE, CParticleProgram REMAPBONEINDICES,
stlport INTROSORT_LOOP. Manifest names respelled to the template instances.
Parked (differ/): 0x009F18F0, 0x00934E50, 0x00A55F90 (register / schedule residues, 12-20 probes each), 0x00681E60, 0x00A229E0,
0x006E94D0 (= CCombatManager::IsCreatureInAttackRingSector; local CRetail* types -- header migration first), CEngineMap Peek* trio
(splitting GetMap into two `if (...) return 0;` fixes the block but needs /d2inlT301, which then moves the y reload),
0x0063D0A0 / 0x0063D4A0 (10 hunks after parenthesising the first plane test). Queued land1011aw: 4 name fixes.
