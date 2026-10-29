# wave 7 family ccp7 (after gpi7): rows whose CIntelligentPointer<CThingCreatureBase> stand-in `CCreaturePointer` declares its own
# out-of-line `CThingCreatureBase* GetPItem() const; // 0x00A01B50` -- a second name for CBaseIntelligentPointer::GetPItem
# (RELOC_DUPLICATE once the base spelling is ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ). Retail's typed
# CIntelligentPointer<T>::GetPItem() is inline over the base call, so: CCreaturePointer derives from the row's
# CBaseIntelligentPointer (same {vfptr, item} layout; its own vfptr / item members go) and GetPItem becomes the inline
# forwarder `return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem();`. Idempotent.
import re, sys
def fix(t):
    m = re.search(r'struct[ \t]+CCreaturePointer[ \t]*(:[^{]*)?\{', t)
    if not m: return None, 'no CCreaturePointer'
    if m.group(1): return (None, 'already') if 'CBaseIntelligentPointer::GetPItem();' in t else (None, 'has bases')
    d = 0; i = m.end() - 1
    while True:
        if t[i] == '{': d += 1
        elif t[i] == '}':
            d -= 1
            if d == 0: break
        i += 1
    body = t[m.end():i]
    b2 = re.sub(r'\n[ \t]*(?:void\*[ \t]+__vftable|virtual[ \t]+void[ \t]+v00\(\))[ \t]*;[^\n]*', '', body, count=1)
    b2 = re.sub(r'\n[ \t]*(?:void|CThingCreatureBase|CThing)\*[ \t]+m_pItem[ \t]*;[^\n]*', '', b2, count=1)
    b2, n = re.subn(r'CThingCreatureBase\*[ \t]+GetPItem\(\)[ \t]*const[ \t]*;[^\n]*',
                    'CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }   // inline over 0x00A01B50', b2)
    if n != 1 or b2.count('\n') != body.count('\n') - 2: return None, 'body shape'
    head = t[m.start():m.end() - 1].rstrip() + ' : CBaseIntelligentPointer {'
    return t[:m.start()] + head + b2 + t[i:], 'ok'
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
