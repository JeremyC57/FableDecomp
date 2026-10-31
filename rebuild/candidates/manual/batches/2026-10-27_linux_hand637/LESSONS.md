# Chunk 637 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **PDB private inline helpers fix register and immediate choice.** `long ConvertDriveLetterToNumber(wchar_t l) { return l - L'@'; }`
  gives the imm8 `add edi,-40h` and the retail this/letter register assignment. A compound `-= '@'` or a `(wchar16)(x - 0x40)`
  cast gives imm16 and an esi/edi swap (0x009E4DA0).
- **A result local declared before an object local hoists its zeroing.** `EDriveReturn result = 0;` before `CWideString fullPath`
  puts `xor ebx,ebx` ahead of the first call (0x009E4DA0).
- **Ego_d accessors over member chains.** `agent->GetRadius(bool)` (forwarding to NavigatorInterface->PeekNavigatorRadius) loads
  the agent into eax and the interface into ecx. A direct chain loads both into ecx (0x00A1DE30). Respelling pointer
  parameters to the PDB's references moved a `pop edi` (same row).
- **Lock temporaries pack only with user ctors.** A 2D-extents struct with user default and copy ctors lets VC7.1 overlay
  the Lock result temporaries on the GetDimensions temporaries (frame 0x38 -> 0x20). Ego_d `GetWidth(level)` /
  `GetHeight(level)` inlines fix the multiply operand order; VC canonicalises a direct `.X * .Y` either way (0x0071BCD0).
- **STLport size() is distance().** `distance(begin(), end())` through an input-iterator `__distance` that copies `__first`
  gives `cmp esi,edi ; mov eax,esi`. A raw node-counting loop copies first (0x00AEB060).
- **Insert-before-previous is `--it; insert(it, x)`.** The walk pointer steps back (`mov esi,[esi+4]`) and the by-value
  iterator is built in the argument slot. `iterator(it->_M_prev)` loads into ecx (0x00AEB060).
- **VC7.1 swaps consecutive parameter-copy stores in pairs.** In a long ctor of `member = param;` lines, retail's store order
  Height, Width, MapY, MapX ... means the source said Width, Height, MapX, MapY. Derive the body order backwards from
  retail. Declaration order, parameter order and an initializer list (308 bytes) are all wrong (0x00BE5E70).
- **`fabs` of a float from <math.h> is the float overload.** It schedules the fstp after a by-value argument slot setup
  (`push ecx ; mov ecx,esp ; push -1`). An implicit double fabs puts the fstp first (0x0056EB48).
- **A named pointer local before a nested block of temporaries.** `CWorldMap* worldMap = WorldMap; { std::vector<CThing*> things; ... }`
  packs the vector into a dead pair slot and sinks its zero stores below `push esp` (0x005223F0).
- **An out-of-line sibling inlined in a loop condition: copy its landed body as an inline member.** WaitWhileBabiesFight's
  loop is `while (IsStillRunnable())` with the landed 0x0095A420 body (named `CircleMarkers[0]` reference, separate
  early-return guards). Re-expressing the same tests inline in the loop (IS_THING_ALIVE(marker), `size() > 0 && ...`)
  rotates esi/edi between the marker and the loop-carried boss (0x0095C0B0).
- **A global read before a discarded call is source order.** `bool show = flag; GetThingManager(); if (!show) return;`
  loads the flag into bl before the call. Reading a static member through `GetThingManager().Flag` loads it after the call
  (0x007471F0).
- **A C3DVector argument temporary from a named reference sets the position schedule.** `CEnginePrimitiveText(0, C3DVector(pos.X,
  pos.Y, pos.Z + 3.0f), ...)` with `const C3DVector& pos = GetPos()` gives retail's `fld ; mov ; fadd ; mov` interleave. A named
  copy with `Z += 3` stores late (0x007471F0, still parked on one frame word).

Review at landing:
- Rows that needed more than 4 grades: 009e4da0 (17 probes), 0071bcd0 (14), 00be5e70 (11), 005223f0 (10), 00a1de30 (8),
  00aeb060 (8), 007471f0 (7 here + 5 in the sweep).
- Local base models next to an existing engine header: 00be5e70 (engine/CLandscapeBackgroundPatch.h flattens the vptr and
  EdgeStrips), 0095c0b0 (engine/CAIStateGroup_HoverHornetQueen.h flattens the vptr and the vectors), 00a1de30
  (engine/CNavigatorBase.h is included under `#define CNavigatorBase CNavigatorBaseLayout` because it flattens the vptr),
  00aeb060 (engine/CParticleEmitter.h flattens the +0x40 bit-field word), 007471f0 (engine/CTCBase.h flattens the vptr; there is
  no CTCStealth header). All say so in their headers.
- 00f35ed0 inlines GFFloatRandom (out of line at 0x004978A0, landed) as a local inline with the landed body. The lander
  should confirm this is acceptable, or provide a shared inline.
- 0095c0b0 keeps the landed models' pointer-returning CVectorMap::LowerBound spelling for 0x0040F020. Ego_r.pdb returns a
  _Vector_iterator by value. That is a bound spelling shared with Update 0x0095C1F0, not a new one.
- Already-landed sources with wrong spellings: IsStillRunnable 0x0095A420 and Update 0x0095C1F0 spell IsStopping as
  QAE (PDB IBE). The landed caller of 0x007AEEC0 spells CountLivingHelperCreatures QBE (PDB CountLivingHelpers QAE). The
  landed DeleteDirectory/DeleteFile spell DriveExists returning long (PDB EDriveReturn). 0x00AEC6F0 has the wrong element
  type. All are listed in NAME_FIXES.tsv.
- 0x005223F0 keeps the bound global g_CCharString_013bab08 for what Ego_r.pdb names CLevelLoader::LoadAllSectionName. Its
  access, and so its mangling, is unproven.
