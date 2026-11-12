
- (chunk Q18, 2026-11-12) Rotation tie (eax/ecx v ecx/edx on a key address): name the intermediate pointer (`CThingCreatureBase* target = ...;`) to take one temporary out of VC7.1's scratch rotation -- 0x008BF500 FinishAction.
- (chunk Q18, 2026-11-12) A CCountedPointer element read via `.Get()` instead of `.Data` flips the scratch register pick in a loop -- 0x00555A90 UpdateVisibleList.
- (chunk Q18, 2026-11-12) Entity-script Main with in-loop terminating returns: `for(;;)` with `if (IsActiveThreadTerminating()) return;` first, not `while (!IsActiveThreadTerminating())` (9 hunks -> match) -- 0x00DE3CE0.
- (chunk Q18, 2026-11-12) An inline `operator=` on a large POD expands to `rep movsd`; when retail calls the out-of-line copy, declare it (opt-in) instead of defining it -- 0x008886E0 (0x006B5DF0).
- (chunk Q18, 2026-11-12) Inlined null-safe bool wrapper tested with `cmp al,bl` instead of retail's `test al,al`: spell it `return PImp.Get() && PImp->IsAlive();`, not two returns -- 0x00ED94D0.
- (chunk Q18, 2026-11-12) CIsThingAlive temporary built in place (vtable store + copy ctor) needs the ctor defined inline in the row and FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -- 0x00ED94D0.
- (chunk Q18, 2026-11-12) Script watcher dtor expansion per exit: `do { if (...) return; } while (0);` expands the by-value CScriptThing dtor, plain `if` calls 0x004AA840 (sibling 0x00ED9790 rule, again at 0x00ED94D0).
- (chunk Q18, 2026-11-12) c2prio cannot find a template instance by its full mangled leaf: pass `--func <plain method name>` -- 0x008780D0.
- (chunk Q18, 2026-11-12) cl spins (timeout) on an `#error` deep in the def headers: run a g++ -fsyntax-only precheck first; fable_script_def_global.h needs FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_CDEFSTRINGTABLE -- 0x00DE3CE0 / 0x00ED94D0.
