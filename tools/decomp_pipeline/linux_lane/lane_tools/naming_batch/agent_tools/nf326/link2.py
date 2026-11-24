# link2.py (NF326) <tag> <obj...>: a real link of one object set, LINK= cleared, /FORCE:UNRESOLVED only (so a duplicate definition is a
# hard error, not a /FORCE warning), /MAP so the kept copy of every pick-any COMDAT is visible. Writes evidence/link2_<tag>.txt:
# the LNK2005 / LNK4006 / LNK1169 lines (duplicates), the exit code, the count of unresolved externals (by name), and for each symbol in
# work/watch.txt the object the map says the linker kept. LINKMODE=force uses /FORCE instead (duplicates become warnings, the image and
# map are still written), for sets that carry pre-existing duplicates (the ~CScriptThing strong definers).
# Object lists longer than the command line go through an @response file (work/lk2_<tag>.rsp). Outputs are removed with the toolkit's clean.py.
import sys, re, subprocess, collections
from pathlib import Path
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V
A = Path(__file__).resolve().parent.parent
tag, objs = sys.argv[1], sys.argv[2:]
e = V.env(); e.pop('LINK', None)
dll = A / 'work' / ('lk2_' + tag)
rsp = A / 'work' / ('lk2_%s.rsp' % tag)
rsp.write_text('\n'.join(V.wpath(o) for o in objs) + '\n')
r = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/DLL', '/NOENTRY', ('/FORCE' if __import__('os').environ.get('LINKMODE') == 'force' else '/FORCE:UNRESOLVED'), '/NODEFAULTLIB', '/OPT:NOREF', '/MAP:%s.map' % V.wpath(str(dll)),
                                          '/out:%s.dll' % dll, '@' + V.wpath(str(rsp))], e)
t = (r.stdout or '') + (r.stderr or '')
out = open(A / 'evidence' / ('link2_%s.txt' % tag), 'w')
print('## link2 %s: %s' % (tag, ' '.join(Path(o).name for o in objs)), file=out)
print('exit', r.returncode, file=out)
print('raw:', ' | '.join(l.strip() for l in t.splitlines() if l.strip() and 'LNK2001' not in l and 'LNK2019' not in l)[:3000], file=out)
dups = [l for l in t.splitlines() if re.search(r'LNK2005|LNK4006|LNK1169', l)]
print('duplicate lines', len(dups), file=out)
for l in dups: print('  ' + l.strip()[-300:], file=out)
u = collections.Counter()
for l in t.splitlines():
    m = re.search(r'(LNK2001|LNK2019): unresolved external symbol (?:"[^"]*" \((\S+)\)|(\S+))', l)
    if m: u[m.group(2) or m.group(3)] += 1
print('unresolved symbols', len(u), file=out)
for s, c in sorted(u.items()): print('UNRESOLVED', c, s, file=out)
mp = Path(str(dll) + '.map')
if mp.exists():
    M = mp.read_text(errors='replace')
    for w in [l.strip() for l in open(A / 'work/watch.txt') if l.strip()]:
        hits = [l.split()[-1] for l in M.splitlines() if (' ' + w + ' ') in (l + ' ')]
        print('KEPT', w, ' '.join(hits) or '-', file=out)
out.close()
subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', str(A)] +
               [str(dll) + x for x in ('.dll', '.lib', '.exp', '.map')], capture_output=True)
print(open(A / 'evidence' / ('link2_%s.txt' % tag)).read())
