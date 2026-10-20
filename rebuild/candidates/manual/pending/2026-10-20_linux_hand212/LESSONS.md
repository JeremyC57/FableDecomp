# Chunk 212 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A Ego_r.pdb `CDefIndex` member is a class, not a `long`: model it as `struct CDefIndex { long Index; CDefIndex(long i) : Index(i) {} }`. Initialised as a plain `long`, the zero store slides one instruction later past the next `push ; lea ecx` (0x005C3947, CTCInventoryWeapons ctor; same bytes otherwise).
- Early `return false` / `return true` statements let VC sink the ebp/esi prologue pushes past the first exits. When retail pushes all four callee-saved registers up front and every exit jumps to one shared tail, wrap the body in the guard (`if (flags & 8) { ...returns... } return false;`) so the guard failure and the switch's missing default reach the same `return false` (0x005DAB10 CTCWeapon::IsUsableBy).
- An inline `HasTC()` used as the left operand of a `return a || b;` is not expanded at /O2 (`push 84h ; mov ecx,edi ; call`); make it `__forceinline` (0x005DAB10).
- HasTC-guarded GetTC with retail `xor ecx,ecx` before `test eax,bit ; je` and a `mov eax,[thing+N]` load is `T* p = 0; if (thing->HasTC(t)) p = (T*)thing->GetTC(t);`. The one-ternary `HasTC ? GetTC : 0` gives `test dword ptr [..],bit` and puts the zero at the branch target (0x007DF6A0 CTCWallMountEffects::DoEffect).
- A `switch` with a 2-value enum compiles to `dec eax ; je caseA ; dec eax ; jne end`, with case 2 as the fall-through block (0x007DF6A0).
- In a const member over a `const CThing&`, the out-param GetInterface reaches the const LowerBound instantiation 0x004365B0 (not 0x0040F020). Its type temporary reuses the dead argument slot (`lea eax,[esp+14h]`) (0x005DAB10).
- (From the earlier rows of this chunk, recorded per row in the source headers) stand-in `GetSubDef_*` bodies under `#pragma auto_inline(off)` keep the def in a register with the short `dec [reg+4] ; jne` release (0x005EA310, 0x005EC990, 0x007D7F10, 0x007D7FD0); an inline persist setter is what gives `or byte ptr [eax+N]` (0x0065C36E); an operator= on an intelligent pointer takes GetPItem into a local first (0x007EE270); RemoveApologiseableCrimes-style erase loops need a separate "next" iterator living in the key slot (0x0065A7A9).

Review at landing:
- Rows that needed more than 4 grades: none. 0x005DAB10 matched on its 4th grade; 0x005C3947 and 0x007DF6A0 on their 3rd.
- Local models next to an existing engine header: CTCTavernGame (0x005EA310, 0x005EC7C0, 0x005EC8A0, 0x005EC990 -- retail 0xF0 bytes, members 8..0x1c below engine/CTCTavernGame.h); CTCTrapBase (0x007887BE); CTCTurncoatSpell (0x007C6DA0); CTCVillage (0x0065A5BB, 0x0065A7A9, 0x0065AC83, 0x0065C08A, 0x0065C1FC, 0x0065C36E -- 4-byte retail lists). Each header comment says why. 0x005DAB10 does include engine/CTCWeapon.h, through the landed `CTCWeapon_Getters` extension.
- Test note: test/005dab10.cpp swaps the `#include "engine/CTCWeapon.h"` for probe/005dab10.inc, a self-contained copy without the header's 32-bit size asserts (they fail on 64-bit g++). mktest.py does this automatically.
- Inconsistent landed identities found while checking callees:
  - 0x00686D60 has six landed spellings (GetWorld, GetGameSubsystem1C, GetRandomOwner, GetSubsystem, GetGameSubsystem, GetFrameSource); I used GetGameSubsystem.
  - 0x00485D30 is landed as both `CEnginePrimitiveHandleVector(unsigned long)` and `operator[](long)`. NPlayerGuiCDrawQuickAccessIconsPC 0x00657D60 uses the map spelling, which looks wrong: retail pushes a count and the call is the vector constructor.
  - 0x0066E734 is landed as both CInventoryListenerBase() and NHeroInformationScreens::CBase().
  - 0x0042ABAF is landed as both CComponentList and CEntryList.
- The catalog rows 0x005DAB10 and 0x007DF6A0 (len 208) include the trailing jump table and int3 padding. Each holds one function.
- Names to confirm: the unidentified 0x006A98D0 creature getter (+0x1e8) and the 0x00851B50 requirement check are modelled as `CThingCreatureBase::GetRecord1E8()` and `CExperienceRequirements::IsMetBy`. Slot 9 of the 0x00686D60 object is the landed `IsSuppressed()`. 0x0065C2D0's name is open (see NOTES).
