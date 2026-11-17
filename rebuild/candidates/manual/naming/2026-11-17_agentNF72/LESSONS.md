# NF72 lessons (round 7)

- **STL self rows go fastest as one explicit-instantiation line on the shared stlport header.** Every _Rb_tree / iterator / vector / list /
  pair row in this slice matched on the first grade. Copy the opt-in set from a landed row of the same member (TREE_DEC_OUTOFLINE,
  TREE_FIND_MEMBER_TEMPLATE, LIST_CLEAR_OUTOFLINE + LIST_DTOR_IN_LIST, TREE_BOUNDS_OUTOFLINE + BODY_PASS22).
- **Binder fixes that only change a callee are minimal swaps.** Replace the one stand-in member type with the shared container (plus
  VECTOR_DTOR_OUTOFLINE / LIST_DTOR_OUTOFLINE so the call stays out of line). Leave the rest of the row alone. All grades were one.
- **A self respell can break the row's old behaviour test** when the test calls the stand-in by name (0x004B8940 LINK_FAIL). Read the test
  before grading. Ship rows/<addr>_test.cpp; pay.py picks it up.
- **Copy the define/include preamble from a landed row of the same class.** Guessing it cost three SRC_FAIL grades on 0x009D68B0. The
  sizeof static assert in CBankFileAsync.h needs the landed 0x009D5DF0 set (CMOVABLERESOURCE + STATICBLOCKS_TEXTURE_HELPERS + DATA).
- **Overlay copies go stale.** The repo fable_containers.h changed mid-session (another agent's landing). Check `cmp hdr/orig/X
  rebuild/include/X` before the final recheck and rebase with `patch`.
- **Tree destructor stand-ins with no caller keep their own name.** Deriving the stand-in from the shared std::map (agentNF50 0x006311D0
  form) rebinds only the _M_erase callee.
- **A derived CCreatureAction destructor that retail calls with no vptr store is compiler-generated.** `~T(){}` adds `mov [esi],??_7`.
  Use an IMPLICIT_DTOR opt-in, not a user destructor.
- **Pin moved static asserts.** Run pin_asserts.py with your `orig/` copy as the base: the repo may be mid-pin, and `orig/` is your real merge
  base.
- **Large ports go fastest with preprocess checks first.** On 0x0047EEE4, `cpp` with the overlay caught the #error before a grade was spent.
  Then diff the relocation list against the XO=o dump. Two include fixes and one release opt-in (CIVCOUNTEDPOINTER_DTOR_DIRECT_CLEAR, once
  the out-of-line Release showed in the diff) took it from SRC_FAIL to RELOCATION_MATCH in four grades.
