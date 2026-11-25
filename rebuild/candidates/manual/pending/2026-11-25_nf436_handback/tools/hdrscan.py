#!/usr/bin/env python3
"""hdrscan.py <Class>: scan engine/<Class>.h outer class body at depth 1: access at each declaration (class default private), ifdef
context, declared ctor / Alloc / overrides, the line index of the outer class's closing brace. Returns a dict (import) or prints."""
import re, sys
R = "/home/user/FableDecomp/rebuild/include/engine/"
def scan(C, path=None):
    t = open(path or (R + C + ".h")).read().split("\n")
    st = None
    for i, l in enumerate(t):
        if re.match(r"\s*class %s\s*:" % C, l): st = i; break
    if st is None: return None
    depth = 0; acc = "private"; ifs = []; out = {"start": st, "decl": {}, "end": None, "lines": t, "access_first": None}
    started = False
    for i in range(st, len(t)):
        l = t[i]; s = l.strip()
        if s.startswith("#if"): ifs.append(s.split()[1] if len(s.split()) > 1 else s); continue
        if s.startswith("#endif"): ifs and ifs.pop(); continue
        if s.startswith("#else"): ifs and ifs.append("!" + ifs.pop()); continue
        code = re.sub(r"//.*", "", l)
        pre = depth
        if not started and "{" in code: started = True
        depth += code.count("{") - code.count("}")
        if not started: continue
        if pre == 1 or (i == st):
            m = re.match(r"\s*(public|private|protected)\s*:", code)
            if pre == 1 and m:
                acc = m.group(1)
                if out["access_first"] is None: out["access_first"] = acc
            if pre == 1:
                for nm in ("RegisterMain", "Main", "Init", "OnPersist"):
                    if re.search(r"virtual\s+void\s+%s\s*\(" % nm, code): out["decl"][nm] = (acc, tuple(ifs), i)
                if re.search(r"\bAlloc\s*\(\s*CScriptDataBase", code): out["decl"]["Alloc"] = (acc, tuple(ifs), i)
                if re.search(r"(?<![~\w])%s\s*\(\s*CScriptDataBase" % C, code): out["decl"]["ctor"] = (acc, tuple(ifs), i)
                if re.search(r"~%s\s*\(" % C, code): out["decl"]["dtor"] = (acc, tuple(ifs), i)
                if re.search(r"\bvirtual\b", code) and not any(re.search(r"virtual\s+void\s+%s\s*\(" % n, code) for n in ("RegisterMain", "Main", "Init", "OnPersist")):
                    out["decl"].setdefault("other_virtual", []).append(i) if isinstance(out["decl"].get("other_virtual", []), list) else None
        if started and depth == 0: out["end"] = i; break
    m = re.search(r"class %s\s*:\s*public\s+CLevelScriptBase<\s*(?:NScript::)?(\w+)\s*>" % C, "\n".join(t))
    out["T"] = m.group(1) if m else None
    m = re.search(r"sizeof\((?:NScript::)?%s\)\s*==\s*(0x[0-9a-fA-F]+)" % C, "\n".join(t))
    out["size"] = m.group(1).lower() if m else None
    return out
if __name__ == "__main__":
    for C in sys.argv[1:]:
        o = scan(C)
        if not o: print(C, "NOCLASS"); continue
        d = {k: v for k, v in o["decl"].items()}
        print(C, "T=%s size=%s first=%s end=%s" % (o["T"], o["size"], o["access_first"], o["end"]), d)
