# gdclass361.py <addr...>: where each row's CDefinitionManager::GetDef(long, CDefPointer<const T>*) bodies come from:
#   LOCAL_TEMPLATE Lnn   a row-local primary template definition (template<class T> ... CDefinitionManager::GetDef(long ...)
#   LOCAL_SPEC<T> Lnn    a row-local explicit specialization
#   GLOBAL_BODY          the row defines FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (engine/CDefinitionManager.h template)
#   SIZE_BODY:<T>        an NF349 per-type size-body define;  REREAD   the SetDynamicCast re-read define
#   CALLS:<types>        the GetDef call sites' deduced pointer types (CDefPointer<const X> locals named in the row)
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF365/tools'); import nf
for a in sys.argv[1:]:
    t = nf.src_of(a).read_text(encoding='utf-8'); L = t.split('\n'); out = []
    for i, l in enumerate(L):
        c = l.split('//')[0]
        if re.search(r'CDefinitionManager::GetDef\s*\(\s*long', c) and not c.rstrip().endswith(';'):
            prev = L[i - 1].split('//')[0] if i else ''
            out.append('LOCAL_TEMPLATE L%d' % (i + 1) if re.search(r'template\s*<\s*class', c + prev) else 'LOCAL_DEF? L%d' % (i + 1))
        m = re.search(r'CDefinitionManager::GetDef\s*<\s*([\w:]+)\s*>\s*\(\s*long', c)
        if m and not c.rstrip().endswith(';'): out.append('LOCAL_SPEC<%s> L%d' % (m.group(1), i + 1))
        if re.match(r'#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY\b', l): out.append('GLOBAL_BODY L%d' % (i + 1))
        m = re.match(r'#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_(\w+)_SIZE_BODY', l)
        if m: out.append('SIZE_BODY:%s' % m.group(1))
        if re.match(r'#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD', l): out.append('REREAD')
        if re.match(r'\s*class\s+CDefinitionManager\b[^;]*$', c) or re.match(r'\s*struct\s+CDefinitionManager\b[^;]*$', c): out.append('LOCAL_CLASS_MODEL L%d' % (i + 1))
    types = sorted(set(re.findall(r'CDefPointer<\s*const\s+([\w:]+)\s*>', t)) - {'T', 'CDefClassBase', 'U'})
    print(a, ' | '.join(out) or '-', '| CDefPointer<const X> types:', ','.join(types))
