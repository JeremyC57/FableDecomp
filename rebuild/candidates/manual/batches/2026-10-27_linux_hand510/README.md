# Chunk 510 (agent510, 2026-10-27/28): 15 of 20 single functions landed (256-512 bytes), 1 held, landed 2026-10-28

Single functions of 256-512 bytes (20 rows, address order, 0x00635E90..0x009C73E0), mostly STLport instantiations. One
Opus 5.5 agent at high effort; a container restart stopped the first agent after 2 drafts, and the resumed agent
regraded them.

- **Landed:** 15 rows with `vland.py --land` (all RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY): two CPlayer
  projectile-weapon mode methods, vector operator= / _M_insert_overflow / _M_fill_insert instantiations (CBlurredAttitudesDef,
  CThingGraphicArgPasser, CMeshMorphSetParts, vector<CThingMapCell>, pair<long,CSkeletalMorph>,
  CEnvironmentThemePriorityStackEntry, CScriptThing, CHeightMapCell via engine/CHeightMapCell.h, CBufferChunk),
  __introsort_loop<CDegradableInfo> and over pair<ulong,CDefClassInfo> (/Gr), __find<CTargetPoint*>, and
  __find_if over NAnimationEvents::CEvent (probable).
- **Held (1, pending/2026-10-27_linux_hand510/held/):** 0x006D4850 vector<pair<CCharString,long>>::_M_fill_insert matches
  but binds `__copy_backward<pair<CCharString,long>*>` to 0x006D3610, while landed 0x009B5990 binds that symbol to
  0x009B1E20. The agent's spelling follows the PDB caller TransferableVector<pair<CCharString,long>>::Resize; the landed
  0x009B5990 family is probably the pair<CCharString,unsigned long> instantiation and needs respelling first.
- **Parked (3, pending/.../differ/):** 0x00780430 CTCFlammable::GetClosestFirePointTo (273v271, ebp/spill swap),
  0x007805D0 NFire::CFirePoint::SetAsSmoldering (300v299, temporary flag/result register placement), 0x008AAC50
  __find_if<C3DVector*, unary_negate<...>> (411v411, one of seven inlined predicate copies squares dy before dx).
- **Skipped (1):** 0x0074B77C is the tail of 0x0074B770 CTCCreatureGenerator::InitialiseCreatureGenerator(bool); the
  boundary should be one 395-byte function at 0x0074B770.

Label corrections (NOTES.md): 0x008F67C0 is `__uninitialized_copy<const CHeightMapCell*, CHeightMapCell*>` (PBV; the PAV
spelling is its twin 0x008F6800); 0x0085DE40 the vector<CThingMapCell> copy ctor; 0x005BC0D3
~vector<CEngineTextureMorph>; 0x0067C490 CBlurredAttitudesDef::operator=; push_back 0x006A3C70's element is
CThingGraphicArgPasser. Review: 0x009C6D60 is landed as a non-template __destroy_aux; 0x0086CC50 bound with a
one-parameter vector spelling; 0x006D4750 declared QAE only to keep its bound spelling.

**2026-10-28: the held row landed.** The conflict was in the landed rows, not in 0x006D4850. FableWin.pdb has 66
`CKeyPairCompareLess<CCharString, unsigned long>` names (the CVectorMap<CCharString, unsigned long> Sort/Insert/Update
comparator) and no `<CCharString, long>` comparator. So the sort family around 0x009B5990 is over pair<CCharString,
unsigned long>, as every other landed binding in it already said, and its `__copy_backward` 0x009B1E20 is the unsigned long
twin; the K `_M_fill_insert` 0x00AE7340 calls it too. 0x006D3610 is the long twin: it is called by
GFSerialiseVectorBinaryOut<pair<CCharString,long>> and this row (CPersistContext's TransferVector<pair<CCharString,long>>, FableWin.pdb).
Respelled and rechecked (verify_twinfix.log): 0x009B5990 and 0x009B47F0 (long -> unsigned long), and 0x006D4AA0 (its
invented `SNamedLongVector` is std::vector<std::pair<CCharString, long>>). `_M_fill_insert` is public (QAE): STLport 4.6 / 4.6.2
`stlport/stl/_vector.h` (identical in both) declares it at line 370, inside the public section that runs from line 171 to 491.
