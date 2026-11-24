# hdredit.py <spec.json>: add-only opt-in blocks. spec: [{"header": "engine/X.h", "class_line": N (1-based line of `class X ...` in the
# HEAD header), "macro": "FABLE_RETAIL_..._RTTI_DECLS", "comment": [lines], "decls": [[access, decl-text], ...]}].
# Inserts, just before the closing `};` of that class (brace depth from the class line, preprocessor lines skipped):
#   #ifdef MACRO / comment / per access group `<access>:` + decls / `<restore>:` / #endif
# restore = the last top-level access label of the class body outside any #if (default `private:` for class, `public:` for struct).
# Edits work/tree/<header> (copy of HEAD), saves hdr/<header> and orig/hdr/<header>. Never touches the repo.
import json, re, sys, shutil
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF328')
R = Path('/home/user/FableDecomp/rebuild/include')
for sp in json.load(open(sys.argv[1])):
    h = sp['header']; src = (R / h).read_text(encoding='utf-8'); L = src.split('\n')
    k = sp['class_line'] - 1; kw = 'struct' if re.match(r'\s*struct\b', L[k]) else 'class'
    depth = 0; started = False; ifd = 0; label = 'public' if kw == 'struct' else 'private'; end = None
    for n in range(k, len(L)):
        s = L[n].strip()
        if s.startswith('#'):
            if re.match(r'#\s*if', s): ifd += 1
            elif re.match(r'#\s*endif', s): ifd -= 1
            continue
        code = L[n].split('//')[0]
        if started and depth == 1 and ifd == 0:
            m = re.match(r'\s*(public|protected|private)\s*:', code)
            if m: label = m.group(1)
        for ch in code:
            if ch == '{': depth += 1; started = True
            elif ch == '}':
                depth -= 1
                if started and depth == 0: end = n; break
        if end is not None: break
    if end is None or not re.match(r'\s*\}\s*;', L[end]):
        print('%s: SKIP no own closing line for the class (end=%s: %s)' % (h, end, (L[end][:100] if end is not None else None))); continue
    blk = ['#ifdef ' + sp['macro']] + ['    // ' + c for c in sp['comment']]
    cur = None
    for acc, d in sp['decls']:
        if acc != cur: blk.append(acc + ':'); cur = acc
        blk.append('    ' + d)
    blk += [label + ':', '#endif']
    out = L[:end] + blk + L[end:]
    t = A / 'work/tree' / h; t.write_text('\n'.join(out), encoding='utf-8')
    for d in (A / 'hdr' / h, A / 'orig/hdr' / h): d.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy(R / h, A / 'orig/hdr' / h)
    print('%s: block at line %d (%d lines), restore %s:' % (h, end + 1, len(blk), label))
