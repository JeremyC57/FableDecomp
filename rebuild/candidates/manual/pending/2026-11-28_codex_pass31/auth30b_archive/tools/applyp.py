#!/usr/bin/env python3
"""applyp.py <PARTIAL_PATCHES.json> [--apply]: show (and with --apply, apply) a Codex row's header patches to the AUTH30 overlay.
Each patch: file (relative to rebuild/include), old_text, new_text. The overlay file starts as a copy of the CURRENT repo header (orig copy
saved as orig/hdr_<basename>). Reports whether each patch is add-only (old_text kept intact inside new_text) and whether old_text is
unique in the current overlay text. Run with python3 -I (the JSON is untrusted data; nothing in it is executed)."""
import json, sys, os, shutil, difflib
R = '/home/user/FableDecomp/rebuild/include'; W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/auth30'
recs = json.load(open(sys.argv[1])); app = '--apply' in sys.argv
for i, r in enumerate(recs):
    f, old, new = r['file'], r['old_text'], r['new_text']
    ov = os.path.join(W, 'hdr', f); rp = os.path.join(R, f)
    exists = os.path.exists(rp)
    cur = open(ov, encoding='utf-8', errors='replace').read() if os.path.exists(ov) else (open(rp, encoding='utf-8', errors='replace').read() if exists else '')
    ol = (old + '\n').splitlines(True); nl = (new + '\n').splitlines(True); ins = []; addonly = True
    for op, a1, a2, b1, b2 in difflib.SequenceMatcher(None, ol, nl, autojunk=False).get_opcodes():
        if op == 'equal': continue
        if op != 'insert': addonly = False
        ins.append(''.join(nl[b1:b2]))
    # each inserted run must be a balanced #if.../#endif block opening with a FABLE_RETAIL_ / defined(FABLE_RETAIL_ test
    for t in ins:
        L = [x.strip() for x in t.splitlines() if x.strip()]
        dep = 0; ok = bool(L) and L[0].startswith(('#ifdef FABLE_RETAIL', '#if defined(FABLE_RETAIL', '#ifndef FABLE_RETAIL'))
        for x in L:
            if x.startswith('#if'): dep += 1
            elif x.startswith('#endif'): dep -= 1
        if dep != 0 or not ok: addonly = False
    cnt = cur.count(old) if old else -1
    already = (new in cur) if new else False
    print('=== patch', i, f, 'repo_exists', exists, 'add_only', addonly, 'old_count', cnt, 'already_applied', already)
    if old:
        for t in ins: print('--- inserted run:\n' + t, end='')
    else:
        print('--- NEW FILE text:\n' + new)
    if app and not already:
        os.makedirs(os.path.dirname(ov), exist_ok=True)
        if exists and not os.path.exists(os.path.join(W, 'orig', 'hdr_' + os.path.basename(f))):
            shutil.copy(rp, os.path.join(W, 'orig', 'hdr_' + os.path.basename(f)))
        if not old:
            if exists: print('!! empty old_text but repo file exists: not applied'); continue
            open(ov, 'w', encoding='utf-8').write(new); print('applied (new file)')
        elif cnt == 1 and (addonly or '--force' in sys.argv):   # --force: add-only checked by hand (#ifdef/#else keeping the default text)
            open(ov, 'w', encoding='utf-8').write(cur.replace(old, new)); print('applied')
        else:
            print('!! NOT applied (count %d, add_only %s)' % (cnt, addonly))
