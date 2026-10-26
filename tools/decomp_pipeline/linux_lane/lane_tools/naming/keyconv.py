#!/usr/bin/env python3
"""keyconv.py [--keys KEYS.tsv] [--dry] [-v] files...

Rewrite every `struct X` / `class X` (definitions, forward declarations, elaborated uses, friend
declarations, template-ids) to the class-key the retail PDBs mangle X with (`V` = class, `U` = struct;
KEYS.tsv from keys.py). MSVC VC7.1 mangles a type with the key of its definition, else of its first
declaration, so every spelling is made consistent.

Semantics are preserved:
  * struct -> class definition: ` public:` is inserted right after `{` (unless the body already
    opens with an access specifier) and each base without an access specifier gets `public`.
  * class -> struct definition: ` private:` is inserted after `{` likewise, bases get `private`.
Scope: names are resolved through the enclosing namespace / class chain (a light brace tracker), the
declared entity of a definition or forward declaration is <scope>::X exactly; a use is looked up
innermost-first like C++. Declarations inside function bodies (local types) are left alone. `std::`
(STLport here, Dinkumware in the PDBs), anonymous-namespace, unknown and union/enum-keyed names are
left alone. Comments and string literals are never touched. Idempotent. Prints per-file and total
counts.
"""
from __future__ import annotations
import os, re, sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ACCESS = re.compile(r"\s*(public|private|protected)\s*:(?!:)")


def load_keys(path):
    types, tmpls = {}, {}
    for line in open(path, encoding="utf-8").read().splitlines()[1:]:
        p = line.split("\t")
        name, kind, keys = p[0], p[1], p[2]
        if keys not in ("U", "V"):
            continue                    # unions, enums, keyed-both-ways: never touched
        if name.startswith("std::") or "`" in name or name.startswith("stdext::"):
            continue
        (tmpls if kind == "template" else types)[name] = "class" if keys == "V" else "struct"
    return types, tmpls


def mask_code(text):
    """same-length copy of text where comments and string/char literals are blanked to spaces
    (newlines kept), so regexes see only code."""
    out = list(text); i = 0; n = len(text)
    while i < n:
        c = text[i]
        if c == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j): out[k] = " "
            i = j; continue
        if c == "/" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if text[k] != "\n": out[k] = " "
            i = j; continue
        if c in "\"'":
            # skip the char literal / string literal; a quote preceded by a digit-separator is not C++03
            j = i + 1
            while j < n and text[j] != c:
                if text[j] == "\\": j += 1
                elif text[j] == "\n": break
                j += 1
            for k in range(i + 1, min(j, n)):
                if text[k] != "\n": out[k] = " "
            i = j + 1; continue
        if c == "#":
            # preprocessor line: keep #define bodies as code, but blank #include/#pragma etc. paths
            ls = text.rfind("\n", 0, i) + 1
            if text[ls:i].strip() == "":
                m = re.match(r"#\s*(include|pragma|error|line)\b[^\n]*", text[i:])
                if m:
                    for k in range(i, i + m.end()): out[k] = " "
                    i += m.end(); continue
        i += 1
    return "".join(out)


HEAD = re.compile(r"^\s*(?:(?:public|private|protected)\s*:(?!:)\s*)*(?:template\s*<.*>\s*)?(?:typedef\s+)?"
                  r"(?:struct|class|union)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)\b", re.S)
NSHEAD = re.compile(r"(?:^|\s)namespace\s+([A-Za-z_]\w*)\s*$")
ANONNS = re.compile(r"(?:^|\s)namespace\s*$")
EXTERNC = re.compile(r'extern\s+"C(\+\+)?"\s*$')


def frames_at(code):
    """returns list `scope_at[i]` lazily: we compute a sorted list of (pos, scope_tuple) change points."""
    stack = []          # entries: ("ns", name) | ("cls", name) | ("other", None) | ("link", None)
    changes = [(0, ())]
    last = 0            # position after last ; { }
    def scope():
        return tuple(stack)
    for m in re.finditer(r"[;{}]", code):
        c = m.group(); p = m.start()
        if c == "{":
            frag = code[last:p]
            # drop preprocessor lines from the fragment
            frag2 = "\n".join(l for l in frag.split("\n") if not l.lstrip().startswith("#"))
            h = HEAD.match(frag2)
            if h and "(" not in frag2[h.end():] and ("=" not in frag2 or frag2.strip().startswith("typedef")):
                stack.append(("cls", h.group(1)))
            elif NSHEAD.search(frag2):
                stack.append(("ns", NSHEAD.search(frag2).group(1)))
            elif ANONNS.search(frag2):
                stack.append(("ns", "`anonymous namespace'"))
            elif EXTERNC.search(frag2.strip()):
                stack.append(("link", None))
            else:
                stack.append(("other", None))
            changes.append((p + 1, scope()))
        elif c == "}":
            if stack: stack.pop()
            changes.append((p + 1, scope()))
        last = p + 1
    return changes


