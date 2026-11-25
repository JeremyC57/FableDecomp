#!/usr/bin/env python3
"""baked_symbols.py (NF418, 2026-11-25; landed by the coordinator): third baked_string pass for crawl/purity.py and
tools/landed_metrics.py -- the data-symbol form that the hex-token passes cannot see.

A row bakes a string when its code references a made-up DATA symbol that rebuild/oracles/reloc_bindings.tsv binds to a retail pooled
string literal (a VA in rebuild/oracles/string_vas.tsv, plus the NF418 additions file STRING_VAS_ADD.tsv, which the coordinator merges
into string_vas.tsv): `extern const char g_PredMsg;` + `&g_PredMsg`, `extern const char kStrA[];`, `static const wchar_t s_newLine[] =
{ '\\n', 0 };`. Symbols spelled `??_C@...` are the literal itself and never count. Keep VAs (vendor arrays such as DirectShow
szOle32Aut / szControl, indexed tables) are simply absent from the key files, so no allowlist is needed.

Two entry points:
  baked_symbol_names(text)  -- SOURCE pass (what landed_metrics can run on every row): the table symbols bound to a key VA whose
      identifier the row DECLARES or DEFINES as data (`extern ... id`, `static|const <type> id[...] =`, `<type> id;`) and uses in code
      (comments and string literals stripped). Requiring the declaration keeps unrelated locals of the same name (`kName` pointer
      locals, test copies) out.
  baked_symbols_in_object(objpath, leaf=None) -- OBJECT pass (exact; for lanes and the lander): every relocation of a code section to
      a non-??_C@ / non-$SG symbol that the table binds to a key VA, plus every relocation to an OBJECT-LOCAL static data symbol
      whose bytes are a printable NUL-terminated string (the file-static array form; reported as ('local', name, text) since it has
      no table line -- 008b3d80 / 008b4200 / 0048f881 at HEAD).
Wiring (proposed): purity.baked_string_addresses(text) keeps passes 1-2; landed_metrics adds
    `or baked_symbols.baked_symbol_names(text_a)` to the baked_string test (same grade, same exclusion from genuine).
CLI: baked_symbols.py census [--replace <dir-with-addr.cpp>...]  -> counts over the landed catalog sources.
"""
import os, re, sys, glob
from pathlib import Path

ROOT = Path(os.environ.get("FABLE_ROOT") or Path(__file__).resolve().parents[3])
_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
_LITERAL = re.compile(r'"(?:\\.|[^"\\\n])*"' + r"|'(?:\\.|[^'\\\n])*'")
_KEY = None
_ANY = None


def _key_paths():
    env = os.environ.get("FABLE_STRING_VAS_KEYS")
    if env: return [Path(p) for p in env.split(os.pathsep)]
    return [ROOT / "rebuild" / "oracles" / "string_vas.tsv"]   # NF418's STRING_VAS_ADD.tsv is merged into it


def ident(sym):
    """C++ identifier of a table symbol: ?name@scope@@3... -> name; extern "C" / file-static _name -> name."""
    if sym.startswith("?"): return sym[1:].split("@", 1)[0]
    return sym[1:] if sym.startswith("_") else sym


def key():
    """{identifier: {(symbol, va), ...}} for table symbols bound to a key string VA (not ??_C@ / $SG / vtables / RTTI)."""
    global _KEY
    if _KEY is None:
        vas = set()
        for p in _key_paths():
            if not p.is_file(): raise FileNotFoundError("baked_symbols: %s is missing" % p)
            for l in p.read_text(encoding="utf-8").splitlines()[1:]:
                f = l.split("\t")
                if f and f[0].strip(): vas.add(int(f[0], 16))
        k = {}; drop = set()
        for p in filter(None, os.environ.get("FABLE_BAKED_DROP", "").split(os.pathsep)):   # TABLE_CHANGES files not yet applied (lane use)
            drop |= {(f[1], int(f[2], 16)) for f in (l.split("\t") for l in open(p).read().splitlines()[1:]) if len(f) > 2 and f[0] == "delete"}
        for l in (ROOT / "rebuild" / "oracles" / "reloc_bindings.tsv").read_text(encoding="utf-8").splitlines()[1:]:
            f = l.split("\t")
            if len(f) < 2 or f[0].startswith(("??_C@", "$SG", "??_7", "??_R", "__real@", "__xmm@", "__imp_")): continue
            va = int(f[1], 16)
            if (f[0], va) in drop: continue
            if va in vas: k.setdefault(ident(f[0]), set()).add((f[0], va))
        _KEY = k
    return _KEY


