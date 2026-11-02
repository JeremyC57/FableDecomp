# h19_c684.py: naming wave 19, family c684 -- the chunk 684 NAME_FIXES rows (QUEUE_W19_input.tsv) respelled in the landed callers (name-only):
#   pdl:  ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ (private, Ego_r / FableWin, singular) in the Allocate* callers
#         0x00BDA360 / 0x00BDA3D0 / 0x00BDA470 (were ProcessDisposalLists / FlushCycle);
#   orbs: ?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z (the CThing& overload beside the landed 0x00779E90) in OnDie 0x00663F50 (was the free CreateExperienceOrbs);
#   nvm:  the NVectorMath namespace on the GFInterpolateVectors overloads 0x00A5B870 (KKM) / 0x00A5BBA0 (K1KKM) in 0x00AE1260 / 0x00AE1600 and the
#         const / class-key spelling of the 0x00A5BEF0 (K1KPBMKK) overload in 0x00AE2CB0 (Ego_r donor order 00BD5D90 / 00BD60F0 / 00BD6470);
#   ramps: CDspGain::ApplyRamps' sixth parameter is a bool (the byte test at 0x00C74573 / 0x00C74784): the landed Apply 0x00C74CE0 / Crossfade
#         0x00C74D30 declare it (and their own pass-through parameter) bool -- applied only when the recheck holds the bytes (runs/excl_c684.txt).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_c684.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
PDL = '// 0x00BDA150 (Ego_r.pdb / FableWin.pdb ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ, private; manifest "CDrawResourcePool_FlushCycle")'
for a, old in (('00bda360', 'void ProcessDisposalLists();                   // 0x00BDA150 (manifest "CDrawResourcePool_FlushCycle")'),
               ('00bda3d0', 'void ProcessDisposalLists();                   // 0x00BDA150 (manifest "CDrawResourcePool_FlushCycle")'),
               ('00bda470', 'void FlushCycle();                             // 0x00BDA150')):
    if a in _X: print('skip', a, 'excl'); continue
    he.edit(a, 'src', 'class CEngineLandscapeMemoryManager { public:\n    ' + old + '\n',
            'class CEngineLandscapeMemoryManager {\nprivate:\n    void ProcessDisposalList();                    ' + PDL + '\npublic:\n')
    he.resub(a, 'src', r'\bProcessDisposalLists\(\)', 'ProcessDisposalList()')
    he.resub(a, 'src', r'\bFlushCycle\(\)', 'ProcessDisposalList()')
    he.resub(a, 'src', r'CEngineLandscapeMemoryManager::FlushCycle\b', 'CEngineLandscapeMemoryManager::ProcessDisposalList')
    he.resub(a, 'src', r"the manager's disposal-list step @ 0x00BDA150 \(thiscall; manifest \"CDrawResourcePool_FlushCycle\"\)", 'CEngineLandscapeMemoryManager::ProcessDisposalList @ 0x00BDA150 (private, thiscall; manifest "CDrawResourcePool_FlushCycle")')
# orbs
a = '00663f50'
if a not in _X:
    he.edit(a, 'src', 'void __fastcall CreateExperienceOrbs(CThing* victim);   // 0x0077A890\n',
            'class CTCDExperienceOrb { public:\n    static void __fastcall Create(CThing& victim);     // 0x0077A890 (Ego_r.pdb / FableWin.pdb ?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z: the CThing& overload beside 0x00779E90)\n};\n')
    he.edit(a, 'src', 'CreateExperienceOrbs(this);', 'CTCDExperienceOrb::Create(*this);')
    he.resub(a, 'src', r'CreateExperienceOrbs @ 0x0077A890', 'CTCDExperienceOrb::Create(CThing&) @ 0x0077A890')
    he.resub(a, 'src', r'\bCreateExperienceOrbs\b', 'CTCDExperienceOrb::Create')
# nvm
for a in ('00ae1260', '00ae1600'):
    if a in _X: continue
    p, rel, t = he._rw(a, 'src')
    if 'namespace NVectorMath {' in t: print('already', a); continue
    s = t.index('// Interpolate `count` vectors from `from` towards `to`')
    e = t.index('// 0x00A5B870', s); e = t.index('\n', e) + 1
    blk = t[s:e].replace('GFInterpolateVectors_00a5bba0', 'GFInterpolateVectors')
    blk = blk.replace('// 0x00A5BBA0', '// 0x00A5BBA0 (Ego_r.pdb ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KKM@Z)')
    blk = re.sub(r'// 0x00A5B870\n', '// 0x00A5B870 (Ego_r.pdb ?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@KKM@Z)\n', blk)
    t = t[:s] + 'namespace NVectorMath {                         // Ego_r.pdb / FableWin.pdb: both overloads live in NVectorMath (donor order 0x00BD5D90 / 0x00BD60F0 = retail 0x00A5B870 / 0x00A5BBA0)\n' + blk + '}\n' + t[e:]
    c = he.re.sub(r'//[^\n]*', '', t)
    t = re.sub(r'(?<![\w:])GFInterpolateVectors_00a5bba0\(', 'NVectorMath::GFInterpolateVectors(', t)
    t = re.sub(r'(?m)^(\s+)GFInterpolateVectors\(', r'\1NVectorMath::GFInterpolateVectors(', t)
    t = t.replace('GFInterpolateVectors @ 0x00A5B870', 'NVectorMath::GFInterpolateVectors @ 0x00A5B870')
    he._w(p, t); print('ok', a)
# 00ae2cb0 already spells the (K1KPBMKK) overload in NVectorMath with const pointers (the bindings table carries a stale pre-respell line)
# ramps: bool sixth parameter (see runs/excl_c684.txt when the bytes do not hold)
for a in ('00c74ce0', '00c74d30'):
    if a in _X: print('skip', a, 'excl'); continue
    he.resub(a, 'src', r'(float\* block,\n\s+)int mode(, unsigned int count\);\s+// 0x00C74510)', r'\1bool mode\2 (?ApplyRamps@CDspGain@@QAEXPBM0PAM01_NI@Z: the sixth argument is a byte tested at 0x00C74573 / 0x00C74784, chunk 684)', 1)
    he.resub(a, 'src', r'float\* block, int mode,(\s+unsigned int count)', r'float* block, bool mode,\1')
