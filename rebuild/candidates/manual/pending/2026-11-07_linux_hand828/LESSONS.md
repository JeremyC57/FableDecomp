# Chunk 828 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 828) The chunk 813 / 824 loop form also fixes a one-pad scheduling swap: CAS_Trickster::Main 0x00E2C100 (2 lines after 22
  attempts over chunks 816 / 818) went RELOCATION_MATCH when its final `while (!T) { ...; NSF; }` became `for (;; NSF) { if (T) return; ... }`
  (the shared return pad 0x00E2E38E gets `mov edx,[ebp]; mov ecx,ebp; push 0`). Try it on every Main's last loop before parking a pad
  schedule.
- (chunk 828) Two identical copies VC7.1 cross-jumps where retail keeps two (a facing-angle wrap in a marker arm and a target arm,
  CAIStateGroup_JackOfBladesStage2::Update 0x00974AA0): writing the wrap out in each arm instead of a shared row-local inline helper
  keeps two copies (2680 -> 2928 of 3080 bytes).
- (chunk 828) A cross-jump retail does that ours does not (0x006C6040, two witness arms): a switch for the arms halves the layout residue
  (md 1026 -> 716) but the copies stay apart while their GetTC key temporaries sit in different frame slots.
- (chunk 828) A top-tested loop that VC7.1 rotates (CBanditForger 0x00D0C410 loop 1) gets retail's head when its last block is
  written `if (!X) continue; { ... } break;`, but the extra loop-exit edges let VC7.1 expand ~7 more return epilogues (+400 B).
- (chunk 828) `long& c = p->member; c = c + 1;` splits `inc [mem]` into load / inc / store with an unrelated load scheduled between, but
  keeps an `add eax,disp` for the reference (0x00D56790); a parent pointer / reference local or a named copy still fuses.
- (chunk 828) Check the parked source against the repo header first: the chunk 810 melee source SRC_FAILed because the repo's
  CQ_GuildTrainingMeleeScript.h (chunk 815) has no CMeleeOpponent body; re-adding it as an opt-in block was enough.

Review at landing:
- 0x00E2C100 is RELOCATION_MATCH with the read-only vland (no RELOC_IDENTITY); its test is the chunk 816/818 one (unchanged semantics).
- NAME_FIXES.tsv: three reloc_bindings.tsv spellings for 0x007E4040 / 0x008B7F80 / 0x008C33E0 differ from the PDB names.
- hdr/engine/CQ_GuildTrainingMeleeScript.h is an opt-in block for the parked melee row only.
