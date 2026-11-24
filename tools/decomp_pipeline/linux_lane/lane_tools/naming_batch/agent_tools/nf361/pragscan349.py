# pragscan.py <tree> <addr...>: condition (c) on the PREPROCESSED new TU (gcc cpp -D_MSC_VER=1310, include tree <tree> = full copy with the
# new header): lists every `#pragma optimize` / `auto_inline` / `inline_depth` line in order and where the first NF361 size-body block
# starts. C_OK when every optimize pragma before that block is either none or a header block that already reset with optimize("", on)
# (the optimize state at the block is the command line); C_FAIL otherwise. Also counts the NF361 specializations reached (must equal the
# row's opt-in defines).
import sys, os, re, subprocess
T = sys.argv[1]; A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
stub = A + '/stubinc_equiv'
for a in sys.argv[2:]:
    f = (A + '/rows/%s.cpp' % a) if '=' not in a else a.split('=', 1)[1]; a = a.split('=', 1)[0]
    args = ['cpp', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-x', 'c++', '-I', T, '-I', stub, '-idirafter', '/home/user/FableDecomp/rebuild/include', f]
    for _ in range(60):   # NF361: create empty stubs for missing system headers (equiv_check's loop)
        r = subprocess.run(args, capture_output=True, text=True, errors='replace')
        m = re.search(r"fatal error: ([^:\n]+): No such file", r.stderr)
        if not m: break
        p = os.path.join(stub, m.group(1).strip()); os.makedirs(os.path.dirname(p), exist_ok=True); open(p, 'a').close()
    out = r.stdout.splitlines()
    want = len(re.findall(r'^#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_\w+_SIZE_BODY', open(f).read(), re.M))
    seq = []; first = None; nspec = 0
    for i, l in enumerate(out):
        s = l.strip()
        if s.startswith('#pragma') and re.search(r'optimize|auto_inline|inline_depth', s): seq.append((i, ' '.join(s.split())))
        if re.match(r'bool CDefinitionManager::GetDef<(CThingBaseDef|CThingObjectDef|NUISystem::CUIMiscThingsDef|CExpressionDef)>\(long index', s):
            nspec += 1
            if first is None: first = i
    if first is None: print(a, 'NO_BLOCK', 'want', want); continue
    # the block's own pragmas are the two optimize lines right before the first specialization
    ys = [i for i, p in seq if i < first and p.replace(' ', '') == '#pragmaoptimize("y",off)']
    bstart = ys[-1] if ys else first
    before = [p for i, p in seq if i < bstart]
    opt = [p for p in before if 'optimize' in p]
    ok = not opt or opt[-1].replace(' ', '') == '#pragmaoptimize("",on)'
    print('%s %s spec=%d/%d optimize_before=%d last=%s | %s' % (a, 'C_OK' if ok and nspec == want else 'C_FAIL', nspec, want, len(opt), opt[-1] if opt else '-', ' ; '.join(p for p in before)[:300]))
