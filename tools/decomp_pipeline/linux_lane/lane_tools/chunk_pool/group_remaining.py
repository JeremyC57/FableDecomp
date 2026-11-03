#!/usr/bin/env python3
"""Group the remaining (unlanded, unreserved) rows into translation-unit-shaped sets that should land together.

Why: VC7.1 lays each object file's functions out contiguously, a class's members and the STL instantiations it
caused share one TU, a vtable's slots are one class, an EH funclet is a slice of its parent's object, and a
same-TU callee is the lever that fixes most "incomplete-model" parks. So the right chunk is a TU, not ten
rows picked by size.

Evidence used (all from the retail exe and the repo, no assumptions):
  * address order and extents (rebuild/manifest/functions.tsv; a row's extent is the gap to the next start)
  * the retail call graph: E8/E9 rel32 targets that hit a known start (scanned from Fable.exe .text)
  * pointer tables in .rdata/.data: runs of absolute function pointers (vtables, dispatch tables), plus the
    RTTI-named vtables in rebuild/manifest/rtti-vtables.tsv
  * class scope of the manifest name (module column / `Scope::Member`), with NScript nesting collapsed to the
    script class (NScript::CQ_X::CEntity::Main -> NScript::CQ_X)
  * EH funclet parents from rebuild/candidates/manual/batches/2026-11-03_funclets_parents/STATUS.tsv
  * the landed neighbours of each row: the dominant class of the landed rows in the same address run is the
    TU the row most likely belongs to

Grouping (union-find), strongest first:
  1. funclet -> parent;  2. same pointer table (vtable);  3. same class scope;  4. a direct call between two
  remaining rows that are within 64 KB of each other;  5. address adjacency: consecutive remaining rows with no
  landed row between them and the same neighbourhood class.
Then every group is annotated: rows, bytes, parked/unworked split, residue classes, size buckets, the TU context
(landed classes around it), and the reserved rows it would depend on.

    S=<lane scratch> python3 group_remaining.py <outdir> [--exclude RESERVED.tsv ...]
Writes GROUPS.tsv (one row per group), ROWS.tsv (one row per function with its group) and GROUPS.md (summary).
"""
from __future__ import annotations

import argparse
import collections
import csv
import os
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[5]
EXE = Path(os.environ.get("FABLE_EXE", os.path.join(os.environ.get("S", ""), "dl/game/Fable The Lost Chapters/Fable.exe")))
BASE = 0x400000


def A(s: str) -> str:
    return s.strip().lower().replace("0x", "").zfill(8)


def scope_of(module: str, name: str) -> str:
    """Class / TU scope for a manifest row. NScript nesting collapses to the script class."""
    q = name if module in ("", "_global") else f"{module}::{name}"
    parts, depth, cur = [], 0, ""
    for ch in q:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        if ch == ":" and depth == 0 and cur.endswith(":"):
            parts.append(cur[:-1]); cur = ""; continue
        cur += ch
    parts.append(cur)
    if len(parts) <= 1:
        return "_global"
    if parts[0] == "NScript" and len(parts) >= 3:
        return "NScript::" + parts[1]
    if parts[0] == "std" or parts[0].startswith("std::"):
        return "std::" + re.sub(r"<.*", "", parts[1]) if len(parts) > 1 else "std"
    return "::".join(parts[:-1])


def load_exe():
    d = EXE.read_bytes()
    pe = struct.unpack_from("<I", d, 0x3C)[0]
    n = struct.unpack_from("<H", d, pe + 6)[0]
    opt = struct.unpack_from("<H", d, pe + 20)[0]
    secs = {}
    for i in range(n):
        name, vs, va, rs, ro = struct.unpack_from("<8sIIII", d, pe + 24 + opt + 40 * i)
        secs[name.rstrip(b"\0").decode()] = (BASE + va, vs, ro, rs)
    return d, secs

