# acctB hand-back, chunk 2042 (1 row PARKED: 1929v1942, one missing local copy)

Authored by an Opus 5.5 agent, parked state re-checked by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x007F2DC0 `?FrameUpdate@CTCLightningNode@@UAEXXZ` | **parked**, DIFFER 1929v1942; test PASS (g++; the agent also ran VC7.1) | `rows/parked/007f2dc0.cpp` + `.test.cpp`; 7 of 30 grades used. True length 1942 (the 2323 estimate includes this TU's unreferenced inline COMDATs 0x007F3560..0x007F36D0) |

Coordinator check: `rows/parked/007f2dc0.cpp` regrades DIFFER(1929v1942) under `hdr/`; the test prints
`007f2dc0_TEST PASS` under g++. Purity grep clean (the `volatile` diagnostic below was never in a delivered source).

## Residue (agent's measurements)

All 75 calls pair with retail's targets in order and the register colouring is retail's. What differs:

1. **Timer copy.** Retail reads `Timer` after the PDef null test, keeps it in a frame word and converts from there
   (0x007F2DE6 `mov eax,[ebx+28h]`, 0x007F2DEF `mov [esp+18h],eax`, 0x007F2DFA `fild [esp+18h]`); ours converts the
   member directly. Sixteen non-volatile spellings were all forwarded by VC7.1.
2. Because that copy has no frame word, [esp+18h] and [esp+1Ch] swap (about 24 operand differences).
3. One store order at 0x007F2E9A (`mov [esp+5Ch],eax` after `fld [ecx+3Ch]` in retail).
4. Two alignment pads that follow from the size.

A diagnostic with the member made `volatile` (never delivered, not allowed) gives retail's frame and everything else
except the two `++Timer` and item 3, so **the Timer copy is the whole frame problem**. `COORD_IDEAS.md` has the
coordinator's reading for the next attempt (the copy looks like a by-value argument of an expanded inline holding the
lifetime test). The row goes to the acctB near-miss lane.

## Other review items

1. `CVectorMap::Insert` forced inline through a new opt-in `FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE`, with an
   `// Inline-budget:` line (plain inline scores 191 and is called; retail expands it).
2. Headers (`hdr/README.md`): `CTCLightningNode.h` `FABLE_RETAIL_CTCLIGHTNINGNODE_FRAMEUPDATE` (nested
   `CLightningBranch` with DESCRIPTIVE member names PTarget +0, PEffect +8, Timer +0x10: no PDB layout record exists),
   `CTCDParticleEmitter.h`, `CHitParameters.h`, `CThingFilter_EnemyCreature.h`, `C2DBoxI.h`
   (`_CENTRE_RADIUS_OUTOFLINE`), `CVectorMap.h`; new `CLightningDef.h`, `CThingFilter_In3DRange.h`,
   `CTCElectrocutedEffect.h`, `NParticleEngine_CParticleUpdateParameters.h`.
3. `NAME_FIXES.tsv`: 0x006E24D0 is the template ctor `??0?$CParticleUpdateParameters@KK@NParticleEngine@@QAE@XZ`;
   0x004C1060 is `GetFocalPos` (bound as `GetHelperPointOrCentre`); stand-in symbols on landed 0x007F4020 / 0x007F42A0 /
   0x007F4500 (three `GetAllThingsInRoughArea` instantiations; catalog says GetPBestThingInRoughArea), 0x007F3E90,
   0x007F3F60, 0x007F3AD0, 0x007F39A0 / 0x007F39F0 / 0x007F3A40.
4. FrameUpdate calls the `CCountedPointer<CLightningBranch>` destructor through 0x004DA6F7, a `jmp` to 0x004D799A that
   is landed as a free `_Forward` stand-in: which symbol each address carries needs a decision. The anonymous-namespace
   hash on `CThingFilter_IsElectrocutable` differs per TU.
