# d3_tags.py <files>: local EAX SDK structs -> the SDK struct tags (EAX40.h `typedef struct _X {..} X`), so every local
# model mangles the parameter types as retail's headers do (U_EAXVECTOR, PBU_EAXREVERBPROPERTIES, ...).
import re, sys
TAGS = ["EAXCONTEXTPROPERTIES", "EAXEXCLUSIONPROPERTIES", "EAXFXSLOTPROPERTIES", "EAXOBSTRUCTIONPROPERTIES",
        "EAXOCCLUSIONPROPERTIES", "EAXREVERBPROPERTIES", "EAXSOURCEPROPERTIES", "EAXVECTOR", "EAXSOURCESENDPROPERTIES",
        "EAXSOURCEALLSENDPROPERTIES", "EAXSOURCEOCCLUSIONSENDPROPERTIES", "EAXSOURCEEXCLUSIONSENDPROPERTIES", "EAXACTIVEFXSLOTS"]
for p in sys.argv[1:]:
    t0 = t = open(p).read()
    if re.search(r'#include\s+"engine/(EAX40|CEAXListener|CEAXBuffer|EAXNativeTypes)\.h"', t): continue
    for x in TAGS:
        if re.search(r"^\s*struct %s\s*\{" % x, t, re.M): t = re.sub(r"\b%s\b" % x, "_" + x, t)
    if t != t0: open(p, "w").write(t); print("changed", p)
