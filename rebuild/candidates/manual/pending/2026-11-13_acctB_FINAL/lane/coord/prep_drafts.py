#!/usr/bin/env python3
# prep_drafts.py -- cut one directory per unstarted reserved row and build workflow args with row hints.
# usage: (S exported, wine env sourced) python3 prep_drafts.py  -> $S/coord/draft_args_{opus1,opus2,opus3,fable1,fable2}.json
import csv, glob, json, os, re, subprocess, sys
S = os.environ["S"]; ROOT = "/home/user/FableDecomp"; P = ROOT + "/rebuild/candidates/manual/pending"; W = S + "/w22"
res = list(csv.DictReader(open(P + "/2026-11-11_acctB/RESERVED.tsv"), delimiter="\t"))
done = set()
for f in glob.glob(P + "/2026-11-1*_acctB_*/STATUS.tsv"):
    for l in open(f):
        c = l.rstrip("\n").split("\t")
        if len(c) > 1 and "MATCH" in c[1] and "DIFFER" not in c[1]:
            done.add(c[0].lower().replace("0x", ""))
inwork = {"0060eeb0", "004804de", "00bd3070", "0048e78e", "007cacb0", "00ba5260", "00bcc800", "00b555a0", "00b8b780"}
skip = {"006224c0", "00ab2520", "0074bc30", "00d2988b", "00bd71cc", "00ba115c", "00bc3f3d"}
rows = [r for r in res if r["address"].lower() not in done | inwork | skip]
rows.sort(key=lambda r: int(r["length_estimate"]))
print(len(rows), "rows")

UI = ("/opt/fable_lane/w22/agent4007 is the matched 12 KB UI list builder CTCInventoryQuests::ConstructQuestList 0x0061B610 (src/, test/, NOTES.md, LESSONS.md, "
      "hdr/ overlay with the CAction / CCountedPointer / CList / CRGBColour opt-ins, tools/cd.sh = full direct-call comparison, tools/bt.sh = branch-target check, "
      "tools/gentest.py = test generator from the decoded call map); /opt/fable_lane/w22/agent4008 (ConstructPCMapList, in progress) has Opus scout files for the same family; "
      "/opt/fable_lane/w22/agent2058 is the matched DisplayScoreboardGUI (lesson c2058: many small callees called instead of expanded = a missing branch level in front). "
      "Copy the hdr/ overlay of the closest of these into your hdr/ as a start. Lesson f4007 in the brief is the order of work for this family.")
RENDER = ("Matched render rows with their hdr/ overlays, NOTES.md and helper texts fitted to FableWin sizes: /opt/fable_lane/w22/agent2048 (RenderClouds / RenderSunFlare, "
          "render-state manager helpers), /opt/fable_lane/w22/agent2059 (CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive: shader / constant set-up, blend helpers, "
          "lesson c2059), /opt/fable_lane/w22/agent3017 (cloth normals: vector helper texts, asm-evidence table harness under probe/asm); /opt/fable_lane/w22/agent2060 "
          "(StaticMeshesBase::RenderMeshPrimitive, close draft) lists the inline helpers of the primitive managers by FableWin size in NOTES.md. Copy the closest overlay into "
          "your hdr/ as a start and run fwsize on every inline helper you expand. fistp / SSE sites reached through shared helper headers need `// Retail-asm:` lines and the "
          "per-site table (ASM_RULES).")
SCRIPT = ("A quest / game script body: long straight-line sequences of script-interface calls with CCharString / CWideString temporaries. docs/scripts/AEON_LUA_PORTS.md "
          "describes the Lua ports of the retail quest scripts (the oracle for WHAT the script does and in which order); the FSE manifest refs/fse_api_manifest.json names "
          "the script API functions. Find the landed rows of the same script class or neighbouring script classes (grep rebuild/src/compiled by class name and by the "
          "neighbouring addresses) and copy their defines, includes, flags and temporaries style. A full direct-call comparison early (/opt/fable_lane/w22/agent4007/tools/cd.sh) "
          "shows every missing or reordered call in one line each.")
DEF = ("Definition / persistence code: long runs of the same helper per member. Models: the landed Transfer / operator= / registration rows of the same class family (grep "
       "rebuild/src/compiled), and /opt/fable_lane/w22/agent2052 (matched CWorld save / load: CPersistContext Transfer helper texts, lesson c2052). Settle ONE member's "
       "statement shape against the bytes first, then write the whole list in retail's member order (a direct-call comparison with /opt/fable_lane/w22/agent4007/tools/cd.sh "
       "gives the order).")
ANIM = ("/opt/fable_lane/w22/agent3022 holds three matched C3DAnimationManager rows (BlendAnimations, CalcMeshTransformations, CalcBlendedMeshTransformations) with ONE "
        "common helper block (GFFastArcCos, GFFastSin, SetSlerp, BlendWith, CalculateSlerpWeights; lesson u3022) and the hdr/ overlay: copy the overlay and the helper block "
        "unchanged.")

