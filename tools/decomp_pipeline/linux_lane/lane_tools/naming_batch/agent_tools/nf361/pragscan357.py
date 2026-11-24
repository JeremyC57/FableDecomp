# pragscan357.py (NF361 retarget of pragscan352: GetSubDef<T> size-body specializations) <tree> <label>=<source> ...: size-pragma condition (c) on the PREPROCESSED TU (gcc cpp -D_MSC_VER=1310 against the full
# include tree <tree> carrying the new header). Lists every `#pragma optimize` / `auto_inline` / `inline_depth` line before the first NF352
# CCountedPointer<NUISystem::CComponent> specialization, and counts the specializations reached (must equal the source's
# FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_*_SIZE_BODY defines). C_OK when no optimize pragma precedes the block, or the last one is
# optimize("", on) (the optimize state at the block is the command line).
import sys, os, re, subprocess
T = sys.argv[1]; A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
stub = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/stubinc_equiv"
SPEC = re.compile(r'^bool CDefClassBase::GetSubDef<(CEntitySoundDef|CWeaponDef|CAppearanceDef|CInventoryItemDef|CCreatureDef)>\(')
for spec in sys.argv[2:]:
    a, f = spec.split('=', 1)
    args = ['cpp', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-x', 'c++', '-I', T, '-I', stub, '-idirafter', '/home/user/FableDecomp/rebuild/include', f]
    out = subprocess.run(args, capture_output=True, text=True, errors='replace').stdout.splitlines()
    want = len(re.findall(r'^#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_\w+_SIZE_BODY', open(f).read(), re.M))
    seq = []; first = None; nspec = 0
    for i, l in enumerate(out):
        s = l.strip()
        if s.startswith('#pragma') and re.search(r'optimize|auto_inline|inline_depth', s): seq.append((i, ' '.join(s.split())))
        if SPEC.match(s):
            nspec += 1
            if first is None: first = i
    if first is None: print(a, 'NO_BLOCK want', want); continue
    ss = [i for i, p in seq if i < first and p.replace(' ', '') == '#pragmaoptimize("s",on)']
    bstart = ss[-1] if ss else first
    # NF361: the GetSubDef blocks open with optimize("y", off) then ("s", on): the block starts at that ("y", off)
    py = [i for i, p in seq if i < bstart and p.replace(' ', '') == '#pragmaoptimize("y",off)']
    if py and not [i for i, p in seq if py[-1] < i < bstart]: bstart = py[-1]
    before = [p for i, p in seq if i < bstart]
    opt = [p for p in before if 'optimize' in p]
    ok = not opt or opt[-1].replace(' ', '') == '#pragmaoptimize("",on)'
    print('%s %s spec=%d/%d optimize_before=%d last=%s | %s' % (a, 'C_OK' if ok and nspec == want else 'C_FAIL', nspec, want, len(opt), opt[-1] if opt else '-', ' ; '.join(before)[:300] or '-'))
