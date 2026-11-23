# gen_ren.py <addr...>: RENAMES section-2 lines (caller old new callee) and self-DEF renames from the HEAD v new objects
# (work/obj/H_<a>.obj v work/obj/<a>.obj), pairing each removed external symbol with the added one of the same name prefix
# (up to the first '@'; ctors / operators / templates included; ?A0x path-hash symbols skipped);
# the callee address comes from ENTRIES (pdb_spelling == new symbol). Appends to work/ren2_auto.tsv / work/self_auto.tsv
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF304'
sys.path.insert(0, A + '/tools'); import nf
E = {e[6]: e[0] for e in nf.entries()}
r2 = open(A + '/work/ren2_auto.tsv', 'a'); s1 = open(A + '/work/self_auto.tsv', 'a')
for a in sys.argv[1:]:
    h = nf.ext_syms(A + '/work/obj/H_%s.obj' % a); n = nf.ext_syms(A + '/work/obj/%s.obj' % a)
    old = sorted(h - n); new = sorted(n - h)
    for o in old:
        k, s = o[:3], o[4:]
        if '?A0x' in s: continue   # anonymous-namespace hash: derived from the compile path (HEAD and new compile in different dirs)
        pre = s[:s.index('@') + 1]
        c = [x for x in new if x[:3] == k and x[4:].startswith(pre) and '?A0x' not in x]
        if len(c) != 1: print('UNPAIRED', a, o, c); continue
        t = c[0][4:]; tgt = E.get(t, '?')
        if k == 'UND': print('0x%s\t%s\t%s\t0x%s' % (a.upper(), s, t, tgt.upper()), file=r2)
        else: print('0x%s\t%s\t%s' % (a.upper(), s, t), file=s1)
        print(k, a, s, '->', t, tgt)
