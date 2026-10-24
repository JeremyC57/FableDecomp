# Hand chunk 358 (_Move<pair<long,CCountedPointer<CParticleEmitter>>> .. _Move_backward<pair<unsigned long,CDefString>>): 40 of 40 byte-exact

Every row carries a propagated Dinkumware `_Move` / `_Move_backward` label. The bodies are the retail SGI/STLport
algorithm back ends from /Gr TUs (`__fastcall`: first in ecx, last in edx, the rest on the stack): `__copy` /
`__copy_backward` with the `random_access_iterator_tag` + `Distance*` arguments (`ret 0Ch`), plus one
`__uninitialized_copy` (`ret 8`), one `remove_copy_if` (`ret 0Ch`, by-value predicate) and four `__destroy_aux`
(`ret 4`). All 40 sources are free `_global::<Algo>_<Element>_<addr>` functions with a `// Leaf:` line (every leaf
checked against the object's symbol table); 12 MATCH + 28 RELOCATION_MATCH, no pragma or flag needed. All 40 tests
PASS under g++ and the lane's VC7.1.

## What matched (40)
- **POD block copies (10: 0071ed80, 0075b5b0, 0071edd0, 00819670, 008197b0, 0085d870, 00a31480 backward; 00819390,
  008194c0, 0085d6e0 forward)** -- `__copy[_backward]` over 12/20/24-byte trivially copyable elements. Shape: an opaque
  `unsigned char _body[N]` element (the landed 0x00ADAB20 / 0x00AE50E0 model) gives the one-shuttle-register
  (`ebx`) dword chain; the count is the reciprocal-multiply quotient held in ecx/edx. 0071ed80/0075b5b0 are byte twins
  of the landed 0x00ADAB20. Matched first try.
- **CCountedPointer pairs (6: 00a5acc0, 00cbb120 forward; 006b9100, 00a802a0, 00bed9a0, 00cbada0 backward)** -- the
  landed 0x0084F130 / 0x00CBAE30 `CCountedPointer::operator=` (source block and object read into locals first) inside
  an implicit pair assignment. The count sits in the dead `result` argument slot because result lives in ebp -- no
  special form needed. Element sizes from the divisor: pair<long, CP> (12), pair<unsigned __int64, CP> (16), and for
  00bed9a0 a 16-byte pair<long, polymorphic {vptr, CP}> (the word at +4 is never copied).
- **CCharString-keyed pairs (9)** -- out-of-line `CCharString::operator=` @ 0x0099EFB0 per string member, plain words
  in between: 009e6410 pair<CCharString, CSymbolMap::CSymbolInfo>, 00af2d60 pair<CCharString,
  NParticleEngine::CRegisteredPSCInfo>, 004b78d0 pair<CCharString, CRuntimeQuestPersistence::CPersistInfo> (all three
  labels right, Ego_r.pdb layouts); 005659d0 {3 strings, 2 bools}; 0077e170 / 00a6e7c0 pair<CCharString, 12/16-byte
  POD> (the POD moves as a block, re-addressed in eax/ecx); 00a45910 {CCharString, long, 12-byte POD} (the CTargetPoint
  shape); 00cb9c80 / 00cb94d0 pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (label right:
  CPersistInfo = {ulong64 UID, CCharString} is 8-aligned, so the 24-byte pair has 4 bytes of uncopied tail padding).
- **Records with out-of-line member assignments (6)** -- 006f4470 {4 dwords, vector @+0x10} (the vector op= 0x006F3C20
  itself divides by 28: a record tree); 0082cd00 60-byte record with four member op= calls and a dword between them;
  00889650 48-byte record with a byte, a vector op=, a byte and a `double` (implicit op= copies it with
  `fld/fstp qword`); 00a85740 40-byte {vector, vector, CVectorMap} (the byte at +0x24 is the CVectorMap's EMPTY
  Compare and is skipped, Dirty at +0x25 is moved); 00bee4e0 / 00bee7b0 pair<pair<long,long>, CEngineTextureDecal>
  (label right once CEngineTextureDecal is taken at its retail 20 bytes: the PDB's 16-byte vector is 12 in retail).
- **006fbc50** pair<unsigned long, CSmallVector<CEvent,32>>: the landed 0x006FB960 model (operator= = `if (&other !=
  this) Copy(other)` around the out-of-line Copy 0x006FB5C0); the implicit pair operator= is enough, VC hoists the
  self-test `cmp` above the key move.
- **008076d0** NPlayerTargeting::CTargetPoint copy_backward: the landed forward 0x00806EA0 model verbatim
  (SetPItem(GetPItem()), Index, implicit C3DVector assignment, FrameRejected).
- **005e0e70** pair<unsigned long, CAnimationEntry>: Ego_r.pdb CAnimationEntry with an inlined intrusive
  `CIVCountedPointer::operator=` -- `if (Object != other.Object) { Release(); Object = other.Object; if (Object)
  ++Object->m_refCount; }` with the CPSCLight-style `Release()` (store 0 inside the non-null branch). Plain MATCH.
- **00af2ba0** STLport `__uninitialized_copy` (`ret 8`) of pair<CCharString, CRegisteredPSCInfo> (the same element
  as its TU neighbour 00af2d60): placement-new through the implicit copy constructor. A flat 16-byte
  {CCharString, long, long, CCharString} scheduled differently (attempt 1); the nested pair matched (attempt 2).
- **0096ba30** STLport `remove_copy_if` over 12-byte points with a by-value {height, tolerance} predicate
  `!(fabs(p.z - height) < tolerance)` -> `fld ; fsub ; fabs ; fcomp ; test ah,5 ; jp`. Plain MATCH first try.
- **__destroy_aux (4: 0047c800, 006f44e0, 00722130, 00cdc000)** -- `for (; first != last; ++first) first->~T();` with
  the element's implicit destructor inlined; each element holds an STLport vector modelled as
  `~CRetailVector() { DestroyRange(_M_start, _M_finish); if (_M_start) free(_M_start); }` (free @ 0x00BFEA14) and
  members die in reverse order (vector before the strings ahead of it). Matched first try.

## Parked for Fable 5.1 (0)
(none)

## Skipped
(none -- no row holds a second function; every extent ends at its own `ret`)

## Label corrections
- All 40 `_Move<...>` / `_Move_backward<...>` labels are Dinkumware names propagated over SGI/STLport bodies:
  34 are `__copy` / `__copy_backward`, 0x00AF2BA0 is `__uninitialized_copy`, 0x0096BA30 is `remove_copy_if`
  (caller 0x0096BAB5), and 0x0047C800 / 0x006F44E0 / 0x00722130 / 0x00CDC000 are `__destroy_aux` (`ret 4`, no
  destination).
- 0x00BEE7B0 is labelled `_Move_backward` but walks forward: SGI `__copy`.
- Element labels that are WRONG (sizes from the divisor): `pair<unsigned __int64, CPersistInfo>` on 0x006F4470 (28 B),
  0x00819390 / 0x0071EDD0 / 0x00819670 (24 B POD), 0x008194C0 / 0x0085D6E0 / 0x008197B0 / 0x0085D870 / 0x00A31480
  (20 B), 0x0082CD00 (60 B), 0x00889650 (48 B), 0x0071ED80 / 0x0075B5B0 (12 B); `pair<unsigned __int64,
  CCountedPointer<CActiveEntityScriptBase>>` on 0x006B9100 / 0x00A802A0 (12 B = pair<long, CP>) and 0x00BED9A0
  (pair<long, polymorphic holder>); `pair<long, CCountedPointer<CParticleEmitter>>` on 0x00CBB120 (16 B, 64-bit key);
  `pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>` on 0x005659D0 (16 B) and 0x00A85740 (40 B);
  `pair<pair<long,long>, CEngineTextureDecal>` on 0x0077E170 (16 B), 0x00A45910 / 0x00A6E7C0 (20 B), 0x00AF2BA0;
  `vector<AttackParticleAndEndPoints>` on 0x0096BA30; `pair<unsigned long, CDefString>` on the four destroy_aux rows.
- Element labels that are RIGHT: 0x00CB9C80 / 0x00CB94D0 (pair<u64, NScript CPersistInfo>), 0x00CBADA0, 0x00A5ACC0 (size-consistent; pointee type not re-checked),
  0x009E6410, 0x00AF2D60, 0x004B78D0, 0x008076D0, 0x005E0E70, 0x006FBC50, 0x00BEE4E0 / 0x00BEE7B0.
- Callee labels: 0x0099EAE0 (`_Cons_val<...EHeroMorphType...>`) is ~CCharString; 0x006F3C20
  (`CCombatWheel::CopyCombatWheel`) is a vector<28-byte record>::operator=; 0x00888EE0 (`Vector_Reallocate`),
  0x0082C980 (`std_Move_UintVector`) and 0x0045BAAA (`CActiveFile_AssignFrom`) are called as member `operator=`
  (`push &src ; lea ecx,&dst ; call`) -- modelled as such, not verified further; 0x009F2E70 / 0x009FA1C0 carry
  CSurface / CTexture `operator=` labels and are used as 16- and 8-byte member assignments.
