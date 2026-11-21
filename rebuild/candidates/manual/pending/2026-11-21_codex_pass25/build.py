#!/usr/bin/env python3
"""Build the Codex hand-off package pass 25, 2026-11-21: 8 large stalled rows from Account B's p25 lane.
No retail bytes are written into the package (the exe is read only to find callers). Ego_d.pdb is read only through the
pre-extracted per-row locals / line tables in $EGOD_EXTRACT; the PDB itself is not copied."""
import csv, glob, os, re, shutil, struct, bisect, collections
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'
S = os.environ['S']
HERE = os.path.dirname(os.path.abspath(__file__))
NAME = 'fabledecomp_codex_pass25_2026-11-21'
OUT = f'{S}/codex25/{NAME}'
P = f'{R}/rebuild/candidates/manual'
EGX = os.environ['EGOD_EXTRACT']          # per-row egod_locals .txt / .lines.txt
EGP = os.environ.get('EGOD_DIR', '/tmp/claude-0/egod_pdb')
if os.path.exists(OUT): shutil.rmtree(OUT)
os.makedirs(OUT)
EXE = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', EXE, 0x3c)[0]; nsec = struct.unpack_from('<H', EXE, pe + 6)[0]; optsz = struct.unpack_from('<H', EXE, pe + 20)[0]

