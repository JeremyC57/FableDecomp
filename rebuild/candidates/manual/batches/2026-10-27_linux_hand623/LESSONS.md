# Chunk 623 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Two divisions by one named float local** make VC7.1 compute a single reciprocal itself, and it emits it as
  `fld [x] ; fdivr 1.0` (the divisor is already on the x87 stack) for every use. A hand-written `1.0f / x` (inline
  helper, named or reference local), or two divisions by the member read in place, give `fld 1.0 ; fdiv [x]`. So when
  retail shows `fld [mem] ; fdivr [1.0]` followed by multiplies, write the source as divisions by a named local
  (0x006D63A0 GetRunningNumHits).
- **Three whole call statements, one per if/else arm** (`r = F(a.G(n))` / `r = F(b->G(n))` / `r = F(c.G(n))`) are
  cross-jumped by VC7.1 into one tail that keeps retail's `mov ecx,this ; push eax` before the outer thiscall. A value
  local assigned in the arms, or a ternary argument, pushes eax first (0x0041DB1D NUISystem::CManager::CreateComponent).
- **`#pragma optimize("s", on)` moves a shared failure block** next to the FIRST test (retail's `je` over
  `xor eax,eax ; ret` layout), but it also brings an ebp frame. When the frame is the only remaining residue, a global
  /Os or /O1 flag may be the lever (0x00C73830, not graded under the 4-attempt cap).
- The `#define Class ClassLayout` + `#include "engine/Class.h"` + `class Class : public ClassLayout { ... }` pattern clears
  vland's SHARED_HEADER_AVAILABLE warning for a callee class without changing the bytes (0x0041DB1D CDefinitionManager).

Review at landing:
- Landed spellings proven wrong (NAME_FIXES.tsv): 0x00BACDB0 is
  `??0CEnginePrimitiveManager@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z` (the landed source says `(long)`, with members
  m_type/m_enabled where the PDB has SceneList/Enable). 0x0051F010 `unattributed_Contains` is
  CThingManager::DoesNPCNameEntryExistForThing. 0x0052B8B0 landed under the invented leaf `??ACNPCNameMap`; the PDB
  spelling is the std::map operator[]. 0x00527E00 is bound as `?Find@CTree_0051f010@@`.
- Several landed CEnginePrimitiveManager* constructors spell the scene-list argument as `CArray<CEngineScene*>*`. The PDB
  has a reference (`AAV?$CArray@PAVCEngineScene@@@@`).
- Local base models next to existing engine headers: NUISystem::CManager (engine/CManager.h has the wrong namespace),
  CEnginePrimitive2DViewportManager (header flattened), and the CTCPerceiveThing `field4` naming.
- All parked rows are same-length, and each got 4 or fewer grades this wave. Total grades per row across waves are above
  4 for 00c73830, 00521a90 and 00bc9b30.
