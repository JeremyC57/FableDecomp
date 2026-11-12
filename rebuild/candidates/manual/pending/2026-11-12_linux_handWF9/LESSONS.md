## 0x00867B40

- A `DIFFER(NvM)` whose only hunk is "insert `add esp,X ; ret`" at the end is a TRUNCATED ORACLE, not code: check the retail bytes after the oracle's end (xdis past the length) before any more grades (0x00867B40: manifest 465, real 468).
- A callee's hidden result slot pushed AFTER the previous call in the chain means the intermediate pointer is a named local (0x00867B40: `info = bank->PeekAnimation(i); stats = info->GetAnimationStats();`).
- `CThing::PeekBaseDef()` is inline only with FABLE_RETAIL_CTHING (body in CThingBaseDef.h); `GetPBaseDef()` returns a CDefPointer copy (inc/dec refcount code).

## 0x00762BA0

An OnSerialise whose retail layout puts the load arm first is `if (!context.IsSaving()) { load } else { save }`; VC7.1 duplicates a single trailing statement + epilogue into every loop exit by itself (0x00762BA0: four exits from one source statement).

## 0x00A12AB0

a float call result is stored before a following integer test (`call ; fst ; test`); retail `call ; test ; fst` means an fp tuple sits between call and store -- a folded double expression (`* 1.0`) reproduces it, `* 1.0f` and casts fold too early (0x00A12AB0)

## 0x00A90900

- (wf9, 2026-11-12) Frame 4 bytes short with every instruction right, an uninitialised `T()` resize value sharing a slot with a later local where retail packs it onto the `this` word: define FABLE_RETAIL_STLP_VECTOR so _M_fill_insert's body is visible (0x00A90900); source-level reshuffles of the temp / locals do nothing.

## 0x006F3A30

_S_sort/list temporaries: a list head cached in a register across an out-of-line _S_merge call (and no null test on its free) means the callee's template BODY was visible in the TU -- put STLport's _S_merge body in the header, do not declare it only (0x006F3A30).

## 0x0085D180

- (wf9, 0x0085D180) One inline callee expanded in the `if` arm and called in the `else` arm of a loop body: write the second arm as `else if (<the complementary test>)`. The test folds away but c2 rates that arm colder, so a callee near the threshold stays a call there. Try it before any /d2inlT flag.
- (wf9, 0x0085D180) A `vector<T>()` resize temporary with no destructor after the call needs FABLE_RETAIL_STLP_VECTOR (visible _M_fill_insert): c2 then knows the callee leaves the temporary empty.
- (wf9, 0x0085D180) A constant stored as an immediate right after `cmp reg, const ; jne` (instead of the register) means the compared local was read before the preceding statement: move its declaration up.

## 0x008C6860

- A null CDefPointer passed by const reference to two different callees in the arms of an `if`: retail's single hoisted zero store + merged release came from one named local PER ARM, not from one local ahead of the `if` (that swaps frame words and callee-saved roles) nor from temporaries (zero stays in each arm). Row 0x008C6860.
- VC7.1 drops a CDefPointer's null tests (copy and release) once the row dereferences it directly for a data member; a read through an inline member of the sub-object (CDefString::GetString on `def->Name`) keeps them. Model def strings as CDefString, not as the flattened TablePos long. Row 0x008C6860.

