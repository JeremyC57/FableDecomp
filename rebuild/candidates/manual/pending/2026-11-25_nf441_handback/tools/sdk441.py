#!/usr/bin/env python3
"""sdk441.py <lib> <out.json>: for every ??_7 in the lib: slot symbol list; for every code section: size, callees, member obj.
Reads the SDK in place (scratch only); writes only names/sizes to the scratch json."""
import sys, json
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/libsig")
import coff_lib
lib, out = sys.argv[1], sys.argv[2]
ar = coff_lib.Archive(lib)
vts, funcs = {}, {}
for m, o in ar.objects():
    for s in o.symbols:
        if s.name.startswith("??_7") and s.section > 0 and s.storage == 2:
            sec = o.sections[s.section - 1]
            nxt = sorted(x.value for x in o.symbols if x.section == s.section and x.value > s.value and x.storage in (2, 3))
            end = nxt[0] if nxt else sec["size"]
            rel = sorted((r.offset, r.symbol) for r in o.relocs(sec) if s.value <= r.offset < end)
            slots = [n for off, n in rel if off >= s.value]
            # slot 0 may be preceded by COL reloc at value-4 (no RTTI here)
            if s.name not in vts or len(vts[s.name]["slots"]) < len(slots):
                vts[s.name] = {"slots": slots, "obj": m.name}
    for cs in o.code_sections():
        nm = cs.primary
        calls = [r.symbol for r in cs.relocs]
        if nm not in funcs or funcs[nm]["size"] < len(cs.data):
            funcs[nm] = {"size": len(cs.data), "relocs": calls, "obj": m.name}
json.dump({"vts": vts, "funcs": funcs}, open(out, "w"), indent=0)
print(len(vts), len(funcs))
