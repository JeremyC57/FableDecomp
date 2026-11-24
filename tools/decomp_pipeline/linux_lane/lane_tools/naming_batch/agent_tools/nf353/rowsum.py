# rowsum.py <addr...>: one line per row -> work/rowsummary.tsv (addr, leafrel, objdiff summary, pk gate, reach, resolve) and the
# STATUS decision inputs. Re-runs objdiff.py, pkcmp346.py, pkreach.py, resolve.py on the current objects. Agent NF353.
import sys, os, re, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353'
ad = sys.argv[1:]
def run(*c): return subprocess.run(['python3', A + '/tools/' + c[0]] + list(c[1:]), capture_output=True, text=True).stdout
od = {l.split()[0]: l.split()[1:] for l in run('objdiff.py', *ad).splitlines() if l.strip()}
pk = run('pkcmp346.py', A + '/evidence/pk', *ad); rc = run('pkreach.py', A + '/evidence/pk', *ad); rs = run('resolve.py', *ad)
out = []
for a in ad:
    o = dict(x.split('=') for x in od[a])
    bad = [l for l in pk.splitlines() if l.startswith(a + '\t') and ('TURNED_NEQ' in l or 'NEWLY_NEQ' in l or 'MISSING' in l)]
    gate = re.search(r'^ROW %s gate=(\w)' % a, pk, re.M).group(1)
    nfx = sum(1 for l in pk.splitlines() if l.startswith(a + '\tpk_all\tDROPPED') and 'leaf-called' in l and 'not-leaf' not in l)
    reach = re.search(r'^ROW %s reach=(\w) flips=(\d+)' % a, rc, re.M)
    res = re.search(r'^%s RESOLVE (\w+) (\d+)' % a, rs, re.M)
    out.append('\t'.join([a, 'identical (data and section-name keyed relocations)' if o['leaf'] == 'SAME' else 'LEAF_DIFF',
        'changed sections %s, dropped COMDATs %s, added %s; external DEF dropped %s / added %s, UND dropped %s / added %s' % (
            o['changed_sections'], o['dropped_sections'], o['added_sections'], o['dropDEF'], o['addDEF'], o['dropUND'], o['addUND']),
        ('PASS (no copy turned NEQ, no new NEQ; %d leaf-called NEQ copies dropped)' % nfx) if gate == 'P' and not bad else 'FAIL ' + ';'.join(bad)[:200],
        '0' if reach.group(1) == 'P' else 'FLIPS %s' % reach.group(2),
        '%s (%s new UND, each defined by the landed owner under the same string)' % (res.group(1), res.group(2))]))
open(A + '/work/rowsummary.tsv', 'w').write('\n'.join(out) + '\n'); print('\n'.join(out))
