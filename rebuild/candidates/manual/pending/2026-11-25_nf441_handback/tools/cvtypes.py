#!/usr/bin/env python3
"""cvtypes.py <out.json> <obj>...: parse llvm-readobj --codeview-merged-types into class records
{name: {size, bases, members:[[off,name,type]], methods:[[name,kind,vfoff,type]]}} (first full definition wins)."""
import sys, re, json, subprocess
out = sys.argv[1]; classes = {}
def records(txt):
    cur = None
    for line in txt.splitlines():
        m = re.match(r"^  (\w+) \((0x[0-9A-F]+)\) \{$", line)
        if m:
            cur = {"kind": m.group(1), "ti": m.group(2), "lines": []}; yield cur; continue
        if cur is not None: cur["lines"].append(line)
for obj in sys.argv[2:]:
    txt = subprocess.run(["llvm-readobj", "--codeview-merged-types", obj], capture_output=True, text=True).stdout
    recs = {}
    order = []
    for r in records(txt):
        recs[r["ti"]] = r; order.append(r)
    def kv(lines, key):
        for l in lines:
            m = re.match(r"^\s+%s: (.*)$" % key, l)
            if m: return m.group(1)
        return None
    def mlist(ti):
        r = recs.get(ti); res = []
        if not r: return res
        cur = None
        for l in r["lines"]:
            if l.strip() == "Method [": cur = {}
            elif l.strip() == "]" and cur is not None: res.append(cur); cur = None
            elif cur is not None:
                m = re.match(r"^\s+(\w+): (.*)$", l)
                if m: cur[m.group(1)] = m.group(2)
        return res
    def fieldlist(ti):
        r = recs.get(ti); items = []
        if not r: return items
        cur = None; depth = 0
        for l in r["lines"]:
            m = re.match(r"^    (\w+) \{$", l)
            if m: cur = {"_k": m.group(1)}; continue
            if l == "    }" and cur is not None: items.append(cur); cur = None; continue
            if cur is not None:
                m = re.match(r"^      (\w+): (.*)$", l)
                if m: cur[m.group(1)] = m.group(2)
        return items
    for r in order:
        if r["kind"] not in ("Class", "Struct"): continue
        fl = kv(r["lines"], "FieldList")
        name = kv(r["lines"], "Name")
        if not fl or fl == "0x0" or name in classes: continue
        size = int(kv(r["lines"], "SizeOf") or 0)
        flti = re.search(r"\((0x[0-9A-F]+)\)$", fl) or re.match(r"(0x[0-9A-F]+)", fl)
        items = fieldlist(flti.group(1))
        c = {"size": size, "bases": [], "members": [], "methods": [], "obj": obj.split("/")[-1]}
        for it in items:
            k = it["_k"]
            if k == "BaseClass": c["bases"].append(re.sub(r" \(0x[0-9A-F]+\)$", "", it.get("BaseType", "")))
            elif k == "DataMember": c["members"].append([int(it.get("FieldOffset", "0"), 16), it.get("Name"), re.sub(r" \(0x[0-9A-F]+\)$", "", it.get("Type", ""))])
            elif k == "OneMethod":
                c["methods"].append([it.get("Name"), it.get("MethodKind", "Vanilla"), it.get("VFTableOffset"), re.sub(r" \(0x[0-9A-F]+\)$", "", it.get("Type", ""))])
            elif k == "OverloadedMethod":
                mi = re.search(r"(0x[0-9A-F]+)", it.get("MethodListIndex", ""))
                for m in mlist(mi.group(1) if mi else ""):
                    c["methods"].append([it.get("Name"), m.get("MethodKind", "Vanilla"), m.get("VFTableOffset"), re.sub(r" \(0x[0-9A-F]+\)$", "", m.get("Type", ""))])
        classes[name] = c
json.dump(classes, open(out, "w"), indent=0)
print(len(classes))
