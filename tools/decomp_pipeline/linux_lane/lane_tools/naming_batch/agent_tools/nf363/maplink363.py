# maplink363.py <tag> <obj>...: /FORCE /MAP link (LINK cleared) of the objects in the given order; prints, for each watched symbol, the
# object the linker kept its definition from (the map's lib:object column). Shows which pick-any body wins a first-wins race. (NF363)
import sys, re, subprocess
from pathlib import Path
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF363')
tag, objs = sys.argv[1], sys.argv[2:]
W = ['??0CTCNoiseDef@@QAE@XZ', '??_GCTCNoiseDef@@UAEPAXI@Z', '??0CThingNoiseDef@@QAE@XZ', '??_GCThingNoiseDef@@UAEPAXI@Z', '??_7CTCNoiseDef@@6B@',
     '??_7CThingNoiseDef@@6B@', '?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ', '??1CThingNoiseDef@@UAE@XZ']
e = V.env(); e.pop('LINK', None); out = A / 'work' / ('map_%s' % tag)
r = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/DLL', '/NOENTRY', '/FORCE', '/NODEFAULTLIB', '/OPT:NOREF', '/MAP', '/out:%s.dll' % out] + objs, e)
mp = Path('%s.map' % out).read_text(errors='replace') if Path('%s.map' % out).exists() else ''
with open(A / 'evidence' / ('maplink_%s.txt' % tag), 'w') as f:
    print('## link order:', ' '.join(Path(o).name for o in objs), file=f)
    for s in W:
        m = [l.split()[-1] for l in mp.splitlines() if (' ' + s + ' ') in (l + ' ')]
        print('%s\tkept from: %s' % (s, ','.join(m) or 'absent'), file=f)
print(r.stdout[-800:] if r.stdout else "", file=open(str(out) + ".linkout", "w"))
print('MAP_DONE', tag)
