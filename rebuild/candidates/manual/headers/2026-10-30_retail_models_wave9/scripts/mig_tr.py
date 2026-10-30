#!/usr/bin/env python3
"""Wave 8 family TR (wave 7 NEXT item 3): byte-neutral master-data T respellings in landed CLevelScriptBase<T> rows, T from
retail CScriptInfoManager::RegisterScripts 0x00CD52D0 (the master-data Alloc paired with each script Alloc; wave 7
HEADERS.md, scripts/regscripts.py + regmap.py). T enters no symbol of these rows. 00eea020 (CExpression_FollowScript) is
registered with no master data (`ebx` = 0): its row keeps its T and says so."""
import sys, re, glob, os
R = os.environ["FABLE_ROOT"]
FIX = {"00eee800": ("CQ_SunnyvaleMasterData", "CEmptyData", "CEmptyScript"),
       "00eeec00": ("CQ_SunnyvaleMasterData", "CEmptyData", "CTestTheInventoryScript"),
       "00efc300": ("CQ_SunnyvaleMasterData", "CDanyalTestScriptsData", "CHeroTurningDebuggerScript"),
       "00efc260": ("CQ_SunnyvaleMasterData", "CDanyalTestScriptsData", "CHeroTurningDebuggerScript"),
       "00d80ba0": ("CEmptyData", "CQ_SunnyvaleMasterData", "CQ_HeroSoulsArenaScript")}
NOTE = "// Master data (wave 8): RegisterScripts 0x00CD52D0 pairs %s's Alloc with the %s Alloc (wave 7 HEADERS.md table), so T is %s.\n"
for a in sys.argv[1:]:
    f = glob.glob(R + "/rebuild/src/compiled/*/*/*_%s.cpp" % a)[0]; t = open(f).read()
    if a == "00eea020":
        t2 = re.sub(r"(class CExpression_FollowScript : public CLevelScriptBase<)", "// Master data (wave 8): RegisterScripts 0x00CD52D0 registers CExpression_FollowScript with no master-data Alloc (`ebx` = 0),\n// so its T is not recoverable from retail; T enters no symbol of this row and stays as landed (unproven).\n\\1", t, count=1)
    else:
        old, new, cls = FIX[a]
        if re.search(r"\b" + new + r"\b", re.sub(r"//[^\n]*", "", t)) and old != "CEmptyData": print("skip", a, "new T named"); continue
        code = re.sub(r"//[^\n]*", "", t)
        if len(re.findall(r"\b" + old + r"\b", code)) != len(re.findall(r"(?:class\s+" + old + r"\s*;|CLevelScriptBase<\s*" + old + r"\s*>)", code)): print("skip", a, "T used otherwise"); continue
        t2 = re.sub(r"(class\s+)" + old + r"(\s*;)", r"\g<1>" + new + r"\2", t)
        t2 = re.sub(r"(CLevelScriptBase<\s*)" + old + r"(\s*>)", r"\g<1>" + new + r"\2", t2)
        m = re.search(r"^[^\n]*class " + cls + r"\b[^\n]*CLevelScriptBase<", t2, re.M)
        if m: t2 = t2[:m.start()] + NOTE % (cls, {"CEmptyData": "CEmptyData 0x00CD4AC0", "CDanyalTestScriptsData": "CDanyalTestScriptsData 0x00CD4B80", "CQ_SunnyvaleMasterData": "CQ_SunnyvaleMasterData 0x00CDBD20"}[new], new) + t2[m.start():]
    if t2 == t: print("skip", a, "nochange"); continue
    open(f, "w", newline="\n").write(t2); print("edit", os.path.relpath(f, R), a)