def scope_of(changes, pos):
    lo, hi = 0, len(changes) - 1
    while lo < hi:
        mid = (lo + hi + 1) // 2
        if changes[mid][0] <= pos: lo = mid
        else: hi = mid - 1
    return changes[lo][1]


KEYRE = re.compile(r"(?<![\w$])(struct|class)(\s+)((?:[A-Za-z_]\w*\s*::\s*)*[A-Za-z_]\w*)\b")


def split_bases(s):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch == "<": depth += 1
        elif ch == ">": depth -= 1
        if ch == "," and depth == 0:
            out.append(cur); cur = ""
        else:
            cur += ch
    out.append(cur)
    return out


def blank_pp(code):
    """blank every preprocessor directive line (with its backslash continuations)."""
    out = list(code); i = 0; n = len(code)
    for m in re.finditer(r"(?m)^[ \t]*#", code):
        if m.start() < i: continue
        j = m.start()
        while True:
            e = code.find("\n", j)
            e = n if e < 0 else e
            if e > j and code[e - 1] == "\\" and e < n:
                j = e + 1; continue
            break
        for k in range(m.start(), e): out[k] = " "
        i = e
    return "".join(out)


def convert(text, types, tmpls, log=None):
    full = mask_code(text)
    code = blank_pp(full)       # structure (scopes, fragments, braces) never sees #define bodies
    if log is not None:
        for m in KEYRE.finditer(full):
            if code[m.start()] == " " and (types.get(m.group(3)) or tmpls.get(m.group(3))) not in (None, m.group(1)):
                log.append(("SKIPPED_PREPROCESSOR", m.group(3), m.group(1), "?"))
    changes = frames_at(code)
    edits = []          # (start, end, replacement)
    n = 0
    for m in KEYRE.finditer(code):
        kw, ws, qname = m.group(1), m.group(2), re.sub(r"\s+", "", m.group(3))
        st = m.start()
        # template parameter (`template<class T, class U>`): previous code char is < or , and the
        # statement fragment contains `template`
        k = st - 1
        while k >= 0 and code[k] in " \t\r\n": k -= 1
        prev = code[k] if k >= 0 else ""
        if prev in "<,":
            fs = max(code.rfind(";", 0, st), code.rfind("{", 0, st), code.rfind("}", 0, st))
            if re.search(r"\btemplate\s*<", code[fs + 1:st]):
                continue
        sc = scope_of(changes, st)
        # what follows the name
        j = m.end()
        while j < len(code) and code[j] in " \t\r\n": j += 1
        nxt = code[j] if j < len(code) else ""
        is_tid = nxt == "<"
        # classify: definition / forward declaration / use
        decl_kind = "use"
        tail_end = j
        if is_tid:
            depth = 0; q = j
            while q < len(code):
                if code[q] == "<": depth += 1
                elif code[q] == ">":
                    depth -= 1
                    if depth == 0: break
                q += 1
            q2 = q + 1
            while q2 < len(code) and code[q2] in " \t\r\n": q2 += 1
            nxt2 = code[q2] if q2 < len(code) else ""
            tail_end = q2
            if code[q2:q2 + 2] == "::":
                # `struct A<T>::B ...`: the key belongs to the nested B, which this pass does not resolve
                if log is not None: log.append(("SKIPPED_NESTED_TID", qname, kw, "?"))
                continue
        else:
            nxt2 = nxt
        fs = max(code.rfind(";", 0, st), code.rfind("{", 0, st), code.rfind("}", 0, st))
        pre = code[fs + 1:st]
        pre_s = re.sub(r"^\s*(?:(?:public|private|protected)\s*:(?!:)\s*)*", "", pre)
        pre_s = "\n".join(l for l in pre_s.split("\n") if not l.lstrip().startswith("#")).strip()
        lead_ok = pre_s == "" or re.fullmatch(r"(template\s*<.*>|typedef|friend|template\s*<.*>\s*friend)", pre_s, re.S)
        friend = bool(re.search(r"\bfriend$", pre_s))
        defshape = nxt2 == "{" or (nxt2 == ":" and code[tail_end:tail_end + 2] != "::")
        if defshape and (not lead_ok or friend):
            # a definition after an unexpected lead-in (`static struct X {...} g;`, a macro): never
            # change its key without also fixing its access -- leave it and report it
            if log is not None and (tmpls if is_tid else types).get(qname, kw) != kw:
                log.append(("SUSPECT_DEF", qname + " after " + repr(pre_s[-40:]), kw, "?"))
            continue
        if nxt2 in "{:" and code[tail_end:tail_end + 2] != "::" and lead_ok and not friend:
            decl_kind = "def"
        elif nxt2 == ";" and lead_ok and not friend and "typedef" not in pre_s:
            decl_kind = "fwd"
        local = any(f[0] == "other" for f in sc)
        names = [f[1] for f in sc if f[0] in ("ns", "cls")]
        table = tmpls if (is_tid or (decl_kind != "use" and pre_s.startswith("template"))) else types
        want = None; target = None
        if decl_kind in ("def", "fwd"):
            if local:
                continue
            # a definition/forward declaration of `A::B` (qualified) names A::B relative to scope
            target = "::".join(names + [qname]) if names else qname
            if "::" in qname:
                # out-of-line nested definition `struct A::B {`: resolve A by lookup
                for d in range(len(names), -1, -1):
                    cand = "::".join(names[:d] + [qname])
                    if cand in table: target = cand; break
            want = table.get(target)
            if want is None and is_tid and decl_kind == "def":
                # explicit/partial specialisation `template<> struct X<int> {`
                want = table.get(target)
        else:
            for d in range(len(names), -1, -1):
                cand = "::".join(names[:d] + [qname])
                if cand in table:
                    target = cand; want = table[cand]; break
        if want is None or want == kw:
            continue
        if any(f[1] and "`" in f[1] for f in sc if f[0] == "ns"):
            continue
        edits.append((m.start(1), m.end(1), want))
        n += 1
        if log is not None: log.append((decl_kind, target, kw, want))
        if decl_kind == "def":
            # base clause
            if nxt2 == ":":
                b = code.find("{", tail_end)
                bases = code[tail_end + 1:b]
                acc = "public" if want == "class" else "private"
                pos = tail_end + 1
                for part in split_bases(bases):
                    if not re.match(r"\s*(public|private|protected)\b", part) and not re.match(r"\s*virtual\s+(public|private|protected)\b", part):
                        lead = len(part) - len(part.lstrip())
                        if part.lstrip().startswith("virtual"):
                            # `virtual B` -> `virtual public B`
                            vpos = pos + lead + len("virtual")
                            edits.append((vpos, vpos, " " + acc))
                        else:
                            edits.append((pos + lead, pos + lead, acc + " "))
                    pos += len(part) + 1
                brace = b
            else:
                brace = tail_end
            if not ACCESS.match(code[brace + 1:]):
                edits.append((brace + 1, brace + 1, " public:" if want == "class" else " private:"))
    if not edits:
        return text, 0
    edits.sort(key=lambda e: (e[0], e[1]))
    out = []; last = 0
    for s, e, r in edits:
        out.append(text[last:s]); out.append(r); last = e
    out.append(text[last:])
    return "".join(out), n


def main(argv):
    keys = os.environ.get("KEYCONV_KEYS", str(HERE.parents[4] / "ghidra_out" / "pdb_class_keys.tsv"))
    dry = verbose = False; files = []
    it = iter(argv)
    for a in it:
        if a == "--keys": keys = next(it)
        elif a == "--dry": dry = True
        elif a == "-v": verbose = True
        else: files.append(a)
    types, tmpls = load_keys(keys)
    total = nf = 0
    for f in files:
        p = Path(f)
        raw = p.read_bytes()
        try: t = raw.decode("utf-8"); enc = "utf-8"
        except UnicodeDecodeError: t = raw.decode("latin-1"); enc = "latin-1"
        log = []
        new, n = convert(t, types, tmpls, log)
        if n:
            nf += 1; total += n
        if verbose and log:
            print(f"{f}\t{n}\t" + "; ".join(f"{d}:{tg}:{a}->{b}" for d, tg, a, b in log))
        if n and not dry:
            p.write_bytes(new.encode(enc))
    print(f"keyconv: {total} declaration(s) rewritten in {nf} of {len(files)} file(s){' (dry run)' if dry else ''}")


if __name__ == "__main__":
    main(sys.argv[1:])
