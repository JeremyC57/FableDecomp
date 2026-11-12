## 0x00868CE0

long index = *it; declared before the CDefPointer out local makes VC7.1 load the element before the out-pointer push (mov eax,[it] ; lea ecx,[out] ; push ecx ; ... ; push eax) -- GetDef(*it,&out) loads it after (0x00868CE0).

## 0x00E2EAC0

- (wf2 00e2eac0) A dead `mov [esp+N],0` at entry with a frame one dword bigger than its objects is a c1 temporary flag: any `?:` with a destructible temporary in an arm allocates and zeroes it even when the conditional folds away (`false ? (void)f().m() : (void)0;` reproduces it byte for byte); `false && ...` does not. It also creates the function's zero register.
- (wf2 00e2eac0) In a `for(;;)` the blocks that end in `break` are exit paths to c2's estimator (cold dtor sites); the same arms under `while (!done)` with `done = true` are body blocks (hot), and the code after such a loop keeps the loop frequency. Compare both before reaching for /d2inlT.
- (wf2 00e2eac0) The order of the inline ~CCountedPointer zero pair (Data / Info) on a stack CScriptThing is not fixed by the header: it flips with the loop's exit structure and with forming the IsAlive test into a bool first. Treat it as evidence about the source, not as a scheduling tie.
- (wf2 00e2eac0) Never pipe fg.py / xdis.py into `head` (BrokenPipe traceback on screen); redirect to a file and `sed -n`.
- (wf2 00e2eac0, pass 2) A generated-script wait loop with a pre-loop check AND a check after NewScriptFrame is ONE source check at the head of `for (;;)` (VC7.1 inverts the loop); spelling it as two checks changes c2's exit frequencies and the scheduling of the merged dtor tail (the "Info-first zero pair" residue vanished with the head form).
- (wf2 00e2eac0, pass 2) Sweep `/d2inlT` in steps of 2 around 74 on the NATURAL form before touching structure: a match window just above 74 (here T86..T100) says the structure is right and one exit is hotter than written; a per-site single / double `do { } while (0)` probe (16 grades) then names the site.
- (wf2 00e2eac0, pass 2) A terminate check nested in a second `do { } while (0)` is an exit about x4 hotter, and the code reached after it keeps the heat (post-loop check and scope-end dtor expand); a nested wrapper on a different arm does not leak to them.
- (wf2 00e2eac0, pass 2) fg.py takes 0.6 s per grade: generate the spelling grammar with a script and grade hundreds of variants in one call (redirect to a file, then grep the result lines) instead of reasoning about c2's estimator.

## 0x005590C0

A scroll-bar style hit test with dead X halves and unused frame words was a C2DBoxI local read through GetBRPos()/GetTLPos(); two identical call+epilogue tails came from one statement after an if / else-if (tail duplication), an explicit early return is cross-jumped (0x005590C0).

## 0x008C2AD0

A CCharString temporary destroyed between a call and the use of its result (lea ecx,[tmp] ; mov edi,eax ; call ~CCharString) = the result was a named local and the use a separate statement, not a chained call.

## 0x006978D0

- (wf2, 0x006978D0) Parentheses around an inlined float call on the right of an assignment (`v.X = (GFGetInterpolatedValue(a, b, t));`) change VC7.1's int/x87 interleaving without changing one instruction: a 373v376 "scheduling tie" (store before `fstp st(0)`, late pushes) matched on that alone. Copy the landed sibling's exact spelling, parentheses included, before theorising about the scheduler.
- (wf2, 0x006978D0) `mov reg,[p+N] ; mov [esp+M],reg` for some components of a vector while others sit on the x87 stack (`fld old.X ; fld old.Y ; fld new.X ; fsub st(2)`) = both vectors copied by value into C3DVector locals (old first), then blended per component.
- (wf2, 0x006978D0) When a toy/probe with the "same" body schedules differently from a landed sibling, diff the two source TEXTS character by character first (it was the parentheses), before bisecting headers, signatures or alias theories.

## 0x00EB3F40

- (wf2 00eb3f40) A dead `mov bl,al` after a bool-returning call = the value is tested by an if / else whose two arms compile to identical code (VC7.1 merges the arms after register allocation); look for a doubled block-start terminate check as the second witness.
- (wf2 00eb3f40) A dead `mov dword ptr [esp+N],0` just below the first temporary in a script function is a conditional-destruction flag word whose uses folded away: the source had a class temporary under `&&`/`||` in a condition the OPTIMISER proved false (`x && false && f("s")` gives it; a front-end constant `false && f("s")` does not).

## 0x008D5300

008d5300: a "this+8 hoisted into ebp" residue was a cached `CThing* creature` local -- write `PCreature->` per access and the 6-arg CThingObject::Create (args right to left) and it matches first grade.

## 0x009ADC90 (not landed)

009adc90	a `new T(...)` written inside a push_back call keeps its argument temporaries alive to the end of the push_back statement (no frame-slot sharing with the push_back value temporary); retail built the record in its own statement.

