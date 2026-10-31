# Chunk 604 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU static whose private register convention will not match: put the WHOLE real callee chain in the TU as
  `static __declspec(noinline) __stdcall` bodies (libvorbis smallft.c drftf1 with dradf2/dradf4/dradfg, 0x00C9BC10). VC7.1
  picks the convention from what the callee's callees clobber.
- A "tag slot gap" park on vector find+erase was a modelling gap (0x009D5A30). It matched with the PDB `CArray<T>::Find`
  wrapper over `std::find`, the STLport `__find` random-access body as a same-TU noinline `__fastcall` template COMDAT,
  `random_access_iterator_tag` with a user ctor returned by `__iterator_category`, and the real STLport `erase`
  (`if (pos+1 != end()) __copy_trivial(pos+1, _M_finish, pos); --_M_finish;`).
- SIB operand order (`[base+index]` v `[index+base]`) flips when the loop sits in an inline routine that takes the text
  pointer as a PARAMETER. It must be non-static `inline`; `static inline` emits its own symbol. Pair it with a preset-plus-if
  accessor (`p = ""; if (d) p = d->PData;`), because the ternary accessor fails (0x009BB0D0).
- `if (!a || b) { Terminate(); return; }` -> two separate early-return ifs fixed a this/end callee-saved swap (0x0091B190).
- Read the real base chain from the exe (vtable-4 -> COL -> CHD base list) before modelling an AI state group. The template
  middle base (`CAIStateGroup_WorkBase<CAIStateGroupBase>`) and PBrain at +4 fixed the register chains (0x0091B190).
- An inline `C3DVector::GetMagnitudeAndNormalise()` written as `sq = Z*Z; sq += Y*Y; sq += X*X;`, with the inverse scaling
  inside the `len > eps` block, gives retail's `fst Z ; fmul Z` order and keeps the inverse until the block end (0x00815420).
- An implicit dtor that calls a member dtor at `this+4` identifies the record. 0x00627400's "action pair" is
  NUISystem::CDeletion {EDeletionMethod Method; list<CComponent*> AssociatedParents} with no user dtor; slot 62 is SetDeletion.
- GetTC key temporaries share one frame slot only when their full-expression lifetimes are disjoint. A GetTC in an
  `if (x && GetTC(&p))` condition lives across the body, so move it into its own statement (`bool has = GetTC(&p);
  if (has)`). That gave 0x0065CB6B retail's 0x18 frame.
- Address-taken `new` temporaries (`push_back(new T(v))` through a `T* const&`) go into a dead parameter's home slot
  ([ebp+8]) when that parameter is not read inside the same full-expression. A named pointer local never does, so drop the
  named local to get retail's frame (0x00584B86).

Review at landing:
- 0x009BB0D0 renames the callee landed callers bind as `?CleanText@CPersistContext@@QAEXJJ@Z` (0x00463334, 0x00475026,
  0x0047550F, 0x007AE3D0) to `?CleanOutLine@CPersistContext@@IAEXJJ@Z`. Respell those callers when this row lands.
- 0x0091B190 binds `?IsWorkTime@CTCVillage@@QBE_NXZ` for 0x00658B20. The landed definer CTCVillage_IsCookTime_00658b20.cpp
  is a free fastcall under the IsCookTime name; respell and retype it.
- The landed 0x0097DB70 / 0x0097EC00 / 0x008FCF40 callers use many invented per-class or QAE/pointer spellings
  (NAME_FIXES.tsv lists them). The PDB forms are the CAIStateGroupBase IAE/QAE ones bound here.
- Local base models next to existing engine headers: CCreatureActionBase and CCriticalSectionLock (0x0091B190, 0x009D5A30;
  `// Layout:` comments explain why), and the flat CAIStateGroup_HandyMan.h / CAIStateGroupBase.h.
- Rows that took more than 4 grades: all six parked rows, and 0x009BB0D0, 0x009D5A30 and 0x0091B190 among the matches.
- 0x00584B86's differ file is 873 v 854 bytes (NDIFF 55); the older 854-byte sec1b file has NDIFF 76. Pick by review.
- GOTCHAS.md is 619 KB. This agent read its newest sections and grepped the rest, not all of it.
- `$A/dl_vorbis/smallft.c` (upstream libvorbis 1.0.1, used only as text) was deleted at cleanup. src/00c9bc10.cpp carries
  the upstream functions verbatim apart from the calling-convention keywords (BSD licence text not copied; check whether
  the landing needs the notice).
