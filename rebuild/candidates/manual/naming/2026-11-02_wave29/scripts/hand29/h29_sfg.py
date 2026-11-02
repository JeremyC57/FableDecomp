# h29_sfg.py: naming wave 29, family sfg (scope 2, DECISIONS_W28 D7): the CSpawnedFunc1<T,P1> / CSpawnedFunc2<T,P1,P2> rows of the wave 28
#  sfb set are re-authored as the compiler's own scalar deleting destructors (the CTB_Bard 0x00ED7DB0 precedent): a TU that constructs the
#  instantiation (MakeOne) emits its vtable with the ??_G the Leaf names. Identity per row: runs/sfg_table.tsv (the vtable's storer / the
#  member function it spawns / the TU the COMDAT sits in) plus the FableWin.pdb instantiation name (??_G or ??_E, every T checked).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
import pe
SF2 = '''#include "engine/CSpawnedFuncBase.h"
template <class T, class P1, class P2> class CSpawnedFunc2 : public CSpawnedFuncBase {   // FableWin.pdb CSpawnedFunc2<T,P1,P2>; no shared header
public:
    CSpawnedFunc2(const CCharString& name, unsigned long id, T* pclass, void (T::*func)(P1, P2), P1 par1, P2 par2)
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass), Par1(par1), Par2(par2) { }
    virtual void SuspendableProcess() { (PClass->*Func)(Par1, Par2); }  // slot 4
    void (T::*Func)(P1, P2);                                             // +0x34
    T* PClass;                                                           // +0x38
    P1 Par1;                                                             // +0x3C
    P2 Par2;                                                             // +0x40
};
'''
MANG = {'CScriptThing': 'VCScriptThing@@', 'CCharString': 'VCCharString@@', 'long': 'J', 'float': 'M'}
for l in open(os.path.join(he.W, 'runs', 'sfg_table.tsv')):
    a, T, P, why = l.rstrip('\n').split('\t')
    p, rel, old = he._rw(a, 'src')
    m = re.search(r'void\* __fastcall (\w+)\(', old); was = m.group(1) if m else '?'
    ol = re.search(r'^// Leaf: (.*)$', old, re.M); oleaf = ol.group(1).strip() if ol else '-'
    vt = [va for nm, va in pe.find(int(a, 16), ('.rdata',))][0]
    Ps = P.split(',')
    two = len(Ps) == 2
    tmpl = 'CSpawnedFunc2' if two else 'CSpawnedFunc1'
    leaf = '??_G?$%s@V%s@NScript@@%s@@UAEPAXI@Z' % (tmpl, T, ''.join(MANG[x] for x in Ps))
    targs = ', '.join(['NScript::' + T] + Ps)
    n = int(open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv').read().split('\n' + a + '\t', 1)[1].split('\t')[1])
    mem = ' / '.join(('~CScriptThing 0x004AA840' if x == 'CScriptThing' else '~CCharString 0x0099EAE0') for x in Ps if x in ('CScriptThing', 'CCharString'))
    off = '0x40' if two else '0x3C'
    params = ', '.join('%s par%d' % (('const %s&' % x) if x.startswith('C') else x, i + 1) for i, x in enumerate(Ps))
    fargs = ', '.join(Ps)
    pars = ', '.join('par%d' % (i + 1) for i in range(len(Ps)))
    txt = '''// %(tmpl)s<%(targs)s>::`scalar deleting destructor'(unsigned int flags) -- destroy the by-value argument, then the CSpawnedFuncBase base; free when bit 0 is set
// Leaf: %(leaf)s
// Naming wave 29 (family sfg, DECISIONS_W28 D7): slot 0 of vtable 0x%(vt)08X (no RTTI), the %(tmpl)s<%(targs)s> vtable: %(why)s.
//   FableWin.pdb names the instantiation (??_G / ??_E %(tmpl)s<..> with these arguments). Was a free __fastcall %(was)s stand-in (Leaf %(oleaf)s).
// Retail: Fable.exe (TLC, VC7.1) @ 0x%(A)s, %(n)d bytes: push esi; mov esi,ecx; lea ecx,[esi+%(off)s]; call %(mem)s; mov ecx,esi; call
//   ~CSpawnedFuncBase 0x00CDD4C0; test byte [esp+8],1; je; push esi; call operator delete 0x00BFE9BC; add esp,4; mov eax,esi; pop esi; ret 4.
// Shape: the compiler's deleting destructor is emitted with the vtable, in a TU that constructs the class: MakeOne is that construction (the
//   0x00ED7DB0 precedent). The implicit ~%(tmpl)s is expanded into it, the implicit ~CSpawnedFuncBase (engine/CSpawnedFuncBase.h) called out of line.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
#include "engine/CScriptBase.h"
''' % dict(tmpl=tmpl, targs=targs, leaf=leaf, vt=vt, why=why, was=was, oleaf=oleaf, A=a.upper(), n=n, off=off, mem=mem)
    txt += SF2 if two else '#include "engine/CSpawnedFunc1.h"\n'
    txt += '''namespace NScript {
class %(T)s : public CScriptBase { };
}

CSpawnedFuncBase* MakeOne(const CCharString& name, unsigned long id, NScript::%(T)s* pclass, void (NScript::%(T)s::*func)(%(fargs)s), %(params)s)
{
    return new %(tmpl)s<%(targs)s>(name, id, pclass, func, %(pars)s);
}
''' % dict(T=T, fargs=fargs, params=params, tmpl=tmpl, targs=targs, pars=pars)
    import textwrap
    L = txt.split('\n'); out = []
    for x in L:
        if len(x) > 150 and x.startswith('//') and out:
            body = x[2:].lstrip(); ind = '// ' if not x.startswith('//   ') else '//   '
            w = textwrap.wrap(body, 140 - 5)
            out.append(ind + w[0]); out += ['//   ' + y for y in w[1:]]
        else: out.append(x)
    txt = '\n'.join(out)
    he.put(a, 'src', txt)
