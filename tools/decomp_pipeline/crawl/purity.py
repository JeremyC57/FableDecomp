#!/usr/bin/env python3
"""Single source of truth for "is this landed source a genuine reconstruction?".

A source is NOT genuine if it is hand-written machine code in any form:
  * `__declspec(naked)` + `_emit <byte>` byte bakes, and
  * `__declspec(naked)` / `__asm` blocks written with real mnemonics.

The second form was invisible to the original `_emit`-only check, which under-counted
the remaining work by ~585 functions. Both are byte-exact oracles at best, not faithful
compiler-emitted reconstructions -- see the byte-purity policy.

A bare `__asm` inside otherwise-real C++ (e.g. a single `__asm int 3`) still counts as
non-genuine here: any hand-written instruction stream is a purity debt.

One exception (owner, 2026-10-24): where retail itself ran inline assembly -- an instruction
VC7.1 cannot emit from C or C++ (`rdtsc`, a bare `fistp dword`, an `fnstcw`/`fldcw` rounding
switch, a rotate of a stack copy) -- the original source had an `__asm` statement too, and
reproducing it is reconstruction, not a bake. Such a source is `retail_asm` when it carries a
`// Retail-asm: <evidence>` line, has no `naked`/`_emit`, keeps all control flow in C++
(no call/jmp/ret/jcc inside `__asm`), and its `__asm` statements total at most
RETAIL_ASM_MAX instructions.
"""
import re

MARKERS = ("_emit", "__declspec(naked)", "declspec(naked)", "__asm")
RETAIL_ASM_TAG = "// Retail-asm:"
RETAIL_ASM_MAX = 8
_FLOW = re.compile(r"^\s*(call|jmp|ret|retn|j[a-z]{1,3}|loop\w*|int)\b", re.I)


def _asm_instructions(text):
    """The instructions inside every `__asm` statement or block (comments stripped)."""
    code = re.sub(r"//[^\n]*", "", text)
    out = []
    for m in re.finditer(r"__asm\s*\{(.*?)\}", code, re.S):
        out += [l.strip() for l in re.split(r"[\n;]", m.group(1)) if l.strip()]
    for m in re.finditer(r"__asm\s+(?!\{)([^\n{}]+)", code):
        out += [p.strip() for p in m.group(1).split("__asm") if p.strip()]
    return out


def _retail_asm(text):
    if RETAIL_ASM_TAG not in text or "_emit" in text or "naked" in text:
        return False
    ins = _asm_instructions(text)
    return 0 < len(ins) <= RETAIL_ASM_MAX and not any(_FLOW.match(i) for i in ins)


def is_genuine_kind(kind):
    return kind in ("genuine", "retail_asm")


def is_genuine(text):
    return is_genuine_kind(classify(text))


def classify(text):
    if "_emit" in text:
        return "emit_bake"
    if "naked" in text or "__asm" in text:
        return "retail_asm" if _retail_asm(text) else "naked_asm"
    return "genuine"
