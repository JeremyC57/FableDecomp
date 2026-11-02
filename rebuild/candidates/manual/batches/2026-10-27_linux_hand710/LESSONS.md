# Chunk 710 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 710) VC7.1 does not devirtualise a virtual call made on an embedded member object: `m_50.Unlock(); m_60.Unlock();` compiles to
  `mov eax,[esi+50h] ; lea ecx,[esi+50h] ; call [eax+14h]` and a tail `jmp [edx+14h]` (0x00C0E5C0 / 0x00C0E5E0, MATCH on probe 1).
  No pointer or reference indirection is needed to get retail's indirect calls.
- (chunk 710) An implicit destructor is emitted out of line, exactly as retail, by defining the class's own PDB constructor in the TU:
  the vtable, the ??_G and the ??1 all follow, and each TU's ??_G COMDAT also grades against the retail caller (seven rows here). A base
  dtor that retail calls out of line from a derived ??_G (~CModeDragonHover 0x008E31A0 from CModeDragonFireBeam's ??_G 0x008E3860) comes
  from defining the DERIVED class's constructor: VC7.1 expands ~Derived in its ??_G and calls ~Base.
- (chunk 710) A member function retail inlines at one site and keeps out of line (RemoveAllModes 0x0073AE70, inlined in ResetModes
  0x0073B0D0 and ~CTCCreatureModeManager 0x0073AEA0) is a plain member defined above its callers; grading the emitted COMDAT against the
  out-of-line row proves that row's name (catalog "ClearModes" was wrong).
- (chunk 710) An embedded def member (CTCNoise::Definition, a CTCNoiseDef by value) destroys as: the CDefPointeeBase vtable store (the dead
  CTCNoiseDef / CSubDefClassBase / CDefClassBase stores vanish), then a `call ~CResource` (a member, so not the tail), then the owner's
  base dtor as the tail (0x00749A70; engine/CSubDefClassBase.h with FABLE_RETAIL_DEF_CLASS_BASE reproduces it directly).
- (chunk 710) engine/CDefPointer.h's inline Release() reproduces the `CDefPointer<const T>` member release in an implicit dtor (`test ;
  je ; dec [ecx+4] ; jne ; call [eax+4] ; mov [esi+N],0`) with no local model (0x006F1770, 0x007E0B20).
- (chunk 710) The FableWin.pdb ??1 of a compiler-generated destructor is the only PDB name for it (Ego_r inlines implicit dtors): pair it
  with the RTTI vtable whose slot-0 ??_G calls the row.

Review at landing:
- Five rows need the hdr/ overlay (engine/CModeStandBase.h, engine/CCombatSequenceGeneric.h; opt-in, default text unchanged).
- Second spellings introduced (NAME_FIXES.tsv): 0x008B0A60 (landed own row QAE), 0x0073AF60 / 0x0073AFC0 (landed own rows public),
  0x0073E6A0 (CRetailList_CountedPointer stand-in), 0x008E31A0 (?CModeDragonFireBeam_Destruct@@YIXPAX@Z in landed 0x008E3860).
- Landed 0x0073B110 / 0x0073B120 (OnCreate / InitialActivate) are extern "C" free forwarders; they are the protected virtuals
  ?OnCreate / ?InitialActivate@CTCCreatureModeManager@@MAEXXZ tail-jumping the private Setup members.
- Landed 0x008B0A60 models CombatActions as an invented CRetailVector and the dtor as non-virtual.
- 0x0073AEA0 (~CTCCreatureModeManager, 93 bytes) is not in the catalog (manifest label ~CVertexShaderData).
- libpk class naming decision for vtable 0x012B25A8 (NOTES.md "Decisions").
- No row needed more than 4 grades; no catalog row holds several functions; every extent is right.