PREFACE = """The binding evidence is retail's own structure, not the catalog's guesses: VC7.1 lays each object file's functions out
contiguously, a vtable's slots are one class, an EH funclet is a slice of its parent's object, a class's members and the STL
instantiations they caused share a TU, and a same-TU callee is the lever that fixes most incomplete-model parks. Links, strongest
first: funclet->parent (retail EH tables), same pointer table in .rdata/.data (RTTI-named where RTTI exists), same non-template
class scope, a direct E8/E9 call to another remaining row in the same landed neighbourhood, address adjacency with no landed row
between. Template helpers (CThingSearchTools, CPersistContext, GF*, std::) never form a TU of their own; they ride with the TU that
instantiated them.

## How to read a group (the reverse-engineer's view)

* **anchor** is the class / TU; **tu_context** lists the landed classes around the group's addresses with their counts. A group
  whose anchor has many landed siblings is a TU we have mostly rebuilt: its remaining rows are the ones whose model needed
  something the siblings have since proven (the retail header, a PDB signature, an inlined helper). Cut the whole group as one
  chunk and give the agent the siblings' sources: the same-TU callee lever is free there.
* A group whose anchor has few landed siblings is a TU we have barely touched. Cut it class-first: ctor, dtor, the vtable slots in
  order (the vtables column names it), then the free helpers the calls column points at.
* Funclet groups land with their parent, never alone (`batches/2026-11-03_funclets_parents/STATUS.tsv` has the parents).
* An impossible-majority group is CRT / LTCG / vendor-asm territory: prove `toolchain_library` or `retail_asm`, or retire it.
* Single rows are mostly leftovers of landed TUs (F2): append them to a neighbouring chunk of the same class rather than cut them
  alone; the F1 giants get one session each with a drafted skeleton.

## Proposed cutting order

1. **A finish-the-TU groups**, largest first (CTCInventoryAbilities, CTCInventory, CTCShop, CTCVillage, CUserProfileManager,
   CNavigatorAStar, CEngineLightingManager, CGameCameraManager, CGameScriptInterface, CWorldMap ...): one chunk per group, the
   brief lists the group's landed siblings as the model.
2. **C vtable / class completion** and **B2 fresh TUs** (CEngine, CEngineWaterRenderer, CEnginePrimitiveManager*MeshesBase,
   QuadricOptimiserInternals, NParticleEngine CPSC*, CChunkedFileChunk): class-first order inside each chunk.
3. **B parked-heavy TUs** only after their class has its `FABLE_RETAIL_<CLASS>` branch (PLAN section 3): the residues there are
   register / x87 ties that re-probing does not fix.
4. **D funclet bundles** are free once their parents land: re-run the funclet procedure.
5. **F1 giants** one per session; **F2 singles** appended to the nearest chunk of the same class; **E** retired with evidence;
   **G gap-scan starts** through `bounds/extent.py` first.

## Categories

"""


