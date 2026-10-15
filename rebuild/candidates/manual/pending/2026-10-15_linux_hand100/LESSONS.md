# Chunk 100 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A trailing member store that retail emits AFTER a stack temporary's destructor call needs the temporary in an inner
  scope `{ CIntelligentPointer p; ...; }` -- at function scope VC7.1 hoists the store above the destructor call
  (0x00712C20, 0x00712C80, 0x00802650, and every CVectorMap::Add row's Dirty store).
- CVectorMap<K,V>::Add (vector +0, empty Compare +0xc, Dirty +0xd) is `{ pair<K,V> p(key, value); m_vector.PushBack(p); }
  Dirty = true;` with an inline pair constructor; seven bodies in this chunk carried the propagated label
  "CTCSkeletalMorph::AddSkeletalMorph" (0x006A3DA0, 0x006FCF40, 0x0077F270, 0x00A41A70, 0x00A72B90, 0x00AF2910, 0x00AF3A40).
- A value copied dword by dword through ecx/edx/eax with eax as the source pointer is a memberwise USER copy constructor over
  dword members; a POD copies with ecx as the base (speed) or `movsd` (size). The same holds for a float C3DVector inside a
  size TU: a user copy constructor `X(o.X), Y(o.Y), Z(o.Z)` keeps integer moves under optimize("s") (0x0077F270, 0x00A72B90,
  0x007BBDCA).
- A vector element used as `this` of a call loads as `mov eax,[first] ; lea ecx,[off+eax]` when it is bound to a named
  reference local first; calling through `v[i].` directly gives `mov ecx,[first] ; add ecx,off` (0x00798DD0).
- `slot.SetPItem(other.GetPItem())` forms the slot address after the GetPItem call; an inline `operator=` doing the same binds
  `this` first and computes the address before the call (0x00798D90, 0x00798E10).
- A pointer member tested and then used as `this` of a virtual call loads straight into ecx when the OWNER is the named local
  and the member is written twice (`if (w->Cam) w->Cam->Get()`); a local copy of the member loads into eax and moves to ecx
  (0x00761B10).
- `if (!a && b()) return 0; return 1;` gives retail's `jne`/`je` pair with `xor eax,eax` / `mov eax,1` exits; two separate
  ifs (`if (a) return 1; if (b()) return 0; return 1;`) fold into `neg al ; sbb eax,eax ; inc eax` (0x004C70F0).
- Early-true tests whose single `mov al,1` exit sits after the main body are nested `!=` ifs with one `return true` at the
  end, not a run of `if (..) return true;` (0x007990C0).
- Under optimize("s") an inline default constructor of a member object (CEnginePrimitiveHandle) is called out of line; mark
  it `__forceinline` (0x004DC991).
- VC7.1 drops a smart pointer destructor's null test on a path where the pointer was just dereferenced (`p->flags & mask`):
  the success path of 0x0087C420 releases without `test ecx,ecx` in both compilers.
- TC vtable slot 13 is OnDie (eight manifest labels agree); slot 1 is the persist hook (OnSerialise).

Review at landing:
- Rows that needed more than 4 grades: none landed that way; 0x00798DD0 matched on the 4th probe form (reference local) and
  0x0077F270 on its 2nd (user copy constructor). The parked rows each used their four attempts (0x00812340 and 0x00779CA0
  ran a few extra probe variants in probe/).
- Local base models next to an existing engine header: 0x004DC991 does not use engine/CTCShotDefinitionProjectile.h (the
  header puts Line1 at +0xa0, retail +0x9c because CTCShotDefinitionBase is 0x9c bytes in retail); 0x00802650 models
  CTCTargetingPlayer locally (engine/CTCTargetingPlayer.h has PTargetLockedThing +0xec / LockedOntoEnemy +0xfc, retail
  +0xe4 / +0xf4); 0x0074F080 and 0x00812370 use retail offsets for CTCContainerRewardHero (Enabled +0x18, the derived counter
  +0x1c; PDB +0x1c / +0x20). 0x00761B10 uses the engine/CWorld.h offset (+0x34) but a local struct. engine/CTCBase.h is
  included by 0x0074F080.
- 0x00AF2910 and 0x00AF3A40 are byte-identical apart from the call displacement (same CVectorMap instantiation, two TUs):
  both sources are the same text; the lander may want one shared name.
- ghidra_out/egor_signatures.tsv names 0x00AF3A40 "CSystemManager::ClearAllInputEvents" -- a propagated signature.
