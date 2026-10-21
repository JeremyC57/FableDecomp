# Chunk 254 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU noinline stand-ins:**
  - Stand-ins for `IS_THING_ALIVE(CThing* const&)` / `IS_THING_ALIVE(const CBaseIntelligentPointer&)` and for the `GetTCDriver(T**)` instantiations tell VC7.1 that the addresses do not escape. VC7.1 then packs the things and the drivers into shared frame words, as retail does (CTCTrapBase::Trigger 0x00788229).
  - Caveat: a same-TU `__fastcall` stand-in also tells VC7.1 which registers the callee preserves. For example, after a same-TU Find it compared through edx where retail used edi (0x007FEDE0). If retail keeps a value in a callee-saved register across the call, keep that callee external.
- **Index loops:** a loop over a vector by index, written `*(vec.begin() + i)`, keeps i in a register and addresses with `lea ecx,[eax+ebx*8]`. `vec[i]` (`operator[]`) strength-reduces `i*8` and spills i (0x00788229).
- **CIntelligentPointer assignment:**
  - From a raw pointer (`PObstruction = thing;`), the implicit CIntelligentPointer temporary's address is taken again with `lea` rather than reusing the ctor's eax (0x00788229).
  - An operator= written as `{ CThing* item = other.GetPItem(); SetPItem(item); return *this; }` schedules the member's address after GetPItem (0x007FEDE0).
- **Opinion-source clearing:** an inline helper `void ClearOpinionSource(CThing* thing) { GetWorld().GetOpinionReactionManager()->ClearOpinionSource(*thing, true); }`, with an inline CWorld getter, loads the parent into edi before GetWorld. Reading the field directly pushes the arguments first and moves `this` into ebp, which changes the whole function's register assignment (CTCTrespasser::LeavingRegion 0x007BAF00; same family as CTCWife 0x006D0410).
- **GetInterface vs GetTC:** the out-parameter `GetInterface(&tc, t)` leaves the component in eax (`mov eax,[eax+4] ; mov byte [eax+0Dh],0`). The plain GetTC pick loads it straight into ecx. Pick per call site (0x007BAF00 uses one of each; 0x00834760 uses the plain pick).
- **Frame-pointer-size TU:** CTCTrapBase::Trigger 0x00788229 needs `#pragma optimize("y", off)` plus `#pragma optimize("s", on)`.
- **Gotcha in this lane (not the compiler):** VC7.1 behaviour tests run as C++03 under `/Od`, so `std::to_string`, `std::snprintf` and brace-initialised std::vector members fail with TCC_FAIL. Use `sprintf` and an assign-from-array helper (three tests in this chunk had to be rewritten).
- **pack(1) model:** CThingAICreature::WorldUpdate 0x00834760 uses the landed player WorldUpdate model: a `#pragma pack(1)` CThingCreatureBase, CurrentlyFrameUpdating as a `bool:1` at +0x15C next to the update flags at +0x15D, and GetLength written as `s = Z*Z; s += Y*Y; s += X*X`.

Review at landing:
- **Rows that needed more than 4 grades:**
  - Matched: 00788229, 007baf00, 007d8c20, 00834760, 00794c10, 0065a28f, 00833390, 007a16f0, 006cf920.
  - Parked: 007fede0, 00863ca0, 0065b142, 0065be6c, 007df130, 006594bb, 0065a075, 0065a8ee.
- **Local base models next to an existing engine header.** None of the 31 sources includes `engine/<Class>.h`. Each carries a `// Layout:` comment explaining why: retail STLport lists are 4 bytes and vectors/maps 12, against the PDB offsets in the headers. The headers involved are CTCVillage.h, CTCWillLightning.h, CTCTrapBase.h, CText.h, CThingAICreature.h, CTestPos.h and CTCWife.h. Check CTCWife (006cf920, 006cfaa0, 006d0410, 006ced50): its Layout line says "PDB = retail", so the shared header may work there directly.
- **Errors in already-landed sources:**
  - 0x007FD3D0 is landed as CAIStateGroupBuyFromSeller_BoughtSomething but is CTCDPhysicalObstruction::SetLifeTimeSeconds.
  - 0x007898A3 is landed as CTCTrapBase::SetThingAsImmune but is CTCDPhysicalObstruction::SetThingToIgnoreObstruction.
  - 0x007512C0 is landed as the CTCDExperienceOrb GetTCDriver, but it is a folded body shared with the explosion driver.
  - 0x0065F4E2 .. 0x0065F5AA are landed as "CVector range constructors" but are STLport `find` instantiations.
- **Catalog rows that hold more than one function:**
  - 004cd1e0 (CThing::vfunc_30) holds int3 padding and then another function. It was skipped.
  - 0x00834760 is catalogued as vfunc_19, but it is CThingAICreature::WorldUpdate.
  - 0x00CA6E10 is catalogued under CTextureRenderer, but it is strmbase CBaseRenderer::Receive and needs `/EHsc`.
- **Special flags:** 006cf920 needs `/GS`; 00ca6e10 needs `/EHsc`.
