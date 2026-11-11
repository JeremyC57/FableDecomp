#!/usr/bin/env python3
"""Build the Codex hand-off package pass 21, 2026-11-11 (the 40 longest parked rows open for an outside pass). No retail bytes are written into the package."""
import csv, glob, os, re, shutil, struct, subprocess, collections, bisect
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'
S = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
OUT = f'{S}/codex1121/fabledecomp_codex_pass21_2026-11-11'
if os.path.exists(OUT): shutil.rmtree(OUT)
os.makedirs(OUT)
UNWORKED = []

sel = [l.rstrip('\n').split('\t') for l in open(f'{S}/pass21_rows.tsv')]
res = {}
for a, ln, nm, path in sel:
    src = open(f'{R}/{path}', encoding='utf-8', errors='replace').read().split('\n')
    g = next((l[3:].strip() for l in src[:6] if l.startswith('// Grade') or 'DIFFER' in l), 'see the draft head and the hand-back notes')
    res[a] = [a, g[:300], ln, '', path]
cands = {}
PARKED = [s[0] for s in sel]
man = {r['address']: r for r in csv.DictReader(open(f'{R}/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t')}
cat = {}
for r in csv.DictReader(open(f'{R}/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t'):
    cat[r['address']] = (r['name'], r['length'])
for a in PARKED: assert a not in cat, a + ' already landed'
t = open(f'{R}/rebuild/build_candidates.ps1', encoding='utf-8-sig').read()
ent = {}
for blk in re.split(r'\[pscustomobject\]@\{', t)[1:]:
    kv = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", blk))
    if 'Address' in kv and 'Source' in kv: ent[kv['Address'].lower()] = kv
landed_sorted = sorted(a for a in ent if not a.startswith('012'))
LEN = {s[0]: int(s[1]) for s in sel}
def length_of(a): return LEN[a]
def xref(a):
    p = subprocess.run(['python3', f'{R}/tools/decomp_pipeline/linux_lane/lane_tools/xref.py', '0x' + a], capture_output=True,
                       text=True, env=dict(os.environ, S=S), timeout=300)
    return p.stdout.strip()
def containing_landed(va):
    i = bisect.bisect_right(landed_sorted, '%08x' % va) - 1
    if i < 0: return None
    a = landed_sorted[i]
    ln = int(cat.get(a, ('', '0'))[1] or 0)
    return a if int(a, 16) <= va < int(a, 16) + max(ln, 1) else None
st895 = {}
rowsout = ['address_va\tfile_offset\tfile_offset_end\tlength\tkind\tmodule\tname\tbest_known_identity\tresidue_or_note']
for kind, L in (('unworked', UNWORKED), ('parked', PARKED)):
    for a in L:
        va = int(a, 16); ln = length_of(a); m = man[a]
        d = f'{OUT}/rows/{a}'; os.makedirs(d)
        ident = st895[a]['name'] if a in st895 else (m['name'] if m['name'].startswith(m['module'] + '::') else f"{m['module']}::{m['name']}")
        note = 'never attempted'
        r = res.get(a)
        if r: note = f"last recorded grade: {r[1]} -- best draft {r[4]}"
        rowsout.append('\t'.join([f'0x{va:08X}', f'0x{va - 0x400000:08X}', f'0x{va - 0x400000 + ln:08X}', str(ln), kind, m['module'],
                                  m['name'], ident, note.replace('\t', ' ')]))
        # callers, annotated with the landed function each call sits in
        xr = xref(a)
        sites = re.findall(r'call ([0-9a-f]{8})|jmp ([0-9a-f]{8})|ref ([0-9a-f]{8})', xr)
        callers = []
        for g in sites:
            s = next(x for x in g if x); c = containing_landed(int(s, 16))
            callers.append(f"- 0x{s.upper()} " + (f"in landed 0x{c.upper()} {cat[c][0]} ({ent[c]['Source']})" if c else '(not inside a landed row)'))
        # neighbours: the 3 landed rows before and after (same address range, usually the same TU)
        i = bisect.bisect_left(landed_sorted, a)
        neigh = landed_sorted[max(0, i - 3):i] + landed_sorted[i:i + 3]
        os.makedirs(f'{d}/neighbours')
        nl = []
        for n in neigh:
            src = f"{R}/rebuild/src/compiled/{ent[n]['Source']}"
            if os.path.exists(src):
                shutil.copy(src, f'{d}/neighbours/{os.path.basename(src)}')
                nl.append(f"- 0x{n.upper()} {cat.get(n, ('?',))[0]} -> neighbours/{os.path.basename(src)}" +
                          (f" (flags: {ent[n]['CompilerFlags']})" if ent[n].get('CompilerFlags') else ''))
        # parked: every earlier draft (+ tests, header overlays)
        pl = []
        if kind == 'parked':
            os.makedirs(f'{d}/prior')
            for f in sorted(glob.glob(f'{R}/rebuild/candidates/manual/pending/**/{a}*.cpp', recursive=True)):
                hb = f.split('/pending/')[1].split('/')[0]
                sub = '/'.join(f.split('/pending/')[1].split('/')[1:-1])
                dst = f"{hb}__{sub.replace('/', '_') + '__' if sub else ''}{os.path.basename(f)}"
                shutil.copy(f, f'{d}/prior/{dst}'); pl.append(f'- prior/{dst}')
                hd = os.path.join(f'{R}/rebuild/candidates/manual/pending', hb, 'hdr')
                if os.path.isdir(hd) and not os.path.exists(f'{d}/prior/{hb}__hdr'):
                    shutil.copytree(hd, f'{d}/prior/{hb}__hdr'); pl.append(f'- prior/{hb}__hdr/ (that hand-back\'s header overlay)')
            shutil.copy(f"{R}/{res[a][4]}", f'{d}/best_2026-11-11.cpp')
            hb = res[a][4].split('/pending/')[1].split('/')[0]
            for fn in ('NOTES.md', 'LESSONS.md', 'STATUS.tsv'):
                p_ = f'{R}/rebuild/candidates/manual/pending/{hb}/{fn}'
                if os.path.exists(p_): shutil.copy(p_, f'{d}/handback_{fn}')
        with open(f'{d}/ROW.md', 'w') as o:
            o.write(f"# 0x{va:08X} -- {ident}\n\n")
            o.write(f"- Kind: **{kind}**. Manifest label: `{m['module']}::{m['name']}` (a hint; often wrong).\n")
            o.write(f"- Retail extent: VA 0x{va:08X}, {ln} bytes (file offset 0x{va - 0x400000:08X} .. 0x{va - 0x400000 + ln:08X}, "
                    f"trailing int3/nop padding excluded). Cut it with `python3 tools/cut_row.py Fable.exe 0x{va:08X} {ln} --dis`.\n")
            if kind == 'parked':
                o.write(f"- Status: {note}\n")
                o.write(f"- Best source so far: `best_2026-11-11.cpp` (copied as it sits in the repo; the grade above is the last one recorded in the draft or its hand-back -- regrade it first). Every earlier draft is in `prior/`; "
                        f"the hand-back notes of the best draft's folder are `handback_NOTES.md` / `handback_LESSONS.md` / `handback_STATUS.tsv` "
                        f"(search them for 0x{va:08X}). Do not repeat a recorded dead end.\n")
            else:
                o.write("- Status: never attempted by anyone. Settle the identity first (callers below, vtable slots in "
                        "`names/rtti-vtables.tsv`, the PDB name sets).\n")
            o.write("\n## Callers / references (from the retail exe)\n" + ('\n'.join(callers) if callers else '- none found (virtual: look the address up in names/rtti-vtables.tsv)') + "\n")
            o.write("\n## Landed neighbours (same address range, usually the same translation unit: reuse their models, includes, pragmas)\n" + '\n'.join(nl) + "\n")
            if pl: o.write("\n## Earlier drafts\n" + '\n'.join(pl) + "\n")
open(f'{OUT}/ROWS.tsv', 'w').write('\n'.join(rowsout) + '\n')

# docs
DOCS = ['docs/pipeline/EXTERNAL_AI_HANDOFF.md', 'docs/BUILDING.md', 'docs/ARCHITECTURE.md', 'docs/pipeline/GOTCHAS.md',
        'docs/pipeline/MATCHING_PLAYBOOK.md', 'docs/pipeline/C2_ALLOCATOR_PROBE.md', 'docs/pipeline/ASM_RULES.md',
        'docs/pipeline/INLINE_DEPTH_REVISIT.md', 'docs/pipeline/VC71_CODEGEN_RESEARCH.md', 'docs/pipeline/REGISTER_SWAP_SUMMARY.md',
        'docs/pipeline/SYMBOLS.md', 'docs/pipeline/NAMING_STRAGGLERS.md', 'docs/pipeline/QFE4035_COMPILER_GATE.md', 'docs/pipeline/LIBRARY_SIGNATURES.md', 'docs/HANDOFF.md',
        'tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md']
os.makedirs(f'{OUT}/docs')
for p in DOCS: shutil.copy(f'{R}/{p}', f'{OUT}/docs/{os.path.basename(p)}')
# lessons: the newest hand-backs + landing READMEs
os.makedirs(f'{OUT}/lessons')
for hb in ['2026-11-11_linux_handH40','2026-11-11_linux_handH40b','2026-11-11_linux_handH40c','2026-11-11_linux_handH40d','2026-11-10_linux_hand1020','2026-11-10_linux_hand1021','2026-11-10_linux_hand1022','2026-11-11_linux_hand1023','2026-11-11_linux_hand1024','2026-11-11_linux_hand1025','2026-11-11_linux_hand1026']:
    for fn in ('LESSONS.md', 'NOTES.md'):
        p = f'{R}/rebuild/candidates/manual/pending/{hb}/{fn}'
        if os.path.exists(p): shutil.copy(p, f"{OUT}/lessons/{hb.split('_')[-1]}_{fn}")
for b in sorted(glob.glob(f'{R}/rebuild/candidates/manual/batches/2026-11-11_landing_*/README.md')):
    shutil.copy(b, f"{OUT}/lessons/landing_{b.split('/')[-2]}.md")
p = f'{R}/rebuild/candidates/manual/pending/PLAYBOOK_CANDIDATES.tsv'
# headers
shutil.copytree(f'{R}/rebuild/include', f'{OUT}/include')
# names
os.makedirs(f'{OUT}/names')
for p in ['rebuild/oracles/reloc_bindings.tsv', 'ghidra_out/egor_pdb_names.tsv', 'ghidra_out/fablewin_pdb_names.tsv',
          'ghidra_out/egod_pdb_names.tsv', 'ghidra_out/struct_layouts_egor.tsv', 'ghidra_out/pdb_class_keys.tsv',
          'rebuild/manifest/rtti-vtables.tsv', 'rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv',
          'rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv', 'ghidra_out/egod_pdb_func_names.tsv', 'ghidra_out/fable_types.h',
          'ghidra_out/naming_stragglers/Fable.exe.signatures.tsv', 'ghidra_out/naming_stragglers/ego_r.exe.signatures.tsv',
          'rebuild/candidates/manual/BOUNDS_FIX.tsv', 'refs/fse_api_manifest.json']:
    shutil.copy(f'{R}/{p}', f'{OUT}/names/{os.path.basename(p)}')
with open(f'{OUT}/names/catalog_names.tsv', 'w') as o:
    o.write('address\tname\tlength\n')
    for a in sorted(cat): o.write(f'{a}\t{cat[a][0]}\t{cat[a][1]}\n')
with open(f'{OUT}/names/manifest_names.tsv', 'w') as o:
    o.write('address\tmodule\tname\n')
    for a in sorted(man): o.write(f"{a}\t{man[a]['module']}\t{man[a]['name']}\n")
# tools (standalone, read the user's own exe)
os.makedirs(f'{OUT}/tools')
for p in ['cut_row.py', 'xref.py']: shutil.copy(f'{S}/codex1121/{p}', f'{OUT}/tools/{p}')
for p in ['PROMPT_CODEX.md', 'README_CODEX.md']: shutil.copy(f'{S}/codex1121/{p}', f'{OUT}/{p}')
P15 = f'{R}/rebuild/candidates/manual/pending/2026-11-09_codex_pass15/return_2026-11-10'
for fn in ('LESSONS.md', 'FINAL_UNFINISHED_GUIDANCE.md', 'ROW_DETAILS.md'): shutil.copy(f'{P15}/{fn}', f'{OUT}/lessons/codex_pass15_{fn}')
shutil.copy(f'{R}/rebuild/candidates/manual/naming/2026-11-10_codex20nf/README.md', f'{OUT}/lessons/naming_codex20nf_2026-11-10.md')
shutil.copy(f'{R}/rebuild/candidates/manual/naming/2026-11-10_agentNF/README.md', f'{OUT}/lessons/naming_agentNF_2026-11-10.md')
P16 = f'{R}/rebuild/candidates/manual/pending/2026-11-10_codex_pass16/return_2026-11-10'
for fn in ('LESSONS.md', 'START_HERE_CLAUDE.md'): shutil.copy(f'{P16}/{fn}', f'{OUT}/lessons/codex_pass16_{fn}')
os.makedirs(f'{OUT}/names/labels')
for p in glob.glob(f'{R}/ghidra_out/labels_*.tsv'): shutil.copy(p, f'{OUT}/names/labels/{os.path.basename(p)}')
shutil.copy(f'{R}/CLAUDE.md', f'{OUT}/docs/PROJECT_RULES_CLAUDE.md')
P17 = f'{R}/rebuild/candidates/manual/pending/2026-11-10_codex_pass17/return_2026-11-10'
for fn in ('LESSONS.md', 'START_HERE_CLAUDE.md', 'STATUS.tsv'): shutil.copy(f'{P17}/{fn}', f'{OUT}/lessons/codex_pass17_{fn}')
shutil.copy(f'{P17}/differ/00871fd0_PROBES.md', f'{OUT}/lessons/codex_pass17_00871fd0_PROBES.md')
P18 = f'{R}/rebuild/candidates/manual/pending/2026-11-10_codex_pass18/return_2026-11-10'
for fn in ('LESSONS.md', 'START_HERE_CLAUDE.md', 'STATUS.tsv', 'HEADER_DEFAULT_AUDIT.md'): shutil.copy(f'{P18}/{fn}', f'{OUT}/lessons/codex_pass18_{fn}')
shutil.copy(f'{P18}/differ/00903f30/PROBES.md', f'{OUT}/lessons/codex_pass18_00903f30_PROBES.md')
P19 = f'{R}/rebuild/candidates/manual/pending/2026-11-10_codex_pass19/return_2026-11-10'
for fn in ('LESSONS.md', 'START_HERE_CLAUDE.md', 'STATUS.tsv', 'FINDINGS.md', 'PACKING_APPROVAL.md'): shutil.copy(f'{P19}/{fn}', f'{OUT}/lessons/codex_pass19_{fn}')
P20 = f'{R}/rebuild/candidates/manual/pending/2026-11-10_codex_pass20/return'
for fn in ('LESSONS.md', 'START_HERE_CLAUDE.md', 'STATUS.tsv', 'ROW_DETAILS.md'): shutil.copy(f'{P20}/{fn}', f'{OUT}/lessons/codex_pass20_{fn}')
for n in ('NF3', 'NF4', 'NF5'):
    for fn in ('NOTES.md', 'LESSONS.md'):
        p_ = f'{R}/rebuild/candidates/manual/naming/2026-11-10_agent{n}/{fn}'
        if os.path.exists(p_): shutil.copy(p_, f'{OUT}/lessons/naming_agent{n}_{fn}')
for n in ('NF8', 'NF9', 'NF10', 'NF11', 'NF12', 'NF13', 'NF14', 'NF15'):
    for fn in ('NOTES.md', 'LESSONS.md'):
        p_ = f'{R}/rebuild/candidates/manual/naming/2026-11-11_agent{n}/{fn}'
        if os.path.exists(p_): shutil.copy(p_, f'{OUT}/lessons/naming_agent{n}_{fn}')
print('rows', len(rowsout) - 1)
