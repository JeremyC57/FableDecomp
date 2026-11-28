#!/usr/bin/env python3
"""Build the Codex hand-off package pass 30, 2026-11-27: the 100 largest parked rows no Codex pass has had (rows.tsv, chosen by
tools/select_rows.py --allow-ungraded --order longest --n 100). Derived from pass 29's build.py. New: names/ is split into answers/ (what to
spell), pdb/ (the PDB tables), state/ (what the tree binds today) and hints/ (labels that are NOT evidence); every row gets NAMES_TO_USE.tsv
(each retail call / data target of the row joined to names/answers/NAME_ANSWERS.tsv); docs/NAMING_FIRST_TIME.md is the naming rule set.
No retail bytes are written into the package (the exe is read to find callers and targets). The PDBs are read only through pre-extracted
per-row text (tools/prep_evidence.py) and the tables in ghidra_out/. No PDB is copied. Any MISSING source fails the build."""
import csv, glob, os, re, shutil, struct, bisect, collections
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'
S = os.environ['S']
HERE = os.path.dirname(os.path.abspath(__file__))
NAME = 'fabledecomp_codex_pass31_2026-11-28'
OUT = f'{S}/codex31/{NAME}'
ANS_PATH = os.environ['NAME_ANSWERS']     # tools/name_answers.py output for this commit
P = f'{R}/rebuild/candidates/manual'
EGX = os.environ['EGOD_EXTRACT']          # per-row egod_locals .txt / .lines.txt
REL = os.environ['REL_EXTRACT']           # per-row <addr>.Ego_r.txt / <addr>.FableWin.txt (pdb_proc.py)
PDBX = os.environ['PDBX']                  # Ego_r.modules.txt / FableWin.modules.txt (llvm-pdbutil dump -modules of the 4 KB repacks)
MISSING = []
import hashlib
def egod_key(en):
    k = re.sub(r'[^A-Za-z0-9_~.-]', '_', en)
    return k if len(k) <= 120 else k[:80] + '_' + hashlib.sha1(en.encode()).hexdigest()[:12]
if os.path.exists(OUT): shutil.rmtree(OUT)
os.makedirs(OUT)
EXE = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', EXE, 0x3c)[0]; nsec = struct.unpack_from('<H', EXE, pe + 6)[0]; optsz = struct.unpack_from('<H', EXE, pe + 20)[0]

