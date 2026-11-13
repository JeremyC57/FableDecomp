# Chunk 3009 -- lessons (0x007C6E50 CTCTurncoatSpell::FrameUpdate, near-miss retry 3, still parked)

- (0x007C6E50) Prove a one-decision residue with a STICKY negative-spill what-if, not spill 0: c2 re-costs a deferred low-spill piece
  as f(sym)+spill (0x10789715, f = 2 for a non-constant) and then 0x1075111c colours it anyway when its copy-preference list (+0x38,
  nodes {next, regdesc, count}) names an allowed register. Spill -5 sticky on the XP piece gave 551/551 equal instructions.
- c2 split-piece benefit (+0x40) = 2*w per operand use (hook 0x10782692) minus 2*w per 0x26A reload pseudo-tuple at each entry into
  the piece from memory (0x1078281c; 0x26B at 0x1078cb73). A piece with as many entries as uses is left in memory -> the codegen-temp
  load + copy shape (`mov edx,[home]` ... `mov esi,edx`).
- A block is "blocked" (+0x34) for a split candidate when a 0x1c pseudo-tuple (op 0x2c7, made by 0x107967b2 at the start of the
  split pass) in it carries the candidate in its live-across set (0x1078b346 builds +0x2c/+0x30/+0x34; 0x1078b372 builds the webs by
  union-find over +0x24 live-in / +0x28 live-out). A copy in a blocked block reads memory and is not part of any piece.
- Probes that did NOT move the XP piece: GetTC body variants (default / ORIGINAL_CHAIN), visible same-TU bodies of IsTurncoated /
  ModifyTurncoatedness / SetTurncoatOriginator (lead E), a release flag joined before MakeNoise (lead B), switch case order, the XP
  lookup on word 0 (CSE'd HasTC address: the CSE is undone, the HasTC still counts as a parent use), turncoat or hero_stats kept live.
- Probes that DID make the XP block retail-like but break the rest: `this` live to the function end (map moves to edi); MakeNoise
  duplicated into the arms (1852 / 1897 / 1851 bytes, no cross-jumping, absorb piece coloured).

Review at landing: row stays parked; differ/ source = the chunk 2027/3013 source with a comment block added (bytes unchanged).
- (0x007C6E50, continuation) c2's live-range split builds REGIONS before placing its 0x1c boundary markers (0x10740a6e..0x10740bb6,
  markers 0x107409c9..0x10740e10 via 0x107967b2): blocks in layout order join a visited pred's (else succ's) region iff the block and
  the region still share a register that is neither clobbered by a call nor held by an already-coloured candidate; a block that
  shares none starts a region, and every region edge gets a marker (a split point: copies in the marked block read memory). So a
  piece that "should" stay in memory needs the block where it is used to have no common free register with the region before it --
  i.e. look at what is live (and coloured) ACROSS that block, not at the block's own statements. Tool: tools/c2p/c2prio_d.py + tools/my/occ.py.
- (0x007C6E50) Neither parent's colouring order (priority what-if -300 / -60) nor 12 hand-written XP-block spellings moved the residue.

## Continuation 2 (matched)
- (0x007C6E50, RELOCATION_MATCH) A one-piece register-allocation residue that ~25 local spellings could not move was closed by
  CONTROL-FLOW SHAPE, not by the statements themselves: `bool give_xp = false;` at the top of the enclosing arm, set where the
  counter wraps, and the guarded lookup in `if (give_xp) { ... }` after the arm's if / else-if chain. VC7.1 threads the flag away
  (no flag bytes), the layout stays the same, but c2's live-range split of the caster changes so its piece in that block stays in
  memory (retail's hoisted `mov edx,[home]` ... `mov esi,edx`). Diagnose with two cheap grades first: (1) swap the if / else arms,
  (2) move the block behind a flag after the whole construct; if either reproduces the target block's prefix (with the wrong
  layout), the lever is where the block sits in the flow graph -- then bring it back inside the arm behind the flag.
- (0x007C6E50) A grade costs ~1.5 s for an 1800-byte row: grade candidates before tracing. The split-pass region model from the
  first continuation did not predict the arm-swap result (the XP map block stayed in the same region in the trace), so trust
  grades over a partial c2 model.
- Review at landing: the `give_xp` flag is FITTED (descriptive name, invisible in the bytes); 0x006D8300's bound spelling `JJ` is
  wrong (NAME_FIXES.tsv); 0x007C76D0 is labelled _unattributed::AssignTarget_007c76d0 in the repo but is
  ?SetTurncoatOriginator@CTCTurncoat@@QAEXPAVCThing@@@Z (LABELS.tsv; vland RELOC_NAME warning).
