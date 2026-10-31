# Chunk 646 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Respell a vcall to its real retail signature before tuning a frame slot** (0x006572A0). The parked "text-extent temporary
  shares width's slot" residue came from a wrong signature. Font slot 1 is `C2DExtentsF GetStringExtents(const wchar_t*, bool)
  const`: an 8-byte non-POD result, plus a bool argument that the old model had attributed to GetString(0). With the right
  signature, through the PDB inline `GetStringWidth(const CWideString&)`, the row matched on the first grade. Check a
  callee's `ret N` and push count before blaming the frame packer.
- **An identical leading call in both arms of an if is hoisted by VC7.1, and that changes scheduling** (0x00712E40).
  `if (c == -1) switch (rand() % 3) {...} else if (rand() % 3 == 0) return c;` gives retail's `mov edi,[c]` before
  `or esi,-1 ; call rand`. A shared `long r = rand() % 3;` above the test schedules the parameter load after the call and
  rotates two later base registers.
- **Counted-pointer release `dec [edi] ; jne` needs every callee that takes the pointer to be visible in the TU** (0x006946C0).
  - The calls are an out-of-line `CCountedPointer<T>::Reset` (write it as a noinline explicit specialisation carrying the
    shared header's Reset body) and the `push_back(const CCountedPointer&)` that copies it (an STLport-shaped stand-in).
  - With both visible, the inline ~CCountedPointer loads Info straight into edi and uses the dec flags.
  - With either one opaque, it loads eax, copies to edi and re-compares `[edi]` with the zero register.
  - This lets the shared engine/CCountedPointer.h replace local count-block models.
- **PDB inline accessors keep fixing ecx/eax roles in member chains**: `GetWorld().GetGameCamera()->GetCameraManager(&m)`
  (0x006946C0) and `Component->PeekCutSceneMainFont()` / `GetTextBank()` (0x006572A0). A raw member read in place of
  `GetGameCamera()` swapped ecx and eax in the out-parameter vcall.
- **The landed sibling of an /Os NPlayerGui TU is the model to copy** (0x00649E8B, partial). Copy the 0x0064A060 visible
  noinline LowerBound plus the IS_THING_ALIVE template body: the hero temporary, the GetTC key and the first CWideString
  then pack into one slot as in retail. The two CCharString argument temporaries still pack together where retail keeps
  them apart.
- Ties confirmed again (four or more new levers each, no movement):
  - single x87 commutation (0x00868E50: k-sweep at three placements, double sums, dead reads, accessor and pointer
    spellings);
  - a global scratch-register rotation (0x009C8C00: k-sweep, stand-ins);
  - single load/store scheduling swaps (0x00697490, 0x008539C0);
  - dead-parameter-slot choice for a GetTC key (0x00687730: visible LowerBound, reference counts, PDB signature).

Review at landing:
- Grades per row this session (one lever per probe, in parallel): 00868e50 32, 006572a0 3 (match), 00712e40 3 (match), 00649e8b 12,
  009c8c00 11, 00697490 7, 006946c0 13 (match), 008539c0 7, 00687730 6, 00537776 2.
- The match at 0x006946C0 includes a same-TU stand-in body for push_back 0x00696FE0 and a noinline explicit specialisation of
  CCountedPointer<NCreatureAction::CActionEventBase>::Reset (the header body, 0x00695A00). Both are emitted as extra COMDATs in
  the TU, as is the usual I-3 practice.
- Local base models next to existing engine headers: 006946c0 (CCreatureActionBase.h flattens the vptr), 006572a0 / 00649e8b
  (CPlayerGui.h is a flattened PDB layout; there is no CDrawBase header), 00697490 (differ: CGameCameraManager.h flattens the vptr).
- Errors in landed sources (see NAME_FIXES.tsv):
  - 0x00657240 IsShowingScreenshotMessage spells 0x013B86A0 as `g_pDisplayEngine_013b86a0` and 0x004167C8 as
    CDisplayEngine::GetFrame.
  - 0x00437210 spells GFAYToVY as `GFAYToVY_009e1dd0`.
  - CDrawNPCName in 0x00657240 is modelled with `void* Name` where the member is a CWideString.
- The inherited 00697490 best attempt carried `#pragma pack(push, 1)` for an unused CCharStringData view. The parked file has
  it removed (same grade).
- The brief's vland command names `chunk602_oracle.tsv`. This chunk's oracle is `$S/batch/chunk646_oracle.tsv`, and that is
  the one used here.
