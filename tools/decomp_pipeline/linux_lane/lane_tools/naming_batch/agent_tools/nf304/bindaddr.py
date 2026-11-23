# bindaddr.py <objdir> <target addr> <row addr...>: what each row's object binds at the target address (nf.py xr), one line per row
import sys, io, contextlib, os
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF304/tools'); import nf
od, t = sys.argv[1], sys.argv[2].lower()
for r in sys.argv[3:]:
    ob = os.path.join(od, r + '.obj')
    if not os.path.exists(ob): print(r, 'NOOBJ'); continue
    out = io.StringIO()
    with contextlib.redirect_stdout(out):
        try: nf.cmd_xr(r, ob)
        except Exception as ex: print('XR_ERR', ex)
    b = [l.split('\t')[1] + ' ' + l.split('\t')[2] for l in out.getvalue().splitlines() if l.startswith(t)]
    print(r, ' | '.join(b) or '-')
