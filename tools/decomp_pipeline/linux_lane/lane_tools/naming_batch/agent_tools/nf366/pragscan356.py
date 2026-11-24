# pragscan356.py <addr>=<source> ...: size-pragma condition (c) on the PREPROCESSED TU (gcc cpp -D_MSC_VER=1310, repo rebuild/include,
# VC7.1 headers as -isystem): every `#pragma optimize` line in order, where the first size-body specialization (GetDef<T> of
# engine/CDefinitionManager.h or CCountedPointer<NUISystem::CComponent> of engine/CCountedPointer.h) starts, and the pragmas after it.
# C_OK when no optimize pragma precedes the block's own `optimize("s", on)` / ("y", off) pair; also counts the specializations reached
# (must equal the source's *_SIZE_BODY defines) and prints the optimize state left at the end of the TU.
import sys, re, subprocess
R = '/home/user/FableDecomp'; S = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
SPEC = re.compile(r'^(bool CDefinitionManager::GetDef<|void CCountedPointer<NUISystem::CComponent>::(ReleaseData|Reset)\()')
for spec in sys.argv[1:]:
    a, f = spec.split('=', 1)
    args = ['cpp', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-x', 'c++', '-I', R + '/rebuild/include', '-isystem', S + '/wine/vc71/include', f]
    out = subprocess.run(args, capture_output=True, text=True, errors='replace').stdout.splitlines()
    want = len(re.findall(r'^#define FABLE_RETAIL_\w+_SIZE_BODY', open(f).read(), re.M))
    seq = []; specs = []
    for i, l in enumerate(out):
        s = ' '.join(l.split())
        if s.startswith('#pragma') and 'optimize' in s: seq.append((i, s.replace(' ', '')))
        if SPEC.match(l.strip()) and not l.rstrip().endswith(';'): specs.append(i)
    if not specs: print(a, 'NO_BLOCK want', want); continue
    first = specs[0]
    blk = [i for i, p in seq if i < first]
    # the block's own pragmas are the last 1 (CComponent) or 2 (GetDef) optimize lines right before the first specialization
    own = 2 if 'GetDef' in out[first] else 1
    before = [p for i, p in seq if i < first][:-own] if len(blk) >= own else []
    after = [p for i, p in seq if i > first]
    ok = not before and len(specs) == want
    print('%s %s spec=%d/%d optimize_before_block=%d | after: %s' % (a, 'C_OK' if ok else 'C_FAIL', len(specs), want, len(before), ' ; '.join(after)))
