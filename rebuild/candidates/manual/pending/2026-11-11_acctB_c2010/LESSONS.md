# Chunk 2010 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x005248B0) Loop-invariant default stores hoisted into a loop preheader (`xor edi,edi ; mov [esp+0A0h],edi ; ... ; mov [esp+30h],edi`
  before the dead-thing loop, 0x00524C6B) come from a VISIBLE CPersistContext::Transfer<T> body (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY):
  VC7.1 then knows the out-of-line call only reads its `const T&` default and moves the temporary's store out of the loop; with the
  declaration only, the stores stay in the loop and `this` takes edi. The plain `(unsigned __int64)0` temporaries are right.
- (0x005248B0) Same-TU small members retail has out of line (AddPersistedDeadEntity 0x00520330, SetGamePersistentScript 0x005237F0) are
  expanded by VC7.1 inside a loop when their bodies are in the TU; define them above the row (resize(n) -> resize(n, T()) expands with them).
- (0x004959E0) `bool dots = name.ToCharString().EndsWith(".");` as its own statement destroys both string temporaries before the next test
  (0x00495AFB..0x00495B0F); folding it into `if (!a && !b)` keeps conditional-destruction flags (an entry `mov [F],0`).
- (0x004959E0) A by-value CWideString argument converted implicitly from `unsigned short*` builds the temporary before the constant pushes
  and re-takes its address with `lea`; `CWideString(buffer)` pushes the constants first (CDiskFileWin32(name, 1, 2), 0x00495BA0).
- (0x00716DE0) A float tag test retail compiles `fild dur ; fmul key ; fild counter ; fcompp ; test ah,1 ; jne` is `key * dur <= counter`;
  `!(counter < key * dur)` and `counter >= key * dur` load the counter first and test `ah,41h` (measured at all three tag sites).
- (0x00716DE0) Retail reading a global int (ConstantFPS) BEFORE an out-of-line float call and spilling it for a later `fimul [frame]`
  (0x007170B0 / 0x007170BA) is a named `long fps = CMainGameComponent::GetConstantFPS();` statement; `fps * call()` or `call() * fps` in one
  expression calls first and `fimul`s the global. The scheduling of that load among the call's argument loads came right only with a
  named `CThingCreatureBase& creature = GetParentCreature();` before it.
- (0x00716DE0) A stack CCreatureAction_* destroyed by a direct `call ~CCreatureActionBase` with no derived vtable store means the derived
  classes have compiler-generated destructors (CCreatureAction_PlayAnimation / ...WithLookTurning / PlayConversationAnimation).
- (0x00492381) VC7.1 counts every local in scope at an inline-asm block (the GFFloatToLongFloor / Ceil helpers) as referenced: +2 IL
  weight each (c2 caller 0x107E25C9 in the frame-packer trace). A record that is still in scope at the helper outweighs the later
  16-byte nodes and is packed with the wrong slot; closing its block before the helper (retail's frame: the record shares the parser's
  words at [ebp+14h]) fixed the frame permutation.
- (0x00492381) The newline-count loop retail tests against the zero in memory every pass (`and [ebp+40h],0` .. `cmp eax,[ebp+40h]`, no
  rotation) is a `bool done` loop: `for(;;){..; if (n == last) break; ..}`, `while ((n = f()) != last)`, `while (true) {if..else break;}`
  are rotated by VC7.1 with the first test peeled against the known 0 (the residue of the parked twin 0x00491AD2, hand784: try it there).
- (0x00492381) A C2DBoxF argument whose Y members are `VIRTUAL_SCREEN_HEIGHT - GFAYToVY(..)` converts the double implicitly at the float
  parameter; an explicit `(float)(...)` cast keeps a no-code tuple that schedules the TLX `fstp` before the TLY copy (0x00492606). A named
  box costs frame words (0xAC v 0x9C): pass it as a temporary.
- (0x00492381) `X += (a - f()) * 0.5f` forms and spills the record address before the call; retail forms it after: a named
  `float offset = (a - f()) * 0.5f; X += offset;` (0x004928D4..0x004928E8).
- The c2 frame-packer trace (agent2003 tools/pk.sh + phook.py, copied to tools/pk/, plus tools/pkslots.py here) is a diagnostic build:
  it names every node, its IL weight and its slot, and with tools/omap.py (built offset -> retail offset over the aligned listings)
  turns a slot permutation into "which node must weigh less" in two or three runs.

Review at landing:
- All four rows need the hdr/ overlay (14 headers, additions only; hdr/README.md), including chunk 2001's CStringParser INLINE_CTOR /
  READLINE and chunk 2003's CENTITYRUNTIMEPERSISTENCE_CLEARINSCRIPT blocks copied from those hand-backs: merge once.
- NAME_FIXES.tsv (12): respell the landed callers / rows, notably 0x00751A70 (landed as a free `EditWorldUpdate` stand-in), 0x00999850
  (landed as CAFile::FindNextFileSequential), 0x0049417D (CRetailVector stand-in), 0x0048DA89 (const char* overload), 0x00842400 (pointer
  / long ctor spelling).
- 0x00716DE0 needed 10 grades (pass 1); 0x005248B0 8; 0x00492381 7.
- Descriptive global g_bLeftAlignText 0x013B861B (no PDB data-symbol dump).
- The parked twin 0x00491AD2 (hand784, ProcessEmbeddedHighlightTags) has the same newline loop and frame residue: the `done` loop and the
  asm-scope lever above are the next tries there.