class UF:
    def __init__(self):
        self.p = {}

    def f(self, x):
        self.p.setdefault(x, x)
        while self.p[x] != x:
            self.p[x] = self.p[self.p[x]]
            x = self.p[x]
        return x

    def u(self, a, b):
        ra, rb = self.f(a), self.f(b)
        if ra != rb:
            self.p[rb] = ra


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("outdir")
    ap.add_argument("--exclude", nargs="*", default=[], help="RESERVED.tsv files whose rows are left out")
    args = ap.parse_args()
    out = Path(args.outdir); out.mkdir(parents=True, exist_ok=True)

    man = {}
    with open(ROOT / "rebuild/manifest/functions.tsv", encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            man[A(r["address"])] = r
    cat = {}
    with open(ROOT / "rebuild/oracles/auto-re-candidates.tsv", encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            cat[A(r["address"])] = (r["name"], int(r["length"]))
    parked = {}
    with open(ROOT / "rebuild/candidates/manual/PARKED_INDEX.tsv", encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            parked[A(r["addr"])] = r
    reserved = set()
    for p in args.exclude:
        with open(p, encoding="utf-8-sig", newline="") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                reserved.add(A(r["address"]))
    funclet_parent = {}
    st = ROOT / "rebuild/candidates/manual/batches/2026-11-03_funclets_parents/STATUS.tsv"
    if st.exists():
        with open(st, encoding="utf-8-sig", newline="") as f:
            for r in csv.DictReader(f, delimiter="\t"):
                if r.get("parent"):
                    funclet_parent[A(r["address"])] = A(r["parent"])

    starts = sorted(int(a, 16) for a in man)
    idx = {f"{v:08x}": i for i, v in enumerate(starts)}
    d, secs = load_exe()
    text_va, text_vs, text_ro, _ = secs[".text"]
    text_end = text_va + text_vs

    def extent(a: str) -> int:
        i = idx[a]
        if a in cat:
            return cat[a][1]
        nxt = starts[i + 1] if i + 1 < len(starts) else text_end
        return max(0, min(nxt - starts[i], 0x10000))

    remaining = [a for a in man if a not in cat and a not in reserved]
    rem = set(remaining)
    known = set(man)

    # --- retail call graph over every known function (E8 / E9 rel32 into a known start)
    calls = collections.defaultdict(set)      # caller -> callees
    callers = collections.defaultdict(set)    # callee -> callers
    for a in man:
        va = int(a, 16); ln = extent(a)
        off = text_ro + (va - text_va)
        blob = d[off: off + ln]
        for m in re.finditer(rb"[\xe8\xe9]", blob):
            i = m.start()
            if i + 5 > len(blob):
                break
            rel = struct.unpack_from("<i", blob, i + 1)[0]
            tgt = va + i + 5 + rel
            if text_va <= tgt < text_end:
                t = f"{tgt:08x}"
                if t in known and t != a:
                    calls[a].add(t); callers[t].add(a)

    # --- pointer tables in .rdata / .data: runs of >=2 absolute pointers to known starts
    table_of = {}      # function -> table id (first table that holds it)
    tables = []        # list of (table_va, [funcs])
    for sec in (".rdata", ".data"):
        if sec not in secs:
            continue
        sva, svs, sro, srs = secs[sec]
        blob = d[sro: sro + min(svs, srs)]
        run = []; run_va = None
        for i in range(0, len(blob) - 3, 4):
            v = struct.unpack_from("<I", blob, i)[0]
            f_ = f"{v:08x}"
            if text_va <= v < text_end and f_ in known:
                if not run:
                    run_va = sva + i
                run.append(f_)
            else:
                if len(run) >= 2:
                    tables.append((run_va, run))
                run = []
        if len(run) >= 2:
            tables.append((run_va, run))
    rtti = {}
    with open(ROOT / "rebuild/manifest/rtti-vtables.tsv", encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f, delimiter="\t"):
            rtti[int(r["vtable_va"], 16)] = r["class"]
    for ti, (tva, funcs) in enumerate(tables):
        for f_ in funcs:
            table_of.setdefault(f_, ti)

    # --- scope and neighbourhood
    scope = {a: scope_of(man[a]["module"], man[a]["name"]) for a in man}
    PLACEHOLDER = re.compile(r"^(_global|_gapscan|_unattributed|_static_init|std::.*|_$|)$")

    def neighbourhood(a: str, k: int = 6) -> collections.Counter:
        i = idx[a]; c = collections.Counter()
        for j in range(max(0, i - k), min(len(starts), i + k + 1)):
            b = f"{starts[j]:08x}"
            if b in cat:
                s = scope_of(*(cat[b][0].rsplit("::", 1) if "::" in cat[b][0] else ("_global", cat[b][0])))
                if s and not PLACEHOLDER.match(s):
                    c[s] += 1
        return c

    # --- union-find
    uf = UF(); why = collections.defaultdict(set)
    for a in remaining:
        uf.f(a)
    for a in remaining:                                   # 1. funclet -> parent
        p = funclet_parent.get(a)
        if p and p in rem:
            uf.u(p, a); why[a].add("funclet-parent")
    by_table = collections.defaultdict(list)              # 2. same pointer table
    for a in remaining:
        if a in table_of:
            by_table[table_of[a]].append(a)
    for ti, fs in by_table.items():
        for b in fs[1:]:
            uf.u(fs[0], b); why[b].add("vtable")
    # helper scopes instantiated from many TUs: never a TU of their own
    HELPER = re.compile(r"^(CThingSearchTools|CPersistContext|CPersistTraits|CPersistContext_\w+|GF\w*|CVectorMap|CSmallVector|CArray|CCountedPointer|CIVCountedPointer|CDefPointer|std(::.*)?|CThingFilter\w*|CFilter_Bind\d)$")
    span = {}                                             # root -> (lo, hi)
    def guarded_union(a, b, limit):
        ra, rb = uf.f(a), uf.f(b)
        if ra == rb:
            return True
        la, ha = span.get(ra, (int(a, 16), int(a, 16))); lb, hb = span.get(rb, (int(b, 16), int(b, 16)))
        if max(ha, hb) - min(la, lb) > limit:
            return False
        uf.u(a, b); span[uf.f(a)] = (min(la, lb), max(ha, hb))
        return True
    by_scope = collections.defaultdict(list)              # 3. same class scope (non-template, non-helper)
    for a in remaining:
        s = scope[a]
        if s and not PLACEHOLDER.match(s) and "<" not in s and not HELPER.match(s):
            by_scope[s].append(a)
    for s, fs in by_scope.items():
        for b in fs[1:]:
            uf.u(fs[0], b); why[b].add("class")
    for a in remaining:                                   # 4. direct call: same landed neighbourhood or within 16 KB
        na = neighbourhood(a)
        for t in calls.get(a, ()):
            if t not in rem:
                continue
            close = abs(int(t, 16) - int(a, 16)) <= 0x4000
            nt = neighbourhood(t)
            same = bool(na) and bool(nt) and na.most_common(1)[0][0] == nt.most_common(1)[0][0]
            if (close or same) and guarded_union(a, t, 0x20000):
                why[t].add("call")
    srem = sorted(remaining, key=lambda x: int(x, 16))   # 5. adjacency, no landed row between, same neighbourhood
    for a, b in zip(srem, srem[1:]):
        ia, ib = idx[a], idx[b]
        if ib == ia + 1:
            na, nb = neighbourhood(a), neighbourhood(b)
            if na and nb and na.most_common(1)[0][0] == nb.most_common(1)[0][0] and guarded_union(a, b, 0x20000):
                why[b].add("adjacent")

    groups = collections.defaultdict(list)
    for a in remaining:
        groups[uf.f(a)].append(a)

    def bucket(n: int) -> str:
        return "<=256" if n <= 256 else "<=1K" if n <= 1024 else "<=4K" if n <= 4096 else ">4K"

    def kind(a: str) -> str:
        n = man[a]["name"]; m = man[a]["module"]
        if re.search(r"(Unwind|Catch)@", n): return "funclet"
        if m == "_gapscan" or n.startswith("sub_"): return "gapscan"
        if a in parked: return "parked"
        return "unworked"

    rows_out = []
    grp_out = []
    for root, fs in groups.items():
        fs.sort(key=lambda x: int(x, 16))
        nb = collections.Counter()
        for a in fs:
            nb.update(neighbourhood(a))
        scopes = collections.Counter(scope[a] for a in fs)
        anchor = next((s for s, _ in scopes.most_common() if not PLACEHOLDER.match(s)), None) or (nb.most_common(1)[0][0] if nb else "_global")
        res = collections.Counter(parked[a]["residue_class"] for a in fs if a in parked)
        kinds = collections.Counter(kind(a) for a in fs)
        b = sum(extent(a) for a in fs)
        vt = sorted({rtti.get(tables[table_of[a]][0], f"table@{tables[table_of[a]][0]:08x}") for a in fs if a in table_of})
        dep_res = sorted({t for a in fs for t in calls.get(a, ()) if t in reserved})
        dep_unl = sorted({t for a in fs for t in calls.get(a, ()) if t in rem and uf.f(t) != root})
        landed_callers = sum(1 for a in fs for c in callers.get(a, ()) if c in cat)
        gid = f"G{fs[0]}"
        grp_out.append({
            "group": gid, "anchor": anchor, "rows": len(fs), "bytes": b,
            "lo": fs[0], "hi": fs[-1], "span": int(fs[-1], 16) - int(fs[0], 16),
            "parked": kinds.get("parked", 0), "unworked": kinds.get("unworked", 0),
            "funclets": kinds.get("funclet", 0), "gapscan": kinds.get("gapscan", 0),
            "sizes": " ".join(f"{k}:{v}" for k, v in sorted(collections.Counter(bucket(extent(a)) for a in fs).items())),
            "residues": " ".join(f"{k}:{v}" for k, v in res.most_common()),
            "vtables": " ".join(vt)[:200], "tu_context": " ".join(f"{k}:{v}" for k, v in nb.most_common(4)),
            "calls_reserved": " ".join(dep_res), "calls_other_groups": len(dep_unl), "landed_callers": landed_callers,
            "members": " ".join(fs),
        })
        for a in fs:
            rows_out.append({"address": a, "group": gid, "anchor": anchor, "name": (man[a]["name"] if man[a]["module"] in ("", "_global") else man[a]["module"] + "::" + man[a]["name"]),
                             "kind": kind(a), "bytes": extent(a), "residue": parked[a]["residue_class"] if a in parked else "",
                             "bound_by": "+".join(sorted(why.get(a, {"seed"}))),
                             "vtable": rtti.get(tables[table_of[a]][0], f"table@{tables[table_of[a]][0]:08x}") if a in table_of else "",
                             "callees_remaining": " ".join(sorted(t for t in calls.get(a, ()) if t in rem)),
                             "callees_landed": len([t for t in calls.get(a, ()) if t in cat]),
                             "callers_landed": len([c for c in callers.get(a, ()) if c in cat])})
    def category(g):
        n, b, p, u, f, gs = g["rows"], g["bytes"], g["parked"], g["unworked"], g["funclets"], g["gapscan"]
        res = dict((k, int(v)) for k, v in (x.split(":") for x in g["residues"].split() if ":" in x))
        imp = res.get("ltcg/crt/asm-impossible", 0)
        m = re.match(r"(\S+):(\d+)", g["tu_context"]); landed = int(m.group(2)) if m else 0
        if f and f >= n / 2: return "D funclet bundle (lands with its parent TU)"
        if gs and gs >= n / 2: return "G gap-scan starts (extent first)"
        if imp >= max(1, n // 2): return "E impossible-majority (retire with evidence)"
        if n == 1:
            return ("F1 single row >4 KB (skeleton, one session)" if b > 4096 else
                    "F2 single row, leftover of a landed TU" if landed >= 5 else "F3 single row, isolated")
        if landed >= 3 * n: return "A finish-the-TU (few rows left in a mostly landed class)"
        if g["vtables"] and u >= p: return "C vtable / class completion (slots mostly unworked)"
        if p > u: return "B parked-heavy TU (re-cut with the retail header + same-TU callees)"
        return "B2 unworked TU (fresh chunk)"
    for g in grp_out:
        g["category"] = category(g)
    for g in grp_out:                                     # category first in the TSV
        g_ = {"category": g.pop("category")}; g_.update(g); g.clear(); g.update(g_)
    grp_out.sort(key=lambda g: (-g["rows"], -g["bytes"]))
    rows_out.sort(key=lambda r: int(r["address"], 16))
    with open(out / "GROUPS.tsv", "w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(grp_out[0].keys()), delimiter="\t"); w.writeheader(); w.writerows(grp_out)
    with open(out / "ROWS.tsv", "w", encoding="utf-8", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows_out[0].keys()), delimiter="\t"); w.writeheader(); w.writerows(rows_out)
    # summary
    sizes = collections.Counter("1" if g["rows"] == 1 else "2-4" if g["rows"] <= 4 else "5-9" if g["rows"] <= 9 else "10-29" if g["rows"] <= 29 else "30+" for g in grp_out)
    cc, cr, cb = collections.Counter(), collections.Counter(), collections.Counter()
    for g in grp_out:
        cc[g["category"]] += 1; cr[g["category"]] += g["rows"]; cb[g["category"]] += g["bytes"]
    with open(out / "GROUPS.md", "w", encoding="utf-8") as f:
        f.write(f"# Remaining rows grouped by translation unit ({len(remaining)} rows, {len(grp_out)} groups; reserved rows excluded: {len(reserved)})\n\n")
        f.write(PREFACE)
        f.write("| category | groups | rows | bytes (est.) |\n|---|---:|---:|---:|\n")
        for k in sorted(cc):
            f.write(f"| {k} | {cc[k]} | {cr[k]} | {cb[k]:,} |\n")
        f.write(f"\nGroup sizes: {dict(sorted(sizes.items()))}. Columns in GROUPS.tsv; per-row detail in ROWS.tsv.\n\n")
        f.write("| group | anchor (class / TU) | rows | bytes | parked | unworked | funclets | residues | vtables | TU context (landed classes around it) |\n|---|---|---:|---:|---:|---:|---:|---|---|---|\n")
        for g in grp_out[:80]:
            f.write(f"| {g['category'][:2]} {g['group']} | {g['anchor']} | {g['rows']} | {g['bytes']:,} | {g['parked']} | {g['unworked']} | {g['funclets']} | {g['residues']} | {g['vtables'][:60]} | {g['tu_context']} |\n")
    print(f"remaining {len(remaining)} rows -> {len(grp_out)} groups; sizes {dict(sorted(sizes.items()))}; "
          f"call edges {sum(len(v) for v in calls.values())}; pointer tables {len(tables)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
