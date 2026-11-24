# mkvar368.py <out dir> <addr...>: the NF368 port text of 0x0060C900 / 0x0060EEB0. The strong same-TU definition of
# CTCInventoryMap::CreateMapMarkers (owner 0x005FF190) is removed; the class declaration in engine/CTCInventoryMap.h (line 239,
# included by both rows) stays, so the leaf's call binds the owner's symbol. The emptied `#pragma auto_inline(off)` / `(on)` pair is
# kept (pragma state as HEAD), with one comment line in place of the body. The file-head comment that said the body must stay is
# rewritten. Everything else is byte-identical to the repo source.
import sys, re
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent)); import nf
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
HEADNOTE = {
    '0060c900': ("// from that matched sibling's text): the same-TU CreateMapMarkers body below must stay (without it DIFFER 9633v9635). The first\n",
                 "// from that matched sibling's text). The first\n"),
    '0060eeb0': (" Pair: the same-TU CreateMapMarkers 0x005FF190 body below must stay visible (without it the frame and colouring change); it is that row's source.\n",
                 " CreateMapMarkers 0x005FF190 is declared only (engine/CTCInventoryMap.h); its landed row defines it.\n"),
}
for a in sys.argv[2:]:
    t = nf.src_of(a).read_text(encoding='utf-8')
    old, new = HEADNOTE[a]
    assert t.count(old) == 1, (a, 'headnote')
    t = t.replace(old, new)
    m = re.search(r'#pragma auto_inline\(off\)\nvoid CTCInventoryMap::CreateMapMarkers\(NUISystem::CComponent\* parent, std::vector<CRegion>& regions\)\n\{\n.*?\n\}\n#pragma auto_inline\(on\)\n', t, re.S)
    assert m and t.count('void CTCInventoryMap::CreateMapMarkers(') == 1, (a, 'body')
    t = t[:m.start()] + ('#pragma auto_inline(off)\n'
                         '// (NF368) CTCInventoryMap::CreateMapMarkers 0x005FF190: declared in engine/CTCInventoryMap.h, defined by its landed row (no same-TU copy).\n'
                         '#pragma auto_inline(on)\n') + t[m.end():]
    (out / (a + '.cpp')).write_text(t, encoding='utf-8')
    print(a, 'written', len(t))
# (control, `L` mode: env NF368_KEEPLINES=1) pad the removed body with comment lines so every later line keeps its HEAD number
# (rules out a __LINE__ effect on the leaf). Applied after the fact to the written file:
import os
if os.environ.get('NF368_KEEPLINES'):
    for a in sys.argv[2:]:
        h = nf.src_of(a).read_text(encoding='utf-8'); t = (out / (a + '.cpp')).read_text(encoding='utf-8')
        pad = h.count('\n') - t.count('\n')
        mark = '// (NF368) CTCInventoryMap::CreateMapMarkers 0x005FF190: declared in engine/CTCInventoryMap.h, defined by its landed row (no same-TU copy).\n'
        t = t.replace(mark, mark + '//\n' * pad)
        assert t.count('\n') == h.count('\n'); (out / (a + '.cpp')).write_text(t, encoding='utf-8'); print(a, 'padded', pad)
