# fam_aisg8.py <file>...: CAIStateGroupBase protected const accessors (Ego_r.pdb + FableWin.pdb; Ego_r order IsRunnableMain,
#   UpdateMain, Terminate, ClearTerminateFlag, GetThingManager, PeekThingManager, PeekPlayerManager, GetPlayerManager, PeekWorld,
#   PeekDefinitionManager, GetWorld, GetPWorldSeed, GetDebugText, PeekWorldMap, PeekThingSearchTools, IsStopping = retail
#   0x008FCF10 .. 0x008FD010 one slot each):
#   0x008FCFD0 ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ          (`unsigned long* GetPWorldSeed()` [const])
#   0x008FCF90 ?GetPlayerManager@CAIStateGroupBase@@IBEAAVCPlayerManager@@XZ   (`CPlayerManager* GetPPlayerManager()` [const])
#   0x008FCF80 ?PeekPlayerManager@CAIStateGroupBase@@IBEABVCPlayerManager@@XZ  (`const CPlayerManager* GetPPlayerManager() const`)
# Only declarations inside a `class/struct CAIStateGroupBase` body are respelled (wrapped in protected: / public:); calls
# `GetPPlayerManager()->` become `GetPlayerManager().` / `PeekPlayerManager().`. Idempotent.
import sys, re
SEED = re.compile(r'^([ \t]*)unsigned\s+long\s*\*\s*GetPWorldSeed\(\s*\)\s*(const\s*)?;([^\n]*)$', re.M)
PMGR = re.compile(r'^([ \t]*)(const\s+)?CPlayerManager\s*\*\s*GetPPlayerManager\(\s*\)\s*(const\s*)?;([^\n]*)$', re.M)
def in_base(t, i):
    head = list(re.finditer(r'\b(class|struct)\s+(\w+)[^;{()]*\{', t[:i]))
    if not head or head[-1].group(2) != 'CAIStateGroupBase': return False
    seg = t[head[-1].end():i]
    return seg.count('{') <= seg.count('}') + 0 or True
for p in sys.argv[1:]:
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    peek = False
    def seed(m):
        if not in_base(t, m.start()): return m.group(0)
        return '%sprotected:\n%sunsigned long* GetPWorldSeed() const;%s\n%spublic:' % (m.group(1), m.group(1), m.group(3), m.group(1))
    def pmgr(m):
        global peek
        if not in_base(t, m.start()): return m.group(0)
        if m.group(2): peek = True; d = 'const CPlayerManager& PeekPlayerManager() const;'
        else: d = 'CPlayerManager& GetPlayerManager() const;'
        return '%sprotected:\n%s%s%s\n%spublic:' % (m.group(1), m.group(1), d, m.group(4), m.group(1))
    t = SEED.sub(seed, t)
    t2 = PMGR.sub(pmgr, t)
    if t2 != t:
        t = t2
        t = re.sub(r'\bGetPPlayerManager\(\s*\)\s*->', 'PeekPlayerManager().' if peek else 'GetPlayerManager().', t)
    t = re.sub(r'(\n([ \t]*)public:\r?\n)\2protected:\r?\n', r'\n', t)   # merge adjacent protected blocks
    if t != o:   # out-of-line definitions (test stand-ins) follow their declarations
        t = re.sub(r'\bunsigned\s+long\s*\*\s*CAIStateGroupBase::GetPWorldSeed\(\s*\)\s*\{', 'unsigned long* CAIStateGroupBase::GetPWorldSeed() const {', t)
        t = re.sub(r'\bconst\s+CPlayerManager\s*\*\s*CAIStateGroupBase::GetPPlayerManager\(\s*\)\s*const\s*\{\s*return\s*&\s*(\w+)\s*;\s*\}',
                   r'const CPlayerManager& CAIStateGroupBase::PeekPlayerManager() const { return \1; }', t)
        t = re.sub(r'(?<!const )\bCPlayerManager\s*\*\s*CAIStateGroupBase::GetPPlayerManager\(\s*\)\s*(?:const\s*)?\{\s*return\s*&\s*(\w+)\s*;\s*\}',
                   r'CPlayerManager& CAIStateGroupBase::GetPlayerManager() const { return \1; }', t)
    if t != o: open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'nothing to respell')
