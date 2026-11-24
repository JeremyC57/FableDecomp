# summ358.py: per-copy summary of evidence/CENSUS_<copy>.tsv -> evidence/SUMMARY_counts.txt (printed too).
import collections, glob, os
E = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF358/evidence'
out = []
for f in sorted(glob.glob(E + '/CENSUS_*.tsv')):
    L = [l.rstrip('\n').split('\t') for l in open(f) if not l.startswith('#') and not l.startswith('row\t')]
    c = os.path.basename(f)[7:15]
    comp = [r for r in L if r[2] == 'yes']; holes = [r for r in L if r[2] == 'no']
    def sel(pk=None, reach=None, pres=None):
        return [r for r in comp if (pk is None or r[7] == pk) and (reach is None or r[10] == reach) and (pres is None or r[5].startswith(pres))]
    neq_y = sel('NEQ', 'y'); neq_n = sel('NEQ', 'n'); eq = sel('EQ'); und = sel(pres='UND'); none = sel(pres='none'); noaddr = sel(pres='DEF(NOADDR')
    pvr_dis = [r for r in comp if r[7] in ('EQ', 'NEQ') and r[8] != r[7]]
    out.append('== 0x%s rows=%d compiled=%d holes=%d owner=%d' % (c.upper(), len(L), len(comp), len(holes), len([r for r in L if r[2] == 'owner'])))
    out.append('  leaf-reached NEQ: %d  [%s]' % (len(neq_y), ' '.join('%s(%s,%s,%s)' % (r[0], r[5][4:], r[11], 'leaf' if r[9] == 'y' else 'via') for r in neq_y)))
    out.append('  unreached NEQ (non-gating): %d  [%s]' % (len(neq_n), ' '.join('%s(%s)' % (r[0], r[11]) for r in neq_n)))
    out.append('  EQ: %d  [%s]' % (len(eq), ' '.join('%s(%s%s)' % (r[0], r[5][4:], '' if r[10] == 'y' else ',unreached') for r in eq)))
    out.append('  UND (call, no copy): %d [%s]  no ref: %d [%s]  DEF but no pk line: %d [%s]' % (len(und), ' '.join(r[0] for r in und), len(none), ' '.join(r[0] for r in none), len(noaddr), ' '.join(r[0] for r in noaddr)))
    out.append('  pk_all v pvr disagreements: %d [%s]' % (len(pvr_dis), ' '.join('%s:%s/%s' % (r[0], r[7], r[8]) for r in pvr_dis)))
    out.append('  emitters not in the retail-caller leg (src only): [%s]' % ' '.join('%s(%s)' % (r[0], r[7]) for r in comp if r[1] == 'src' and r[5].startswith('DEF')))
    out.append('  grades: %s' % dict(collections.Counter(r[3] for r in comp)))
open(E + '/SUMMARY_counts.txt', 'w').write('\n'.join(out) + '\n'); print('\n'.join(out))
