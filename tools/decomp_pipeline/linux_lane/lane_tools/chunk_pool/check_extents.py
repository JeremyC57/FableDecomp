#!/usr/bin/env python3
"""Settle the "length uncertain" rows of the never-attempted analysis: the extent scan stopped before the next start over
bytes that are not filler. For each row: what stopped the scan (noreturn call / call-then-int3 / tail jump / ret with an
unreached tail), what the tail decodes to, who references the tail (branches, calls, data refs from inside the row or
elsewhere), and whether a scan started at the tail ends cleanly at the next start.

    export S=<lane scratch>; python3 .../check_extents.py  -> pending/never_attempted/EXTENTS.tsv, updates CHUNKS.tsv lengths

Verdicts: EXTEND (the tail is this function's own cold code: a catch/SEH handler, a continuation after a call the scanner
took for noreturn, or a block only a jump table reaches; length = size to the padding before the next start),
SPLIT (the tail is a separate function: referenced from outside, starts after a terminator with a prologue; length = extent;
the tail start is reported as a new function), DATA (tail is a jump table / constant pool already inside the row: length =
extent), KEEP (tail unreferenced non-code junk after the ret: length = extent), MANUAL (nothing decides it).
"""
import sys, os, re, csv, bisect, collections
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'bounds'))
from extent import Exe, fmt, is_filler, ROOT
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
D = ROOT + '/rebuild/candidates/manual/pending/never_attempted'
R = list(csv.DictReader(open(D + '/ANALYSIS.tsv'), delimiter='\t'))
unc = [r for r in R if 'length uncertain' in r['reason']]
E = Exe(); md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = False
PROLOG = re.compile(r'^(push|sub|mov|xor|cmp|test|lea|fld|fild|and|or|inc|dec)$')
# landed-row names for the noreturn targets
cat = {}
for r in csv.DictReader(open(ROOT + '/rebuild/oracles/auto-re-candidates.tsv'), delimiter='\t'): cat[r['address'].lower()] = r['name']
man = {}
for r in csv.DictReader(open(ROOT + '/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t'): man[r['address'].lower()] = r['name']
def strip_fill(va, n):
    b = E.read(va, n); k = n
    while k > 0 and b[k - 1] in (0xCC, 0x90): k -= 1
    return k
out = []
for r in unc:
    va = int(r['address'], 16); size = int(r['size']); ext = int(r['extent'])
    ns = E.next_start(va); tail = va + ext; tail_len = (ns - tail) if ns else 0
    sc = E.scan(va); f = sc['flags']
    stop = next((k for k in ('noret_call', 'noret_iat', 'call_then_int3', 'call_then_start', 'tailcall', 'hit_int3', 'indirect_jmp', 'vjmp') if f.get(k)), 'ret')
    stop_t = f.get('noret_call', f.get('call_then_int3', ['']))[0] if stop in ('noret_call', 'call_then_int3') else ''
    stop_name = (cat.get('%08x' % stop_t) or man.get('%08x' % stop_t) or '') if isinstance(stop_t, int) else (f.get('noret_iat', [''])[0] if stop == 'noret_iat' else '')
    # the last reached instruction before the tail
    last = max(sc['seen']); li = E.insn(last); last_mn = li[2] if li else '?'
    # tail decode
    tb = E.read(tail, min(tail_len, 4096)); ins = list(md.disasm_lite(tb, tail))
    tfirst = ins[0][2] + ' ' + ins[0][3] if ins else '?'
    tail_code_len = sum(i[1] for i in ins)
    # references into the tail
    inside = collections.Counter(); outside = collections.Counter()
    for a in range(tail, tail + tail_len):
        for s in E.call_src.get(a, ()): (inside if va <= s < tail else outside)['call'] += 1
        for s in E.jmp_src.get(a, ()): (inside if va <= s < tail else outside)['jmp'] += 1
        for sec, s in E.abs_src.get(a, ()):
            if sec == '.text' and va <= s < tail: inside['imm'] += 1
            else: outside['data' if sec != '.text' else 'imm'] += 1
    # short jcc (rel8) from inside the row into the tail are not indexed: scan the row's reached instructions
    jcc_in = 0
    for a in sc['seen']:
        i = E.insn(a)
        if i and i[2] != 'jmp' and (i[2].startswith('j') or i[2].startswith('loop')) and i[3].startswith('0x') and tail <= int(i[3], 16) < tail + tail_len: jcc_in += 1
    # jump tables inside the row whose entries point into the tail
    jt_in = 0
    for (tv, tl) in sc['tables']:
        for k in range(0, tl, 4):
            v = E.dword(tv + k)
            if v and tail <= v < tail + tail_len: jt_in += 1
    # scan from the tail
    ts = E.scan(tail); tf = ts['flags']; tend = tail + ts['extent']
    tail_clean = ns is not None and tend <= ns and (tend == ns or is_filler(E.read(tend, ns - tend)))
    after_term = E.after_terminator(tail)
    eh = bool(f.get('eh_catch') or f.get('seh_scope') or f.get('eh_continuation'))
    # verdict: strip the filler that begins the tail first -- [row][int3 pad][unlisted function] is the common shape
    tb0 = E.read(tail, min(tail_len, 64)); pad = 0
    while pad < len(tb0) and tb0[pad] in (0xCC, 0x90): pad += 1
    tail2 = tail + pad; ins2 = list(md.disasm_lite(E.read(tail2, min(tail_len - pad, 64)), tail2, 3)) if tail_len > pad else []
    t2first = (ins2[0][2] + ' ' + ins2[0][3]) if ins2 else ''
    full_len = strip_fill(va, size)
    if jcc_in or jt_in or (inside['jmp'] and ext > 6) or (eh and not outside and not pad):
        v, L = 'EXTEND', full_len; why = f'tail reached from inside the row (jcc {jcc_in}, jtable {jt_in}, jmp {inside["jmp"]}, eh {int(eh)})'
    elif pad and tail_len > pad:
        v, L = 'SPLIT', ext; why = f'tail = {pad} filler bytes then an unlisted function at {tail2:08x} ({t2first[:24]}; refs outside {dict(outside)}): row length is the extent'
    elif stop in ('noret_call', 'call_then_int3') and not outside and not pad and ins2 and PROLOG.match(ins2[0][2]) and tail_clean and not after_term:
        v, L = 'EXTEND', full_len; why = f'continuation right after a call the scanner took for noreturn ({stop_name or stop_t}); the tail scan ends cleanly'
    elif ext <= 6 and inside['jmp']:
        v, L = 'SPLIT', ext; why = f'row is a jmp thunk onto the function that follows at {tail2:08x}'
    elif outside['call'] or outside['data'] or outside['imm'] or outside['jmp']:
        v, L = 'SPLIT', ext; why = f'code right after the row end, referenced from outside ({dict(outside)}): a separate function at {tail2:08x} with no padding (same object section)'
    elif not ins2 or tail_code_len < tail_len // 2:
        v, L = 'DATA', ext; why = 'tail does not decode as code (constant pool / table)'
    elif after_term and not inside and ins2 and PROLOG.match(ins2[0][2]):
        v, L = 'SPLIT', ext; why = f'unreferenced function-shaped code right after the terminator at {tail2:08x} (unlisted start reached through a pointer the index does not see)'
    else:
        v, L = 'MANUAL', ext; why = f'unreferenced code right after the row with no padding (a cold block of a neighbour reached by a short jcc, or an unlisted start): row length = extent; stop={stop} pad={pad} tail2={t2first[:24]} outside={dict(outside)} inside={dict(inside)} tail_clean={tail_clean} after_term={after_term}'
    out.append(dict(address=r['address'], module=r['module'], name=r['name'], size=size, extent=ext, tail_len=tail_len, tail_pad=pad, unlisted_start=('%08x' % tail2) if v == 'SPLIT' else '', unlisted_first=t2first[:30], stop=stop, stop_target=('%08x' % stop_t if isinstance(stop_t, int) else stop_t), stop_name=stop_name[:50],
                    last_insn=last_mn, tail_first=tfirst[:40], tail_code_len=tail_code_len, refs_inside=str(dict(inside)), refs_outside=str(dict(outside)), jcc_in=jcc_in, jtable_in=jt_in,
                    tail_scan_clean=int(tail_clean), tail_after_term=int(after_term), eh=int(eh), verdict=v, length=L, why=why, tail_scan=fmt(ts)[:120]))
with open(D + '/EXTENTS.tsv', 'w') as fh:
    w = csv.DictWriter(fh, list(out[0].keys()), delimiter='\t', lineterminator='\n'); w.writeheader(); w.writerows(out)
V = collections.Counter(o['verdict'] for o in out); print(V.most_common())
with open(D + '/UNLISTED_STARTS.tsv', 'w') as fh:
    fh.write('start\tafter_row\trow_name\tgap_to_next_start\tfirst_insn\trefs_outside\n')
    for o in out:
        if o['verdict'] == 'SPLIT' and o['unlisted_start']:
            fh.write(f"{o['unlisted_start']}\t{o['address']}\t{o['name'][:40]}\t{o['tail_len'] - o['tail_pad']}\t{o['unlisted_first']}\t{o['refs_outside']}\n")
print('unlisted starts', sum(1 for o in out if o['verdict'] == 'SPLIT' and o['unlisted_start']))
print('stops', collections.Counter(o['stop'] for o in out).most_common())
# apply to CHUNKS.tsv (length column + reason suffix) and ANALYSIS.tsv
vd = {o['address']: o for o in out}
for fn in ('CHUNKS.tsv', 'ANALYSIS.tsv'):
    rows = list(csv.DictReader(open(f'{D}/{fn}'), delimiter='\t')); cols = list(rows[0].keys()); n = 0
    for x in rows:
        o = vd.get(x['address'])
        if not o: continue
        x['length'] = str(o['length']); x['reason'] = re.sub(r'(; )?scan stops [^;]*length uncertain[^;]*', '', x['reason']).strip('; ') + f'; extent {o["verdict"]}: {o["why"][:70]}'; n += 1
    with open(f'{D}/{fn}', 'w') as fh:
        w = csv.DictWriter(fh, cols, delimiter='\t', lineterminator='\n'); w.writeheader(); w.writerows(rows)
    print(fn, n, 'rows updated')
