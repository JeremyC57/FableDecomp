# mkstatus349.py: STATUS.tsv from work/eval.tsv and the batch membership (batches/<b>/rows.txt).
import os, glob
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
bat = {}
for p in glob.glob(A + '/batches/*/rows.txt'):
    for l in open(p):
        if l.strip(): bat[l.strip()] = os.path.basename(os.path.dirname(p))
ev = [l.rstrip('\n').split('\t') for l in open(A + '/work/eval.tsv')][1:]
out = ['address\tstatus\tbatch\tfixed_getdef\tremain_getdef\tobjsecHN\tobjsecRN\tpragscan\tnote']
for e in ev:
    a, c = e[0], e[1]
    st = 'done' if c == 'ACCEPT' and a in bat else 'skipped' if c != 'ACCEPT' else 'accept-unbatched'
    note = {'ACCEPT': 'REREAD + size-body opt-in(s): every leaf-called NEQ GetDef<T> copy now EQ (both checkers); grade unchanged; leaf identical',
            'LEAF_REREAD': 'out of every batch (brief): REREAD changes the leaf section; measured only, see NOTES'}.get(c, c + ' ' + e[11][:200])
    out.append('\t'.join([a, st, bat.get(a, '-'), e[5], e[6], e[8], e[9], e[10], note]))
open(A + '/STATUS.tsv', 'w').write('\n'.join(out) + '\n'); print(len(out) - 1)