def baked_symbol_names(text):
    """Table data symbols bound to retail string literals that the row declares/defines and uses; [] when none."""
    global _ANY
    code = _LITERAL.sub('""', _COMMENT.sub("", text))
    out = []
    k = key()
    if _ANY is None:   # one pass over the row for every key identifier, then the exact checks on the few present
        _ANY = re.compile(r"\b(" + "|".join(sorted(map(re.escape, k), key=len, reverse=True)) + r")\b") if k else re.compile(r"(?!)")
    for i in sorted(set(_ANY.findall(code))):
        syms = k[i]
        decl = re.search(r"(?:\bextern\b|\bstatic\b|\bconst\b)[^;{}()]*?\b" + re.escape(i) + r"\s*(?:\[[^\]]*\]\s*)?(?:;|=|,)", code)
        if not decl: continue
        if re.search(r"\*\s*(?:const\s+)?" + re.escape(i) + r"\s*=\s*L?\"\"", code): continue   # `const char* const id = "..."`: the literal itself
        uses = len(re.findall(r"\b" + re.escape(i) + r"\b", code))
        if uses < 2: continue                                     # declared but never used: a dead declaration, no reference
        out.extend(sorted(s for s, _ in syms))
    return out


def baked_symbols_in_object(objpath, leaf=None):
    """[(kind, symbol, va_or_text)] for relocations of code to table-bound string data symbols or object-local string arrays."""
    sys.path.insert(0, str(ROOT / "tools" / "decomp_pipeline"))
    import reloc_identity as RI
    secs, syms = RI.coff_read(objpath)
    bound = {}
    for i, ss in key().items():
        for s, va in ss: bound.setdefault(s, set()).add(va)
    out = set()
    for sc in secs:
        if not sc["chars"] & 0x20: continue
        for _o, si in sc["relocs"].items():
            s = syms[si]; n = s["name"]
            if n.startswith(("??_C@", "$SG", "__real@", "__xmm@")): continue
            if n in bound:
                out.update(("table", n, "0x%08X" % v) for v in bound[n]); continue
            if s["cls"] == 3 and s["sec"] > 0 and not n.startswith("."):
                d = secs[s["sec"] - 1]
                if d["chars"] & 0x20 or not d["data"]: continue
                for wide in (False, True):
                    b = RI._cstring(d["data"][s["value"]:], wide)
                    if b and RI._looks_text(b, wide):
                        out.add(("local", n, RI.literal_text(b, wide))); break
    return sorted(out)


def _census(replace_dirs):
    sys.path.insert(0, str(ROOT / "tools" / "decomp_pipeline"))
    import naming_status as ns
    sys.path.insert(0, str(ROOT / "tools" / "decomp_pipeline" / "crawl")); import purity
    rep = {}
    for d in replace_dirs:
        for f in glob.glob(os.path.join(d, "*.cpp")):
            a = os.path.basename(f)[:-4]
            if re.fullmatch(r"[0-9a-f]{8}", a): rep[a] = f
    hits = {}
    for a, r in ns.landed_rows().items():
        p = rep.get(a) or str(ROOT / "rebuild" / "src" / "compiled" / r["Source"])
        try: t = open(p, encoding="utf-8", errors="ignore").read()
        except OSError: continue
        if not purity.is_genuine_kind(purity.classify(t)): continue
        h = baked_symbol_names(t)
        if h: hits[a] = h
    return hits


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "census":
        dirs = sys.argv[sys.argv.index("--replace") + 1:] if "--replace" in sys.argv else []
        h = _census(dirs)
        for a in sorted(h): print("%s\t%s" % (a, " ".join(h[a])))
        print("baked_symbol rows: %d" % len(h), file=sys.stderr)
    else:
        for p in sys.argv[1:]:
            print(p, baked_symbols_in_object(p) if p.endswith(".obj") else baked_symbol_names(open(p, encoding="utf-8").read()))
