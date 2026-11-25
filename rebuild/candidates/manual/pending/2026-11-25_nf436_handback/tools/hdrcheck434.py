#!/usr/bin/env python3
"""hdrcheck434.py <batch>: every batches/<b>/hdr/engine/<X>.h is add-only: drop each `#ifdef/#ifndef <MACRO>` .. matching `#endif` block
whose macro is NOT in orig/hdr_<X>.h (NF434 opt-ins), then compare with orig/hdr_<X>.h byte for byte. NEW = no orig and not in the repo."""
import sys, re, os
W = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436"; B = W + "/batches/" + sys.argv[1]
out = []
for f in sorted(os.listdir(B + "/hdr/engine")):
    o = W + "/orig/hdr_" + f; n = open(B + "/hdr/engine/" + f).read().split("\n")
    if not os.path.exists(o):
        out.append("%s\t%s" % (f, "NEW" if not os.path.exists("/home/user/FableDecomp/rebuild/include/engine/" + f) else "NO_ORIG_BUT_IN_REPO")); continue
    O = open(o).read(); keep = []; depth = 0; skip = 0; ins = 0; unwrap = 0
    for l in n:
        m = re.match(r"#\s*if(n?def)?\s+(?:defined\()?(\w+)", l)
        if skip:
            ins += 1
            if re.match(r"#\s*if", l): skip += 1
            elif re.match(r"#\s*endif", l): skip -= 1
            continue
        if m and m.group(2) not in O and m.group(2).startswith("FABLE_RETAIL_"):
            if m.group(1) == "ndef": unwrap = 1; ins += 2; continue   # NF436: #ifndef <new opt-in> around an original line: keep the line
            skip = 1; ins += 1; continue
        if unwrap and re.match(r"#\s*endif", l): unwrap = 0; continue
        keep.append(l)
    keep = [re.sub(r"^(\s*)FABLE_STATIC_ASSERT_AT\(\d+, (.*?)\);(.*)$", r"\1FABLE_STATIC_ASSERT(\2);\3", l) if ("FABLE_STATIC_ASSERT_AT" in l and l not in O) else l for l in keep]
    same = "\n".join(keep) == O.rstrip("\n") + ("\n" if O.endswith("\n") else "") or "\n".join(keep) == O
    rep = open("/home/user/FableDecomp/rebuild/include/engine/" + f).read()
    out.append("%s\t%s\tinserted_lines=%d\torig==repo:%s" % (f, "SAME" if same else "DIFF", ins, rep == O))
os.makedirs(B + "/evidence", exist_ok=True); open(B + "/evidence/hdrcheck.txt", "w").write("\n".join(out) + "\n"); print("\n".join(out))
