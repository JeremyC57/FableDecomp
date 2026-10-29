# Chunk 562 lessons

## GOTCHAS one-liners
- (chunk 562) An implicit (compiler-generated) destructor has no vptr store. Emit it out of line with a qualified explicit call
  in a one-line helper: `void EmitImplicitDestructor(T* p) { p->T::~T(); }`. 0x005EDA50 ~CTCTavernGame matched on grade 1.
- (chunk 562) When a member is written from a static call (`GetWorld().X = CWorld::GetFrame()`), VC7.1 calls the static first.
  Retail called GetWorld first because the PDB inline member `CWorld::SetAsStarting3DRendering()` was used. Look for a PDB
  inline setter before reordering statements.
- (chunk 562) Per-type gates of the form `cmp 1; jne; mov al,[f1]; jmp T; cmp 5; jne SKIP; mov al,[f5]; T: test al,al` are
  `if (t == 1) { if (!f1) continue; } else if (t == 5) { if (!f5) continue; }`. VC7.1 tail-merges the two tests.
- (chunk 562) A vtable slot pad that carries a real name must carry its full PDB signature
  (`FindChild(CCharString)`, not `FindChild()`). The symbol is emitted in the vtable even when nothing calls it.
- (chunk 562) STLport `_Rb_tree_iterator` declared before a loop (`map::const_iterator it; for (it = ...)`) needs a user default
  ctor, or the source fails with C2512. A `pair<K, Outer::Nested>` map member needs the full nested type (C2079).
- (chunk 562) NUISystem component chain retail offsets: derived members are the Ego_r offset minus 0x30, and 8 less again for each
  Dinkumware list before the member. CComponent::PParentViewport is +0xDC (ClearHandle is slot 91). Retail CThing
  UpdateListNext is +0x88 and the flag byte (Dead/Activated/InLimbo = bits 0/3/5) is +0x91.
- (chunk 562) Layout-rename works for a flattened header without a vptr (engine/CVideoSys.h): `#define CVideoSys
  CVideoSysLayout` / include / `#undef`, then `class CVideoSys : public CVideoSysLayout { public: void Close(); ~CVideoSys(); };`.
  The calls mangle as CVideoSys and the bytes are unchanged.
- (chunk 562) Tooling: udiff.py needs a file of full q.py output (g.sh pipes through `head -120`, which truncates it and gives an
  IndexError). The agent's gall.sh has no exec bit, so run it with `bash`.

## Review at landing
- 0x013B8AB8 is bound three ways: CMovie::PEngine (used here), CMovie::Engine (0x005482D0) and g_pRenderDevice (0x00547C90).
- 0x013B8AC0 is bound as CMovie::MovieTexture plus two g_pMovieTexture spellings (0x00547C90 and 0x00547F60).
- 0x013B8A98 NUISystem::CList::GPreviousSelected has 13 invented bound spellings. Its element type (CList*) is probable.
- 0x005356A0 and 0x004B8C00 are bound to ActiveFile read-callback spellings from 0x005357E0 and 0x004BA590. They are STLport
  list clears (NAME_FIXES.tsv).
- 0x0051FCC0 declares four descriptive globals (0x01375755, 0x0137575A, 0x01375759, 0x013B8A84). None of them has a PDB name.
