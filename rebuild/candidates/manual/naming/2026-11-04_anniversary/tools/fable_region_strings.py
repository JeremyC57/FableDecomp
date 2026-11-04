"""Mark Anniversary ASCII strings as TLC-common or not; emit Anniversary-only strings that sit in
Fable-engine string neighbourhoods (window of neighbours mostly TLC-common).
usage: fable_region_strings.py <ann a.txt> <tlc_a_u.txt> <out.tsv> [window] [threshold]
"""
import sys
ann = [l.rstrip("\n").split("\t", 1) for l in open(sys.argv[1], encoding="latin-1")]
ann = [(int(o, 16), s) for o, s in ann if len(s) >= 6]
tlc = set(l.rstrip("\n") for l in open(sys.argv[2], encoding="latin-1"))
W = int(sys.argv[4]) if len(sys.argv) > 4 else 10
T = float(sys.argv[5]) if len(sys.argv) > 5 else 0.4
flags = [s in tlc for _, s in ann]
pref = [0]
for f in flags: pref.append(pref[-1] + f)
n = len(ann); out = 0
with open(sys.argv[3], "w", encoding="latin-1") as fo:
    for i, (o, s) in enumerate(ann):
        if flags[i]: continue
        a, b = max(0, i - W), min(n, i + W + 1)
        dens = (pref[b] - pref[a]) / (b - a - 1)
        if dens >= T:
            fo.write("%08x\t%.2f\t%s\n" % (o, dens, s)); out += 1
print("ann strings", n, "tlc-common", sum(flags), "ann-only in fable neighbourhoods", out)
