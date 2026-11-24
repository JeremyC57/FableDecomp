# addpre.py <addr> [-d DEF ...] [-i "include" ...] [-r old new]: insert #define lines after the last FABLE #define and #include lines
# before the row's first #include of an engine/ class header (rows/<addr>.cpp, in place); -r replaces a literal once.
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF320'
a = sys.argv[1]; p = A + '/rows/%s.cpp' % a; s = open(p).read(); L = s.split('\n')
defs, incs, reps = [], [], []; k = 2; args = sys.argv
while k < len(args):
    if args[k] == '-d': defs.append(args[k + 1]); k += 2
    elif args[k] == '-i': incs.append(args[k + 1]); k += 2
    elif args[k] == '-r': reps.append((args[k + 1], args[k + 2])); k += 3
    else: raise SystemExit('bad arg ' + args[k])
di = max([i for i, l in enumerate(L) if re.match(r'\s*#\s*define\s+FABLE_', l)] or [-1])
for d in reversed(defs):
    if not re.search(r'^\s*#\s*define\s+' + d + r'\b', s, re.M): L.insert(di + 1, '#define ' + d)
ii = next(i for i, l in enumerate(L) if re.match(r'\s*#\s*include\s+"engine/', l))
for inc in reversed(incs):
    if ('#include "%s"' % inc) not in s: L.insert(ii, '#include "%s"' % inc)
s = '\n'.join(L)
for o, n in reps:
    assert o in s, (a, o); s = s.replace(o, n, 1)
open(p, 'w').write(s); print(a, 'ok')
