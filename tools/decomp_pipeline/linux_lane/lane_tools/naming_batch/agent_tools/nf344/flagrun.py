#!/usr/bin/env python3
"""flagrun.py <addr> <variant> <flagspec> <out.jsonl>   (agent NF344, 2026-11-24; adapted from NF340's pk_all/audit_run.py)

Compile ONE landed row the way recheck.py + vland --recheck-landed do (recheck.py payload: catalog CompilerFlags over BASE, the /Oy- rule,
the Leaf, the funclet offset; V.parity_of with a 180 s tool limit, no pragma / flag sweep) under one flag variant, then run pk_all.check
and the official pickany_vs_retail.py on the object. Appends one JSON line; keeps the object as work/objs/<addr>_<variant>.obj.

flagspec:  CAT        the catalog flags unchanged (HEAD)
           NOD        the catalog flags with every /d2inl* flag removed
           "<flags>"  the catalog flags with every /d2inl* flag removed, then these flags appended (e.g. "/d2inlT60 /d2inlS15")

Copy addresses: pk_all sources a / a2 (when the leaf grades) plus, always, source o = the addresses NF340's flagged run mapped through
a / a2 / o (work/nf340_d2inl2.jsonl), so a variant whose leaf no longer grades still compares its copies at the right retail addresses.
Every variant compiles at the same path (work/obj/<addr>.cpp) so `?A0x` hashes agree between variants.
Run with python3 -I -B, lane env sourced, TMPDIR / TMP unset, under its own `timeout 900`."""
import json, os, re, shutil, subprocess, sys, time
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF344'
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
PK = os.path.join(ROOT, 'tools/decomp_pipeline/linux_lane/lane_tools/pk_all')
sys.path.insert(0, os.path.join(ROOT, 'tools', 'decomp_pipeline')); sys.path.insert(0, PK); sys.dont_write_bytecode = True
from pathlib import Path
import verify_and_land as V
from crawl import purity
import pk_all

def run_tool(exe, args, e, _t=180):     # vland.py's 180 s limit
    cmd = ([V.WINE] if V.WINE else []) + [str(exe)] + V.tool_args(args)
    try: return subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=e, timeout=_t)
    except subprocess.TimeoutExpired: return subprocess.CompletedProcess(cmd, 124, '', 'timeout')
V.run_tool = run_tool
OLD = os.path.join(ROOT, 'rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py')
TAB = os.path.join(ROOT, 'rebuild/oracles/reloc_bindings.tsv')

def payload(a):
    pdir = os.path.join(A, 'work', 'pay', a)
    pj = os.path.join(pdir, 'payload.json')
    if not os.path.exists(pj):
        os.makedirs(pdir, exist_ok=True)
        subprocess.run([sys.executable, '-I', '-B', 'tools/decomp_pipeline/linux_lane/lane_tools/recheck.py', pdir, a],
                       cwd=ROOT, capture_output=True, text=True)
    c = {e['address']: e for e in json.load(open(pj))['result']['authored']}[a]
    orc = {}
    for l in open(os.path.join(pdir, 'oracle.tsv')).read().splitlines()[1:]:
        f = l.split('\t'); orc[f[0].lower()] = f
    return c, orc[a]

def main():
    a, variant, spec, outp = sys.argv[1].lower(), sys.argv[2], sys.argv[3], sys.argv[4]
    t0 = time.time()
    c, o = payload(a)
    flags = list(c.get('flags') or [])
    if spec != 'CAT':
        flags = [f for f in flags if not f.startswith('/d2inl')]
        if spec != 'NOD': flags += spec.split()
    ov = {}
    for l in open(os.path.join(A, 'work', 'nf340_d2inl2.jsonl')):
        r = json.loads(l)
        if r['row'] != a: continue
        for x in r.get('copies', []):
            if x['address'] != '-' and set(x['sources'].split(',')) & {'a', 'a2', 'o'} and not x['status'].startswith('ADDR_CONFLICT'):
                ov.setdefault(x['symbol'], int(x['address'], 16))
    rec = dict(row=a, variant=variant, spec=spec, flags=' '.join(V.BASE_FLAGS + flags), extra=flags, overrides=len(ov), leaf_given=c.get('leaf'))
    src0 = V.vc71(c['source_cpp']); leaf = c.get('leaf')
    retail = bytes.fromhex(o[3])
    ext = V.funclet_extractor(int(c['funclet_offset']), len(retail), retail) if c.get('funclet_offset') is not None else None
    wd = Path(A) / 'work' / 'obj'; wd.mkdir(exist_ok=True); e = V.env()
    try:
        st, built, sec = V.parity_of(src0, a, leaf, retail, wd, e, extra=flags, text_extractor=ext)
    except Exception as ex:
        st = f'ERR_{type(ex).__name__}'
    rec['grade'] = st
    obj = wd / f'{a}.obj'
    if obj.exists() and st != 'SRC_FAIL':
        keep = Path(A) / 'work' / 'objs'; keep.mkdir(exist_ok=True)
        shutil.copyfile(obj, keep / f'{a}_{variant}.obj')
        try:
            res = pk_all.check(str(obj), a, leaf=leaf, funclet=ext is not None,
                               use_leaf_relocs=st.startswith(('MATCH', 'RELOCATION')), overrides=ov)
            rec.update(leaf=res['leaf'], source_a=res['leaf_ok'], note=res['leaf_note'], copies=res['lines'])
        except Exception as ex:
            rec.update(pk_error=f'{type(ex).__name__}: {ex}'[:400])
        r = subprocess.run([sys.executable, '-I', '-B', OLD, os.environ['FABLE_EXE'], TAB, str(obj), a], capture_output=True, text=True)
        rec['old'] = [l for l in r.stdout.splitlines() if l.strip()]
        if r.returncode: rec['old_error'] = r.stderr.strip().splitlines()[-1:] if r.stderr.strip() else ['rc %d' % r.returncode]
    rec['secs'] = round(time.time() - t0, 1)
    with open(outp, 'a') as out: out.write(json.dumps(rec) + '\n')
    print(a, variant, st, len(rec.get('copies', [])), rec['secs'], flush=True)

if __name__ == '__main__':
    main()
