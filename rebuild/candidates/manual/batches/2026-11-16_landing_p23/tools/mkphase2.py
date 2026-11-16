# mkphase2.py <out.json> [--inline-for addr,...]: pass-23 funclet rows as wrappers that #include their landed parent (landing gb shape).
import json, re, sys, csv, os
S = os.environ['S']; X = S + '/codex_x'
d = json.load(open(S + '/p23l/payload.json'))['result']['authored']
st = {r['address'][2:].lower(): r for r in csv.DictReader(open(X + '/STATUS.tsv'), delimiter='\t')}
ps = open('rebuild/build_candidates.ps1', encoding='utf-8').read()
src_of = {m.group(1).lower(): m.group(2) for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'\s+Module = '[^']*'\s+Source = '([^']+)'", ps)}
inline = set(sys.argv[3].split(',')) if len(sys.argv) > 3 and sys.argv[2] == '--inline-for' else set()
out = []; miss = []
for e in d:
    if e.get('funclet_offset') is None: continue
    a = e['address']; rs = st[a]['real_start'][2:].lower()
    if a in inline: out.append(e); continue
    if rs not in src_of: miss.append((a, rs)); continue
    own = open(X + '/rows/%s/%s.cpp' % (a, a), encoding='utf-8').read().split('\n')
    leaf = e['leaf']; flags = ' '.join(e['flags']) or 'none'
    title = own[0][3:].split(' parent')[0].strip()
    hdr = ['// EH funclet @ 0x%s of %s (parent @ 0x%s): VC7.1 emits the unwind funclets and the __ehhandler thunk of an' % (a.upper(), title.split(' ', 1)[1] if ' ' in title else title, rs.upper()),
           '// EH-framed function as one block after it; the linker placed Fable\'s blocks at 0x0121xxxx. This row has no code of its own:',
           '// it is produced by the parent\'s translation unit, included below (Codex pass 23 proved the parent, its EH data and this block).',
           '// Leaf: ' + leaf, '// Flags: ' + flags,
           '#include "../src/compiled/%s"' % src_of[rs], '']
    e2 = dict(e); e2['source_cpp'] = '\n'.join(hdr); out.append(e2)
json.dump({'result': {'authored': out}}, open(sys.argv[1], 'w'), indent=1)
print(len(out), 'funclet entries; parents not landed:', miss)