man = {r['address'].lower(): r for r in csv.DictReader(open(f'{R}/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t')}
cat = {r['address']: (r['name'], r['length']) for r in csv.DictReader(open(f'{R}/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t')}
sel = [l.rstrip('\n').split('\t') for l in open(f'{HERE}/rows.tsv') if l.strip()]
# header overlays the drafts on record need (their FABLE_RETAIL_* opt-ins are not in today's include/; found by grepping each draft's
# defines against rebuild/include and the draft chunk's hdr/ folder). Older copies: port the opt-in blocks, do not drop the files over include/.
EXTRAS = {}   # pass 30: no per-row overlays (the drafts on record compile against today's include/ or are ungraded hints)
for s_ in sel:
    if s_[0] in EXTRAS: s_[7] = ';'.join(x for x in (s_[7], EXTRAS[s_[0]]) if x)
for s in sel:
    assert s[0] not in cat, s[0] + ' already landed'
    if s[3].endswith('.test.cpp') and os.path.exists(f"{R}/{s[3][:-9]}.cpp"): s[3] = s[3][:-9] + '.cpp'   # pass 28: the record names the test

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
LN = {s_[0]: int(s_[1]) for s_ in sel}
# ---- pass 30: the spelling to use for every retail target a row references (names/answers/NAME_ANSWERS.tsv) ----
ANS = {}
for l in open(ANS_PATH, encoding='utf-8').read().splitlines()[1:]:
    f_ = l.split('\t'); ANS[f_[0]] = f_
import capstone
CS = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); CS.detail = True
SECS = []
for i in range(nsec):
    o = pe + 24 + optsz + 40 * i; nm_ = EXE[o:o + 8].rstrip(b"\0").decode()
    vsz, rva, rsz, raw = struct.unpack_from('<IIII', EXE, o + 8); SECS.append((nm_, 0x400000 + rva, max(vsz, rsz), raw, rsz))
def sec_of(va):
    for nm_, b0, sz, raw, rsz in SECS:
        if b0 <= va < b0 + sz: return nm_
    return None
def row_targets(a):
    va = int(a, 16); nm_, b0, sz, raw, rsz = next(s_ for s_ in SECS if s_[1] <= va < s_[1] + s_[2])
    code = EXE[raw + va - b0: raw + va - b0 + LN[a]]; out = []; end = va; pad = None
    for ins in CS.disasm(code, va):
        end = ins.address + ins.size
        if pad is None and ins.mnemonic in ('ret', 'jmp') and code[end - va: end - va + 2] in (b'\xcc\xcc', b'\x90\x90'): pad = end   # VC7.1 pads with int3 after a function
        for op in ins.operands:
            v = None
            if op.type == capstone.x86.X86_OP_IMM: v = op.imm & 0xffffffff
            elif op.type == capstone.x86.X86_OP_MEM and op.mem.base == 0 and op.mem.index == 0 and op.mem.disp: v = op.mem.disp & 0xffffffff
            elif op.type == capstone.x86.X86_OP_MEM and op.mem.disp and (op.mem.disp & 0xffffffff) >= 0x401000: v = op.mem.disp & 0xffffffff
            if v is None or not (0x401000 <= v < 0x1600000) or va <= v < va + LN[a] or not sec_of(v): continue
            kind = 'call' if ins.mnemonic == 'call' else ('jmp' if ins.mnemonic.startswith('j') else ('data' if sec_of(v) != '.text' else 'code-ref'))
            out.append((kind + ('(after-padding)' if pad and ins.address >= pad else ''), ins.address, v))
    DECODED[a] = end - va; PAD[a] = pad
    return out
DECODED = {}; PAD = {}
NEED_LANDED = set()   # landed rows a RESPELL may have to edit: binders of every non-proven target the rows reference
def names_to_use(a):
    lines = ['kind\tat\ttarget\tverdict\tuse_this_spelling\tbasis\tlanded_wrong\thint_spellings\tlanded_binders (sources in landed/)']
    s_ = ANS.get(a)
    lines.append('\t'.join(['SELF', f'0x{int(a,16):08X}', a] + (s_[1:6] + [s_[10]] if s_ else ['NOT_NAMED_YET', '', 'no landed code names this address yet: settle the self from the PDB evidence in ROW.md', '', '', ''])))
    if s_ and s_[10]: NEED_LANDED.update(s_[10].split(','))
    seen = set()
    for kind, at, v in row_targets(a):
        k = f'{v:08x}'
        if (kind, k) in seen: continue
        seen.add((kind, k)); s_ = ANS.get(k)
        if s_:
            lines.append('\t'.join([kind, f'0x{at:08X}', k] + s_[1:6] + [s_[10] if s_[1] not in ('PROVEN', 'PROVEN_RTTI') else '']))
            if s_[1] not in ('PROVEN', 'PROVEN_RTTI') and s_[10]: NEED_LANDED.update(s_[10].split(','))
        else:
            c = containing_landed(v) if sec_of(v) == '.text' else None
            if kind in ('call', 'jmp') and c and c != k: continue          # a branch into another function's body: not a symbol
            lines.append('\t'.join([kind, f'0x{at:08X}', k, 'NOT_NAMED_YET', '', 'no landed code names this target yet (string / float literal, switch table, or an unnamed function / global): '
                                     'name it from the PDB tables (docs/NAMING_FIRST_TIME.md 3) or spell the literal', '', '', '']))
    if PAD.get(a) and any('(after-padding)' in x.split('\t')[0] for x in lines[1:]):
        lines.append('\t'.join(['NOTE', f'0x{PAD[a]:08X}', '', 'PAST_PADDING?', '', 'a return / jump followed by int3 or nop padding ends here: lines marked (after-padding) may lie past the function (the extent is an estimate); confirm the true end first', '', '', '']))
    if DECODED[a] < LN[a] - 16:
        lines.append('\t'.join(['NOTE', f'0x{int(a,16) + DECODED[a]:08X}', '', 'PARTIAL', '', f'decoding stopped {LN[a] - DECODED[a]} B before the extent end (padding, a jump table or data, or the estimated extent runs past the function): targets after this point are not listed', '', '', '']))
    return lines
catn = collections.defaultdict(list)
for a_, (n_, l_) in cat.items(): catn[n_].append(a_)
funcs = collections.defaultdict(list); EGODN = collections.defaultdict(list)
for l in open(f'{R}/ghidra_out/egod_carved_functions.tsv'):
    if l.startswith('#'): continue
    o_, off, ln_, k_, n_ = l.rstrip('\n').split('\t', 4); funcs[o_.split('\\')[-1].lower()].append((int(off), int(ln_), n_))
    if (o_, int(ln_)) not in EGODN[n_]: EGODN[n_].append((o_, int(ln_)))
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


# ---- per-row naming and header evidence (pass 26 build, kept) ----
import subprocess
NAMEIDX = collections.defaultdict(list)   # method token -> [(source, address, symbol)]
for src_, fn_ in (('Ego_r', 'ghidra_out/egor_pdb_names.tsv'), ('FableWin', 'ghidra_out/fablewin_pdb_names.tsv')):
    for l in open(f'{R}/{fn_}', errors='replace'):
        f_ = l.rstrip('\n').split('\t')
        if len(f_) < 2 or not f_[1].startswith('?'): continue
        tok = re.match(r'\?+[0-9A-Z_]?([A-Za-z_]\w*)@', f_[1])
        if tok: NAMEIDX[tok.group(1)].append((src_, f_[0], f_[1]))
FIXQ = collections.defaultdict(list)
for l in open(f'{R}/rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv', encoding='utf-8', errors='replace'):
    f_ = l.rstrip('\n').split('\t')
    if len(f_) > 2 and re.fullmatch(r'(0x)?[0-9A-Fa-f]{8}', f_[1]): FIXQ[f_[1].lower().replace('0x', '')].append(f_[2][:200])
def leaf_of(path):
    for x in open(f'{R}/{path}', errors='replace'):
        if x.startswith('// Leaf:'):
            s_ = x.split(); return s_[2] if len(s_) > 2 else None
    return None
def naming_section(a, en, m):
    out = ["\n## Naming evidence (check every name against two sources: docs/NAMING_AND_HEADERS.md)\n"]
    out.append(f"- Manifest label: `{m['module']}::{m['name']}`. Manifest labels are often placeholders (vfunc_N, `_global::Class_Method`, sub_, Unproven): never copy one without a PDB source.\n")
    out.append(f"- Ego_d (VC10 debug PDB) procedure name: `{en}`" + (" (none found: name the function from Ego_r / FableWin / FSE / strings, else descriptive)" if en == '-' else '') + "\n")
    tok = None
    if en != '-': tok = en.split('::')[-1].split('<')[0].lstrip('~')
    elif m['name'] and not m['name'].startswith(('vfunc', 'sub_', 'FUN_')): tok = m['name'].split('::')[-1]
    hits = NAMEIDX.get(tok, []) if tok else []
    cls = en.split('::')[-2] if en != '-' and '::' in en else (m['module'].split('::')[-1] if m['module'] not in ('_global', '_gapscan') else None)
    if cls: hits = [h for h in hits if f'@{cls}@' in h[2]] or hits
    if hits:
        out.append(f"- Mangled candidates for `{tok}` in Ego_r / FableWin (address is in THAT build's image, not Fable.exe):\n")
        for h in hits[:8]: out.append(f"  - {h[0]} {h[1]} `{h[2]}`\n")
        if len(hits) > 8: out.append(f"  - ... {len(hits) - 8} more in names/pdb/egor_pdb_names.tsv / names/pdb/fablewin_pdb_names.tsv\n")
    if en != '-':   # pass 27: identity checks on the Ego_d record (egod_locals.txt shows the first complete record of the name)
        recs = EGODN.get(en, [])
        if len(recs) > 1:
            out.append(f"- **Ego_d holds {len(recs)} records named `{en}`** (overloads, statics, or one function from several incremental links: " + ', '.join(f'{ln_} B in {o_.split(chr(92))[-1]}' for o_, ln_ in recs[:6])
                       + "): `egod_locals.txt` / `egod_lines.txt` show the first complete one; check its parameters against the row's signature "
                       "(`tools/egod_locals.py` lists any other).\n")
        try:
            dl_ = int(re.search(r'debug length (\d+)', open(f'{EGX}/{egod_key(en)}.txt').read()).group(1))
            r_ = dl_ / LN[a]
            if r_ < 0.6 or r_ > 4.5:
                out.append(f"- **Check the Ego_d identity:** its record is {dl_} B (VC10 /Od) for a {LN[a]} B retail row (ratio {r_:.2f}; /Od is usually "
                           "1-3x retail). Heavy retail inlining can explain a low ratio; otherwise it may be another overload or a different function.\n")
        except (OSError, AttributeError): pass
    rel = [x for x in ('Ego_r', 'FableWin') if os.path.exists(f'{REL}/{a}.{x}.txt')]
    if rel: out.append("- Ego_r / FableWin procedure records (VS 2010 game-release / editor-debug builds of the same engine: the function's full "
                       "signature, parameter / local names and types, call-site signatures; sizes, frame offsets and registers are NOT retail; "
                       "the file's first line says which name it matched): "
                       + ', '.join(f'`{x.lower()}_proc.txt`' for x in rel) + "\n")
    else: out.append("- No Ego_r / FableWin procedure record matched the names above (searched by exact qualified name and by Class::Method).\n")
    if FIXQ.get(a): out.append("- Name fixes already queued for this address (names/state/NAME_FIXES_QUEUE.tsv): " + ' | '.join(f'`{x}`' for x in FIXQ[a][:3]) + "\n")
    lf = None
    return ''.join(out)
INCD = f'{R}/rebuild/include'
OPTIN = re.compile(r'#\s*(?:if|ifdef|elif)\b[^\n]*?\b(FABLE_RETAIL_\w+)')
def header_section(a, en, m, path):
    out = ["\n## Headers (model `this` and every callee with the shared headers in include/)\n"]
    names = set()
    if en != '-' and '::' in en: names |= {p_ for p_ in en.split('::')[:-1] if p_ and p_[0].isupper()}
    if m['module'] not in ('_global', '_gapscan', '?'): names |= {p_ for p_ in m['module'].split('::') if p_}
    for cls in sorted(names):
        for sub in ('engine', 'libpk'):
            hp = f'{INCD}/{sub}/{cls}.h'
            if os.path.exists(hp):
                ops = sorted(set(OPTIN.findall(open(hp, errors='replace').read())))
                out.append(f"- `include/{sub}/{cls}.h`: {len(ops)} opt-in macros" + (": " + ', '.join(f'`{o_}`' for o_ in ops[:25]) + (' ...' if len(ops) > 25 else '') if ops else '') + "\n")
    try:
        drafts = open(f'{R}/{path}', errors='replace').read()
        inc = sorted(set(re.findall(r'#include\s+"([^"]+)"', drafts))); dfs = sorted(set(re.findall(r'#define\s+(FABLE_\w+)', drafts)))
        out.append(f"- The draft includes {len(inc)} headers and defines {len(dfs)} opt-ins; the same-TU landed neighbours (below) show which opt-ins the original TU needs.\n")
    except Exception: pass
    out.append("- A class with no header: add one as `include/engine/<Class>.h` from `names/pdb/struct_layouts_egor.tsv` (real member names and offsets), never a row-local struct.\n")
    return ''.join(out)

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
    shutil.copy(f'{R}/{path}', f'{d}/best_draft.cpp')
    fold = os.path.dirname(f'{R}/{path}'); hf = []
    for p_ in sorted(glob.glob(f'{fold}/*.md') + glob.glob(f'{fold}/NOTES*')):
        if os.path.getsize(p_) < 400000 and not SKIP.search(p_):
            dst = 'handback_' + os.path.basename(p_); shutil.copy(p_, f'{d}/{dst}'); hf.append(dst)
    bf = []
    for ex in [x for x in extras.split(';') if x]:
        kind, rest = ex.split(':', 1); src, dst = rest.split('=')
        base = {'N': f'{P}/pending', 'W': S}[kind]
        sp = f'{base}/{src}'
        if os.path.isdir(sp): copy_tree_clean(sp, f'{d}/{dst}'); bf.append(f'{dst}/ (header overlay: drop these files over include/ for this row only)')
        elif os.path.exists(sp):
            os.makedirs(os.path.dirname(f'{d}/{dst}'), exist_ok=True); shutil.copy(sp, f'{d}/{dst}')
            bf.append(dst + (' (header overlay the draft on record was graded with: an older copy carrying opt-ins today\'s include/ lacks; '
                             'port its FABLE_RETAIL_* blocks onto include/ as patches, do not drop the file over include/)' if dst.startswith('hdr_overlay/') else ''))
        else: print('MISSING', sp); MISSING.append(sp)
    key = egod_key(en)
    eg = []
    for suf, dst in (('.txt', 'egod_locals.txt'), ('.lines.txt', 'egod_lines.txt')):
        p_ = f'{EGX}/{key}{suf}'
        if en != '-' and os.path.exists(p_) and os.path.getsize(p_) > 40 and 'no complete record' not in open(p_).read(200):
            shutil.copy(p_, f'{d}/{dst}'); eg.append(dst)
    for x in ('Ego_r', 'FableWin'):
        p_ = f'{REL}/{a}.{x}.txt'
        if os.path.exists(p_): shutil.copy(p_, f'{d}/{x.lower()}_proc.txt'); eg.append(f'{x.lower()}_proc.txt')
    if ob != '-' and funcs.get(ob.split('\\')[-1].lower()):
        open(f'{d}/egod_tu.txt', 'w').write(egod_tu(ob, en)); eg.append('egod_tu.txt')
    with open(f'{d}/ROW.md', 'w') as o:
        o.write(f"# 0x{va:08X} -- {en if en != '-' else m['module'] + '::' + m['name']}\n\n")
        o.write(f"- Manifest label: `{m['module']}::{m['name']}`. Ego_d name: `{en}`; Ego_d object: `{ob}`.\n")
        o.write(f"- Retail extent: VA 0x{va:08X}, {ln} bytes (file offset 0x{va - 0x400000:08X} .. 0x{va - 0x400000 + ln:08X}). "
                f"Cut it with `python3 tools/cut_row.py Fable.exe 0x{va:08X} {ln} --dis`.\n")
        o.write(f"- Best grade on record: {grade}" + (" (never graded: the draft is a starting point only and the length is an estimate to the next function start; find the true end first)" if grade == 'UNGRADED' else '') + "\n")
        o.write(f"\n## Where it stands (UNMATCHED_ROWS.tsv)\n{summary}\n")
        ntu = names_to_use(a); open(f'{d}/NAMES_TO_USE.tsv', 'w').write('\n'.join(ntu) + '\n')
        vc = collections.Counter(x.split('\t')[3] for x in ntu[1:] if not x.startswith('NOTE'))
        sv = ntu[1].split('\t')
        if 'NAME CONFLICT' in summary and sv[3] in ('PROVEN', 'PROVEN_RTTI', 'PROVEN_LANDED_WRONG', 'REGISTERED_GUESS'):
            o.write(f"- **NAME CONFLICT settled by names/answers/NAME_ANSWERS.tsv ({sv[3]}):** the self is `{sv[4]}`; the other spelling above is wrong.\n")
        for x in [x for x in ntu if x.startswith('NOTE')]: o.write('- **NAMES_TO_USE.tsv note:** ' + x.split('\t')[5] + '.\n')
        o.write("\n## Names to use (`NAMES_TO_USE.tsv`, from names/answers/NAME_ANSWERS.tsv; rules: docs/NAMING_FIRST_TIME.md)\n"
                f"- {len(ntu) - 1} lines (the row itself + each retail call / jump / data target): " + ', '.join(f'{k} {n}' for k, n in vc.most_common()) + ".\n"
                "- PROVEN / PROVEN_RTTI / REGISTERED_GUESS: spell `use_this_spelling` exactly. PROVEN_LANDED_WRONG: spell the PDB name (fixing the listed landed rows is "
                "optional, NAMING_FIRST_TIME 4). FOLDED: one of the listed PDB names, the one the caller's source used. UNSETTLED / NOT_NAMED_YET: settle it "
                "from the PDB tables before you write the call, list it in NEW_NAMES.tsv, and respell any landed row it contradicts (required).\n")
        for x in ntu[1:]:
            f_ = x.split('\t')
            if f_[3] not in ('PROVEN', 'PROVEN_RTTI', 'REGISTERED_GUESS'): o.write(f"  - {f_[0]} {f_[2]}: **{f_[3]}** " + (f"use `{f_[4]}`" if f_[4] else '') + (f"; landed wrong: {f_[6][:160]}" if f_[6] else '') + (f"; hints: {f_[7][:160]}" if f_[7] else '') + "\n")
        o.write(naming_section(a, en, m))
        o.write(header_section(a, en, m, path))
        o.write(f"\n## Files\n- `best_draft.cpp`: the draft on record (from `{path}`); regrade it first on the package headers (+ the row's overlay). Its names are NOT evidence: check every one against NAMES_TO_USE.tsv.\n")
        for x in bf: o.write((f"- `{x.split(' (', 1)[0]}` ({x.split(' (', 1)[1]}" if ' (' in x else f"- `{x}`") + "\n")
        for x in hf: o.write(f"- `{x}` (notes of the folder the draft came from; search for {a})\n")
        for x in eg: o.write(f"- `{x}`\n")
        o.write("\n`egod_locals.txt` = the source's parameter / local names, types and scope blocks (frame offsets are VC10 /Od, not retail). "
                "`egod_lines.txt` = Ego_d's per-line byte counts (`+offset line bytes`), a VC10 /Od size oracle: calibrate with the VC7.1 /Od size of known "
                "statements (docs/FINDINGS_LOG.md, 2026-11-20/21 calibration lines). `egod_tu.txt` = the row's object file in Ego_d order. "
                "`ego_r_proc.txt` / `fablewin_proc.txt` = the function's records in Ego_r (VS 2010 game release) and FableWin (VS 2010 editor "
                "debug): signature, parameter / local names and types, S_CALLSITEINFO call signatures. Naming and type evidence only; a local "
                "Ego_r lacks was optimised away, not absent from the source (Ego_d decides locals).\n")
        o.write("\n## Callers / references (from the retail exe)\n" + ('\n'.join(callers) if callers else '- none found') + "\n")
        o.write("\n## Landed neighbours (usually the same TU: reuse their includes, opt-ins, pragmas, flags)\n" + '\n'.join(nl) + "\n")
        if pl: o.write("\n## Earlier files on record for this address\n" + '\n'.join(pl) + "\n")
open(f'{OUT}/ROWS.tsv', 'w').write('\n'.join(rowsout) + '\n')
# pass 30: the landed rows a RESPELL unit may edit (binders of the rows' non-proven targets), with their tests, as they stand at BASE
nl_ = 0
os.makedirs(f'{OUT}/landed', exist_ok=True)
with open(f'{OUT}/landed/INDEX.tsv', 'w') as ix:
    ix.write('address\tsource\ttest\tflags\tlock\n')
    for b_ in sorted(NEED_LANDED):
        e_ = ent.get(b_)
        if not e_: continue
        src = f"{R}/rebuild/src/compiled/{e_['Source']}"; tst = f"{R}/rebuild/tests/{e_.get('TestSource', '')}"
        if not os.path.exists(src): continue
        os.makedirs(f'{OUT}/landed/{b_}', exist_ok=True); shutil.copy(src, f'{OUT}/landed/{b_}/{b_}.cpp')
        if e_.get('TestSource') and os.path.exists(tst): shutil.copy(tst, f'{OUT}/landed/{b_}/{b_}_test.cpp')
        lk_ = open(f'{S}/w22/nflocks/{b_}/owner').read().strip() if os.path.exists(f'{S}/w22/nflocks/{b_}/owner') else ''
        ix.write(f"{b_}\t{e_['Source']}\t{e_.get('TestSource', '')}\t{e_.get('CompilerFlags', '')}\t{'' if lk_ == 'released' else lk_}\n"); nl_ += 1
print('landed rows shipped for respells', nl_)
BASE = subprocess.run(['git', '-C', R, 'rev-parse', '--short=10', 'HEAD'], capture_output=True, text=True).stdout.strip()

def cp(src, dst):
    if os.path.exists(src): os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copy(src, dst)
    else: print('MISSING', src); MISSING.append(src)
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
cp(f'{P}/batches/2026-11-21_landing_p25/README.md', f'{OUT}/lessons/codex_pass25_landing_README.md')
cp(f'{P}/pending/2026-11-21_codex_pass25_open/held/HELD.md', f'{OUT}/lessons/codex_pass25_HELD_pickany.md')
# pass 26: the landing README and the five open rows' notes
cp(f'{P}/batches/2026-11-22_landing_p26/README.md', f'{OUT}/lessons/codex_pass26_landing_README.md')
for f in sorted(glob.glob(f'{P}/pending/2026-11-22_codex_pass26_open/*/NOTES.md')):
    cp(f, f"{OUT}/lessons/p26_open_notes/{f.split('/')[-2]}__NOTES.md")
cp(f'{P}/pending/2026-11-22_codex_pass26_open/LESSONS.md', f'{OUT}/lessons/p26_open_notes/LESSONS.md')
# pass 27: the landing README, the next-package questions, the Fable 5.2 authoring landings of today
cp(f'{P}/batches/2026-11-22_landing_p27/README.md', f'{OUT}/lessons/codex_pass27_landing_README.md')
cp(f'{P}/batches/2026-11-22_landing_p27/logs/inline_devices_without_marker.txt', f'{OUT}/lessons/codex_pass27_inline_devices_without_marker.txt')
cp(f'{P}/pending/NEXT_CODEX_QUESTIONS.md', f'{OUT}/lessons/NEXT_CODEX_QUESTIONS.md')
for b in ('2026-11-22_landing_auth24_createeffectonhit', '2026-11-22_landing_auth23_playallsoundevents', '2026-11-22_landing_nf235'):
    cp(f'{P}/batches/{b}/README.md', f'{OUT}/lessons/{b}.md')
# pass 28: why each row was stuck and how Codex unstuck it (lessons + authoring checklist; next passes copy this line forward)
cp(f'{R}/docs/pipeline/CODEX_P28_LESSONS.md', f'{OUT}/lessons/CODEX_P28_LESSONS.md')
cp(f'{P}/batches/2026-11-22_landing_p28/README.md', f'{OUT}/lessons/codex_pass28_landing_README.md')
for fn in ('PER_ROW.md', 'PER_ROW.tsv', 'CAUSE_MATRIX.tsv'): cp(f'{P}/batches/2026-11-22_landing_p28/lessons/{fn}', f'{OUT}/lessons/p28_{fn}')
cp(f'{HERE}/NAMING_AND_HEADERS.md', f'{OUT}/docs/NAMING_AND_HEADERS.md')
for f in glob.glob(f'{P}/pending/2026-11-18_codex_pass24/return/*.md'): cp(f, f"{OUT}/lessons/codex_pass24_return__{os.path.basename(f)}")
# tools: row cutter, xref, Ego_d extractors, and the c1xx / c2 compile-order harness (gdb under Wine)
for p in ('cut_row.py', 'xref.py', 'egod_locals.py', 'egod_type.py', 'eh_parent.py', 'egod_proc.py', 'egod_lines.py', 'pickany_vs_retail.py',
          'pdb_proc.py', 'repack4k.py', 'fwdis.py', 'fwsym.py', 'select_rows.py', 'prep_evidence.py', 'name_answers.py'): cp(f'{HERE}/tools/{p}', f'{OUT}/tools/{p}')
shutil.copytree(f'{P}/pending/2026-11-20_c2_symbol_order', f'{OUT}/tools/c2_symbol_order_kit')
# headers: today's include tree (every landing through this commit)
shutil.copytree(f'{R}/rebuild/include', f'{OUT}/include')
# names (pass 30): four folders, so evidence and hints never mix (owner 2026-11-27: "the files and tables that contain accurate names")
#   answers/  what to spell: NAME_ANSWERS.tsv (one line per address landed code names, with its verdict), the registered best guesses,
#             the proven data spellings, the anonymous-namespace rule note
#   pdb/      the PDB-derived tables (Ego_r / FableWin / Ego_d names, publics, signatures, layouts, modules, source maps, RTTI slots, FSE)
#   state/    what the tree binds today (binding table, landed selves, open queues, locks): read it to find the landed rows your names touch
#   hints/    labels that are NOT evidence (manifest / catalog labels, Ghidra label files, landed source paths)
cp(ANS_PATH, f'{OUT}/names/answers/NAME_ANSWERS.tsv')
for p in ('rebuild/oracles/descriptive_names.tsv', 'rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv'):
    cp(f'{R}/{p}', f'{OUT}/names/answers/{os.path.basename(p)}')
for p in ['ghidra_out/egor_pdb_names.tsv', 'ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egod_pdb_names.tsv', 'ghidra_out/egod_carved_names.tsv',
          'ghidra_out/egod_carved_functions.tsv', 'ghidra_out/egod_pdb_func_names.tsv', 'ghidra_out/egor_obj_functions.tsv',
          'ghidra_out/fablewin_obj_functions.tsv', 'ghidra_out/egor_publics_noproc.tsv', 'ghidra_out/fablewin_publics_noproc.tsv',
          'ghidra_out/egor_signatures.tsv', 'ghidra_out/egor_srcfiles.tsv', 'ghidra_out/srcmap_ego_r.exe.tsv', 'ghidra_out/fablewin_editor_symbols.tsv',
          'ghidra_out/cgsi_vtable_slots.tsv', 'ghidra_out/struct_layouts_egor.tsv', 'ghidra_out/pdb_class_keys.tsv', 'ghidra_out/fable_types.h',
          'ghidra_out/naming_stragglers/Fable.exe.signatures.tsv', 'rebuild/manifest/rtti-vtables.tsv', 'refs/fse_api_manifest.json',
          'rebuild/oracles/library_signatures.tsv', 'rebuild/oracles/vendor_signatures.tsv']:
    cp(f'{R}/{p}', f'{OUT}/names/pdb/{os.path.basename(p)}')
for p in ('Ego_r.modules.txt', 'FableWin.modules.txt', 'Ego_r.publics.txt', 'FableWin.publics.txt'): cp(f'{PDBX}/{p}', f'{OUT}/names/pdb/{p}')
for p in ('rebuild/oracles/reloc_bindings.tsv', 'rebuild/oracles/self_symbols.tsv', 'rebuild/oracles/data_vas.tsv',
          'rebuild/candidates/manual/naming/status/OPEN.tsv', 'rebuild/candidates/manual/naming/status/SHARED_PDB.tsv',
          'rebuild/candidates/manual/naming/status/SUMMARY.md', 'rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv',
          'rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv'):
    cp(f'{R}/{p}', f'{OUT}/names/state/{os.path.basename(p)}')
with open(f'{OUT}/names/state/LOCKS.tsv', 'w') as o:   # landed / parked rows another lane holds: never respell them in this pass
    o.write('address\towner\n')
    for d_ in sorted(glob.glob(f'{S}/w22/nflocks/*/owner')):
        ow = open(d_).read().strip()
        if ow != 'released': o.write(f"{d_.split('/')[-2]}\t{ow}\n")
os.makedirs(f'{OUT}/names/hints', exist_ok=True)
with open(f'{OUT}/names/hints/catalog_names.tsv', 'w') as o:
    o.write('address\tname\tlength\n')
    for a in sorted(cat): o.write(f'{a}\t{cat[a][0]}\t{cat[a][1]}\n')
with open(f'{OUT}/names/hints/manifest_names.tsv', 'w') as o:
    o.write('address\tmodule\tname\n')
    for a in sorted(man): o.write(f"{a}\t{man[a]['module']}\t{man[a]['name']}\n")
with open(f'{OUT}/names/hints/landed_sources.tsv', 'w') as o:
    o.write('address\tsource\tflags\n')
    for a in sorted(ent): o.write(f"{a}\t{ent[a]['Source']}\t{ent[a].get('CompilerFlags', '')}\n")
for p in glob.glob(f'{R}/ghidra_out/labels_*.tsv'): cp(p, f'{OUT}/names/hints/labels/{os.path.basename(p)}')
cp(f'{HERE}/NAMES_README.md', f'{OUT}/names/README.md')
# pass 29: the coordinator's naming / header gate and the tracker it mirrors (reference: they run in the coordinator's lane, not standalone)
for p in ('tools/decomp_pipeline/linux_lane/lane_tools/name_gate.py', 'tools/decomp_pipeline/naming_status.py',
          'tools/decomp_pipeline/reloc_identity.py', 'tools/decomp_pipeline/crawl/purity.py', 'tools/decomp_pipeline/crawl/build_data_vas.py',
          'tools/decomp_pipeline/check_portable_spellings.py',
          'rebuild/candidates/manual/pending/2026-11-25_nf425_handback/tools/vtw425.py'):
    cp(f'{R}/{p}', f'{OUT}/tools/reference/{os.path.basename(p)}')
# pass 29: hand-authoring lessons from AUTH26 (two matched rows and the set-aside row)
for b in ('2026-11-24_landing_auth26_u1', '2026-11-24_landing_auth26_u3'): cp(f'{P}/batches/{b}/README.md', f'{OUT}/lessons/{b}.md')
cp(f'{P}/pending/2026-11-25_auth26/BRIEF.md', f'{OUT}/lessons/auth26_BRIEF.md')
cp(f'{P}/pending/2026-11-25_auth26/HISTORY.md', f'{OUT}/lessons/auth26_HISTORY.md')
cp(f'{P}/audits/2026-11-25_baked_address/README.md', f'{OUT}/lessons/baked_address_grade_README.md')
# pass 30: what the pass 29 landing and its name fixes cost (the reason for docs/NAMING_FIRST_TIME.md)
cp(f'{HERE}/NAMING_FIRST_TIME.md', f'{OUT}/docs/NAMING_FIRST_TIME.md')
cp(f'{P}/batches/2026-11-27_landing_p29/README.md', f'{OUT}/lessons/codex_pass29_landing_README.md')
cp(f'{P}/pending/2026-11-25_codex_pass29/CHECKPOINT1_CHECK.md', f'{OUT}/lessons/codex_pass29_CHECKPOINT1_CHECK.md')
cp(f'{P}/batches/2026-11-27_landing_p29/codex/LESSONS.md', f'{OUT}/lessons/codex_pass29_LESSONS.md')
cp(f'{P}/batches/2026-11-27_landing_p29/logs/REGISTRY_REMOVED.tsv', f'{OUT}/lessons/codex_pass29_REGISTRY_REMOVED.tsv')
for fn in ('README.md', 'STATUS.tsv', 'RULINGS_NEEDED.tsv', 'REJECTED.tsv'):
    cp(f'{P}/pending/2026-11-27_nf479_handback/{fn}', f'{OUT}/lessons/codex_pass29_name_fixes_NF479_{fn}')
LINK = os.environ.get('EGOD_PDB_LINK', '(ask the maintainer for the Ego_d.pdb link)')
for p in ('PROMPT_CODEX.md', 'README_CODEX.md'): open(f'{OUT}/{p}', 'w').write(open(f'{HERE}/{p}').read().replace('@@EGOD_PDB_LINK@@', LINK).replace('@@BASE_COMMIT@@', BASE))
for p in ('names/README.md', 'docs/NAMING_FIRST_TIME.md'): open(f'{OUT}/{p}', 'w').write(open(f'{OUT}/{p}').read().replace('@@BASE_COMMIT@@', BASE))
print('rows', len(rowsout) - 1)
if MISSING: raise SystemExit('build FAILED: %d missing source(s): %s' % (len(MISSING), ' | '.join(MISSING)))
