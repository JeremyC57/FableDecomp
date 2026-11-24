#!/usr/bin/env python3
"""audit_run.py <addrs.txt> <set-name> <results.jsonl> <workdir> [--max N] [--control-from=<flagged results.jsonl>]

--control-from: the /d2inl control. Drops every /d2inl* flag from the catalog flags and passes pk_all the addresses the flagged
run mapped through sources a / a2 / o (one per copy) as overrides (source o), since the stripped leaf usually no longer grades.

Compile each landed row exactly as recheck.py + vland --recheck-landed do (recheck.py builds the payload: catalog CompilerFlags
over BASE, the /Oy- rule, the Leaf / mangled leaf, the funclet offset; vland compiles V.vc71(source) once with those flags through
V.parity_of with a 180 s tool limit; no pragma / flag sweep here: a row that does not grade at its catalog flags is recorded with
its grade), then run pk_all.check and the official pickany_vs_retail.py on the object. One JSON line per row, flushed; rows
already in <results.jsonl> are skipped (resumable). The row's .cpp / .obj are deleted after each row (inside <workdir> only).
Run with python3 -I -B, lane env sourced, TMPDIR / TMP unset."""
import json, os, re, subprocess, sys, time
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'decomp_pipeline')); sys.path.insert(0, HERE); sys.dont_write_bytecode = True
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

def safe_rm(root, p):                    # naming_batch/clean.py's rule: only strictly inside the agent work folder
    root = os.path.realpath(root).rstrip('/') + '/'
    ap = os.path.abspath(p); tgt = os.path.realpath(os.path.dirname(ap)).rstrip('/') + '/' + os.path.basename(ap)
    if root.count('/') >= 4 and tgt.startswith(root) and tgt.rstrip('/') + '/' != root and (os.path.isfile(tgt) or os.path.islink(tgt)):
        os.unlink(tgt)

def main():
    addrf, setname, outp, work = sys.argv[1:5]
    mx = int(sys.argv[sys.argv.index('--max') + 1]) if '--max' in sys.argv else 10 ** 9
    ctl = next((x.split('=', 1)[1] for x in sys.argv if x.startswith('--control-from=')), None)
    prior = {}
    if ctl:
        for l in open(ctl):
            r = json.loads(l); prior[r['row']] = r
    work = os.path.abspath(work); os.makedirs(work, exist_ok=True)
    want = [l.strip().lower() for l in open(addrf) if l.strip()]
    done = set()
    if os.path.exists(outp):
        for l in open(outp):
            try: done.add(json.loads(l)['row'])
            except Exception: pass
    todo = [a for a in want if a not in done][:mx]
    if not todo: print('nothing to do'); return
    pdir = os.path.join(work, 'payload'); os.makedirs(pdir, exist_ok=True)
    r = subprocess.run([sys.executable, '-I', '-B', 'tools/decomp_pipeline/linux_lane/lane_tools/recheck.py', pdir] + todo,
                       cwd=ROOT, capture_output=True, text=True)
    pay = {e['address']: e for e in json.load(open(os.path.join(pdir, 'payload.json')))['result']['authored']}
    orc = {}
    for l in open(os.path.join(pdir, 'oracle.tsv')).read().splitlines()[1:]:
        f = l.split('\t'); orc[f[0].lower()] = f
    ps = open(os.path.join(ROOT, 'rebuild/build_candidates.ps1'), encoding='utf-8-sig').read()
    ent = {}
    for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
        kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
        if 'Address' in kv: ent[kv['Address'].lower()] = kv
    e = V.env(); wd = Path(work) / 'obj'; wd.mkdir(exist_ok=True)
    with open(outp, 'a') as out:
        for a in todo:
            t0 = time.time(); kv = ent.get(a, {})
            rec = dict(row=a, set=setname, catalog_flags=kv.get('CompilerFlags', ''), compiler=kv.get('Compiler', ''))
            c = pay.get(a)
            if not c:
                rec.update(grade='NO_PAYLOAD'); out.write(json.dumps(rec) + '\n'); out.flush(); continue
            src0 = V.vc71(c['source_cpp']); leaf = c.get('leaf'); flags = list(c.get('flags') or [])
            ov = None
            if ctl:
                flags = [f for f in flags if not f.startswith('/d2inl')]
                ov = {}
                for x in prior.get(a, {}).get('copies', []):
                    if x['address'] != '-' and set(x['sources'].split(',')) & {'a', 'a2', 'o'} and not x['status'].startswith('ADDR_CONFLICT'):
                        ov.setdefault(x['symbol'], int(x['address'], 16))
                rec['overrides'] = len(ov)
            rec.update(flags=' '.join(V.BASE_FLAGS + flags), leaf_given=leaf)
            k = purity.classify(src0)
            if k == 'toolchain_library' or not purity.is_genuine(src0):
                rec.update(grade='SKIP_' + k); out.write(json.dumps(rec) + '\n'); out.flush(); continue
            retail = bytes.fromhex(orc[a][3])
            ext = V.funclet_extractor(int(c['funclet_offset']), len(retail), retail) if c.get('funclet_offset') is not None else None
            try:
                st, built, sec = V.parity_of(src0, a, leaf, retail, wd, e, extra=flags, text_extractor=ext)
            except Exception as ex:
                st = f'ERR_{type(ex).__name__}'
            rec['grade'] = st
            obj = wd / f'{a}.obj'
            if obj.exists() and st not in ('SRC_FAIL',):
                try:
                    res = pk_all.check(str(obj), a, leaf=leaf, funclet=ext is not None,
                                       use_leaf_relocs=st.startswith(('MATCH', 'RELOCATION')), overrides=ov)
                    rec.update(leaf=res['leaf'], source_a=res['leaf_ok'], note=res['leaf_note'], copies=res['lines'])
                except Exception as ex:
                    rec.update(pk_error=f'{type(ex).__name__}: {ex}'[:400])
                o = subprocess.run([sys.executable, '-I', '-B', OLD, os.environ['FABLE_EXE'], TAB, str(obj), a], capture_output=True, text=True)
                rec['old'] = [l for l in o.stdout.splitlines() if l.strip()]
                if o.returncode: rec['old_error'] = o.stderr.strip().splitlines()[-1:] if o.stderr.strip() else ['rc %d' % o.returncode]
            for p in (wd / f'{a}.obj', wd / f'{a}.cpp'): safe_rm(work, str(p))
            rec['secs'] = round(time.time() - t0, 1)
            out.write(json.dumps(rec) + '\n'); out.flush()
            print(a, st, len(rec.get('copies', [])), rec['secs'], flush=True)

if __name__ == '__main__':
    main()
