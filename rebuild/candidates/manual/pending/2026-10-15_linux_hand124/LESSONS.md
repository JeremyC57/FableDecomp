# Chunk 124 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A by-value class argument built in the argument area whose source words are read `this`-relative
  (`mov ecx,[esi+0Ch]` for `Me` at +8) is the class's IMPLICIT copy constructor (base part + member copy
  constructors); a user-written copy constructor reads through the reference (`[edi+4]`). 0x00D9E9E0.
- Two `new T(...)` expressions in the two arms of a length test share ONE hoisted `operator new` call (each arm keeps
  its own null test); STLport string blocks need `_String_base(const allocator&)` with the end-of-storage pointer in an
  `_STLP_alloc_proxy` (the 24i rule applies to basic_string too) and the `?:` form for retail's block order.
  0x0099B3C0, 0x0099EA60.
- A one-character temporary text (`T text[2] = {c, 0}`) is homed in the argument's own slot; an empty iterator-tag
  temporary in a function without arguments takes the high byte of the `push ecx` frame word. 0x0099B860, 0x0099B560.
- `!(*this == c)` with a strcmp-intrinsic operator== needs `__forceinline` on operator== and the null test written
  positive (`if (data) { ... } return false;`) for retail's block order (null exit last, the loop entered by a jump over
  its alignment pad). 0x0099E520.
- An open-coded strlen that loads the first character BEFORE the block pointer is the guarded `if (t[0]) do ++n;
  while (t[n]);` with the block read into a local first. 0x0099F100.
- `GetText()[i]` inside a loop whose head tests the same block folds GetText's null test and gives retail's
  `[eax+edx]` operand order; `PStringData->PData[i]` gives `[edx+eax]` and a different register assignment. 0x0099F450.
- A by-value class result assigned with an inline operator= whose self test re-forms the address with `lea` must go
  into a NAMED local (`CCharString rest = GetSubString(...); *this = rest;`); the bare temporary reuses the call's eax.
  0x0099F450.
- An if/else that picks between two virtual calls with identical argument lists keeps one `bool` flag register only
  when the arguments are named locals taken before the if (retail order: the element value, then the object);
  writing the member expressions in each call lets the compiler jump-thread the constant flag and duplicate the
  tail. 0x005BD9B2.
- A constructor's by-value smart-pointer parameter (count bump into the member, release at the end) needs
  `__forceinline` copy constructor and destructor in a size TU; `member = (T*)0` through a converting constructor is
  an implicit conversion temporary (lea after the constructor). 0x005BFCD9.
- STLport `_Rb_tree` copy constructor: accessors that return references (`_M_root() = _M_copy(...)`) put the header
  in a callee-saved register before the argument pushes; the base constructor needs `__forceinline` under /Os.
  0x004F9960.
- A comparator class wider than 4 bytes passed by value to a /Gr `__fastcall` STL helper is addressed in its own
  argument slot as `this` of the out-of-line operator() (`lea ecx,[esp+1Ch]`). 0x006F04C0.
- `x.A = x.B = 1.0f` gives one `mov eax,3F800000h` with the B store first; two separate assignments give immediates.
  0x0052C7E0.
- A constant initialiser stored before a later conditional overwrite (`FOVFlags = 1; if (v > 0) FOVFlags = 3;`)
  schedules like retail only from the init list; the body store pulls an unrelated member store above `test ah`.
  0x00A0C130.
- Entity-script vtables follow `[dtor, Main, Init, GetParentScript, OnPersist, ...]` (docs/engine/QUEST_SCRIPTS.md
  E9); script classes are named from the factory-name table (`push <name> ; ... mov [edi+10h],<factory>`) next to
  the factory that stores the vtable. 0x00D69F50, 0x00EAED90.
- A thrown class object is visible through `_CxxThrowException`'s ThrowInfo: ThrowInfo+0xC -> CatchableTypeArray
  -> CatchableType+4 -> TypeDescriptor+8 is the decorated name (`.?AVCBBBFileException@@`). 0x00A5B020.

Review at landing:
- Rows that needed more than 4 grades: 00a0c1b0 (7 variants), 0091a670 (5), 00a5b020 (5), 00ab15f0 (5), all parked.
- Local base models next to an existing engine header: 004c5ea0 (rebuild/include/engine/CBulletTimeManager.h puts
  PlayerManager at +0x1c, retail reads +0x18: the retail map is 12 bytes), every CCategory row (engine/CCategory.h is
  4 bytes high after the 12-byte vector; the landed CCategory sources say the same). 007e1e30 uses the
  CCarriedReadableDef header as is.
- Already-landed source naming errors: `rebuild/src/compiled/00/5b/CCategory_SetSelectedItemIndex_005bc479.cpp` writes
  the header fields GlobalDefIndex/SelectedItemIndex, which are the retail SelectedItemIndex (+0x20) /
  HighlightedItemIndex (+0x24) (bytes are right, names 4 bytes off); the landed
  `CFadeinSummoner_Init_00cdf430.cpp` and `CSecretWhisperingFriend_Init_00ec0b00.cpp` sit in entity vtable slot 1,
  which docs/engine/QUEST_SCRIPTS.md E9 calls Main (this chunk names its slot-1 rows Main, its slot-2 row Init).
- 008f0660 uses `#pragma pack(push, 1)` because the PDB really places the C3DVectors at +0x27/+0x33/+0x3f (not a
  layout trick); 0099ea60/0099f100/0099e520 use the landed packed 17-byte CCharStringData.
- 00ab15f0 reaches 109v109 only with `/Oa`; if the lander's flag sweep finds a module that also matches the
  register choice, the source in differ/ already carries `// Flags: /Oa`.
- No catalog row of this chunk holds several functions.
