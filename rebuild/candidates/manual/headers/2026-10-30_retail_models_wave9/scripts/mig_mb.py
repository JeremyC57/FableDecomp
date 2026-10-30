#!/usr/bin/env python3
"""mig_mb.py [--dry] <file>...: wave 9 MovieBase migration. A row-local `class CScriptGameResourceObjectMovieBase :
public CScriptGameResourceObjectBase { <ptr> PImp; [empty ctor] };` (no declared dtor, no virtuals), its local
CScriptGameResourceObjectBase (empty ctor / empty dtor only) and its local PImp pointer struct (used nowhere else) are
replaced by engine/CScriptGameResourceObjectMovieBase.h over engine/CCountedPointer.h (implicit dtor, the shared
plain-inline release: wave 8 lesson 7)."""
import sys, re, os, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import find_block
from cputil import strip_comments, depth_at, pp_depth, items, member_name
MB = "CScriptGameResourceObjectMovieBase"; GB = "CScriptGameResourceObjectBase"
def blk(c, name):
    b = find_block(c, name)
    if not b: return None
    if find_block(c, name, b[1]): return "dup"
    if depth_at(c, b[0])[0] != 0 or pp_depth(c, b[0]) != 0: return "nested"
    return b
def body(c, b): return c[c.find("{", b[0]) + 1:c.rfind("}", b[0], b[1])]
def analyse(t):
    c = strip_comments(t)
    if 'MovieBase.h"' in t: return None, "already"
    mb = blk(c, MB)
    if not isinstance(mb, tuple): return None, "MB block " + str(mb)
    head = c[mb[0]:c.find("{", mb[0])]
    if not re.search(r":\s*public\s+%s\s*$" % GB, head.strip()): return None, "MB base"
    ptype = None
    for kind, h in items(body(c, mb)):
        n, isf = member_name(h)
        if isf:
            if n != MB: return None, "MB member " + n
            if not re.search(r"%s\s*\(\s*\)\s*$" % MB, re.sub(r"__forceinline|inline", "", h).strip()): return None, "MB ctor shape"
            continue
        if n != "PImp": return None, "MB data " + n
        ptype = re.sub(r"\s+", " ", h.strip()[:-len("PImp")]).strip()
    if not ptype: return None, "no PImp"
    if re.search(r"\b%s\s*::" % MB, c[:mb[0]] + c[mb[1]:]): return None, "MB out-of-class member"
    spans = [mb]
    gb = blk(c, GB)
    if gb == "dup" or gb == "nested": return None, "GB " + gb
    if gb:
        gh = c[gb[0]:c.find("{", gb[0])]
        if not re.search(r":\s*public\s+CBaseObject\s*$", gh.strip()): return None, "GB base"
        for kind, h in items(body(c, gb)):
            n, isf = member_name(h)
            hh = re.sub(r"__forceinline|virtual|inline", "", h).strip()
            if not isf or n not in (GB, "~" + GB) or not re.search(r"\(\s*\)\s*$", hh): return None, "GB member " + n
        if re.search(r"\b%s\s*::" % GB, c[:gb[0]] + c[gb[1]:]): return None, "GB out-of-class member"
        spans.append(gb)
    if re.search(r"(?m)^\s*(class|struct)\s+(CBaseObject|CBaseClass)\b[^;]*\{", c): return None, "local CBaseObject/CBaseClass"
    m = re.fullmatch(r"(?:struct\s+|class\s+)?(\w+)(\s*<[^>]*>)?", ptype)
    if not m: return None, "ptype " + ptype
    if m.group(1) == "CCountedPointer":
        if 'engine/CCountedPointer.h"' not in t: return None, "CCountedPointer local"
    else:
        pb = blk(c, m.group(1))
        if not isinstance(pb, tuple): return None, "ptype block " + str(pb)
        rest = c[:pb[0]] + c[pb[1]:]
        rest = rest[:0] + "".join(c[s:e] if False else "" for s, e in []) + rest
        # uses outside the MovieBase block and its own block (and its out-of-line members) are not allowed
        r2 = c
        for s, e in sorted([pb, mb], reverse=True): r2 = r2[:s] + r2[e:]
        if re.search(r"\b%s\b" % m.group(1), r2): return None, "ptype used " + m.group(1)
        spans.append(pb)
    return spans, "ok"
def apply(t, spans):
    spans = sorted(spans)
    inc = ""
    if not re.search(r"(?m)^#define FABLE_RETAIL_BASE_OBJECT\b", t): inc += "#define FABLE_RETAIL_BASE_OBJECT\n"
    if 'engine/CCountedPointer.h"' not in t: inc += '#include "engine/CCountedPointer.h"\n'
    inc += '#include "engine/CScriptGameResourceObjectMovieBase.h"   // : GROBase, vtable 0x01260EF4, PImp +0x08; implicit dtor (wave 9)\n'
    out = t
    for s, e in reversed(spans):
        rep = inc if (s, e) == spans[0] else ""
        if rep and s > 0 and out[s - 1] != "\n": rep = "\n" + rep
        out = out[:s] + rep + out[e:]
    return out
if __name__ == "__main__":
    dry = "--dry" in sys.argv; why = collections.Counter()
    for f in [a for a in sys.argv[1:] if not a.startswith("--")]:
        t = open(f, encoding="utf-8").read(); res, w = analyse(t); why[w] += 1
        if res is None: print("skip", f, w); continue
        if not dry: open(f, "w", encoding="utf-8", newline="\n").write(apply(t, res))
        print("edit", f, w)
    for k, v in why.most_common(): print("#", v, k, file=sys.stderr)
