# Chunk 626 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A `C2DCoordI((long)p.X, (long)p.Y)` TEMPORARY inside a call expression is built before the outer argument pushes, X first,
  with the X `__ftol2` store moved past the Y `fld`. A named C2DCoordI stores X at once (and the (long,long) named form even
  truncates Y first). For a "store after the next fld" residue, move the coordinate into the call (0x007881AD; same shape as
  0x00749D20).
- A TU-STATIC global whose address is never taken does not kill loads. `Active = info;` written first then lets VC7.1 keep
  earlier reads in registers and move the store down to the call (0x009D87F0). An extern global gives `sub edx,[esi]` re-reads
  and the store before the pushes. Use it when every xref of the global sits in the one TU (xref.py).
- Unmangled PDB names like `MicroThread::RestoreStack` are TU-statics. Write them as `static inline` helpers. Retail may also
  keep an out-of-line copy with a private register argument (0x009D87B0, vector in edi).
- Argument types change push scheduling: a BOOL result (`IsMovementUsingPhysics()`) passed to a `(const C3DVector&, bool)`
  virtual schedules `add esi,68h ; mov ecx,edi` before both pushes. A long result pushes eax straight after the call
  (0x00692AA0). Read the retail vtable from the image for base slot bodies (`xor al,al ; ret` = bool) before typing a slot.
- An inline accessor on the pointee (`PCreature->GetPhysicsTC()->SetAsMoving(true)`) completes the object expression before the
  immediate `push 1`. A direct `->PhysicsTC->` pushes the argument before the GetPItem call (0x00692AA0).
- FableWin.pdb names filter combinators: `GFFilter_Bind2<A,B>(const A&, const B&)` (__fastcall) and `std::not1` give the real
  types of GetAllThingsInRoughArea filters (CFilter_Bind2<X, std::unary_negate<Y> >). Grep fablewin_pdb_names for `GFFilter_Bind`
  before inventing operator&& models (0x0095D4C0).
- Research trick: when an inlined constructor's shape is unclear, grade candidate ctor forms against its OUT-OF-LINE retail
  copy in the catalog (CGameEvent(long) 0x004159B0: the plain init list matches, with its shared `xor cl,cl`).
- Getter runs: a run of identical one-byte getters (+0x48/49/4A/4C/4D) lines up 1:1 with an Ego_r donor run of bool getters
  and names them (0x00445B80 = IsToggleZTarget).

Review at landing:
- finish.py was pointed at a local `chunk626_plus.tsv` (chunk TSV + the two extras' rows from the chunk622/retry1 batch
  oracles); the original is finish.py.orig. Extras 0x007AF940 / 0x009FD3A0 are not in rebuild/oracles/auto-re-candidates.tsv.
- 0x00686D90 is PeekThingSearchTools (IBE); every landed caller that spells GetThingSearchTools for 0x00686D90 needs the respell
  (NAME_FIXES.tsv). The landed 0x0095DB90 (GetAllThingsInRoughArea, catalog label with list/Bind3) is a `CThingSearchTools_Calls`
  stand-in with a wrong name.
- Landed 0x004159B0 CGameEvent::CGameEvent is a `__fastcall` emulation with a dummy edx parameter (struct CGameEventT). The plain
  `CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }` MATCHES it
  (probe/ge/004159b0_A.cpp) and should replace it.
- Parked rows that used all 4 grades: 009fd700, 0095d4c0, 008e8ff0 (0095d4c0 has a concrete ungraded next idea).
- Local models next to existing headers: CTCTrapBase/CTCBase/CThingSearchTools (007881AD), CTCPhysicsBase (00692AA0),
  CTextureManager/CCountedPointer (extra 009FD3A0) -- vland SHARED_HEADER_AVAILABLE warnings only.
