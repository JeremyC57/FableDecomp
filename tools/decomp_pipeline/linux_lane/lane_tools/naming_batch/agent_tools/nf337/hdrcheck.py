# hdrcheck.py (NF337): for every hdr/engine/<X>.h: drop the inserted `#ifdef FABLE_RETAIL_<X>_COPY_DECLS` ... `#endif` block, undo the
# FABLE_STATIC_ASSERT_AT pins that orig/hdr_<X>.h lacks, and compare with orig/hdr_<X>.h byte for byte (SAME = the edit is add-only).
import re, glob, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF337'
for f in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(f)[:-2]; N = open(f).read().split('\n'); O = open(A + '/orig/hdr_%s.h' % X).read()
    i = N.index('#ifdef FABLE_RETAIL_%s_COPY_DECLS' % X.upper()); j = next(k for k in range(i, len(N)) if N[k] == '#endif')
    ins = j - i + 1; R = N[:i] + N[j + 1:]; oset = set(O.split('\n')); pins = 0
    for n, l in enumerate(R):
        m = re.match(r'FABLE_STATIC_ASSERT_AT\((\d+), (.*)$', l)
        if m and l not in oset: R[n] = 'FABLE_STATIC_ASSERT(' + m.group(2); pins += 1
    print('%-42s %s inserted=%d pinned=%d nested_if=%d' % (X, 'SAME' if '\n'.join(R) == O else 'DIFF', ins, pins, sum(1 for l in N[i + 1:j] if l.startswith('#'))))
