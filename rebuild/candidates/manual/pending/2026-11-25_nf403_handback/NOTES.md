# NF403 hand-back (2026-11-25): 0x004DF4E7 copy ctor (b1), 0x0057FEFB _Destroy lever (b2)

## Summary
- b1, 0x004DF4E7: ACCEPT. Self respelt from the made-up `??0CObstacleNavigatorExternalInterface@@QAE@ABUCObstacleOwner@@@Z`
  to the copy constructor `??0CObstacleNavigatorExternalInterface@@QAE@ABV0@@Z` (the spelling the table already binds here, witness 004df4ca).
  One additive default-off header opt-in: engine/CNavigatorExternalInterface.h FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_INLINE_DTOR.
  New behaviour test (batches/b1/tests/004df4e7_test.cpp; the HEAD test used the stand-in types). No table change.
- b2, 0x0057FEFB: HOLD_RULING Q1, gate-clean. FABLE_RETAIL_STLP_DESTROY_FASTCALL turns NF396's homeless cdecl _Destroy residue into a
  5-byte `jmp ~CCharString` __fastcall copy that has a retail home, but not a unique one (4 candidates in the row's own TU region).
  The brief asked for unique tier 2 or tier 1; ruling R1's last clause accepts a non-unique home. On yes, land batches/b2/rows/0057fefb.cpp.
- Locks: 004df4e7 kept as NF403; 0057fefb released.
- About 75 VC7.1 compiles: 17 b1 evidence compiles (HEAD controls 4e7 / 4ca / 507 / 365 / 532; probes n1 n2 m507 pA pB pC pD pE;
  v365 / v507 twice, the first pair SRC_FAIL because my splice cut the class text out), 3 b2 evidence compiles (h, held, n1), the two
  vland pairs (b1 4 rows x 2 sides, b2 1 x 2, plus tests), 35 more HEAD census objects for b1 and 4 for b2.

## b1: identity (two sources)
- FableWin.pdb tc_register.obj lists `??0CObstacleNavigatorExternalInterface@@QAE@ABV0@@Z` and `...@@QAE@AAVCThing@@@Z` (no default ctor).
- Retail: the only caller is Clone 0x004DF4CA (`push 14h ; call new ; push esi ; call 0x004DF4E7`, `return new X(*this)`), whose landed row
  already declares and binds this spelling. Body: `mov eax,ecx ; mov ecx,[esp+4] ; mov [eax],0x01241D04 ; mov ecx,[ecx+4] ; mov [eax+4],ecx ;
  xor ecx,ecx ; mov [eax+8/0Ch/10h],ecx ; ret 4` -- Obstacle copied, DummyAgentList default-constructed, so a user copy ctor
  `: Obstacle(other.Obstacle) {}` (the implicit one would copy the vector).

## b1: model (this departs from the brief's letter; say so at landing)
- The brief: use 0x004DF507's model. That class holds +4 as `long m_4` in a stand-in base `CNavigatorExternalInterface { virtual v00(); }`,
  so the copy has no member-init spelling. Two measured spellings failed: `: CNavigatorExternalInterface(other)` DIFFER 38v32 (the base copy's
  vtable store survives) and `{ m_4 = other.m_4; }` DIFFER 32v32 (the vector is zeroed before the Obstacle copy).
- Used instead: 0x004DF365's landed class (Ego_r.pdb members Obstacle +4, DummyAgentList +8 as the real STLport
  `vector<const CSteeringAgent*>`, sizeof 0x14, on the shared engine/CNavigatorExternalInterface.h base) with 0x004DF507's destructor form:
  an empty inline derived destructor and `~vector` out of line (FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE, as 0x004DF507).
