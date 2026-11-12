# Chunk 2051 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x004A6E30) A long "new + reset" init function matched at its first full grade once every owned member was typed as the PDB
  declares it (boost::scoped_ptr<T> / CCountedPointer<T>) and every class whose destructor retail expands had its REAL implicit
  destructor visible: c2 then splits boost::checked_delete<T> into expanded (vector-only dtors) and called (map dtors: 0x004ADD30,
  0x004AE080, 0x004AE810) copies by itself. Do not declare a user `~T();` for such classes (opt-in _IMPLICIT_DTOR).
- (0x004A6E30) boost::checked_delete<T> copies are `??$checked_delete@V<T>@@@boost@@YIXPAV<T>@@@Z` (__fastcall: the TU is /Gr).
- (0x004A6E30) A stack interface temporary passed by const reference (`CGameNavigatorManagerExternalInterface(*this)`) shows one
  vtable store and no destructor call: inline ctor + empty inline virtual dtor in the header.
- (0x004A6E30) `CClassTypeInit(long type, ...)` storing an unsigned char TypeID reads only the low byte of the long global
  (`mov cl,byte ptr [0129CBA4h]`).
- (0x0042EC7C) Biased `lea ebp,[esp-N]` frame: `#pragma optimize("s", on)` alone (GOTCHAS chunk 247 confirmed); the landed
  CNewFrontendGameComponent siblings use ("s", on) + ("y", off) because their frames are small.
- (0x0042EC7C) In a size TU a by-value CRGBColour argument built from a member (`SetClearColour(movie.ClearColour)`) needs the copy
  ctor forced (refused with no score); retail copies the bytes R, G, B, A in place.
- (0x0042EC7C) `T()` on a POD with no user ctor zero-fills with `stosd`; retail's single `mov [slot],ebx` after the allocation's null
  test was the PDB's inline default ctor (??0CFrontendGameComponentInit@@QAE@XZ).
- (0x0042EC7C) `v[rand() % (long)v.size()]` through a def pointer: one expression CSEs the def pointer across rand(); retail
  reloads it -> a named `long index = ...;` statement first.
- (0x0042EC7C, open) c2 hoists a constant address used in two separate if-blocks to their common dominator (`mov edi,imm` in the
  test block); retail sank the def into the first block and a skip-path block. Not yet reproduced (see NOTES.md).

Review at landing:
- NAME_FIXES.tsv: three landed checked_delete stand-ins, SetInput's enum scope, CCombatManager::Init, StatRegisterClass,
  CTBaseSingleton<CSkeletalMorphResourceManager>::PClass, CanStartAttractMode.
- 0x0129CBA4 has three landed names (g_NavigatorTypeDefault, g_groundWanderMode, g_flyingMode): pick one.
- vland on 0x004A6E30: 3 RELOC_NAME warnings only (catalog labels CWorld_InitFactions, ResetDialogue, a __destroy_aux label).
