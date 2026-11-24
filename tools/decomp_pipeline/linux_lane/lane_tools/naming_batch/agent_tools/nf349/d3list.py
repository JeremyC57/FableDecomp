# d3list.py <pk dir> <prefix>: rows with leaf-called (leafref y or reach y) NEQ GetDef<T> / GetSubDef<T> copies in pk_all TSVs (<prefix>_<addr>.pkall.tsv).
# Prints addr<TAB>n_copies<TAB>copy addresses/types; last line totals.
import sys, os, re, glob
d, pre = sys.argv[1], sys.argv[2]; rows = {}
for p in sorted(glob.glob(os.path.join(d, pre + '_*.pkall.tsv'))):
    a = re.search(pre + r'_([0-9a-f]{8})\.pkall\.tsv$', p).group(1)
    for l in open(p):
        f = l.rstrip('\n').split('\t')
        if len(f) < 10 or l.startswith('#'): continue
        if f[0].startswith('NEQ') and (f[7] == 'y' or f[8] == 'y') and re.match(r'\?\?\$Get(Sub)?Def@', f[9]):
            t = re.match(r'\?\?\$Get(?:Sub)?Def@(.*?)@@@', f[9]).group(1)
            rows.setdefault(a, []).append('%s:%s:%s' % (f[2], t, 'sub' if 'GetSubDef' in f[9] else ('idx' if 'QBE_NJPAV' in f[9] else 'other')))
n = 0
for a in sorted(rows): print('%s\t%d\t%s' % (a, len(rows[a]), ' '.join(rows[a]))); n += len(rows[a])
print('# rows %d copies %d' % (len(rows), n))
