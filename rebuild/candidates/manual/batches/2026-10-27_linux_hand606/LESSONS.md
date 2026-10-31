# Chunk 606 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Invented accessor stand-ins hide register allocation: replacing them with the PDB's inline accessor chain (GetWorld().
  GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->Field) and the shared engine/CDefPointer.h took 0x009713E0
  from 18 lines to 2 at once. Grep the PDB name sets for `Get<Member>@<Class>` before keeping any stand-in.
- engine/CDefPointer.h by-value copy: VC7.1 folds the destructor's null test after the copy constructor's null-tested AddRef
  when the pointee is dereferenced in between (`test ecx,ecx; je; inc [ecx+4]; ... dec eax; jne` with no second test) --
  "release without a null test" is not evidence against CDefPointer (0x009713E0).
- A call result compared after a temporary's destructor must sit in a named local (`mov esi,eax` ... `cmp esi,2`); in the
  return expression VC7.1 compares first and keeps the bool in bl (0x009713E0).
- `return x == k ? true : false;` gives a bare `sete al`; `return x == k;` adds `xor eax,eax` (0x009713E0).
- Named float locals with compound assignment (`float x = a*b; x -= c*d; X = x;`, `r = p*q; r += s*t;`) pin the x87 term and
  operand order where one-expression sums are canonicalised: 0x00C1C330 20 -> 2 lines, 0x00C40590 12 -> 8 lines.
- x87 operand rank: in an inlined helper the offset-0 member (`[reg]`) of one argument is demoted to the memory operand; which
  argument's X loses flips between a member helper (this = result) and a free pointer/reference helper, and every product
  swaps when the helper is a member of the first operand (0x00C40590). No spelling reaches retail's mixed order.
- Fleet pattern (0x008098B0, 0x007FA160, 0x0052EAF0 and the store in 0x00C1C330): retail delays a float/word store past the
  next argument push or call setup; no source form moved it here (a diagnostic grade with the 4035 QFE toolset via FABLE_VC71 differs the same way on 0x008098B0, 0x007FA160, 0x00C1C330). Treat as an
  LTCG scheduling gap after two grades.
- When a parked file cites a callee address, re-disassemble the call: 0x0052EAF0 calls 0x0052E530 (UnConvertVector), not the
  0x0052E520 its parked comment gave.
- vland warns SHARED_HEADER_AVAILABLE for local classes even when only layout-renamed use is possible; a `// Layout:` line
  explaining why (vptr-as-data header vs a needed virtual dtor) clears the warning.

Review at landing:
- 0x009713E0: rows needing more than 4 grades (6 here); local polymorphic CCreatureActionBase/CCombatActionBase model next to
  engine/CCreatureActionBase.h (vptr-as-data header cannot host the virtual dtor chain); EAIActionReturn enumerator name
  AI_ACTION_DONE descriptive.
- NAME_FIXES.tsv: 13 callee addresses whose landed/bound spellings are wrong, notably the landed bodies 0x0041CF47 (GetUIScale
  should be const QBE), 0x008FCFC0 (landed as GetPWorld QAEPAXXZ; PDB GetWorld IBE CWorld&), 0x007F9E70
  (GetThingCompareMultiplier should be private const ABEMPBV), 0x00BEB540 (landed as extern "C" New_beb540; it is
  CEngineSubPrimitiveBase::operator new). The 0x0052EA30 / 0x0052EAF0 pending differs cite a wrong converter address.
- Parked differ models still carry filler virtual slots (vNNN) for NUISystem::CComponent, CTCPhysicsBase, CEnvironment
  (as the landed CComponent rows do); slot names other than those noted are not proven.
