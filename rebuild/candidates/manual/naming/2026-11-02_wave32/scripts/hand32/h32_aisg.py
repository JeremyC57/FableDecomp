# h32_aisg.py: naming wave 32, family aisg (DECISIONS_W31 D11, chunk 703 priority 1): engine/CAIStateGroupBase.h gains the retail
#  polymorphic branch (opt-in FABLE_RETAIL_CAISTATEGROUPBASE: CAIStateGroupBase : CBaseClass, Ego_r members, ctor 0x008FD255), then
#  the 68 generated CAIStateGroup ctors (chunk batch "One of 73 / 16 bodies of this shape") that spelled the base ctor
#  ??0CBase_008fd255@@QAE@XZ (or another CBase_<addr> stand-in for an intermediate CAIStateGroup base) and the member base
#  ??0CMemberBase_00a01b10@@QAE@XZ are re-authored on it: the base is the class's RTTI base with its PDB ctor
#  (??0CAIStateGroupBase@@QAE@XZ, ??0CAIStateGroup_UsesSeat@@IAE@XZ protected, ...), the tracked pointer is the Ego_r.pdb member
#  CIntelligentPointer<T> (T from the member vtable's RTTI) on the chunk 703 CBaseIntelligentPointer model (0x00900B41), whose ctor is
#  ??0CBaseIntelligentPointer@@QAE@XZ 0x00A01B10. Facts per row: runs/aisg_data.json (scripts/aisg_data.py: retail calls / vtable stores /
#  RTTI names, Ego_r.pdb layouts, PDB ctor spellings).
import os, sys, json, re, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REL = 'rebuild/include/engine/CAIStateGroupBase.h'
dst = os.path.join(W, os.environ.get('HE_OV', 'ov_hand'), REL)
new = open(os.path.join(W, 'hdr', 'CAIStateGroupBase.h'), encoding='utf-8', newline='').read()
if os.path.exists(dst) and open(dst, encoding='utf-8', newline='').read() == new: print('already', REL)
else:
    os.makedirs(os.path.dirname(dst), exist_ok=True); open(dst, 'w', encoding='utf-8', newline='').write(new); print('ok', REL)
D = json.load(open(os.path.join(W, 'runs', 'aisg_data.json')))
INC = ('#define FABLE_RETAIL_CAISTATEGROUPBASE\n'
       '#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255\n')
BIP = '''class CBaseObjectPointer;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // Ego_r.pdb: vptr + PData (8 bytes), 1-aligned (chunk 703 0x00900B41)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
#pragma pack(pop)
'''
# retail size of an intermediate base: the derived member's retail offset minus the PDB distance from the base end (Dinkumware -> STLport)
BASE_DEF = {}
def acc(sym): return {'QAE': 'public', 'IAE': 'protected', 'AAE': 'private'}[sym.split('@@')[1][:3]]
def ptype(mt): return ('const ' if mt[1] else '') + mt[0]
def pmem_named(ms, lo, hi):
    return ', '.join('%s +0x%x' % (m[2], m[0]) for m in ms if lo <= m[0] < hi)
def base_block(b, rsize, chain_rest):
    c = b['class']; parent = chain_rest[0] if chain_rest else 'CAIStateGroupBase'
    if c == 'CAIStateGroupBase': return ''
    tmpl = re.match(r'(\w+)<(\w+)>$', c)
    if tmpl:
        return 'template <class T> class %s : public T { };              // RTTI %s (no ctor of its own)\n' % (tmpl.group(1), c)
    pad = rsize - (b.get('parent_size') or 0x14)
    lines = ['class %s : public %s {                    // RTTI %s : %s' % (c, parent, c, parent)]
    if b['ctor'][0] and 'E' in b['ctor'][1]:
        lines.append('%s:\n    %s();                         // %s (%s)' % (acc(b['ctor'][0]), c, b['ctor'][0], 'Ego_r.pdb / FableWin.pdb'))
        if acc(b['ctor'][0]) != 'public': lines.append('public:')
    else:
        lines.append('public:                                  // ctor FableWin.pdb only (%s): inlined into this row' % (b['ctor'][0] or '-'))
    if pad > 0:
        lines.append('    unsigned char _unmodeled%x[0x%x - 0x%x];       // Ego_r.pdb: %s' % (rsize - pad, rsize, rsize - pad, pmem_named(b['pdb_members'], 0, 1 << 20) or '-'))
    lines.append('};')
    return '\n'.join(lines) + '\n'
