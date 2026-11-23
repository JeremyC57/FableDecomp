# vslot.py <addr..>: every (class, vtable, slot) of rebuild/manifest/rtti-vtables.tsv whose slot function is the address, plus the
# labels_rtti_port.tsv name (NF317 w22: the slot a local model's `virtual` declaration must take)
import sys
R = '/home/user/FableDecomp'
V = [l.rstrip('\n').split('\t') for l in open(R + '/rebuild/manifest/rtti-vtables.tsv')][1:]
N = dict(l.rstrip('\n').split('\t')[:2] for l in open(R + '/ghidra_out/labels_rtti_port.tsv') if '\t' in l)
for a in sys.argv[1:]:
    a = a.lower().replace('0x', '')
    print(a, N.get(a, '-'), '; '.join('%s %s slot %s' % (c, v, s) for c, v, s, f in V if f.lower() == a) or 'NO_SLOT')