def family(mod, name):
    s = mod + " " + name
    if re.search(r"C3DAnimationManager", s): return ANIM
    if re.search(r"NScript|CQ_|CScriptGameResource|ArenaScript", s): return SCRIPT
    if re.search(r"Transfer@|operator=|StatRegisterClass|RegisterModes|InitAndCompile|CInventoryDef|CPlayerGuiDef", s): return DEF
    if re.search(r"CTCInventory|CTCInGameMenu|CTCQuestCompletionUI|ConstructQuestsScreen|CFrontEndManager|CDrawAbilityButtons|CDrawQuickAccessIcons|Scoreboard", s): return UI
    if re.search(r"CEngine|CRenderStateManager|COptimisedPrimitive|CPSCOrbit", s): return RENDER
    return ("Find the landed rows of the same class / translation unit (grep rebuild/src/compiled by class name and by the neighbouring addresses) and copy their defines, "
            "includes and flags; a full direct-call comparison early (/opt/fable_lane/w22/agent4007/tools/cd.sh) settles the inline decisions.")

def sh(cmd):
    return subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=ROOT).stdout

out = {"opus1": [], "opus2": [], "opus3": [], "fable1": [[], []], "fable2": [[], []]}
no, nf = 2064, 4011
io = ifb = 0
for r in rows:
    a = r["address"].lower(); est = int(r["length_estimate"]); mod = r["catalog_module"]; cname = r["catalog_name"]
    big = est >= 8000
    n = nf if big else no
    if big: nf += 1
    else: no += 1
    d = f"{W}/agent{n}"
    if not os.path.exists(d + "/dis_all.txt"):
        subprocess.run(["python3", S + "/coord/cut.py", str(n), a], cwd=ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        subprocess.run(["python3", S + "/coord/brief.py", str(n)], cwd=ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if not os.path.exists(d + "/dis_all.txt"):
        print("CUT FAILED", n, a); continue
    lines = open(d + "/dis_all.txt").read().split("\n")
    m = re.search(r"len=(\d+)", lines[0]); ext = int(m.group(1)) if m else est
    body = [l for l in lines[1:] if l.strip()]
    pro = "; ".join(re.sub(r"^\S+\s+\S+\s+", "", l).strip() for l in body[:4])
    last = re.sub(r"^\S+\s+\S+\s+", "", body[-1]).strip()
    fistp = sum(1 for l in body if "fistp" in l)
    jt = sum(1 for l in body if re.search(r"jmp dword ptr \[.*\*4", l))
    cls = mod.split("::")[-1]
    sib = sh(f"find rebuild/src/compiled -name '{cls}_*' | sed 's#.*/##' | head -10").split()
    oth = [x for x in sh(f"grep -rl -i '{a}' rebuild/candidates/manual/pending --include=NOTES.md --include=STATUS.tsv --include=hdr_README.md 2>/dev/null").split() if "acctB" not in x][:4]
    disp = cname if cname.startswith("?") or "::" in cname else f"{mod}::{cname}"
    hint = (f"Prologue `{pro}`, last instruction `{last}`" + (f", {fistp} fistp site(s) (ASM_RULES evidence if reached through shared helper headers)" if fistp else ", no fistp")
            + (f", {jt} jump-table switch(es)" if jt else "") + ". "
            + ("The start address is not 16-aligned: confirm the function start / extent before authoring. " if int(a, 16) % 16 else "")
            + ("The catalog name is a placeholder: find the PDB name first (vtable slot / RTTI / callers / both PDB name sets). " if re.search(r"vfunc_|sub_|_gapscan|_global", mod + cname) else "")
            + (f"Landed rows of the same class (copy their defines, includes, flags): {', '.join(sib)}. " if sib else "No landed row of this class was found by file name: grep rebuild/src/compiled by the neighbouring addresses for the same-TU rows. ")
            + (f"Other lanes' notes that mention this row (read them, they may name it or hold a parked draft): {', '.join('/home/user/FableDecomp/' + x for x in oth)}. " if oth else "")
            + family(mod, cname)
            + " BREADTH-FIRST PASS: the project owner wants every remaining row to have at least a close draft. Author the WHOLE function early (every block, no stubs) and grade it, "
              "then close the big structural differences first (inline v call decisions, frame, block layout), then residues, and keep going until it matches or your context is "
              "nearly gone. Keep NOTES.md current every few grades (best candidate, exact remaining difference grouped by kind, what moved it, do-not-retry list) and always keep the "
              "best source in differ/, so the row is never left without a usable draft.")
    row = {"n": n, "addr": a, "kind": "new", "name": disp, "state": f"{ext} bytes, no source yet", "hint": hint}
    if big:
        row["scouts"] = ["outline", "headers"]
        k = "fable1" if ifb % 2 == 0 else "fable2"
        out[k][(ifb // 2) % 2].append(row); ifb += 1
    else:
        row["models"] = ["opus"]
        out[("opus1", "opus2", "opus3")[io % 3]].append(row); io += 1
    print(n, a, ext, disp[:60], "fable" if big else "opus", len(sib), len(oth))
for k, v in out.items():
    args = {"lanes": v, "maxSessions": 1} if k.startswith("fable") else {"rows": v, "maxSessions": 1}
    json.dump(args, open(f"{S}/coord/draft_args_{k}.json", "w"), indent=1)
    print(k, sum(len(x) for x in v) if k.startswith("fable") else len(v))
