# Chunk 644 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport `list::size()` as `distance(begin(), end())` also fixes an edi/ebx swap in a list-to-list move block** (0x00A618D0,
  CBusyResourceManager::OnPostSwapScreens). This is the second confirmation of agent637's 0x00AEB060 lesson. The raw node-counting
  size() kept two dead walks but coloured the free-list sentinel ebx where retail has edi. Any STLport list row parked as an
  "edi/ebx colouring tie" should be retried with the distance() size first.
- **The std::bitset reference path sets the register of a reloaded object pointer** (0x007751A0). `UsingComponents[type]` through
  the non-const `bitset::reference` loads the reloaded ParentThing into eax and the bitset word into ecx, as retail does (`test ch,8`).
  The const HasTC paths (unsigned-long mask, `_Unchecked_test`, if/return-true) all load the object into ecx. The reference path
  leaves a dead `add eax,34h` (the `_M_wp` pointer) when the object register is dead, so it is not the retail spelling either.
  But it shows that the residue comes from how the word address is formed, not from operand order.
- **A bool HasTC written `(w[t / 32] & (1UL << (t % 32))) != 0` compiles to `shr ecx,0Bh ; test cl,1`** (+17 bytes over three
  tests, 0x007751A0). The STLport `_Unchecked_test` with a size_t position keeps `test ch,8`.
- **Same-TU stand-in bodies for GetDef and GetSubDef remove the def-slot escape** (0x0057AEB4). The 0x007F90B0 idiom applied to
  both out-parameter callees brought the frame from 0x2c to retail's 0x28. Replacing the raw slots with destructor-carrying
  CDefPointer locals (404 bytes) is wrong for this row.
- **An explicit `CCharString("X", -1)` argument temporary loads the object chain first** (0x0057AEB4 tail,
  `g_pMainGameComponent->GetTextBank()->GetTextBySymbol(...)`: `mov esi,[eax+14h]` before the string pushes). This confirms the
  agent640 observation for a member chain.
- Ties confirmed again (one or two new levers each, no movement): x87 operand order (0x00B4B8B0 with the PDB operator^, 0x00B9D730
  with the PDB signature), resize-temporary colouring (0x009A3460), GetDefName hidden-pointer scratch rotation (0x006AEEA0: the
  agent641 copy-ctor lever made it shorter), push/store scheduling (0x00B81820), zero-register CSE of a call-result compare (0x00A10560).

Review at landing:
- Grades per row this session (one lever each): 007751a0 11, 0057aeb4 6, 006aeea0 5, 006e94d0 4, 009a3460 3, 00b9d730 2,
  00b4b8b0 2, 00b81820 2, 00a10560 2, 00a618d0 1 (match).
- 0x00A618D0 models IDirect3DDevice9 / IDirect3DQuery9 locally with the real d3d9.h method names. No shared D3D header exists
  under rebuild/include; it would be good to add one.
- NAME_FIXES.tsv: SetStockState (ESTOCK_STATE), IsPermanentlyStocked (const), IsCreatureValidFollower (const, const
  CThingAICreature&), IsChild, GetDefName, NGameText::CDataBank::GetTextBySymbol and GDefStringTable carry wrong landed spellings.
- 0x00B4B8B0's differ still spells the function public `QAE` with a pointer argument. The PDB has private `AAE` returning class
  CMatrix3x4 (the parameter list is truncated in the name sets).
- 0x00B81820's differ uses address-suffixed global names (see NOTES.md, Header debt).
