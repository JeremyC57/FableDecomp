#!/usr/bin/env python3
"""purity_string_vas_patch.py (NF415, 2026-11-25; PROPOSED, not applied): add a second pass to
tools/decomp_pipeline/crawl/purity.py baked_string_addresses() that flags ANY hex token in a row's code (comments and literals
stripped, as pass 1) whose value is a retail string VA listed in rebuild/oracles/string_vas.tsv (the committed STRING_VAS.tsv of
NF415: every retail string VA a landed row spells, each verified narrow/wide + NUL-terminated + start of string).

Why: pass 1 (_BAKED_CAST) only sees casts to char / wchar_t / unsigned short pointers. NF413/NF414 found 27 casts through void* /
const void* / a typedef, and NF415 found six rows that pass the VA as a bare integer (`ProfileEnter(0x122d70e)`,
`CCharString2 s(0x122eea4, -1)`, `return 0x12b5b78;`): none of them is a cast, so no cast pattern can catch them. Keying on the
list of VAs that are known strings catches every spelling of the address and cannot fire on a flag constant that merely lies in the
image range (0x01400000 in 0x00C41E40 / 0x00C41FB0, 0x01234567 in tests).

Usage:  python3 purity_string_vas_patch.py <path/to/purity.py>            # print the unified diff only
        python3 purity_string_vas_patch.py <path/to/purity.py> --write    # rewrite that file (coordinator, at landing)
The coordinator also commits STRING_VAS.tsv as rebuild/oracles/string_vas.tsv (column 1 `va`, 0x-prefixed hex)."""
import sys, difflib

OLD = '''def baked_string_addresses(text):
    """Retail addresses a row casts to a string pointer in its code (comments and literals ignored); [] when none."""
    code = _BAKED_LITERAL.sub('""', _BAKED_COMMENT.sub("", text))
    out = []
    for m in _BAKED_CAST.finditer(code):
        va = int(m.group(1), 16)
        if BAKED_STRING_RANGE[0] <= va < BAKED_STRING_RANGE[1]:
            out.append(va)
    return out
'''

NEW = '''# Second pass (NF415, 2026-11-25): any hex token in the code -- a cast through void* or a typedef, a bare integer argument, a
# return value, a macro body -- whose value is a retail string VA in rebuild/oracles/string_vas.tsv (every string VA a landed row
# spells, verified narrow/wide + NUL-terminated + start of string; NF415 STRING_VAS.tsv). Keyed on known strings, so a flag
# constant that only lies in the image range is not reported. FABLE_STRING_VAS overrides the path (tests, agent lanes).
_BAKED_HEX = re.compile(r"\\b0[xX]([0-9A-Fa-f]{6,8})(?:[uUlL]{0,3})\\b")
_STRING_VAS = None


def string_vas():
    """The committed set of retail string VAs (rebuild/oracles/string_vas.tsv, column `va`); empty when the file is absent."""
    global _STRING_VAS
    if _STRING_VAS is None:
        p = Path(os.environ.get("FABLE_STRING_VAS") or (_ROOT / "rebuild" / "oracles" / "string_vas.tsv"))
        vas = set()
        if p.is_file():
            for line in p.read_text(encoding="utf-8").splitlines()[1:]:
                f = line.split("\\t")
                if f and f[0].strip():
                    vas.add(int(f[0], 16))
        _STRING_VAS = vas
    return _STRING_VAS


def baked_string_addresses(text):
    """Retail string addresses a row bakes into its code (comments and literals ignored); [] when none.
    Pass 1: a cast of a retail-image address to a string pointer. Pass 2: any hex token whose value is a known retail string
    VA (string_vas()), whatever the spelling around it."""
    code = _BAKED_LITERAL.sub('""', _BAKED_COMMENT.sub("", text))
    out = []
    for m in _BAKED_CAST.finditer(code):
        va = int(m.group(1), 16)
        if BAKED_STRING_RANGE[0] <= va < BAKED_STRING_RANGE[1]:
            out.append(va)
    known = string_vas()
    if known:
        seen = set(out)
        for m in _BAKED_HEX.finditer(code):
            va = int(m.group(1), 16)
            if va in known and va not in seen:
                out.append(va)
                seen.add(va)
    return out
'''


def patch(src):
    if NEW in src:
        return src
    assert src.count(OLD) == 1, "baked_string_addresses() is not the 2026-11-25 text; re-cut the patch"
    assert "_ROOT = Path(__file__).resolve().parents[3]" in src and "\nimport os\n" in src and "from pathlib import Path" in src
    return src.replace(OLD, NEW)


if __name__ == "__main__":
    p = sys.argv[1]
    src = open(p, encoding="utf-8").read(); new = patch(src)
    if "--write" in sys.argv[2:]:
        open(p, "w", encoding="utf-8").write(new); print("patched", p)
    else:
        sys.stdout.writelines(difflib.unified_diff(src.splitlines(True), new.splitlines(True), p, p + " (NF415)"))
