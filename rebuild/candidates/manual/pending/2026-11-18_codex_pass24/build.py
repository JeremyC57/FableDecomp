#!/usr/bin/env python3
"""Build the Codex hand-off package pass 24, 2026-11-18: 100 parked near misses that Ego_d.pdb (the TLC debug build's PDB) can help --
35 the Ego_d lane already narrowed to one residue and 65 more whose best draft is within 0-3 bytes of retail's size.
No retail bytes are written into the package (the exe is read only to measure extents and find callers). Ego_d.pdb is read (if
EGOD_PDB is set) only to write each row's named locals and TU list; the PDB itself is not copied (Codex downloads it, README_CODEX.md)."""
import csv, glob, os, re, shutil, struct, subprocess, bisect, sys, collections
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'
S = os.environ['S']
HERE = os.path.dirname(os.path.abspath(__file__))
NAME = 'fabledecomp_codex_pass24_2026-11-18'
OUT = f'{S}/codex24/{NAME}'
P = f'{R}/rebuild/candidates/manual'
PDB = os.environ.get('EGOD_PDB', ''); TYP = os.environ.get('EGOD_TYPES', '')
if os.path.exists(OUT): shutil.rmtree(OUT)
os.makedirs(OUT)
EXE = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', EXE, 0x3c)[0]; nsec = struct.unpack_from('<H', EXE, pe + 6)[0]; optsz = struct.unpack_from('<H', EXE, pe + 20)[0]
SECS = [struct.unpack_from('<IIII', EXE, pe + 24 + optsz + 40 * i + 8) for i in range(nsec)]

