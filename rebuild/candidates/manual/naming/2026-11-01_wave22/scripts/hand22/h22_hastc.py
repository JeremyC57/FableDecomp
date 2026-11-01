# h22_hastc.py: naming wave 22, family hastc -- wave 21 DEFERRED HasTC 0x006A80A0 (11 rows): the PDB filter call is
# ??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z (Ego_r.pdb CThingFilter_HasTC::operator()(const CThing&) const); the rows' generic filter templates
# (CFilter_Bind2/3, GetPFirstThing, GetPBestThing ...) pass the thing pointer, and the other filters they bind keep their pointer spellings, so
# each row declares the PDB operator()(const CThing&) and an inline pointer overload that forwards to it (`(*this)(*thing)`: the reference is
# the same pushed pointer, the overload is inlined and emits no symbol). Name-only. DECISIONS_W22 item (the inline forwarding overload).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
N = '// Naming (naming wave 22): CThingFilter_HasTC::operator() 0x006A80A0 takes the PDB `const CThing&` (??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z); the templates keep passing the pointer through an inline forwarding overload.'
def note(a):
    p, rel, t = he._rw(a, 'src')
    if N in t: print('already', a, 'note'); return
    m = re.search(r'^// Leaf: [^\n]*\n', t, re.M); i = m.end() if m else t.index('\n') + 1
    he._w(p, t[:i] + N + '\n' + t[i:]); print('ok', a, 'note')
# 0x004A9DA0 / 0x0093F8A0 / 0x006186F0 recheck DIFFER (3 bytes) with the forwarding overload: DEFERRED (per-row filter re-model);
# 0x004A9EF0 models CThing as `Thing`: DEFERRED.
for a in ('004a9e50', '00615750', '0065f744', '00782670', '00799c40', '00965e80'):
    he.edit(a, 'src', '    bool operator()(const CThing* thing) const;      // 0x006A80A0',
            '    bool operator()(const CThing& thing) const;      // 0x006A80A0 (Ego_r.pdb)\n    bool operator()(const CThing* thing) const { return (*this)(*thing); }')
    note(a)
for a, pad in (('005c0d9d', '                '),):
    he.edit(a, 'src', '    bool operator()(CThing* thing) const;' + pad + '// 0x006A80A0',
            '    bool operator()(const CThing& thing) const;          // 0x006A80A0 (Ego_r.pdb)\n    bool operator()(CThing* thing) const { return (*this)(*thing); }')
    note(a)
