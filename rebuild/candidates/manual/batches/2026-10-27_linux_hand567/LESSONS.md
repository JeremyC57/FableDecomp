# Chunk 567 lessons (GOTCHAS one-liners)

- VC7.1 stack packing follows lexical scope: a CDefPointer local inside `if (chosen) { ... }` was packed into a dead
  argument slot (frame 0x1C); the same statements at function scope after `if (!chosen) return;` got retail's own frame
  word (frame 0x20). When only the frame size and a few `[esp+N]` offsets differ, move a block's locals between scopes
  before anything else (0x00502B00).
- Two `if (cond) { chosen = it; break; }` with identical bodies emit two break blocks; retail's single shared block is
  one `||` condition (0x00502B00).
- A CDefPointer whose release also nulls the pointer (`if (Object) { release; Object = 0; }`) shows the `mov [slot],0`
  only where a loop passes the slot to the next PeekP... call; break paths and the function end drop the dead store.
- STLport containers in a local model need their allocator default template argument (`template <class T, class A =
  allocator<T> > class list`), or every mangled name that mentions the container loses `V?$allocator@..@std@@` and the
  leaf / callee spellings stop matching the PDB.
- STLport vector::erase(pos) of a pointer vector is `__copy_trivial` (memmove(pos, pos+1, bytes)) with `end() !=
  pos + 1` operand order: retail compares `cmp eax(finish), ecx(pos+1)` and subtracts the already-held `pos+1`.
- GFRandom(2, seed) == 0 inlines as `not al ; and al,1` after the LCG + GFROR13 call; the result can ride in a dead
  argument slot (the CWorld& here).
- Row extents: two of this chunk's rows were catalog over-runs (jmp-tail or jump-table end followed by other
  functions); seven were mid-function fragments (the row opens on registers/flags the parent set). Check the first and
  last instructions of a row before spending a grade.

## Review at landing
- 0x00502B00 / 0x005D64DD: the sources declare CWorld, CThingCreatureBase and NUISystem::CManager as method-only shells
  after including their engine headers under a rename; confirm the lander accepts this form (vland: no
  SHARED_HEADER_AVAILABLE warning now).
- 0x00502B00 emits the CIntelligentPointer<CThingBuilding> vtable and its `??_E` / `??_G` COMDATs (inline template
  dtor); retail slot 0 is 0x0050F3A0, labelled for CLandscapeBackgroundPatch (probably an ICF-folded body).
- 0x004291DE: bound as `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ`; the catalog calls it a Release -- the
  dtor and a ReleaseData helper are byte-alike candidates.
- 0x013B8A50 CSubtitledMessage::MostRecentGroupQueried: access (private `0` vs public `2`) unproven.
- 0x009CC410 FindIndexBySymbolCRC: probable (PDB signature + semantics; donor order differs).
- 0x0049B8D0: the landed "Transfer<CCharString>" is likely TransferVector<CCharString> (same jump-table shape as
  0x005E9840) -- not proven here, so not in NAME_FIXES.
- Re-split rows 0x0055A5D0 (82 bytes) and 0x005E9840 (180 bytes): byte-exact true extents in differ/ with tests; the
  remaining bytes of each row are other functions (listed in NOTES.md).
- 0x00564D50 (real start of the SKIP row 0x00564D5D) is an uncatalogued 42-byte copy helper with no proven name.
