# Chunk 210 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- One `(flags >> n) & 1` assignment per `bool : 1` field, with the fields filled from the same word, compiles into one shifted
  and or-ed byte build per flag byte plus a `keep` mask (`and cl,3Ch`). A field read just before its own store comes from the
  merged byte (0x007234C0 SetInteractionFlags).
- A hidden-pointer call result that retail reads back from its frame slot (`fld [esp+1Ch]`) is a named local. The unnamed
  temporary reads through the returned eax (0x00759EC0, `C3DVector rotated = M * v;`).
- A def field tested and then passed (`mov ecx,[eax+N] ; test ecx ; ... mov eax,ecx ; push eax`) is the member read twice
  (`if (PDef->X > 0) f(PDef->X)`). A named index local loads eax directly (0x00702310 / 0x00702470).
- `GetWorld()->PSubsystem->Call(args)` through a boost scoped_ptr's inline operator-> loads the global/member into eax before
  the pushes, where the raw pointer member loads ecx (0x00771540). When retail fetches the manager before the pushes and moves
  it into ecx after them, read it into a named local (0x007E60F0 bank, 0x007FF0C0 search tools).
- STLport `resize(n)` as the inline forwarder to an out-of-line `resize(n, const T&)` stores the `T()` temporary AFTER the
  argument pushes. Written as `resize(n, T())` in the caller, the zero store comes first (0x007673C0).
- A small struct temporary whose ctor zeroes two fields in its initialiser list and assigns the third from a call result in
  its body keeps that result on the x87 stack until after the zero stores (0x00788555 CAnimFrameTime).
- In the trap TU (`optimize("y", off)` + `("s", on)`) the CDefPointer release (`dec [eax] ; cmp [eax],0`) inlines only with
  `__forceinline` on the dtor and on ReleaseTemporary/DecRef/GetRefCount (0x00787CA5).
- A creature action passed by `const&` to SetCurrentAction, built from a derived class with an inline ctor over an out-of-line
  base ctor and an implicit dtor, gives `ctor ; vtable store ; lea ; push ; call ; base dtor` (0x007F04C0).
- `T* p = 0; if (thing->HasTC(k)) p = thing->CachedMember;` is how the effect thing's cached driver (+0x5c) is read (0x007F04C0).
- A component method that forwards to its owner (`mov ecx,[ecx+24h] ; jmp`) is used as the melee test in CTCPerceiveThing;
  name it after the PDB getter on the component (0x00733190).
- `empty()` written as `size() == 0` stops VC7.1 CSE-ing the end pointer into the back() access that follows, at the price of
  a sub/test instead of retail's pointer compare (0x007CC710, parked).
- Review at landing:
  - 0x00702310 matched on its 5th grade: after four tries it took the shape found on its twin 0x00702470 (the def field read twice).
  - Landed name conflicts to sweep: 0x006869D0 is `GetDefinitionManager() const` in the PDB (9 landed files say
    `GetDefinitions`); 0x004C7A70 is CThing::PeekComponent (landed "GetDisplayEngine"); 0x0049D9D0 is CWorld::PeekMyPlayer (landed
    "GetCurrentPlayer"); 0x0045EF2D is COpinionSet::operator= here (11 landed say CFloatVector::operator=); 0x0129BA3C is declared
    `extern const float g_epsilon_0129ba3c` here (landed files use both const and non-const); 0x0066B280 returns unsigned long
    (the 3 landed callers declare `void PlaySound(...)`); 0x0076C7F0 takes `const CFaction*` (the one landed caller declares `long`);
    0x00787BF1 is CTCTrapBase::Reset (landed ManualReset's model calls it "Cleanup").
  - Landed-source issues: CTCPhysicalShieldSpell_CancelMove 0x0079E300 names the def's +0x60 string "CancelSound", but it is
    EndSound (PDB +0x64, def bodies 4 bytes low). CTCPhysicsBase_UpdateCollisionGroundIneractions 0x006AFF00 declares
    CWorldMap::GetGroundFaceNormalAt(pos, long flags), but the callee 0x0063AB10 takes one argument (`ret 8` with the hidden
    result; PDB QBE?AVC3DVector@@ABV2@@Z): check its pushes.
  - Local models next to engine headers: CTCPhysicsStandard/CTCPhysicsBase/CTCPhysicsRigidBody/CTCQuestCard/CTCPerceiveThing/
    CTCNamedUserActionPoints headers are flattened PODs (opaque bitfields or containers, no virtuals, PDB offsets for 16-byte
    containers), so every row models the class locally with the retail offsets (as the landed relatives do).
  - pack(1) is used only on the intelligent-pointer classes in 0x007673C0 (ProjectileTrailEffect at the PDB's odd +0x65).
    pack(2) on CEngineGraphic appears only in the parked 0x00727C20.
