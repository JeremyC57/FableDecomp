# 2026-10-14_linux_hand46 — hand-authored chunk 46 of the class-labelled 17–96-byte pool

Chunk 46 (`mkchunk.py 46`, 40 rows, CGameScriptThing .. CGroundNavigatorExternalInterface) was authored by the session coordinator on the
Linux VC7.1 lane; the lander re-graded every row: **34 of 40** landed as MATCH/RELOCATION_MATCH with behaviour tests (compiled with the
lane's cl.exe). Parked sources stay in the lane's `differ/`. The lane notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe), authored by the coordinator. Chunk 46 (`chunk46.tsv`, 40 rows): **34 of 40** graded
`RELOCATION_MATCH`/`MATCH` with behaviour tests (g++ and the lander's cl.exe). Sources from `gen.py`, tests from `gen_tests.py`,
`payload.json` / `oracle.tsv` from `finish.py`. 6 parked in `differ/` (evidence below).

## Label corrections
- 0x00877160 "CGameflowAssistanceScript::Init" is the CONSTRUCTOR (`mov eax,ecx` at the top returns `this`): owner, three cleared words, four tuning floats.
- 0x00556930 (labelled "CGameScriptThingManager::HasScriptThing", propagated) is a `__stdcall` free function `FindScriptThingByUid(uid, list, map)` over a
  thing list (identity record at +0x19c -> +0 -> +8, map word +0x1a0); the real HasScriptThing is 0x00704790 (64-bit UID list).
- 0x0090df30 "CGameScriptThing::GetPThing" is the +0x48 twin (the +0x50 one is 0x006AF520): the thing when all five low flag bits (+0x6c) are set.
- 0x00814c10 / 0x009fd0c0 / 0x00a4ef90 / 0x00a50bf0 / 0x00ca8e10 "CGraphicDataBank::ChangeScene" are five unrelated bodies: only 0x009FD0C0 is ChangeScene
  (two resource lists get the scene word); 0x00814C10 is a TransferBinaryOut (byte + float), 0x00A4EF90 / 0x00A50BF0 are by-value node-handle inserts,
  0x00CA8E10 a `__stdcall` critical-section flag setter (IAT 0x0143FE0C / 0x0143FE10).
- 0x006bb1e0 / 0x006bb2c0 / 0x006bb790 "IsDay" are three bodies: (0.1667, 0.875], (0.3333, 0.7083], and the NEGATION of the first (`IsNotDay`).
- The two "CGlobalDef::~CGlobalDef" bodies (0x004E37BF, 0x009006DD) destroy intelligent pointers / 12-byte members then tail-jump to a base destructor
  (0x004DA8B7 / 0x008FD288) WITHOUT a vtable store: the classes are not polymorphic in these bodies.
- 0x009fcba0 "CGraphicFrameData::Uninitialise" clears bit 4 of +0x1a and, for bit 2, resets the counted texture at +8 (count word cleared before the object word).

## Lessons
- `delete data` on a class with an inline destructor goes through the out-of-line `??_G` deleting destructor (28 v 94); write the retail inline chain as
  `data->~T(); operator delete(data);` with `__forceinline` destructors (0x0041C150 and its two byte-identical twins).
- A destructor that stores the vtable BEFORE releasing a counted pointer needs the counted pointer as a MEMBER OBJECT with its own `__forceinline`
  destructor (24i's rule, destructor side); the CBase base is `__declspec(novtable)` with a virtual destructor (0x004BB600).
- A destructor with no vtable store: drop the `virtual` from the base model entirely (0x004E37BF, 0x009006DD).
- `cmp edi,esi ; jne ; mov edi,[eax+0Ch] ; cmp edi,edx` over two words = a 64-bit `__int64` equality (0x00704790); two `long` compares stay in memory.
- `mov eax,ecx` as the first instruction of a 0-return "Init" = a constructor (0x00877160).
- A zero-initialised result local that is ASSIGNED the call result (`result = GetPItem();` then `return result;`) keeps the zero in edi and still
  tail-jumps the last call; `return GetPItem();` in the arm loses the edi retention (0x0090DF30).
- `je ; call ; add eax,8 ; jmp ; mov eax,8` = a ternary into a local (`long entry = bank ? bank->Sizeof() + 8 : 8; size += entry;`), not an if/else on
  the accumulator (0x009F8380).
- `mov eax,1Bh` hoisted before the flag test with the unfolded `mov eax,8 ; add eax,13h` arm = `long size = 0x13 + 8; if (flag) size = Helper(tex) + 0x13;`
  with the null case inside a `__forceinline` helper (0x009FCBF0).
- The 12-byte frame of ReadFromParserAsSLONG (`lea ecx,[esp+0Ch]` string ctor, `[esp+4]` item) is ONE local: the parsed item owns its CCharString at +8
  (`struct SParsedItem { long type; long value; CCharString text; }`); two separate locals land the other way round whatever the declaration order.
- A by-value 4-byte handle parameter used as `this` for an out-of-line call (`lea ecx,[esp+18h]`) after a call on its pointee: the pointee pointer is
  read once into edi (a local), the lane still loads it into ecx first (parked, see below).
- A re-read of a vector's first pointer after a division by the element size (`mov edx,[esi+1E0h]` twice) = `volatile` on that member plus the count in a
  `long` local (0x009FD910).
- Two Win32 calls on the same member (`mov esi,[edi+14h] ; push esi ... push esi`) = the member in a local (0x00CA8E10).
- `CStopperSet` (a tree of handles) is 12 bytes: the counter after it sits at +0x38, not +0x30 (0x006BBE30).
- `float ... bool` static member with `ret 0Ch`: `static bool __stdcall` (0x006BB5B0; parked on the compare form).

## Parked (6; sources in differ/)
- 0x006BB5B0 `CGameTimeManager::IsTimeBetween(float time, float start, float end)`: retail duplicates the `time < end` compare in both arms and shares the
  `mov al,1` / `xor al,al` tails; the lane either CSEs the compare (if/return form, 60 v 75) or materialises the && / || result as `mov eax,1` (expression form,
  78 v 75). Semantics are fixed (`start < end ? time >= start && time < end : time >= start || time < end`, `test ah,1 ; jne` = `!(a >= b)`).
- 0x006CDA10 `PeekNavigatorRadius(bool)`: retail hoists `fld [esp+4]` (the radius homed in the dead argument slot) above the `test al,al` and adds the
  0.1f margin in one arm; the lane keeps the load per arm and loads the constant first (`fld [const] ; fadd [esp+4]`), 56 v 49 for `+=`, `g + r`, ternary and
  early-return forms (float-local scheduling gap).
- 0x009FD3A0 `CGraphicDataBank::UpdateAvailableMemory` / 0x009FD660 `FrameEnd` (the same clamp inlined): retail stores the clamped spare at +0x278, THEN loads
  the scene word (+0x26c) and pushes it; the lane hoists the scene load above the clamp (`volatile` on either member, a pointer local and an inline accessor
  all keep the hoist) — store/load scheduling gap, 59 v 59 / 91 v 91 otherwise identical.
- 0x00A4EF90 / 0x00A50BF0 by-value node-handle inserts: retail `push edi ; mov edi,[esp+0Ch] ; ... ; mov ecx,edi` (the handle word loaded straight into the
  callee-saved register), the lane `mov ecx,[esp+8] ; ... ; mov edi,ecx` (42 v 42 / 46 v 48) for the direct, pointer-local, handle-by-value and handle-copy
  forms — register-choice gap.
