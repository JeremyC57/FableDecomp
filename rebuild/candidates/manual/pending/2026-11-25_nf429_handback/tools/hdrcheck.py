# hdrcheck.py: for every hdr/engine/<X>.h, prove the edit is add-only round the flat model: drop the inserted
# `#ifdef FABLE_RETAIL_<X>_VIRTUAL_MODEL` ... `#else` block and the closing `#endif // FABLE_RETAIL_<X>_VIRTUAL_MODEL` line, undo the
# FABLE_STATIC_ASSERT_AT(<n>, ...) pins that differ from orig/hdr_<X>.h, and compare with orig/hdr_<X>.h byte for byte.
# Prints SAME / DIFF per header (+ the counts of inserted lines and pinned asserts) -> evidence/hdrcheck.txt
import re
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429')
out = []
for f in sorted((A / 'hdr/engine').glob('*.h')):
    X = f.stem; SW = 'FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()
    N = f.read_text(encoding='utf-8').split('\n'); O = (A / 'orig' / ('hdr_%s.h' % X)).read_text(encoding='utf-8').split('\n')
    if '#ifdef ' + SW not in N:   # NF325: not a generated _VIRTUAL_MODEL header: list every non-equal diff block (edited existing lines named)
        import difflib
        blk = []
        for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, O, N, autojunk=False).get_opcodes():
            if tag != 'equal': blk.append('%s old%d-%d new%d-%d: %s => %s' % (tag, i1 + 1, i2, j1 + 1, j2, ' | '.join(O[i1:i2])[:160], ' | '.join(N[j1:j2])[:200]))
        out.append('%s\tOTHER\t%d blocks\n  ' % (X, len(blk)) + '\n  '.join(blk)); continue
    i = N.index('#ifdef ' + SW); j = N.index('#else', i); k = N.index('#endif // ' + SW)
    R = N[:i] + N[j + 1:k] + N[k + 1:]
    oset = set(O); pins = 0
    for n, l in enumerate(R):
        m = re.match(r'FABLE_STATIC_ASSERT_AT\((\d+), (.*)$', l)
        if m and l not in oset:
            R[n] = 'FABLE_STATIC_ASSERT(' + m.group(2); pins += 1
    out.append('%s\t%s\tinserted=%d\tpinned=%d' % (X, 'SAME' if R == O else 'DIFF', (j - i + 1) + 1, pins))
(A / 'evidence/hdrcheck.txt').write_text('\n'.join(out) + '\n')
print('\n'.join(out))
