# toy.py <tag> <src> [flags...]: VC7.1 compile of a probe TU (BASE_FLAGS + flags) -> work/toy/<tag>.obj, then compare every
# GetDef<CMaterialDef> / GetDef<CEngineThemeDef> COMDAT it defines with Fable.exe 0x0066C340 / 0x0063F3A0 (pickany.py).
# FABLE_HDR_OVERLAY is honoured (verify_and_land.env). Launch in the background with `timeout 900`.
import sys, os, subprocess
from pathlib import Path
A = Path(__file__).resolve().parent.parent
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
tag, src, fl = sys.argv[1], sys.argv[2], sys.argv[3:]
wd = A / 'work' / 'toy'; wd.mkdir(parents=True, exist_ok=True)
sp = wd / (tag + '.cpp'); sp.write_text(V.vc71(Path(src).read_text(encoding='utf-8')), encoding='utf-8')
obj = wd / (tag + '.obj')
if obj.exists(): subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', str(A), str(obj)])
cp = V.cl(['/nologo', '/c'] + V.BASE_FLAGS + fl + ['/Fo' + str(obj), str(sp)], V.env())
out = open(wd / (tag + '.log'), 'w')
print('rc', cp.returncode, (cp.stdout or '')[-1500:], (cp.stderr or '')[-500:], file=out)
SY = {'0066c340': '??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z',
      '0063f3a0': '??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z'}
if obj.exists():
    for a, s in SY.items():
        r = subprocess.run(['python3', str(A / 'tools/pickany.py'), str(obj), s, a], capture_output=True, text=True)
        print(a, (r.stdout + r.stderr).strip().splitlines()[-1][-80:], file=out)
out.close(); print(open(wd / (tag + '.log')).read())
