# Landing 2026-11-05s: chunk 792 (2 of 6 rows landed, 2 parked, 2 not attempted)

One expert agent authored the rows. Both landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00AB6E40 | CJoystickDX::UpdateEvents | RELOCATION_MATCH PASS (6 grades; both switch tables paired by hand) |
| 0x00AB8E10 | CStaticFontBank::CStaticFontBank | RELOCATION_MATCH PASS (11 grades; `// Flags: /GS`) |

- **Key spellings:**
  - 0x00AB6E40: a two-level switch (18 dwords at 0x00AB77C4, 60 index bytes at 0x00AB780C). The button arms set a flag and one
    AddEventToStore / ProcessMaintainedEvents block follows the switch. The RX / RY case must store `addEvent = false`, or VC7.1
    folds it into the default and the table loses an entry.
  - 0x00AB8E10: the destination C2DBoxI is a temporary built in the call; `operator new[]` must be declared (else scalar new and a
    RELOC_IDENTITY against 0x00BFEB22); retail calls GFGetSystemManager twice and drops the first result. Header debt:
    engine/CBankFile.h is on HOLD, so the row declares a local CBankFile with GetEntryDataSize / ReadEntryData only.
- **Headers** (`hdr_README.md`): 14 headers gain opt-in branches and CEngineColourFilterSettings.h is new. hdrcheck2 reads
  `default=same`; only the new macros read DIFF. The agent rechecked all 143 landed direct includers with the overlay: 141 win +
  PASS. GetXAxis 0x00AB6AD0 and GetYAxis 0x00AB6AE0 report test FAIL with and without the overlay (their exes print PASS when run
  directly): a pre-existing test-harness issue, not this chunk's.
- **vland warnings:** four RELOC_NAME warnings, all labels the agent proved wrong (EmptyEventStore, HasInputFocus, the joystick
  button setters). The labels are queued.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05s.tsv` (the six rows, plus unlanded 0x009F4390 CSurface::CopyFromMemory,
    0x009A23E0 GFUpdateFontTextureCache and 0x009A1140 CStreamingFontBank::GlyphLookup).
  - 18 queue lines (tag `land0511s`): the CInputEvent joystick setters, CJoystick::EmptyEventStore, CSystemManager::HasInputFocus,
    DrawAsIndexedTris 0x00A0AF40, GlyphLookup (bound under the stand-in CStreamingFontBankX), and stand-in spellings of
    Uninitialise, GetSurfaceLevel, CTexture::Initialise, ReadEntryData and the CCharString default ctor.
  - 0x009BE870 is bound under three spellings. The PDB one (`...QAE_NKAAVCPixelFormat@@@Z`, both name sets) is used by the landed
    row itself and by 0x00AB8E10; 0x0041E3F6 (`...Format2...PAV`) and 0x0066CB00 (`...PAV`) misspell it (queued).
- **Landing:** `vland --land` (`logs/land.log`); labels applied (`logs/apply_labels.log`); the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand792/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter | DIFFER(2923v2916), 23 lines, ~40 spellings | `UpdateFlags \|= 2` as load/or/store, the CRenderStateManager bookmark flip one block late (whole-function), a loop-alignment nop; uses GFFloatToLongNear (would grade retail_asm) |
| 0x00B5AF80 CEngineScreenEffectColourFilter::RenderColorFilteredQuad | DIFFER(2863v2865), 33 lines, ~22 spellings | x87 stack keeps A*c where retail keeps R*c, plus the same flip. **The oracle row holds two functions**: the real end is 0x00B5BAB1 and a 10-byte setter with no xrefs follows at 0x00B5BAC0. Split the row before it lands. |

Not attempted (moved to chunk 793 with the agent's analysis): 0x009A25D0 CStreamingFontBank::Render and 0x00715A70
NTCScriptedControl::CActionTalkBase::SetUpCamera (two jump tables, 7 + 13 entries; needs a retail CActionTalkBase model).
