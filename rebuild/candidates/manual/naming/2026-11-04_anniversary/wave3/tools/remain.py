import sys, re, csv, json, struct, collections
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
E = Exe(); R = '/home/user/FableDecomp'
NV = {int(k, 16): v for k, v in json.load(open('named_vts.json')).items()}
J = json.load(open('scan.json'))
rows = {l.split('\t')[0]: l.split('\t')[1] for l in open('labels_alloc.tsv') if not l.startswith('address')}
script_vts = {int(v[0], 16) for v in json.load(open('script_vts.json')).values()}
tmpl_vts = {}
for l in open('labels_alloc.tsv'):
    for m in re.finditer(r'(CSpawnedFunc|CActiveEntityScript)<(\w+)> vtable ([0-9a-f]{8})', l):
        tmpl_vts[int(m.group(3), 16)] = '%s<NScript::%s>' % (m.group(1), m.group(2))
    m = re.search(r'^\S+\tCEntityScriptBinding<NScript::(\w+)>.*?binding vtable ([0-9a-f]{8})', l)
    if m: tmpl_vts[int(m.group(2), 16)] = 'CEntityScriptBinding<NScript::%s>' % m.group(1)
fn = collections.defaultdict(set)  # va -> addresses carrying CVtable_<va>
RX = re.compile(r'CVtable_([0-9a-fA-F]{8})')
for l in open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8'):
    c = l.split('\t')
    for m in RX.finditer(c[1] if len(c) > 1 else ''): fn[int(m.group(1), 16)].add(c[0])
for r in csv.DictReader(open(R + '/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t'):
    for m in RX.finditer(r['module'] + ' ' + r['name']): fn[int(m.group(1), 16)].add(r['address'])
total = len(fn)
binding_vts = {s['binding_vt'] for s in J['sites']}
def shape(va):
    if va in NV: return 'named_entity_vt'
    if va in script_vts: return 'named_script_vt'
    if va in tmpl_vts: return 'named_template_vt'
    if va in binding_vts: return 'binding_vt(CEntityScriptBinding<T>)'
    s = [E.dword(va + 4 * k) for k in range(8)]
    if not s[0] or not (E.tlo <= s[0] < E.thi): return 'not_a_vtable_start'
    if s[1] == 0x0042D930: return 'def_or_resource(CDefClassBase-derived)'
    if s[3] and E.tlo <= s[3] < E.thi and E.read(s[3], 4) == bytes.fromhex('8b4114c3'): return 'entity_shaped_unregistered'
    if s[1:4] == [0x00A44880, 0x00A44840, 0x00A447D0]: return 'suspendable_process(CSpawnedFunc/CActiveEntityScript-like)'
    rm = s[1]
    if rm and E.tlo <= rm < E.thi and struct.pack('<I', 0x012C2F94) in E.read(rm, 0x90): return 'script_shaped(other)'
    if s[4] == 0x00CDD440 or any(x == 0x00CDD440 for x in s): return 'spawned_func?'
    return 'other'
out = collections.Counter(); rem = collections.defaultdict(list)
for va, addrs in fn.items():
    left = {a for a in addrs if a not in rows}
    sh = shape(va)
    status = 'cleared' if not left else 'partial' if len(left) < len(addrs) else 'untouched'
    out[(sh, status)] += 1
    if status != 'cleared': rem[(sh, status)].append(('%08x' % va, sorted(left)[:4], len(left)))
print('placeholder classes', total)
for k, v in sorted(out.items()): print(k, v)
json.dump({'%s|%s' % k: v for k, v in rem.items()}, open('remaining.json', 'w'), indent=0)
