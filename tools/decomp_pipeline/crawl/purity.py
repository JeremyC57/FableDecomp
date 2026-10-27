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

One exception (owner, 2026-10-24; evidence rule 2026-10-27): where retail itself ran inline assembly --
an instruction or shape VC7.1 cannot emit from C or C++ (`rdtsc`, a bare `fistp dword`, an
`fnstcw`/`fldcw` rounding switch, `cpuid`, a hand SIMD kernel, a published `_X86_` asm block) -- the
original source had an `__asm` statement too, and reproducing it is reconstruction, not a bake. Such a
source is `retail_asm` when:
  * it carries a `// Retail-asm: <evidence>` line that cites at least one retail address (0x00......)
    and names at least one instruction that its `__asm` code contains (the evidence ties the block to
    retail bytes, and the grader's byte match proves the block reproduces them);
  * it has no `naked` and no `_emit` (the function's prologue and epilogue stay compiler-emitted);
  * no `__asm` statement returns (`ret`/`retn`/`iret`): the function exits through C++.
There is no instruction-count cap and branches inside the block are allowed (owner, 2026-10-27: a fixed
cap is arbitrary when retail is proven to run a longer block). The old 8-instruction / no-branch rule
excluded proven original code such as the 10-instruction fnstcw/fldcw ceil/floor block and the
published DirectShow wxutil.h EnlargedUnsignedDivide.
"""
import re

MARKERS = ("_emit", "__declspec(naked)", "declspec(naked)", "__asm")
RETAIL_ASM_TAG = "// Retail-asm:"
RETAIL_ASM_MAX = None   # no cap (owner, 2026-10-27); kept for callers that import the name
_EXIT = re.compile(r"^\s*(ret|retn|retf|iret|iretd)\b", re.I)
_VA = re.compile(r"\b0x0*[4-9a-fA-F][0-9a-fA-F]{5}\b")


def _asm_instructions(text):
    """The instructions inside every `__asm` statement or block (comments stripped)."""
    code = re.sub(r"//[^\n]*", "", text)
    out = []
    for m in re.finditer(r"__asm\s*\{(.*?)\}", code, re.S):
        out += [l.strip() for l in re.split(r"[\n;]", m.group(1)) if l.strip()]
    for m in re.finditer(r"__asm\s+(?!\{)([^\n{}]+)", code):
        out += [p.strip() for p in m.group(1).split("__asm") if p.strip()]
    return out


def _evidence(text):
    return " ".join(l.split(RETAIL_ASM_TAG, 1)[1] for l in text.splitlines() if RETAIL_ASM_TAG in l)


def _retail_asm(text):
    if RETAIL_ASM_TAG not in text or "_emit" in text or "naked" in text:
        return False
    ins = _asm_instructions(text)
    if not ins or any(_EXIT.match(i) for i in ins):
        return False
    ev = _evidence(text)
    if not _VA.search(ev):
        return False
    mnems = {re.split(r"[\s,]+", i.strip(), 1)[0].lower() for i in ins if i.strip() and not i.strip().endswith(":")}
    mnems = {m for m in mnems if re.fullmatch(r"[a-z][a-z0-9]*", m)}
    return any(re.search(r"(?<![A-Za-z0-9_])" + re.escape(m) + r"(?![A-Za-z0-9_])", ev, re.I) for m in mnems)


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
