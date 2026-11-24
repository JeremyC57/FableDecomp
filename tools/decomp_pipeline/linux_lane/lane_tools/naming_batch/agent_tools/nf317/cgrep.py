# cgrep.py <idx..>: per entry, the census rows (work/census.tsv) whose landed source mentions the method name outside comments, with
# the matching lines (declaration / call), and the lock owner / off-limits flags (NF317 w22: which census rows can bind the symbol by name)
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF317'
sys.path.insert(0, A + '/tools'); import nf
E = nf.entries(); C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
off = set(open(A + '/work/offlimits.txt').read().split()); oth = set(open(A + '/work/other_agents_entries.txt').read().split()) | set(open(A + '/work/other_agents_witnesses.txt').read().split())
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; m = re.match(r'\?(\w+)@', e[3]); meth = m.group(1) if m else e[3]
    print('#%d %s %s' % (i, e[0], meth))
    for r in C.get(i, []):
        p = nf.src_of(r)
        if not p or not p.exists(): print('   %s NOSRC' % r); continue
        hits = []
        for n, l in enumerate(p.read_text(encoding='utf-8', errors='replace').split('\n'), 1):
            c = l.split('//')[0]
            if re.search(r'\b%s\b' % re.escape(meth), c): hits.append('L%d: %s' % (n, c.strip()[:140]))
        fl = ('OFF ' if r in off else '') + ('OTHER_AGENT ' if r in oth else '') + 'lock=' + nf.lock_owner(r)
        print('   %s %s hits=%d' % (r, fl, len(hits)))
        for h in hits[:6]: print('      ' + h)
