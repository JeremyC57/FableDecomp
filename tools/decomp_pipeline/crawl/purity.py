#!/usr/bin/env python3
"""Single source of truth for "is this landed source a genuine reconstruction?".

The policy this grader implements is docs/pipeline/ASM_RULES.md (owner-confirmed 2026-11-05). The in-row `__asm`
allowance described below is being retired by that rule: new rows reach asm only through a shared helper header.

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

Second exception (owner, 2026-10-30): bytes the linker copied from a prebuilt toolchain library (a VC7.1 CRT
object such as msvcrt.lib's sehprolg.obj, or the `jmp [__imp_X]` thunk it synthesises for an import-library
member) are not Fable source at all; the project links the same library instead of reconstructing them. Such
a landed file is `toolchain_library` when:
  * it carries `// Toolchain-library: <lib>(<member>) <symbol> -- <n> bytes verified against Fable.exe 0x<ADDR>`
    (reproduced by tools/decomp_pipeline/libsig/lib_verify.py; docs/pipeline/LIBRARY_SIGNATURES.md);
  * it has no function body (no `{` outside comments and string literals: declarations only) and no
    `__asm`, `naked` or `_emit` anywhere.
It counts with the genuine rows (the bytes are the toolchain's, the build links them), and is checked before
the asm rules so the two never overlap.

Shared helper headers (owner, 2026-11-03: "clean C++ for a port; identical logic; asm only where retail itself ran it"):
a retail inline-asm helper lives in one header (rebuild/include/engine/GFFloatToLongNear.h, ...) with the byte-matching
`__forceinline` asm body and a FABLE_PORTABLE C++ twin, and the rows call it with no `__asm` of their own. Such a row
still runs retail asm, so it must not read as `genuine`. A helper header declares itself with
`// Retail-asm-helper: <Name>[, <Name>...]` next to its `// Retail-asm:` evidence. `classify(text)` follows the row's
quoted / angle `#include`s (recursively, through FABLE_HDR_OVERLAY then rebuild/include) and, when the row's code calls
a declared helper (the name as an identifier: a call, or `&Name` to emit the out-of-line copy), grades it:
  * `retail_asm` when the helper header itself passes the evidence rule above (address + one of its instructions, no
    naked/_emit/ret) AND the row keeps its own `// Retail-asm:` line citing a retail address (where retail runs the
    block in this function) and naming one instruction of the called helpers' asm;
  * `naked_asm` otherwise (an undocumented asm caller is a purity debt like an undocumented asm block).
A row that includes a helper header but never calls a helper stays `genuine`. Limitation: a helper reached only through
an inline function of another header (the row never spells the helper's name) is not seen.
"""
import os
import re
from pathlib import Path

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


TOOLCHAIN_LIBRARY_TAG = "// Toolchain-library:"
_TOOLCHAIN_CLAIM = re.compile(r"^//\s*Toolchain-library:\s*\S+?\(.+\)\s+\S+\s+--\s+\d+\s+bytes verified against "
                              r"Fable\.exe\s+0x[0-9A-Fa-f]{8}\s*$", re.M)


def _toolchain_library(text):
    """A documentation stub for library bytes: the proof line, declarations only, no machine code."""
    if not _TOOLCHAIN_CLAIM.search(text):
        return False
    if any(m in text for m in ("_emit", "naked", "__asm")):
        return False
    code = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    code = re.sub(r"//[^\n]*", "", code)
    code = re.sub(r'"(?:\\.|[^"\\\n])*"', '""', code)
    code = re.sub(r"'(?:\\.|[^'\\\n])*'", "''", code)
    return "{" not in code and "}" not in code


def is_genuine_kind(kind):
    return kind in ("genuine", "retail_asm", "toolchain_library")


def is_genuine(text):
    return is_genuine_kind(classify(text))


RETAIL_ASM_HELPER_TAG = "// Retail-asm-helper:"
_ROOT = Path(__file__).resolve().parents[3]
_INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
_header_cache = {}


def _include_dirs():
    ov = [Path(x) for x in os.environ.get("FABLE_HDR_OVERLAY", "").split(os.pathsep) if x]
    return ov + [_ROOT / "rebuild" / "include"]


def _resolve(name, here, dirs):
    for d in ([here] if here else []) + list(dirs):
        p = Path(d) / name
        if p.is_file():
            return p.resolve()
    return None


def _helper_header(path):
    """(helper names, asm mnemonics, header passes the evidence rule) for one header file, cached."""
    key = str(path)
    if key not in _header_cache:
        text = path.read_text(encoding="utf-8", errors="ignore")
        names = []
        for line in text.splitlines():
            if RETAIL_ASM_HELPER_TAG in line:
                names += [n for n in re.split(r"[\s,]+", line.split(RETAIL_ASM_HELPER_TAG, 1)[1]) if n]
        mnems = {re.split(r"[\s,]+", i.strip(), 1)[0].lower() for i in _asm_instructions(text)
                 if i.strip() and not i.strip().endswith(":")}
        mnems = {m for m in mnems if re.fullmatch(r"[a-z][a-z0-9]*", m)}
        _header_cache[key] = (names, mnems, bool(names) and _retail_asm(text), text)
    return _header_cache[key]


def included_asm_helpers(text, dirs=None, here=None):
    """Every Retail-asm-helper header reachable through the row's #includes: [(path, names, mnemonics, ok)]."""
    dirs = _include_dirs() if dirs is None else dirs
    seen, out, todo = set(), [], [(here, text)]
    while todo:
        cur_dir, cur = todo.pop()
        code = re.sub(r"/\*.*?\*/", "", cur, flags=re.S)
        code = re.sub(r"//[^\n]*", "", code)
        for name in _INCLUDE.findall(code):
            p = _resolve(name, cur_dir, dirs)
            if p is None or str(p) in seen:
                continue
            seen.add(str(p))
            names, mnems, ok, htext = _helper_header(p)
            if names:
                out.append((p, names, mnems, ok))
            todo.append((p.parent, htext))
    return out


def _helper_grade(text, dirs=None):
    """None when the row calls no shared retail-asm helper, else 'retail_asm' / 'naked_asm' (see the module doc)."""
    if "#include" not in text:
        return None
    helpers = included_asm_helpers(text, dirs)
    if not helpers:
        return None
    code = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    code = re.sub(r"//[^\n]*", "", code)
    code = re.sub(r"^\s*#[^\n]*", "", code, flags=re.M)     # the #include line spells the header, not a use
    called = [(p, ok, mnems) for p, names, mnems, ok in helpers
              if any(re.search(r"(?<![A-Za-z0-9_])" + re.escape(n) + r"(?![A-Za-z0-9_])", code) for n in names)]
    if not called:
        return None
    if not all(ok for _, ok, _ in called):
        return "naked_asm"
    ev = _evidence(text)
    if not _VA.search(ev):
        return "naked_asm"
    mnems = set().union(*[m for _, _, m in called])
    if not any(re.search(r"(?<![A-Za-z0-9_])" + re.escape(m) + r"(?![A-Za-z0-9_])", ev, re.I) for m in mnems):
        return "naked_asm"
    return "retail_asm"


def classify(text, include_dirs=None):
    if TOOLCHAIN_LIBRARY_TAG in text and _toolchain_library(text):
        return "toolchain_library"
    if "_emit" in text:
        return "emit_bake"
    if "naked" in text or "__asm" in text:
        return "retail_asm" if _retail_asm(text) else "naked_asm"
    return _helper_grade(text, include_dirs) or "genuine"