man = {r['address'].lower(): r for r in csv.DictReader(open(f'{R}/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t')}
cat = {r['address']: (r['name'], r['length']) for r in csv.DictReader(open(f'{R}/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t')}
sel = [l.rstrip('\n').split('\t') for l in open(f'{HERE}/rows.tsv') if l.strip()]
for s in sel: assert s[0] not in cat, s[0] + ' already landed'

t = open(f'{R}/rebuild/build_candidates.ps1', encoding='utf-8-sig').read()
ent = {}
for blk in re.split(r'\[pscustomobject\]@\{', t)[1:]:
    kv = dict(re.findall(r"(\w+)\s*=\s*'([^']*)'", blk))
    if 'Address' in kv and 'Source' in kv: ent[kv['Address'].lower()] = kv
landed_sorted = sorted(a for a in ent if not a.startswith('012'))

def build_xref(targets):
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
    return {a: ' | '.join(res[int(a, 16)][:30]) for a in targets}
XR = build_xref([s[0] for s in sel])
def containing_landed(va):
    i = bisect.bisect_right(landed_sorted, '%08x' % va) - 1
    if i < 0: return None
    a = landed_sorted[i]; ln = int(cat.get(a, ('', '0'))[1] or 0)
    return a if int(a, 16) <= va < int(a, 16) + max(ln, 1) else None
catn = collections.defaultdict(list)
for a_, (n_, l_) in cat.items(): catn[n_].append(a_)
funcs = collections.defaultdict(list)
for l in open(f'{R}/ghidra_out/egod_carved_functions.tsv'):
    if l.startswith('#'): continue
    o_, off, ln_, k_, n_ = l.rstrip('\n').split('\t', 4); funcs[o_.split('\\')[-1].lower()].append((int(off), int(ln_), n_))
def egod_tu(ob, target):
    fl = sorted(funcs.get(ob.split('\\')[-1].lower(), []))
    out = [f"== {ob}: {len(fl)} functions in Ego_d object order (debug length; retail address(es) when the landed catalog has the name)"]
    for off, ln_, n_ in fl:
        hits = catn.get(n_) or catn.get(n_.split('<')[0]) or []
        out.append(f"{'>>' if n_ == target else '  '} {ln_:6d}  {n_}" + (f"   retail {' '.join(sorted(hits)[:4])} (landed)" if hits else ""))
    return '\n'.join(out) + '\n'
SKIP = re.compile(r'(oracle|payload|\.out$|\.gm$|\.bin$|\.obj$|\.json$|\.lst$|\.asm$)', re.I)
def copy_tree_clean(src, dst):
    for root, dirs, files in os.walk(src):
        for f in files:
            if SKIP.search(f) or not f.endswith(('.h', '.hpp', '.cpp', '.md', '.txt', '.tsv', '.inl')): continue
            s_ = os.path.join(root, f); d_ = os.path.join(dst, os.path.relpath(s_, src))
            os.makedirs(os.path.dirname(d_), exist_ok=True); shutil.copy(s_, d_)

rowsout = ['address_va\tfile_offset\tfile_offset_end\tlength\tmodule\tname\tegod_name\tbest_grade\tresidue_summary']
for a, ln, grade, path, en, ob, summary, extras in sel:
    va = int(a, 16); ln = int(ln); m = man.get(a, {'module': '?', 'name': '?'})
    d = f'{OUT}/rows/{a}'; os.makedirs(d)
    rowsout.append('\t'.join([f'0x{va:08X}', f'0x{va - 0x400000:08X}', f'0x{va - 0x400000 + ln:08X}', str(ln), m['module'], m['name'], en, grade, summary]))
    callers = []
    for g in re.findall(r'(call|jmp|abs \S+) ([0-9a-f]{8})', XR[a]):
        c = containing_landed(int(g[1], 16))
        callers.append(f"- {g[0]} at 0x{g[1].upper()} " + (f"in landed 0x{c.upper()} {cat[c][0]} ({ent[c]['Source']})" if c else ('(data reference: in .rdata usually a vtable slot)' if g[0].startswith('abs') else '(caller not landed)')))
    i = bisect.bisect_left(landed_sorted, a); neigh = landed_sorted[max(0, i - 4):i + 4]
    os.makedirs(f'{d}/neighbours'); nl = []
    for n in neigh:
        src = f"{R}/rebuild/src/compiled/{ent[n]['Source']}"
        if os.path.exists(src):
            shutil.copy(src, f'{d}/neighbours/{os.path.basename(src)}')
            nl.append(f"- 0x{n.upper()} {cat.get(n, ('?',))[0]} -> neighbours/{os.path.basename(src)}" + (f" (flags: {ent[n]['CompilerFlags']})" if ent[n].get('CompilerFlags') else ''))
    pl = []
    for f in sorted(glob.glob(f'{P}/pending/**/{a}*', recursive=True)):
        if os.path.isdir(f) or not f.endswith(('.cpp', '.h', '.md', '.txt')) or SKIP.search(os.path.basename(f)): continue
        rel = f.split('/pending/')[1].split('/'); dst = '__'.join(rel)
        os.makedirs(f'{d}/prior', exist_ok=True); shutil.copy(f, f'{d}/prior/{dst}'); pl.append(f'- prior/{dst}')
    shutil.copy(f'{R}/{path}', f'{d}/best_acctC_or_B_draft.cpp')
    fold = os.path.dirname(f'{R}/{path}'); hf = []
    for p_ in sorted(glob.glob(f'{fold}/*.md') + glob.glob(f'{fold}/NOTES*')):
        if os.path.getsize(p_) < 400000 and not SKIP.search(p_):
            dst = 'handback_' + os.path.basename(p_); shutil.copy(p_, f'{d}/{dst}'); hf.append(dst)
    bf = []
    for ex in [x for x in extras.split(';') if x]:
        kind, rest = ex.split(':', 1); src, dst = rest.split('=')
        base = {'R': f'{S}/w26/p25R', 'Q': f'{S}/w26/p25Q', 'N': f'{P}/pending', 'W': S}[kind]
        sp = f'{base}/{src}'
        if os.path.isdir(sp): copy_tree_clean(sp, f'{d}/{dst}'); bf.append(f'{dst}/ (header overlay: drop these files over include/ for this row only)')
        elif os.path.exists(sp): shutil.copy(sp, f'{d}/{dst}'); bf.append(dst)
        else: print('MISSING', sp)
    key = en.replace(':', '_').replace('<', '_').replace('>', '_')
    eg = []
    for suf, dst in (('.txt', 'egod_locals.txt'), ('.lines.txt', 'egod_lines.txt')):
        p_ = f'{EGX}/{key}{suf}'
        if en != '-' and os.path.exists(p_) and os.path.getsize(p_) > 100: shutil.copy(p_, f'{d}/{dst}'); eg.append(dst)
    if ob != '-' and funcs.get(ob.split('\\')[-1].lower()):
        open(f'{d}/egod_tu.txt', 'w').write(egod_tu(ob, en)); eg.append('egod_tu.txt')
    with open(f'{d}/ROW.md', 'w') as o:
        o.write(f"# 0x{va:08X} -- {en if en != '-' else m['module'] + '::' + m['name']}\n\n")
        o.write(f"- Manifest label: `{m['module']}::{m['name']}`. Ego_d name: `{en}`; Ego_d object: `{ob}`.\n")
        o.write(f"- Retail extent: VA 0x{va:08X}, {ln} bytes (file offset 0x{va - 0x400000:08X} .. 0x{va - 0x400000 + ln:08X}). "
                f"Cut it with `python3 tools/cut_row.py Fable.exe 0x{va:08X} {ln} --dis`.\n")
        o.write(f"- Best grade on record: {grade}\n")
        o.write(f"\n## Where it stands (Account B, 2026-11-21)\n{summary}\n")
        o.write(f"\n## Files\n- `best_acctC_or_B_draft.cpp`: the draft on record (from `{path}`); regrade it first on the package headers (+ the row's overlay).\n")
        for x in bf: o.write(f"- `{x}`\n")
        for x in hf: o.write(f"- `{x}` (notes of the folder the draft came from; search for {a})\n")
        for x in eg: o.write(f"- `{x}`\n")
        o.write("\n`egod_locals.txt` = the source's parameter / local names, types and scope blocks (frame offsets are VC10 /Od, not retail). "
                "`egod_lines.txt` = Ego_d's per-line byte counts (`+offset line bytes`), a VC10 /Od size oracle: calibrate with the VC7.1 /Od size of known "
                "statements (docs/FINDINGS_LOG.md, 2026-11-20/21 calibration lines). `egod_tu.txt` = the row's object file in Ego_d order.\n")
        o.write("\n## Callers / references (from the retail exe)\n" + ('\n'.join(callers) if callers else '- none found') + "\n")
        o.write("\n## Landed neighbours (usually the same TU: reuse their includes, opt-ins, pragmas, flags)\n" + '\n'.join(nl) + "\n")
        if pl: o.write("\n## Earlier files on record for this address\n" + '\n'.join(pl) + "\n")
open(f'{OUT}/ROWS.tsv', 'w').write('\n'.join(rowsout) + '\n')

def cp(src, dst):
    if os.path.exists(src): os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copy(src, dst)
    else: print('MISSING', src)
LT = 'tools/decomp_pipeline/linux_lane/lane_tools'
for p in ['docs/pipeline/EXTERNAL_AI_HANDOFF.md', 'docs/BUILDING.md', 'docs/ARCHITECTURE.md', 'docs/pipeline/GOTCHAS.md', 'docs/pipeline/MATCHING_PLAYBOOK.md',
          'docs/pipeline/C2_ALLOCATOR_PROBE.md', 'docs/pipeline/ASM_RULES.md', 'docs/pipeline/INLINE_DEPTH_REVISIT.md', 'docs/pipeline/VC71_CODEGEN_RESEARCH.md',
          'docs/pipeline/REGISTER_SWAP_SUMMARY.md', 'docs/pipeline/SYMBOLS.md', 'docs/pipeline/NAMING_STRAGGLERS.md', 'docs/pipeline/LIBRARY_SIGNATURES.md',
          'docs/pipeline/METHODOLOGY.md', 'docs/pipeline/DECOMP_ACCELERATORS.md', 'docs/HANDOFF.md', 'CONTRIBUTING.md', f'{LT}/chunk_pool/EXTRA_RULES.md',
          'docs/journal/FINDINGS_LOG.md', 'docs/journal/2026-11/2026-11-20_ADVISOR_DECISIONS_B.md', 'docs/journal/2026-11/EGOD_FINDINGS.md']:
    cp(f'{R}/{p}', f'{OUT}/docs/{os.path.basename(p)}')
cp(f'{R}/CLAUDE.md', f'{OUT}/docs/PROJECT_RULES_CLAUDE.md')
cp(f'{R}/{LT}/lane_merge/README.md', f'{OUT}/docs/HEADER_MERGE_AND_PROOF.md')
cp(f'{P}/UNMATCHED_ROWS.tsv', f'{OUT}/docs/UNMATCHED_ROWS.tsv')
for p in ('pin_asserts.py', 'equiv_check.py'): cp(f'{R}/{LT}/lane_merge/{p}', f'{OUT}/tools/lane_merge/{p}')
# lessons: the p25 landings of 2026-11-20/21 (what closed each row), the open-row notes, the Ego_d lane, the pass 24 package's README
for b in sorted(glob.glob(f'{P}/batches/2026-11-2[01]_*p25*/author_README.md')):
    cp(b, f"{OUT}/lessons/p25_landings/{b.split('/')[-2]}.md")
for b in sorted(glob.glob(f'{P}/batches/2026-11-21_report_*/README.md')):
    cp(b, f"{OUT}/lessons/p25_landings/{b.split('/')[-2]}.md")
for f in sorted(glob.glob(f'{P}/pending/2026-11-20_p25_open/**/*.md', recursive=True) + glob.glob(f'{P}/pending/2026-11-20_p25_open/**/NOTES*.txt', recursive=True)):
    cp(f, f"{OUT}/lessons/p25_open_notes/{f.split('2026-11-20_p25_open/')[1].replace('/', '__')}")
for fn in ('LESSONS_EGOD.md',): cp(f'{P}/pending/2026-11-17_egod_parked/lane/{fn}', f'{OUT}/lessons/{fn}')
cp(f'{P}/pending/2026-11-18_codex_pass24/README_CODEX.md', f'{OUT}/lessons/codex_pass24_README.md')
for f in glob.glob(f'{P}/pending/2026-11-18_codex_pass24/return/*.md'): cp(f, f"{OUT}/lessons/codex_pass24_return__{os.path.basename(f)}")
# tools: row cutter, xref, Ego_d extractors, and the c1xx / c2 compile-order harness (gdb under Wine)
for p in ('cut_row.py', 'xref.py', 'egod_locals.py', 'egod_type.py', 'eh_parent.py', 'egod_proc.py', 'egod_lines.py'): cp(f'{HERE}/tools/{p}', f'{OUT}/tools/{p}')
shutil.copytree(f'{P}/pending/2026-11-20_c2_symbol_order', f'{OUT}/tools/c2_symbol_order_kit')
# headers: today's include tree (every landing through this commit)
shutil.copytree(f'{R}/rebuild/include', f'{OUT}/include')
# names: PDB function / data / type lists
for p in ['rebuild/oracles/reloc_bindings.tsv', 'ghidra_out/egor_pdb_names.tsv', 'ghidra_out/fablewin_pdb_names.tsv',
          'ghidra_out/struct_layouts_egor.tsv', 'ghidra_out/pdb_class_keys.tsv', 'rebuild/manifest/rtti-vtables.tsv',
          'rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv', 'rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv',
          'ghidra_out/fable_types.h', 'ghidra_out/naming_stragglers/Fable.exe.signatures.tsv', 'ghidra_out/egod_carved_functions.tsv',
          'ghidra_out/egod_carved_names.tsv', 'refs/fse_api_manifest.json']:
    cp(f'{R}/{p}', f'{OUT}/names/{os.path.basename(p)}')
for p, dst in (('procnames.txt', 'egod_procnames.txt'), ('datanames.txt', 'egod_datanames.txt'), ('modules.txt', 'egod_modules.txt')):
    cp(f'{EGP}/{p}', f'{OUT}/names/{dst}')
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
LINK = os.environ.get('EGOD_PDB_LINK', '(ask the maintainer for the Ego_d.pdb link)')
for p in ('PROMPT_CODEX.md', 'README_CODEX.md'): open(f'{OUT}/{p}', 'w').write(open(f'{HERE}/{p}').read().replace('@@EGOD_PDB_LINK@@', LINK))
print('rows', len(rowsout) - 1)