for a, d in sorted(D.items()):
    if not d['gen']: continue
    c = d['class']; off = d['off'][0]; ctor, srcs = d['ctor']
    assert ctor and 'E' in srcs and 'F' in srcs, (a, ctor, srcs)
    chain = d['chain'][:-1]          # drop CBaseClass
    # target member (PDB name)
    if d['cchar']:
        mtype, kind = 'CCharString', 'cchar'
        cand = [m for m in d['pdb_members'] if m[1] == 'CCharString']
    else:
        mt = d['mem_types'][0]; mtype = 'CIntelligentPointer<%s>' % ptype(mt); kind = 'ip'
        pdbt = 'CIntelligentPointer<%s%s>' % (mt[0], '_const_' if mt[1] else '')
        cand = [m for m in d['pdb_members'] if m[1] == pdbt]
    assert len(cand) == 1, (a, cand)
    pm = cand[0]; mname = pm[2]
    # retail size of the direct base = retail member offset - (PDB member offset - PDB base size) (no own members before it in PDB?)
    bases = d.get('bases', [])[:-1]
    sizes = {}
    if bases:
        b0 = bases[0]
        own_before = [m for m in d['pdb_members'] if m[0] < pm[0]]
        sizes[b0['class']] = off - (pm[0] - b0['pdb_size'])
        for i, b in enumerate(bases[1:], 1): sizes[b['class']] = b['pdb_size'] if b['class'] != 'CAIStateGroupBase' else 0x14
        for i, b in enumerate(bases):
            nxt = bases[i + 1]['class'] if i + 1 < len(bases) else None
            b['parent_size'] = sizes.get(nxt, 0x14) if nxt else 0
    dbase = bases[0]['class'] if bases else 'CAIStateGroupBase'
    dbase_size = sizes.get(dbase, 0x14)
    blocks = ''
    for i in range(len(bases) - 1, -1, -1):
        b = bases[i]
        if b['class'] == 'CAIStateGroupBase': continue
        blocks += base_block(b, sizes[b['class']], [x['class'] for x in bases[i + 1:]]) + '\n'
    tm = re.match(r'(\w+)<(\w+)>$', dbase)
    dbase_spelled = '%s<%s>' % (tm.group(1), tm.group(2)) if tm else dbase
    own_pad = off - dbase_size
    fwd = ''
    if kind == 'ip': fwd = 'class %s;\n' % d['mem_types'][0][0]
    pdb_note = 'Ego_r.pdb %s::%s +0x%x' % (c, mname, pm[0]) + ('' if pm[0] == off else ' (retail +0x%x: the base\'s two STLport vectors are 8 bytes smaller than Ego_r\'s Dinkumware ones)' % off)
    calls = 'CAIStateGroupBase::CAIStateGroupBase 0x008FD255' if d['base_ctor'] == '008fd255' else '%s::%s 0x%s (%s)' % (dbase, dbase, d['base_ctor'].upper(), next(b['ctor'][0] for b in bases if b['class'] == dbase))
    if kind == 'ip':
        calls += ', CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb)'
        ident = 'member vtable 0x%s = RTTI .?AV?$CIntelligentPointer@%sV%s@@@@' % (d['mem_vts'][0].upper(), '$$CB' if d['mem_types'][0][1] else '', d['mem_types'][0][0])
        shape = 'user default ctor, empty; CIntelligentPointer\'s ctor inlines to the base call + vtable store after this class\'s own vtable store'
    else:
        calls += ', CCharString::CCharString 0x0099E4B0 (??0CCharString@@QAE@XZ)'
        ident = 'the CCharString member is built out of line'
        shape = 'user default ctor, empty; the CCharString member\'s ctor is called out of line after the own vtable store'
    pads = ''
    if own_pad > 0:
        pads = '    unsigned char _unmodeled%x[0x%x - 0x%x];       // Ego_r.pdb: %s\n' % (dbase_size, off, dbase_size, pmem_named(d['pdb_members'], 0, pm[0]) or '-')
    src = '''// %(c)s::%(c)s() -- base %(dbase)s(), own vtable, then the %(mname)s member (+0x%(off)x)
// Leaf: %(ctor)s
// Retail: Fable.exe (TLC, VC7.1) @ 0x%(A)s, %(len)d bytes, thiscall, `ret`.
// Identity: stores own vtable 0x%(vt)s (RTTI .?AV%(c)s@@, base %(chainr)s); %(ident)s; Ego_r.pdb / FableWin.pdb
//   %(ctor)s.
// Callees: %(calls)s.
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); %(pdb_note)s; the members before it
//   are left unmodelled.%(bipn)s
// Shape: %(shape)s.
// History: generated in a chunk batch for a repeated body shape over the CBase_%(bc)s / %(oldm)s stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
%(INC)s
%(BIP)s%(fwd)s
%(blocks)sclass %(c)s : public %(dbase_spelled)s {      // vtable 0x%(vt)s
public:
%(pads)s    %(mtype)s %(mname)s;                     // +0x%(off)x
    %(c)s();
};

%(c)s::%(c)s()
{
}
''' % dict(c=c, dbase=dbase_spelled, mname=mname, off=off, ctor=ctor, A=a.upper(), len=d['len'], vt=d['own_vt'].upper(),
           chainr=' : '.join(d['chain']), ident=ident, calls=calls, pdb_note=pdb_note, shape=shape, bc=d['base_ctor'], INC=INC,
           BIP=BIP if kind == 'ip' else '', bipn=' CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).' if kind == 'ip' else '', oldm='CMemberBase_00a01b10' if kind == 'ip' else 'CMember_0099e4b0', fwd=fwd, blocks=blocks, dbase_spelled=dbase_spelled, pads=pads, mtype=mtype)
    he.put(a, 'src', src)