- Why the destructors matter: VC7.1 emits the vtable, and with it ??_G, in every TU that defines a constructor. On 0x004DF365's
  out-of-line destructors the object newly carries the 28-byte ??_G copies 0x004DF365 already has (NEQ v 0x004DF507's 43 bytes and v
  0x004DF532's 29). With the inline derived destructor only, ??_G calls `??1CNavigatorExternalInterface` where retail stores 0x01241D74.
  Retail's base destructor is inline: 0x004DF532 and 0x004DF507 store the vtable themselves, and the out-of-line copy is the unreferenced
  7-byte `mov [ecx],0x01241D74 ; ret` at 0x006CB510. The shared header declares it out of line, so the opt-in
  FABLE_RETAIL_CNAVIGATOREXTERNALINTERFACE_INLINE_DTOR gives `virtual ~CNavigatorExternalInterface() { }` (default unchanged).
  With both, both ??_G copies are the retail bodies (relocation targets checked by hand: tools/retcmp.py).
- The leaf is RELOCATION_MATCH in every variant (n2, pA, pC, pE). An implicit derived destructor drops the vtable store (37 B ??_G, pD);
  plain `inline` and FABLE_FORCEINLINE give identical objects (pE = pC), so the row uses plain inline. Probe texts and grades: batches/b1/probe/.
- The quarantine header engine/_quarantine/CObstacleNavigatorExternalInterface.h cannot be used: vptr as a data member, DummyAgentList as
  opaque bytes, pack(1). It gives no vtable store and no ??_7 relocation.
- Convergence evidence (not a port): 0x004DF365's leaf and 0x004DF507's leaf both stay RELOCATION_MATCH on this class text (probe v365,
  v507), and 0x004DF365's two 28-byte ??_G copies become the retail bodies. T3 line below.

## b1: extra COMDATs (rulings R1 / R10)
- Tier 1 (byte-and-reloc equal to 0x004DF365's or 0x004DF507's HEAD copies, evidence/b1_tier1_census.txt): vector / _Vector_base /
  allocator / alloc_proxy ctors, ~allocator, ~alloc_proxy, deallocate, ??0CNavigatorExternalInterface, ??_7CObstacleNavigatorExternalInterface
  and ??_7CNavigatorExternalInterface (both = 0x004DF365's shared-header vtables; 0x004DF507's are stand-ins, existing debt).
- EQ at retail: ~_Vector_base 0x004DF554, ??_GCNavigatorExternalInterface 0x004DF532 (pk_all), ??_GCObstacleNavigatorExternalInterface
  0x004DF507 (by hand: 0x004DF507 has no self_symbols line, so pk_all prints NOADDR).
- Tier 2, unique: ??1CNavigatorExternalInterface@@UAE@XZ at 0x006CB510 (309 masked hits of `c7 01 ???????? c3`, 1 with 0x01241D74;
  16-aligned, padding both sides, no E8/E9 or data reference, not a catalog start). 006cd890 / 006cdd50 / 006ce670 define byte-equal copies
  beside their own stand-in base vtables, so I do not count them as tier 1.
- Nothing is residue. Two bodies under one pick-any name already existed at HEAD for both ??_G names (28 / 43 and 28 / 29 / 31); the new
  object adds the retail body of each, not a new one.

## b2: the lever
- FABLE_RETAIL_STLP_DESTROY_FASTCALL on NF396's held text changes only the _Destroy COMDAT: cdecl 9 B (`mov ecx,[esp+4] ; jmp ~CCharString`,
  no retail home) -> __fastcall 5 B (`jmp ??1CCharString`). The leaf and every other section are byte-equal.
- Retail: the row's TU region 0x0057D000..0x00582000 holds exactly 4 `jmp 0x0099EAE0` sites, all unreferenced function starts
  (0x0057EC36, 0x0057EC3B, 0x0057ED02, 0x0057F841), and 0 cdecl-shape sites (NF396 destroyscan). The TU is /Gr: its landed STLport free
  helpers bind @@YI (0x0057F188, 0x0057F279, 0x0057F29E, 0x0057F338). So the __fastcall copy matches retail's convention, and retail has
  homes of its exact shape; the other __fastcall shape (`push 0 ; call ??_G pair ; ret`) has 0 sites for any ??_G pair<const CCharString, X>
  (49 such sites in .text, none to the five homes). It is not unique: the 4 stubs are byte-identical, and ~pair<const CCharString, X> instances share the shape (R9).
- Not tier 1: no landed object defines the __fastcall name (none of the 23 landed sources / tests defining one of the three fastcall
  _Destroy macros reaches MirroredCriminalStatus after preprocessing; no header defines them).
- What is left: only the uniqueness question (RULINGS_NEEDED Q1). Everything else is clean.

## Callee names
- b1: none new. ??_7CObstacleNavigatorExternalInterface 0x01241D04 and ??_7CNavigatorExternalInterface 0x01241D74 (table),
  ~vector<const CSteeringAgent*> 0x004DF54F (as 0x004DF507), operator delete 0x00BFE9BC.
- b2: unchanged from NF396 (~CCharString 0x0099EAE0, free 0x00BFEA14).

## Header debt
- engine/CNavigatorExternalInterface.h declares the destructor out of line; retail's is inline (opt-in added, default unchanged).
- 0x004DF507 and 0x004DF4CA stay on local stand-in models (T3).

## Deviations
- The brief said to use 0x004DF507's model. I used 0x004DF365's class plus 0x004DF507's destructor form, for the measured reasons above.
- tools/ are copies of NF398's tools with paths changed to agentNF403; runpair.sh keeps the control rows in the new payload.
- No git, no repo write, no process killed.
