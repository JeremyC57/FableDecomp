# Chunk 483 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Loops and control flow
- **Counter stored after the `jle` (00a10800).**
  - Source form: `int n = wcslen(w); if (n > 0) { int remaining = n; do ... while (--remaining != 0); }`.
  - Retail stores the counter after the `jle`. Counting down `n` itself stores it before the test.
  - A `for (; count != 0; --count)` count-down gives `je`/`jne`, not `jbe` (00a10560).
- **Inline search helper stops jump-threading (009f1d20).**
  - Write a linear search as an inline helper (`char* FindChar(char* p, char* end, char ch)`).
  - Written in place, VC7.1 threads the empty-search exit past a following `p < found` test; retail jumps to that test.
- **VC7.1 keeps an empty tree walk (009f9020).** The loop is not provably finite, so it survives.

## Strings and STLport
- **Same-TU Compare body (009e6a70).**
  - STLport `__find_if` / `__find` over CCharString-bearing elements needs Compare's real body (0x00411570) in the TU under `#pragma auto_inline(off)`.
  - The predicate's string block then stays in esi across every Compare call. A declaration only reloads it after each call.
- **Recursive text TransferIn (009e2590).**
  - Declare the right-hand side `long rhs;` inside each operator branch, so it shares the dead argument slot. A function-scope local adds a frame slot.
  - Operator literals passed to `SkipPastNextItemIfIs` are implicit `const char*` -> CCharString conversions (`lea` re-address after the ctor).
  - Reach the symbol map through an inline `GetSymbolMap()` accessor so `this` loads before the argument pushes.
- **STLport containers.**
  - A `pair<iterator,bool>` with STLport's user ctors returns through a hidden pointer.
  - `push_back` + `_M_insert_overflow` must be `__forceinline` to inline (009c02d0).
  - `__uninitialized_copy` must be `__forceinline`, or the second instance stays out of line (009e14b0, 00a01120).
  - Model deque `_M_map` / `_M_map_size` as `_STLP_alloc_proxy`; plain members swap the `lea [eax+ecx+2]` operands (00a0ee50).
  - deque::erase needs same-TU noinline `copy` / `copy_backward` so their unused results share the dead `last` slot (00a0f040).
  - On an STLport deque, `front()` gives `*_M_start`; `[0]` builds an iterator copy through operator+ (00a0dcd0).
- **Implicit destructors (009ecf30).** An implicit dtor inlined into a list clear stores no vtable.

## Evaluation order and named locals (009bd910, 009c02d0, 009c4120)
- **Named GFGetSystemManager() result (009bd910).** Retail calls it before pushing the next call's hidden-result pointer.
- **Two-C2DVector screen box (009bd910).**
  - Build the C2DBoxF from two C2DVector temporaries (top-left, bottom-right).
  - That reproduces retail's x87 spill: the width goes to the scratch slot and the bottom edge through a float temporary moved as an int.
  - The 4-float constructor keeps everything on the FPU.
- **Global read before a call (009bd910).** A global read before a call whose result is an earlier-evaluated argument (`*g_pSubtitleFont` before `GetText()`) is a named reference local.
- **Inline CSoundInit constructor (009bd910).** It assigns in the out-of-line 0x0041C930 order (PlaybackType last), and an override (`GainFaderBits = 8`) replaces its zero store.
- **Declared memberwise copy constructor (009c02d0).** One on C2DExtentsI fixes an element-copy register swap.
- **Field read into a local (009c4120).** Read a field into a local once when later stores could alias it (TransferSize); this also moves a flag into a dead argument slot.
- **Inline accessor on a call result (009f9160).** It leaves a dead `add eax,N`; read the member plainly.

## Constants and unoptimised TUs
- **0.0 in an /Od TU (009f70e0, 009f9160).** It loads from the shared double pool 0x0122ED70. 009f70e0 is `#pragma optimize("", off)`.
- **Pool width follows literal type (00a04200).** A double literal gives a qword pool constant (`TWO_PI = 6.2831854820251465`); a float literal gives a dword one, even inside a double expression.
- **Anonymous-namespace functions (00a04200).** One needs a global function pointer to stay emitted.
- **Math intrinsics.** `#pragma intrinsic(acos, fmod, asin)` still calls the `_CIacos` / `_CIfmod` / `_CIasin` thunks (0xBFEBA2 / 0xBFEB28 / 0xBFEDC4).
- **`__forceinline` retail helpers.** HighestSetBit, NormaliseTurns and GetAngleXY/YZ need it where retail inlines them.

## Gaps and checks
- **Leaf class key (009e6a70).** A leaf with the wrong class key (`VCCharString` while the source said `struct`) matches no symbol and silently grades the largest function. Confirm leaves with `objdump -t`.
  - Unmangled leaves (`TransferIn`) grade through q.py but give calls.py 0 blocks; use the `?Name@@` prefix.
- **QFE-4035 x87 output (00a0c450 / 00a0c6f0).** It is identical to RTM for the CCamera x87 family, so do not spend grades on it.
- **Unsolved families:**
  - Retail `fld st(2) ; fmulp st(1)` v our `fmul st(2)` + `fstp st(0)` (CCamera).
  - Retail `fst [slot]` keeping the value v our `fstp` + reload (00a04200).
  - Retail separate frame slots v our packed `push ecx` frame (00a0db60, 00a112f0).
  - `cmp eax,esi` against the zero register v `test eax,eax`, tried `== 0` and `!x` (00a10560).
  - Bit test `shr ecx,1 ; test cl,1` v the `test byte ptr` fold (009fa620).
- **LTCG custom conventions in the word-wrap helpers.** 0x00A0FE80, 0x00A10170 and 0x00A10240 take a wchar_t in ax; 0x009C33A0 takes its hidden result in esi. Park after 1 grade.
- **ltcg_grade.py.** `$S/tools/ltcg_grade.py` (/GL + /LTCG grader) exists but is outside the PROMPT rules; the LTCG rows were parked, not graded with it.

## Review at landing:
- **Landed STL models.**
  - Landed CRenderManager2D Add2DTri / AddTri model the `__true_type` overload; retail calls the `__false_type` one.
  - Landed 0x0048C820 is really deque<C2DVector>::_M_destroy_nodes.
  - Landed 0x009B5DB0 declares `_M_range_insert` as a non-template member (retail mangles the member template).
- **Engine headers.** engine/CActiveFile.h and engine/CCompressorZlib.h carry Dinkumware (Ego_r) layouts that disagree with retail STLport.
- **Rows at the 4-grade budget.** 009bd910, 009e2590 and 009c02d0 needed all 4 grades; all three match.
- **Local models next to engine headers.**
  - Matched rows model CCharString / CWideString / CSymbolMap / CPersistContext locally: fable_string.h inlines the CCharString copy ctor that retail calls, and there are no engine headers for the rest.
  - 009e2590 uses engine/CStringParser.h.
  - 009e2590's CCharString is a `struct` (callee names mangle `U`), as in the landed 0x0040F7E0. Relocation targets are correct, but switch it to `class` if the lander checks callee symbol names.
- **ICF labels.** 0x009E5C60 and 0x009917E0 are ICF-shared bodies whose catalog / BSim labels name another class. Check before attributing.
