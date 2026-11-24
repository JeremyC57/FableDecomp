# scanrows361.py: per row, the defines / pragmas / includes that matter for the NF352 Reset / NF349 GetDef size-body opt-ins
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361/tools'); import nf
KEYS = ['GETDEF_GLOBAL_BODY', 'SETDYNAMICCAST_REREAD', 'SIZE_BODY', 'ORIGINAL_COMPLETE_MEMBERS_PASS22', 'GETSUBDEF_BODY']
for a in open(sys.argv[1]).read().split():
    L = nf.src_of(a).read_text(encoding='utf-8').split('\n')
    inc = [i + 1 for i, l in enumerate(L) if l.startswith('#include')]
    dfn = [i + 1 for i, l in enumerate(L) if l.startswith('#define')]
    prag = ['L%d:%s' % (i + 1, l.strip()[:50]) for i, l in enumerate(L) if re.match(r'\s*#pragma\s+(optimize|inline|auto_inline)', l)]
    fold = ['L%d:%s' % (i + 1, l.strip()) for i, l in enumerate(L) if '#include' in l and 'src/compiled' in l]
    hits = ['L%d:%s' % (i + 1, l.split('//')[0].strip()) for i, l in enumerate(L) if any(k in l for k in KEYS) and l.startswith('#')]
    print('##', a, 'lines=%d firstinc=L%s lastdef=L%s defs<inc=%s' % (len(L), inc[0] if inc else '-', dfn[-1] if dfn else '-', all(d < inc[0] for d in dfn) if inc and dfn else '-'))
    for h in hits: print('   ', h)
    for p in prag: print('    PRAGMA', p)
    for f in fold: print('    FOLD', f)
