# assemble.py: fin/<rel> = (hand/<rel> or seqout/<rel>) + post/<rel>.py patches, then assert pins re-applied (HEAD's effective
# numbers first, then each folder original's) so every pre-existing FABLE_STATIC_ASSERT keeps its typedef name.
import json, os, sys, shutil
sys.argv = ['seq.py']; exec(open(os.path.dirname(os.path.abspath(__file__)) + '/seq.py').read().split('for rel in sys.argv[1:]:')[0])
OUTD = L + '/fin/'
shutil.rmtree(OUTD, ignore_errors=True)
n = 0
for rel, v in sorted(items.items()):
    if v[0][0] == 'new': continue
    src = L + '/hand/' + rel if os.path.exists(L + '/hand/' + rel) else L + '/seqout/' + rel
    raw = open(src, encoding='latin-1', newline='').read(); text = raw.replace('\r\n', '\n')
    crlf = '\r\n' in open(R + rel, encoding='latin-1', newline='').read()
    pp = L + '/post/' + rel + '.py'
    if os.path.exists(pp):
        ns = {}; exec(open(pp).read(), ns)
        for o, nn in ns['PAIRS']:
            assert text.count(o) == 1, (rel, o[:60]); text = text.replace(o, nn)
    srcs = [effmap(rd(R + rel))] + [effmap(rd(x[3])) for x in v]
    text = repin(text, srcs)
    if crlf: text = text.replace('\n', '\r\n')
    os.makedirs(os.path.dirname(OUTD + rel), exist_ok=True)
    open(OUTD + rel, 'w', encoding='latin-1', newline='').write(text); n += 1
print('assembled', n)
