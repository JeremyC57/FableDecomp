# Chunk 207 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Hidden-return temporaries (a CCountedPointer returned by value) get their address pushed before the receiver
  chain is evaluated. If retail calls the receiver's getter first (`GetMeshInfo` before `lea ecx,[esp+..]; push ecx`),
  make the receiver a named local (`const C3DMeshInfo* meshInfo = bank->GetMeshInfo(i); meshInfo->GetMeshStats()->..`).
- An inline `clear() { erase(_M_start, _M_finish); }` wrapper flips the this/&member register pair (retail `edi=this,
  esi=&Handles`) compared with calling `Handles.erase(Handles._M_start, Handles._M_finish)` directly.
- 2D boxes passed by const reference: a C2DBoxF ctor taking two C2DVector references copies the corners through
  frame slots; a four-float ctor builds the box in place (`fstp` straight into the temporary).
- Frame size pins the size of a named local you only know by stack usage (CEnginePrimitive2DSprite is 0x54 in retail).
- Behaviour tests for by-value counted pointers: make the stub `return g_global;` (a copy of a global) so no local
  destructor runs on either compiler; reference counts then don't depend on NRVO (VC7.1 has none, g++ elides).
- Delete spies: count only while `g_armed` is set, never free.
- GetInterface shapes keep recurring: out-param template (component in eax), HasTC + one-ternary GetTC (component in
  ecx), unset-local accessor (garbage on a miss, lives in a dead arg slot).
- Two named bools in sibling blocks share one frame byte; a CCharString temporary can live in a dead argument slot.
- CTCHero-style classes with an int64 member need a plain vptr word, not `virtual`, to keep the PDB layout.
- `*(begin()+i)` is the operator[] form for STLport vectors in index loops (CTCGhostSword::CancelMove).
- Register-allocation and x87 spill-order DIFFERs (4 grades each, no source-level lever found) are the bulk of the
  parked rows; they look like lane gaps rather than wrong shapes.

Review at landing:
- 0x00686A80 GetWorld const: landed code is split (22 x `CWorld* GetWorld() const`, 9 x `const CWorld* GetWorld()
  const`); this chunk uses the majority `CWorld*` spelling. PDB says `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`
  (protected, returns `CWorld&`), which neither landed spelling matches.
- 0x00A25530 C3DMeshInfo::GetMeshStats: declared `const` here (PDB `QBE`); the landed C3DMeshObject::SetMesh
  (0x00A36010) declares it non-const.
- 0x00A2EA60 C3DMeshStats::HasDummyObject: declared `bool HasDummyObject(unsigned long) const` here (PDB `QBE_NK`);
  the landed definition uses `unsigned char` non-const (its own codegen needs the uchar result).
- 0x004014A0 is spelled `Calc_004014a0` (the landed majority, 97 uses).
- Pragmas: `#pragma pack(push, 1)` for CBaseIntelligentPointer in 006ca4e0 and 00793d20 (the PDB puts CTCHero's
  LastWeaponEquipped at the odd offset +0x1D); `#pragma pack(push, 2)` for the 14-byte CEngineGraphic in 004bc750 and
  007a69f0.
- Leaves that aren't the default: `??0CTCDrainLife`, `??0CTCForcePushPower`, `??0CTCForcePushed`, `FrameUpdate`
  (007e76b0), and the method names for every NGraphicAppearance row (004bc410, 004bc750, 004bd200, 004bd2e0,
  004c11f0, 007a69f0).
