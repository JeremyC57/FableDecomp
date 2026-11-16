# NEW_ headers: same-as-HEAD -> HEAD; fable_assert.h -> HEAD (wfC1's copy, a superset of the 16 acctC copies);
# newfin/<rel> (hand merges of HEAD + acctC) re-pinned; genuinely new -> the folder copy (all folder copies identical).
import json, os, sys
sys.argv = ['seq.py']; exec(open(os.path.dirname(os.path.abspath(__file__)) + '/seq.py').read().split('for rel in sys.argv[1:]:')[0])
OUTD = L + '/fin/'
for rel, v in sorted(items.items()):
    if v[0][0] != 'new': continue
    texts = set(rd(x[2]) for x in v); assert len(texts) == 1 or rel == 'engine/CEngineInternalPrimitiveMorphedStaticMesh.h', rel
    inhead = os.path.exists(R + rel)
    if os.path.exists(L + '/newfin/' + rel):
        t = repin(rd(L + '/newfin/' + rel), [effmap(rd(R + rel))] + [effmap(rd(x[2])) for x in v]); how = 'merged'
    elif inhead:
        t = rd(R + rel); how = 'HEAD'
        assert rel == 'fable_assert.h' or t == list(texts)[0], rel
    else:
        t = list(texts)[0]; how = 'new'
    crlf = inhead and '\r\n' in open(R + rel, encoding='latin-1', newline='').read()
    if crlf: t = t.replace('\n', '\r\n')
    os.makedirs(os.path.dirname(OUTD + rel), exist_ok=True)
    open(OUTD + rel, 'w', encoding='latin-1', newline='').write(t); print('%-7s %s' % (how, rel))
