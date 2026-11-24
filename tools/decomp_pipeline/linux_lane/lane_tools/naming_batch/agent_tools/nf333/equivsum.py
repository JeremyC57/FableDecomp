# equivsum.py <equiv tsv> <my_hdrs.txt>: DONE line, DIFF / ERROR counts, and per edited header the number of landed files the planted
# control flags (CTL); for a header with 0 CTL, the count of landed sources/tests that include it by name (grep) as a cross-check.
import sys, collections, subprocess
L = open(sys.argv[1]).read().splitlines(); H = open(sys.argv[2]).read().split()
done = [l for l in L if l.startswith('#DONE')]; print(done[0] if done else 'NOT DONE')
print('DIFF', sum('\tDIFF\t' in l for l in L), 'ERROR', sum('\tERROR\t' in l for l in L))
c = collections.Counter()
for l in L:
    p = l.split('\t')
    if len(p) > 3 and p[2] == 'CTL':
        for h in p[3].split(','): c[h] += 1
for h in H:
    n = c.get(h, 0)
    g = int(subprocess.run('grep -rl "engine/%s.h" /home/user/FableDecomp/rebuild/src /home/user/FableDecomp/rebuild/tests /home/user/FableDecomp/rebuild/include | wc -l' % h, shell=True, capture_output=True, text=True).stdout)
    print('%-55s CTL %3d   direct-includers(grep) %d' % (h, n, g))
