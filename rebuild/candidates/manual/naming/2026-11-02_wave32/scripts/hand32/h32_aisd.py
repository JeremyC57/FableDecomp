# h32_aisd.py: naming wave 32, family aisd (DECISIONS_W31 D11, with aisg): the 25 generated derived-only CAIStateGroup ctors ("no
#  members of its own to initialise": base ctor out of line, then the own vtable store) whose base ctor was spelled with the invented
#  ??0CAIStateGroupBehaviourBase@N_<addr>@@QAE@XZ / ??0CAIStateGroupBase@N_008fe862@@QAE@XZ are re-authored on the RTTI chain to
#  engine/CAIStateGroupBase.h's retail branch: the called base is the class whose own vtable the callee stores, spelled with its PDB
#  ctor (??0CAIStateGroup_MinionSitOnSeatBase@@QAE@XZ, ??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ, ...); classes between the row's
#  class and that base whose ctor is FableWin.pdb-only (inlined in retail: HoverMovementBase, HoverAttack, MinionSitOnSeat) are
#  declared without a ctor. Facts per row: runs/aisd_data.json (scripts/aisd_data.py).
import os, sys, json, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
D = json.load(open(os.path.join(W, 'runs', 'aisd_data.json')))
INC = ('#define FABLE_RETAIL_CAISTATEGROUPBASE\n'
       '#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255\n')
def acc(sym): return {'QAE': 'public', 'IAE': 'protected', 'AAE': 'private'}[sym.split('@@')[1][:3]]
for a, d in sorted(D.items()):
    assert d['gen'] and d['stores'] == 1 and d['calls'] == [d['base_ctor']], a
    c = d['class']; ctor = d['ctor'][0]; bc = d['base_class']; bsym = d['base_ctor_sym'][0]
    assert ctor and bsym and 'E' in d['base_ctor_sym'][1] and 'F' in d['base_ctor_sym'][1], a
    chain = d['chain'][:-1]                 # down to CAIStateGroupBase (CBaseClass dropped)
    assert bc in chain, (a, bc, chain)
    blocks = []
    for i in range(len(chain) - 1, -1, -1):
        k = chain[i]
        if k == 'CAIStateGroupBase': continue
        parent = chain[i + 1]
        t = 'class %s : public %s {                   // RTTI %s : %s\n' % (k, parent, k, parent)
        if k == bc:
            t += '%s:\n    %s();                     // 0x%s %s (Ego_r.pdb / FableWin.pdb)\n' % (acc(bsym), k, d['base_ctor'].upper(), bsym)
        else:
            t += 'public:                                 // ctor FableWin.pdb only (%s): inlined into this row, which calls the base ctor directly\n' % (d['bases'][i]['ctor'][0] or '-')
        t += '};\n'
        blocks.append(t)
    inline = [k for k in chain[:chain.index(bc)]]
    src = '''// %(c)s::%(c)s() -- base %(bc)s(), then this class's own vtable
// Leaf: %(ctor)s
// Retail: Fable.exe (TLC, VC7.1) @ 0x%(A)s, %(len)d bytes, thiscall, `ret`.
// Identity: stores own vtable 0x%(vt)s (RTTI .?AV%(c)s@@, base chain %(chainr)s); FableWin.pdb %(ctor)s.
// Callees: %(bc)s::%(bc)s 0x%(B)s (%(bsym)s, Ego_r.pdb / FableWin.pdb; the callee stores the %(bc)s vtable).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass); no member is touched, so none is modelled.
// Shape: a polymorphic class with no members of its own to initialise%(inl)s.
// History: generated in a chunk batch ("One of 265 bodies of this shape") over the invented base %(old)s;
//   re-authored on the RTTI / PDB classes in naming wave 32 (family aisd).
%(INC)s
%(blocks)s
class %(c)s : public %(p)s {      // vtable 0x%(vt)s
public:
    %(c)s();
};

%(c)s::%(c)s()
{
}
''' % dict(c=c, bc=bc, ctor=ctor, A=a.upper(), len=d['len'], vt=d['own_vt'].upper(), chainr=' : '.join(d['chain']), B=d['base_ctor'].upper(),
           bsym=bsym, old=d['old'], INC=INC, blocks='\n'.join(blocks), p=chain[0],
           inl=('; the ctor of %s is inlined (FableWin.pdb-only symbol), so the call goes to %s' % (' / '.join(x for x in inline if x != c), bc)) if inline else '')
    he.put(a, 'src', src)
