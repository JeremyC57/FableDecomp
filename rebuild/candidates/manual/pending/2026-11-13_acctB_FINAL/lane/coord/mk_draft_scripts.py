#!/usr/bin/env python3
# Build five self-contained workflow scripts for the breadth-first draft pass (row lists embedded, one session per row).
import json, os
S = "/opt/fable_lane"
SP = "/tmp/claude-0/-home-user-FableDecomp/ec5e96c7-c2a3-5632-9fee-64af8822e954/scratchpad"
FAB = "/root/.claude/projects/-home-user-FableDecomp/ec5e96c7-c2a3-5632-9fee-64af8822e954/workflows/scripts/fable-rows-opus-scouts-wf_c25ff3a1-7e8.js"
OPU = SP + "/rows-and-tasks.js"

def rep(s, old, new, count=1):
    assert s.count(old) >= 1, old
    return s.replace(old, new, count)

for k in ("fable1", "fable2"):
    a = json.load(open(f"{S}/coord/draft_args_{k}.json"))
    s = open(FAB).read()
    s = rep(s, "name: 'fable-rows-opus-scouts',", f"name: 'draft-pass-{k}',")
    s = rep(s, "const LANES = args.lanes", "const LANES = " + json.dumps(a["lanes"]))
    s = rep(s, "for (let s = 1; s <= 3; s++) {", "for (let s = 1; s <= 1; s++) {")
    s = rep(s, "the lessons r3009, f4006, c2048 (both), r2044, c2051, r2034, c2052 are binding.",
            "the lessons r3009, f4006, c2048 (both), r2044, c2051, r2034, c2052, f4007, r3017..r3021, c2058 / c2054 / c2055, c2059 and u3022 are binding.")
    open(f"{SP}/draft-{k}.js", "w").write(s)
    print(k, len(s), sum(len(x) for x in a["lanes"]))

for k in ("opus1", "opus2", "opus3"):
    a = json.load(open(f"{S}/coord/draft_args_{k}.json"))
    s = open(OPU).read()
    s = rep(s, "name: 'rows-and-tasks-until-match',", f"name: 'draft-pass-{k}',")
    s = rep(s, "const ROWS = args.rows", "const ROWS = " + json.dumps(a["rows"]))
    # one session per row: stages 2 and 3 pass the result through
    s = rep(s, "    if (prev && prev.matched) return prev\n    return session(r, 2, prev || UNKNOWN).then(x => x || prev)", "    return prev")
    s = rep(s, "    if (prev && prev.matched) return prev\n    log(", "    return prev\n    log(")
    s = rep(s, "f4007 and r3017..r3021 are binding.", "f4007, r3017..r3021, c2058 / c2054 / c2055, c2059 and u3022 are binding.", 2)
    open(f"{SP}/draft-{k}.js", "w").write(s)
    print(k, len(s), len(a["rows"]))
