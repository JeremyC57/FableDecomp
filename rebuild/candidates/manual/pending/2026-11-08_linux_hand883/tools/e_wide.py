# Broad code-identical spelling family over the whole function body (one edit per occurrence), generated from the differ.
import re, os
SRC = os.environ.get('WIDE_SRC', os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'differ', '006096e0.cpp'))
lines = open(SRC).read().split('\n')
start = next(i for i, l in enumerate(lines) if l.startswith('NUISystem::CComponent* CTCInventoryMap::ConstructLogbookListPC'))
EDITS = {}
seen = {}
def occ(line):
    # occurrence index of this exact line among previous lines of the whole file
    return sum(1 for l in lines[:cur] if l == line)
for cur in range(start, len(lines)):
    l = lines[cur]
    o = occ(l)
    def add(tag, new):
        if new != l: EDITS[f'L{cur+1}{tag}'] = [(l + '\n', new + '\n', o)]
    # pointer tests
    m = re.match(r'^(\s*)if \((\w+)\)( \{)?$', l)
    if m and m.group(2) not in ('basics','objects','towns','hero','combat','quests','story'):
        add('ne0', f'{m.group(1)}if ({m.group(2)} != 0){m.group(3) or ""}')
    if m and m.group(2) in ('basics','objects','towns','hero','combat','quests','story'):
        add('ne0', f'{m.group(1)}if ({m.group(2)} != 0){m.group(3) or ""}')
    if ', false)' in l: add('f0', l.replace(', false)', ', 0)'))
    if 'true, true)' in l: add('t1', l.replace('true, true)', '1, 1)'))
    m = re.match(r'^(\s*)CCountedPointer<(\w+)> (\w+)\((.*)\);$', l)
    if m and m.group(4) and '(' in m.group(4): add('ci', f'{m.group(1)}CCountedPointer<{m.group(2)}> {m.group(3)} = {m.group(4)};')
    if '.Get())' in l: add('data', l.replace('.Get())', '.Data)'))
    m = re.match(r'^(\s*)(\w+)->(\w+)\((.*)$', l)
    if m and m.group(2) in ('entry','subtitleText','detailsText','mesh','icon','model','title','ring','content','booksList'):
        add('get', f'{m.group(1)}{m.group(2)}.Get()->{m.group(3)}({m.group(4)}')
    if '++book' in l: add('pinc', l.replace('++book', 'book++'))
    if '++item' in l: add('pinc', l.replace('++item', 'item++'))
    if '++it)' in l: add('pinc', l.replace('++it)', 'it++)'))
    if l.strip() == 'long index = *book;': add('const', l.replace('long index', 'const long index'))
    if 'NUISystem::UI_TYPE_SPRITE' in l: add('swap', l.replace('icon->GetType() == NUISystem::UI_TYPE_SPRITE', 'NUISystem::UI_TYPE_SPRITE == icon->GetType()'))
# coordinator family (2026-11-09): then-arm statement order / scope / declaration position
SUBC = '            CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));\n'
APC = '            frame->AddPositionChild(subtitle.Get());\n'
DECL = '            CCountedPointer<CText> subtitleText;\n'
SSC = '            subtitleText.SetStaticCast(subtitle);\n'
SV = '            subtitleText->SetValue(description, true, true);\n'
ACH = '            entry->AddChild(subtitle);\n'
EDITS.update({
 'K1': [(SUBC + APC + DECL, DECL + SUBC + APC, 0)],
 'K1e': [(SUBC + APC + DECL, DECL + SUBC + APC, 1)],
 'K1b': [(SUBC + APC + DECL, DECL + SUBC + APC, 0), (SUBC + APC + DECL, DECL + SUBC + APC, 0)],
 'K3': [(SSC + SV + ACH, ACH + SSC + SV, 0)],
 'K4': [(APC + DECL + SSC + SV, DECL + SSC + SV + APC, 0)],
 'K5': [(DECL + SSC + SV, '            {\n' + DECL + SSC + SV + '            }\n', 0)],
 'K6': [(SUBC + APC + DECL + SSC + SV + ACH, SUBC + DECL + SSC + APC + SV + ACH, 0)],
})
