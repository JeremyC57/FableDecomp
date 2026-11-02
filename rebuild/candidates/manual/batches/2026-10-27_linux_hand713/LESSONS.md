# Chunk 713 lessons (GOTCHAS one-liners)

- (chunk 713) A switch over a contiguous range whose retail prologue is `sub eax,N; cmp eax,M; ja` (not `add eax,-N`) comes only from a named
  unsigned local `unsigned long index = v - N; if (index > M) return ...; switch (index)`; a plain `switch(v)` with cases N..N+M always emits
  `add eax,-N` under 3077 and 4035 with any flags (12 one-file experiments). Only 0x006817F0 and 0x00681850 use the `sub` form in the exe.
- (chunk 713) Sparse cases in a switch give a compare chain; list every case of the range (both arms) to get the jump table + byte table.
- (chunk 713) Implicit dtor of a class with an implicit-dtor base template (CLevelScriptBase<T>) tail-jumps to the grand-base ??1CScriptBase only
  if the template's dtor is NOT user-declared; a declared `virtual ~CLevelScriptBase();` makes the tail target ??1?$CLevelScriptBase.
- (chunk 713) Byte-twin implicit dtors (CModeJackOfBladesFlash / CModeScorpionKingTailPlunge, 59 bytes) are identified from each one's slot-0 ??_G
  and RTTI vtable, never from the catalog label (both were labelled ~CAIStateGroup_CarryCrateAndFollow).
- (chunk 713) An NScript level-script dtor is identified by vtable -> ctor -> Alloc `push size` -> S_V.. registration tag -> conversion_queue.tsv.
- (chunk 713) A header overlay copy goes stale when another chunk lands an edit to the same header: re-diff against the repo copy before the
  includer recheck (0x0055D590 SRC_FAIL was chunk 712's RemoveFromGlobalTCs missing from my copy).

## Review at landing
- Merge the two CTCBase.h lines (class CWorld; PeekWorld) into whatever CTCBase.h is current; do not overwrite.
- Apply NAME_FIXES respellings in landed callers of 0x00CB6E60, 0x0049BA20, 0x00701840, 0x0099A330, 0x0099A360 and the four ??_G rows.
- 0x00681850's local `class COpinionReactionManager` declaration should move to a shared header when one exists.
