# 2026-11-12 landing fn -- 0x006D8F10 CHeroExperienceDef::Transfer (one Fable author pass)

Source: `pending/2026-11-12_linux_handWF16F/`. Three Opus author passes in workflow wf16 left this row at DIFFER(693v692): every
instruction the same except where ebp is saved (retail pushes it mid-function at 0x006D9064). Second row of the Opus-only
workflows that Opus could not match; under the owner's rule it got ONE author pass with `args.authorModel = 'fable'` (scout, both
verifiers and the repair stage on Opus). The Fable author matched it on its first attempt (about 80 minutes for the whole
one-row workflow, including verification and a comment-only repair).

**What made it match:** the one-argument TransferVector has both binary modes as cases and a braced unreachable default,
`default: { __assume(0); break; }` (the NODEFAULT idiom; 66 landed rows already use `__assume`). VC7.1 keeps that dead-end block
until after register allocation, so ebx has no hoistable restore and ebp's save nests inside it, around the blocks that use it.
With an empty default the range check disappears and ebp is saved in the prologue (one byte longer). CPersistTraits<float> is its
own class whose writer takes `const float&` (FableWin leaf), which stops the by-value float getting a frame home.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1; both Opus verifiers passed it. Headers (additions under new opt-ins,
unused elsewhere, defaults unchanged per hdrcheck2): CHeroExperienceDef.h (TRANSFER: real STLport member types; the existing ONHIT
arm becomes the `#elif` of a re-headed chain, unchanged), CPersistContext.h, CPersistTraits.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fn): eight GFSerialise* template instances landed under bare or wrong names (0x006DD1C0, 0x006DC450, 0x006DBA50,
0x00466A75, 0x006DC350, 0x006DCFA0, 0x006DC550, 0x006DD580).
