# Chunk Q24 notes

Result: 3 matched (RELOCATION_MATCH, strict headers), 3 DIFFER, 0 skipped. 17 grades used in total (see ATTEMPTS.log).

## Matched rows

All three are in payload.json and oracle.tsv. Each passes its behaviour test under VC7.1 (`behaviour_of`) and under g++.

### 0x00584B86 CTCInventory::AssignThingPC (854 bytes, 4 grades)
- **Headers:** ported onto the shared headers. It uses the overlay `FABLE_RETAIL_CTCINVENTORY_ASSIGNTHINGPC`.
- **TU flags:** `#pragma optimize("y", off)` and `#pragma optimize("s", on)`. The pragmas sit in the source, so no extra flags are needed.
- **Shape:**
  - The old-contents lookup is an if/else of two FindChild calls. VC7.1 cross-jumps them into one call with the name pushed in each arm.
  - The GetDef<CThingObjectDef> and GetSubDef<CAbilityDef> bodies are visible as noinline. This gives frame 0x34 and drops the dead `Object = 0` stores.
  - The swap action's slot `push_back` is written inside both arms, which gives retail's block order.
- **Local class:** CAbilityDef stays local, because no shared header exists (only `_quarantine/`).
- **Made-up names (descriptive):** none. The locals are descriptive.

### 0x00622A00 CTCInventoryQuests::OpenInventory (877 bytes, 4 grades)
- **Headers:** ported. It uses three overlays:
  - CTCInventoryQuests `_OPENINVENTORY`
  - CTCInventoryBase `_SELECTEDCATEGORYINDEX`
  - CTCHeroCentre `_GETQUESTCARDS`
- **The coordinator should review the stand-in body.** The row defines a same-TU `ConstructQuestList` (0x0061B610), out of line under `auto_inline(off)`, whose body is only `NUISystem::CManager::GetInstance();`.
  - **Why it is needed:** retail's TU holds the real ~2 KB body. c2 sees from it that the quest vector does not escape, so `_M_finish` stays in ebp across the card loop. With only a declaration, the bytes differ.
  - **Effect on landing:** the stand-in compiles to its own COMDAT. The graded row still calls 0x0061B610, but if the lander links per-TU, that symbol must not collide with a future landed ConstructQuestList.
  - **Alternative:** if the policy rejects stand-in bodies, the row needs the real ConstructQuestList landed first, in the same TU.
- **Menu pointer:** the menu is held as a `NUISystem::CList*`, which is ConstructQuestList's parameter type. Pushing it into Components then goes through a temporary, as retail does.

### 0x009289B0 CAIStateGroup_BanditPlot::Act (1131 bytes, 5 grades)
- **Identity:**
  - **Base class:** RTTI shows `CAIStateGroup_BanditPlot : CAIStateGroup_BanditPlayDice`, and Ego_r gives it no own members.
  - **The "plot" is the dice game:** the "plot" at +0x18 is the inherited `CTCDiceGame* DiceGame`, and CTCPlot derives CTCDiceGame. Ego_r also has `?GetDiceGame@CAIStateGroup_BanditPlot@@MAEPAVCTCDiceGame@@XZ`.
  - **0x00929550:** this is `CTCDiceGame::SetCurrentPlayer(CThing*)` (see NAME_FIXES).
- **Shape:**
  - The draft wrapped the body in `if (result == 2) {...}` with a shared final return, which gave 1133v1131 with a duplicated tail.
  - The landed sibling 0x009285D0's flow fixes this: an early `return result`, `return` inside the loop, and `return` of the OUTOF call. It also puts the end frame in ebp and the constant 2 in ebx.
  - GFRandom(300, GetPWorldSeed()), the inline from engine/GFRandom.h, puts the div before the GetFrame call.
- **Headers:** ported onto the shared headers:
  - CAIStateGroupBase MEMBERS/GETCREATURE/PERFORMACTION
  - CAIBrain GETCREATURE
  - CCreatureAction_PlayAnimation IMPLICIT_DTOR
  - CSoundPair, CWorld, GFRandom, GFFloatRandom, CIntelligentPointer ASSIGN
  - two new overlays: CAIStateGroup_BanditPlayDice.h and CAIStateGroup_BanditPlot.h
- **Local class:** CTCDiceGame stays local, because it has no shared header.
- **Enum names:** the enumerators are the PDB's (`AI_ACTION_FINISHED` = 2).

## DIFFER rows

The best source for each is in differ/<addr>.cpp, with line 2 carrying the details.

- **0x00AE1BC0 CParticleSimulation::ReadFromFile, DIFFER(1158v1158), 1 grade.**
  - **Port:** ported onto the shared retail CDataInputStream.h (ReadFloat/ReadSLONG/ReadULONG/ReadEBOOL/Read) and the CParticleSimulation.h overlay. It uses the real PDB signature (references, global class). The draft's `NParticleEngine::` qualification and pointer parameters were wrong.
  - **Residue:** one instruction pair at the join after the alpha-array read. Retail has `mov eax,[ebx+4]; lea ecx,[eax+4]` where ours has `mov ecx,[ebx+4]; add ecx,4`. This is the same residue as the earlier drafts.
  - **Tried with no change:** 11 toy spellings and the flags /Gr, /Ow, /G6. /Oa fixes the join but breaks the frame. It looks like a register-allocation tie.
- **0x009AF7A0 CDefinitionManager::SaveBinaryDefinitions, DIFFER(1007v1007), 1 grade (baseline).**
  - **Residue:** `this` and the stream swap ebx/ebp.
  - **Why it was not ported:**
    - It needs typed CDefinitionManager arrays (CArray<unsigned char>, CVectorMap<unsigned long,unsigned long>, CArray<CInstantiatedDefInfo*>) behind a new opt-in.
    - The CInstantiatedDefInfo member names are in neither PDB, so they would be made-up names.
    - 0x009AE390 has no PDB name.
  - **Callee PDB spellings:** line 2 of the differ file records them:
    - SaveTable(bool) and GetTableRandomID
    - WriteEBOOL, WriteULONG and WriteSLONG
    - GetLength
    - Transfer (slot 18) and InternalSaveBinary (slot 21)
- **0x00B4A700 CEnginePrimitiveRenderer2D::Render, DIFFER(926v926), 2 grades.**
  - **Not ported:** the port is heavy. It needs renderer members, the camera desc copy ctor, Peek/SetCameraState, the lighting-manager Peek/Set calls, the graphic-bank delays and five globals.
  - **What was found:**
    - The per-primitive temp is a `CEngineSubPrimitiveChain {LastOpaque, LastAlpha}` passed to `Render2DPrimitive` (slot 10).
    - Naming the manager and the chain inside the loop fixes the push order. The remaining residue is the eax/edx choice in the ThemeChannels[0] colour copy (work/00b4a700_p1.cpp, 927v926).

## Made-up (descriptive) names
- **Function-local variables in all rows** (previous, manager, old, icon, list, quests, building, centre, cards, wait, endFrame, game, ...).
- **No new class, member or function names were invented.**

## Coordinator notes
- **0x00B30B50:** this is `CEngineCamera::SetCameraState(const CEngineCameraDesc&)`; the PDB names the type struct U.
- **Possible name fixes outside this chunk (not acted on):**
  - **Landed sibling 0x009285D0:** its Leaf line uses the QAE (public, non-virtual) spelling. The PDB has MAE (protected virtual). Its local EAIActionReturn enumerator names are also not the PDB's.
  - **Draft names for 0x009AF7A0:** the draft used Save, Persist and SaveHeader. The PDB names are SaveTable, Transfer and InternalSaveBinary.