man = {r['address'].lower(): r for r in csv.DictReader(open(f'{R}/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t')}
cat = {r['address']: (r['name'], r['length']) for r in csv.DictReader(open(f'{R}/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t')}
SELF = f'{HERE}/rows.tsv'   # fixed selection (address, retail length, kind, last grade, best draft)
sel = [l.rstrip('\n').split('\t') for l in open(SELF)]
for s in sel: assert s[0] not in cat, s[0] + ' already landed'

t = open(f'{R}/rebuild/build_candidates.ps1', encoding='utf-8-sig').read()
ent = {}
for blk in re.split(r'\[pscustomobject\]@\{', t)[1:]:
    kv = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", blk))
    if 'Address' in kv and 'Source' in kv: ent[kv['Address'].lower()] = kv
landed_sorted = sorted(a for a in ent if not a.startswith('012'))
XR = {}
def build_xref(targets):
    import collections
    tv = set(int(a, 16) for a in targets); res = collections.defaultdict(list)
    for i in range(nsec):
        o = pe + 24 + optsz + 40 * i; name = EXE[o:o + 8].rstrip(b"\0").decode()
        vsz, rva, rsz, raw = struct.unpack_from('<IIII', EXE, o + 8); blob = EXE[raw:raw + rsz]; b0 = 0x400000 + rva
        if name == '.text':
            for op, kind in ((0xE8, 'call'), (0xE9, 'jmp')):
                j = blob.find(bytes([op]))
                while j >= 0 and j + 5 <= len(blob):
                    t_ = b0 + j + 5 + struct.unpack_from('<i', blob, j + 1)[0]
                    if t_ in tv: res[t_].append('%s %08x' % (kind, b0 + j))
                    j = blob.find(bytes([op]), j + 1)
        for v in tv:
            pat = struct.pack('<I', v); j = blob.find(pat)
            while j >= 0:
                res[v].append('abs %s %08x' % (name, b0 + j)); j = blob.find(pat, j + 1)
    for a in targets: XR[a] = ' | '.join(res[int(a, 16)][:30])
def xref(a): return XR[a]
def containing_landed(va):
    i = bisect.bisect_right(landed_sorted, '%08x' % va) - 1
    if i < 0: return None
    a = landed_sorted[i]; ln = int(cat.get(a, ('', '0'))[1] or 0)
    return a if int(a, 16) <= va < int(a, 16) + max(ln, 1) else None
build_xref([s[0] for s in sel])
EGN = {r['address']: r for r in csv.DictReader(open(f'{P}/pending/2026-11-17_egod_parked/EGOD_NAMED.tsv'), delimiter='\t')}
EGS = {r['address']: r for r in csv.DictReader(open(f'{P}/pending/2026-11-17_egod_parked/STATUS.tsv'), delimiter='\t')}
PIX = {r['addr']: r for r in csv.DictReader(open(f'{P}/PARKED_INDEX.tsv'), delimiter='\t')}
catn = collections.defaultdict(list)
for a_, (n_, l_) in cat.items(): catn[n_].append(a_)
funcs = collections.defaultdict(list)
for l in open(f'{R}/ghidra_out/egod_carved_functions.tsv'):
    if l.startswith('#'): continue
    o_, off, ln_, k_, n_ = l.rstrip('\n').split('\t', 4); funcs[o_.split('\\')[-1].lower()].append((int(off), int(ln_), n_))
def egod_tu(objs, target):
    out = []
    for ob in objs:
        fl = sorted(funcs.get(ob.split('\\')[-1].lower(), []))
        out.append(f"== {ob}: {len(fl)} functions in Ego_d object order (debug length; retail address(es) when the landed catalog has the name)")
        for off, ln_, n_ in fl:
            hits = catn.get(n_) or catn.get(n_.split('<')[0]) or []
            out.append(f"{'>>' if n_ == target else '  '} {ln_:6d}  {n_}" + (f"   retail {' '.join(sorted(hits)[:4])} (landed)" if hits else ""))
    return '\n'.join(out) + '\n'
rowsout = ['address_va\tfile_offset\tfile_offset_end\tlength\tkind\tmodule\tname\tbest_known_identity\tresidue_or_note']
reserved = ['address\tlength_estimate\tcatalog_module\tcatalog_name\tkind\tclosest_version\tnote']
for a, ln, kind, grade, path in sel:
    va = int(a, 16); ln = int(ln); m = man[a]
    d = f'{OUT}/rows/{a}'; os.makedirs(d)
    ident = m['name'] if m['name'].startswith(m['module'] + '::') or m['module'].startswith('_') else f"{m['module']}::{m['name']}"
    note = f'last recorded grade: {grade[:300]} -- best draft {path}' 
    rowsout.append('\t'.join([f'0x{va:08X}', f'0x{va - 0x400000:08X}', f'0x{va - 0x400000 + ln:08X}', str(ln), kind, m['module'], m['name'], ident, note]))
    reserved.append('\t'.join([a, str(ln), m['module'], m['name'], kind, path, grade or 'no draft']))
    xr = xref(a); callers = []
    for g in re.findall(r'(call|jmp|abs \S+) ([0-9a-f]{8})', xr):
        c = containing_landed(int(g[1], 16))
        callers.append(f"- {g[0]} at 0x{g[1].upper()} " + (f"in landed 0x{c.upper()} {cat[c][0]} ({ent[c]['Source']})" if c else ('(a data reference: in .rdata usually a vtable slot, see names/rtti-vtables.tsv; in .text an EH or jump table)' if g[0].startswith('abs') else '(the caller is not landed)')))
    i = bisect.bisect_left(landed_sorted, a); neigh = landed_sorted[max(0, i - 3):i + 3]
    os.makedirs(f'{d}/neighbours'); nl = []
    for n in neigh:
        src = f"{R}/rebuild/src/compiled/{ent[n]['Source']}"
        if os.path.exists(src):
            shutil.copy(src, f'{d}/neighbours/{os.path.basename(src)}')
            nl.append(f"- 0x{n.upper()} {cat.get(n, ('?',))[0]} -> neighbours/{os.path.basename(src)}" + (f" (flags: {ent[n]['CompilerFlags']})" if ent[n].get('CompilerFlags') else ''))
    pl = []
    files = sorted(glob.glob(f'{P}/pending/**/{a}*', recursive=True))
    if files: os.makedirs(f'{d}/prior')
    for f in files:
        if os.path.isdir(f) or not f.endswith(('.cpp', '.h', '.md')): continue
        rel = f.split('/pending/')[1].split('/'); hb = rel[0]; sub = '_'.join(rel[1:-1])
        dst = f"{hb}__{sub + '__' if sub else ''}{rel[-1]}"
        shutil.copy(f, f'{d}/prior/{dst}'); pl.append(f'- prior/{dst}')
    shutil.copy(f'{R}/{path}', f'{d}/best_2026-11-18.cpp')
    hb = path.split('/pending/')[1].split('/')[0]; fold = os.path.dirname(f'{R}/{path}')
    hf = []
    for p_ in sorted(set(glob.glob(f'{P}/pending/{hb}/*.md') + glob.glob(f'{P}/pending/{hb}/STATUS.tsv') + glob.glob(f'{fold}/*.md'))):
        if os.path.getsize(p_) < 400000:
            dst = 'handback_' + os.path.relpath(p_, f'{P}/pending/{hb}').replace('/', '__'); shutil.copy(p_, f'{d}/{dst}'); hf.append(dst)
    # two Ego_d-lane rows from its IDENTIFY list (made-up catalog names; the lane identified them): name and object from the lane's ledger
    OVR = {'0099f7a0': ('operator+', 'BBBLibrary\\Debug\\lib_char_string.obj', False),   # ??H@YI?AVCCharString@@ABV0@D@Z; several operator+ records: locals not extracted
           '009f18f0': ('CGameEventDataPackage::GetData', 'BBBLibrary\\Debug\\lib_game_event.obj', True)}
    E = EGN.get(a, {}); en = E.get('egod_name', ''); objs = [x for x in E.get('egod_objects', '').split(' | ') if x]
    if a in OVR: en, objs, E = OVR[a][0], [OVR[a][1]], {'egod_records_same_name': 'identified by the Ego_d lane;', 'egod_debug_length': '?'}
    if objs: open(f'{d}/egod_tu.txt', 'w').write(egod_tu(objs, en))
    if PDB and en and OVR.get(a, (0, 0, True))[2]:
        raw = subprocess.run(['python3', '-I', f'{HERE}/tools/egod_locals.py', PDB, en], capture_output=True, text=True).stdout
        open(f'{d}/egod_locals.raw.txt', 'w').write(raw)
        if TYP:
            rl = subprocess.run(['python3', '-I', f'{HERE}/tools/egod_type.py', TYP, '--locals', f'{d}/egod_locals.raw.txt'], capture_output=True, text=True).stdout
            open(f'{d}/egod_locals.txt', 'w').write(rl); os.remove(f'{d}/egod_locals.raw.txt')
    px = PIX.get(a, {})
    with open(f'{d}/ROW.md', 'w') as o:
        o.write(f"# 0x{va:08X} -- {ident}\n\n")
        o.write(f"- Kind: **{kind}**. Manifest label: `{m['module']}::{m['name']}`. Ego_d name: `{en}` ({E.get('egod_records_same_name', '?')} record(s) with that name; "
                f"debug length {E.get('egod_debug_length', '?')}; object {' | '.join(objs) or '?'}).\n")
        o.write(f"- Retail extent: VA 0x{va:08X}, {ln} bytes (file offset 0x{va - 0x400000:08X} .. 0x{va - 0x400000 + ln:08X}); the extent scan agrees. "
                f"Cut it with `python3 tools/cut_row.py Fable.exe 0x{va:08X} {ln} --dis`.\n")
        if kind == 'egod_parked':
            o.write(f"- Status: worked on 2026-11-17 by the Ego_d lane WITH Ego_d's locals and TU; parked one residue short. Its verdict: {grade}\n")
        else:
            o.write(f"- Status: parked before Ego_d existed. Best grade on record: {grade[:400]}\n")
        if px.get('residue_class') or px.get('residue'):
            o.write(f"- Parked-index residue class: `{px.get('residue_class') or '-'}`; diagnosis: {(px.get('residue') or '')[:600]}\n")
        o.write(f"- Best source so far: `best_2026-11-18.cpp` (copied from `{path}`; regrade it first on the package headers). Notes of the folder it came from: "
                f"{', '.join('`' + x + '`' for x in hf) or 'none'} (search them for {a}). Earlier drafts and tests are in `prior/`. Do not repeat a recorded dead end.\n")
        o.write("\n## Ego_d evidence (read README_CODEX.md \"Ego_d.pdb\" first)\n")
        o.write("- `egod_locals.txt`: the source's own parameter and local names, their types and scope blocks (S_BLOCK32 = a `{ }` the source declared locals in), and "
                "function-local statics. Use the names and the scope nesting; the frame offsets are VC10 /Od, not retail. No local listed = the source wrote one expression.\n" if os.path.exists(f'{d}/egod_locals.txt') else "- no locals record extracted\n")
        o.write("- `egod_tu.txt`: every function of the row's object file in Ego_d's object order (`>>` marks this row), with the retail address of those already landed. "
                "Same-TU callees defined in the row in that order closed most of the Ego_d lane's matches; landed ones show which includes / opt-ins that TU used.\n" if objs else "")
        o.write("\n## Callers / references (from the retail exe)\n" + ('\n'.join(callers) if callers else '- none found') + "\n")
        o.write("\n## Landed neighbours (same address range, usually the same translation unit: reuse their models, includes, pragmas)\n" + '\n'.join(nl) + "\n")
        if pl: o.write("\n## Earlier files on record for this address\n" + '\n'.join(pl) + "\n")
open(f'{OUT}/ROWS.tsv', 'w').write('\n'.join(rowsout) + '\n')
open(f'{HERE}/RESERVED.tsv', 'w').write('\n'.join(reserved) + '\n')

def cp(src, dst):
    if os.path.exists(src): os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copy(src, dst)
    else: print('MISSING', src)
# docs: rules, compiler research, and every document on fixes that made rows match
LT = 'tools/decomp_pipeline/linux_lane/lane_tools'
for p in ['docs/pipeline/EXTERNAL_AI_HANDOFF.md', 'docs/BUILDING.md', 'docs/ARCHITECTURE.md', 'docs/pipeline/GOTCHAS.md', 'docs/pipeline/MATCHING_PLAYBOOK.md',
          'docs/pipeline/C2_ALLOCATOR_PROBE.md', 'docs/pipeline/ASM_RULES.md', 'docs/pipeline/INLINE_DEPTH_REVISIT.md', 'docs/pipeline/VC71_CODEGEN_RESEARCH.md',
          'docs/pipeline/REGISTER_SWAP_SUMMARY.md', 'docs/pipeline/SYMBOLS.md', 'docs/pipeline/NAMING_STRAGGLERS.md', 'docs/pipeline/QFE4035_COMPILER_GATE.md',
          'docs/pipeline/LIBRARY_SIGNATURES.md', 'docs/pipeline/LIBRARY_LINKING.md', 'docs/pipeline/METHODOLOGY.md', 'docs/pipeline/DECOMP_ACCELERATORS.md',
          'docs/pipeline/MANIFEST_GAP_RECOVERY.md', 'docs/HANDOFF.md', 'docs/ROADMAP.md', 'CONTRIBUTING.md', f'{LT}/chunk_pool/EXTRA_RULES.md']:
    cp(f'{R}/{p}', f'{OUT}/docs/{os.path.basename(p)}')
cp(f'{R}/CLAUDE.md', f'{OUT}/docs/PROJECT_RULES_CLAUDE.md')
cp(f'{R}/{LT}/lane_merge/README.md', f'{OUT}/docs/HEADER_MERGE_AND_PROOF.md')
cp(f'{P}/pending/2026-11-13_acctC/README.md', f'{OUT}/docs/HANDBACK_RULES_FROM_LANDING_FY.md')
cp(f'{P}/UNMATCHED_ROWS.tsv', f'{OUT}/docs/UNMATCHED_ROWS.tsv')
cp(f'{P}/PARKED_INDEX.tsv', f'{OUT}/docs/PARKED_INDEX.tsv')
for p in ('pin_asserts.py', 'equiv_check.py'): cp(f'{R}/{LT}/lane_merge/{p}', f'{OUT}/tools/lane_merge/{p}')
# lessons: every hand-back LESSONS / NOTES since 2026-11-10, every landing README since 2026-11-11, the naming passes, the Codex returns
n = 0
for hb in sorted(os.listdir(f'{P}/pending')):
    if hb < '2026-11-10': continue
    for fn in ('LESSONS.md', 'NOTES.md', 'VERIFIER_FINDINGS.md'):
        p = f'{P}/pending/{hb}/{fn}'
        if os.path.exists(p) and os.path.getsize(p) > 0: cp(p, f"{OUT}/lessons/handbacks/{hb}__{fn}"); n += 1
for b in sorted(glob.glob(f'{P}/batches/2026-11-1[1-8]_landing_*/README.md')): cp(b, f"{OUT}/lessons/landings/{b.split('/')[-2]}.md")
for d_ in sorted(glob.glob(f'{P}/naming/2026-11-*')):
    for fn in ('README.md', 'NOTES.md', 'LESSONS.md'):
        if os.path.exists(f'{d_}/{fn}'): cp(f'{d_}/{fn}', f"{OUT}/lessons/naming/{os.path.basename(d_)}__{fn}")
for ps, sub in (('2026-11-09_codex_pass15', 'return_2026-11-10'), ('2026-11-10_codex_pass16', 'return_2026-11-10'), ('2026-11-10_codex_pass17', 'return_2026-11-10'),
                ('2026-11-10_codex_pass18', 'return_2026-11-10'), ('2026-11-10_codex_pass19', 'return_2026-11-10'), ('2026-11-10_codex_pass20', 'return'),
                ('2026-11-11_codex_pass21', 'return')):
    for f in glob.glob(f'{P}/pending/{ps}/{sub}/*.md') + glob.glob(f'{P}/pending/{ps}/{sub}/STATUS.tsv'):
        cp(f, f"{OUT}/lessons/codex/{ps.split('_')[-1]}__{os.path.basename(f)}")
for f in glob.glob(f'{P}/pending/2026-11-14_codex_pass22/return/*.md') + [f'{P}/pending/2026-11-14_codex_pass22/return/STATUS.tsv']:
    cp(f, f"{OUT}/lessons/codex/pass22__{os.path.basename(f)}")
for f in glob.glob(f'{P}/pending/2026-11-14_codex_pass22/return/parked/agentP6/*.md') + glob.glob(f'{P}/pending/2026-11-14_codex_pass22/return/parked/agentP6/fable_session/*.md'):
    cp(f, f"{OUT}/lessons/codex/pass22_after__{f.split('/agentP6/')[1].replace('/', '__')}")
for f in glob.glob(f'{P}/pending/2026-11-15_codex_pass23/return/*.md') + [f'{P}/pending/2026-11-15_codex_pass23/return/STATUS.tsv', f'{P}/pending/2026-11-15_codex_pass23/README.md']:
    cp(f, f"{OUT}/lessons/codex/pass23__{os.path.basename(f)}")
# Ego_d: the findings, the lane's brief, lessons and every batch's notes (the 2026-11-17 lane that matched 68 rows with it)
cp(f'{R}/docs/journal/2026-11/EGOD_FINDINGS.md', f'{OUT}/egod/EGOD_FINDINGS.md')
for fn in ('README.md', 'EGOD_NAMED.tsv', 'EGOD_IDENTIFY.tsv', 'STATUS.tsv'): cp(f'{P}/pending/2026-11-17_egod_parked/{fn}', f'{OUT}/egod/lane_{fn}')
for fn in ('LESSONS_EGOD.md', 'BRIEF_TEMPLATE.md'): cp(f'{P}/pending/2026-11-17_egod_parked/lane/{fn}', f'{OUT}/egod/{fn}')
cp(f'{P}/batches/2026-11-18_landing_egod/README.md', f'{OUT}/egod/LANDING_EGOD_README.md')
for f in sorted(glob.glob(f'{P}/pending/2026-11-17_egod_e*/*.md') + glob.glob(f'{P}/pending/2026-11-17_egod_open/*/*.md')):
    cp(f, f"{OUT}/egod/batches/{f.split('/pending/')[1].replace('/', '__')}")
for p in ('egod_carved_functions.tsv', 'egod_carved_names.tsv'): cp(f'{R}/ghidra_out/{p}', f'{OUT}/names/{p}')
cp(f'{P}/pending/2026-11-13_codex_released/README.md', f'{OUT}/lessons/codex/released_2026-11-13__README.md')
cp(f'{P}/pending/2026-11-13_codex_released/STATUS.tsv', f'{OUT}/lessons/codex/released_2026-11-13__STATUS.tsv')
shutil.copytree(f'{P}/pending/2026-11-13_acctB_parked/notes', f'{OUT}/lessons/acctB_parked_notes', ignore=lambda d, n: [x for x in n if not (x.endswith(('.md', '.tsv', '.cpp', '.h')) or os.path.isdir(os.path.join(d, x)))])
cp(f'{P}/pending/2026-11-13_acctB_parked/README.md', f'{OUT}/lessons/acctB_parked_notes/README.md')
# headers
shutil.copytree(f'{R}/rebuild/include', f'{OUT}/include')
# names
for p in ['rebuild/oracles/reloc_bindings.tsv', 'ghidra_out/egor_pdb_names.tsv', 'ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egod_pdb_names.tsv',
          'ghidra_out/struct_layouts_egor.tsv', 'ghidra_out/pdb_class_keys.tsv', 'rebuild/manifest/rtti-vtables.tsv',
          'rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv', 'rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv',
          'ghidra_out/egod_pdb_func_names.tsv', 'ghidra_out/fable_types.h', 'ghidra_out/naming_stragglers/Fable.exe.signatures.tsv',
          'ghidra_out/naming_stragglers/ego_r.exe.signatures.tsv', 'rebuild/candidates/manual/BOUNDS_FIX.tsv', 'refs/fse_api_manifest.json']:
    cp(f'{R}/{p}', f'{OUT}/names/{os.path.basename(p)}')
with open(f'{OUT}/names/catalog_names.tsv', 'w') as o:
    o.write('address\tname\tlength\n')
    for a in sorted(cat): o.write(f'{a}\t{cat[a][0]}\t{cat[a][1]}\n')
with open(f'{OUT}/names/manifest_names.tsv', 'w') as o:
    o.write('address\tmodule\tname\n')
    for a in sorted(man): o.write(f"{a}\t{man[a]['module']}\t{man[a]['name']}\n")
with open(f'{OUT}/names/landed_sources.tsv', 'w') as o:
    o.write('address\tsource\tflags\n')
    for a in sorted(ent): o.write(f"{a}\t{ent[a]['Source']}\t{ent[a].get('CompilerFlags', '')}\n")
for p in glob.glob(f'{R}/ghidra_out/labels_*.tsv'): cp(p, f'{OUT}/names/labels/{os.path.basename(p)}')
for p in ('cut_row.py', 'xref.py', 'egod_locals.py', 'egod_type.py'): cp(f'{HERE}/tools/{p}', f'{OUT}/tools/{p}')
LINK = os.environ.get('EGOD_PDB_LINK', '(ask the maintainer for the Ego_d.pdb link)')
for p in ('PROMPT_CODEX.md', 'README_CODEX.md'): open(f'{OUT}/{p}', 'w').write(open(f'{HERE}/{p}').read().replace('@@EGOD_PDB_LINK@@', LINK))
print('rows', len(rowsout) - 1, 'handback lesson files', n)
