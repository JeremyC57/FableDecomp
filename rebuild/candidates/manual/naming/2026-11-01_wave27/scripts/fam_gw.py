# fam_gw.py <ov> <rows.txt>: wave 14 family gw -- the stand-in getters bound to 0x008FCFC0 become the PDB protected const
# reference getter ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (DECISIONS_W13_RESOLVED GetPWorld row). Per row: the stand-in
# member declaration goes; the row's local CAIStateGroupBase model gets `protected: CWorld& GetWorld() const;` at the end of its
# body (or, when the row has no local CAIStateGroupBase -- its class sits on a flattened engine header -- GW_EMPTYBASE=1 adds a
# leading `class CAIStateGroupBase { protected: CWorld& GetWorld() const; };` base to the declaring class); `g()->` becomes
# `GetWorld().`, a pointer use `&GetWorld()` (a row-local world model X keeps its type: `((X*)&GetWorld())`), a discarded call
# `GetWorld();`. check: the object loses exactly the stand-in and gains exactly the final spelling.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, f2m
T = '008fcfc0'; FIN = '?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ'
DECL = 'CWorld& GetWorld() const;'
CMT = '   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)'
BT, BS = inputs.bindings(); B = BT[T]
E, IX = inputs.index()
SYM = re.compile(r'^\?(\w+)@(\w+)@@([QI])([AB])E(.+)XZ$')
def blk_end(c, i):   # c[i] == '{' -> index of the matching '}'
    d = 0
    for j in range(i, len(c)):
        if c[j] == '{': d += 1
        elif c[j] == '}':
            d -= 1
            if d == 0: return j
    return -1
def cc(t):
    # code only, with the non-MSVC branch of `#ifdef _MSC_VER ... #else ... #endif` (and `#ifndef _MSC_VER` blocks) masked
    c = f2m.code_only(t)
    c = re.sub(r'(?s)(#ifdef\s+_MSC_VER\b.*?#else\b)(.*?)(#endif)', lambda m: m.group(1) + re.sub(r'[^\n]', ' ', m.group(2)) + m.group(3), c)
    c = re.sub(r'(?s)(#ifndef\s+_MSC_VER\b)(.*?)(#else|#endif)', lambda m: m.group(1) + re.sub(r'[^\n]', ' ', m.group(2)) + m.group(3), c)
    return c
# per-row model preparation (kept only when the whole row passes: famlib restores the file otherwise)
LAYOUT_D = {'0093dcd0', '0094f090'}            # derive directly from engine/CAIStateGroupBase.h: `#define ...Layout` include pattern
LAYOUT_M = {'0094feb0', '009547d0', '00954b50', '00954e30', '00961880', '00972490'}   # Methods on a flattened engine header
SPLIT = {'0091eb60', '00956ce0', '0095e260', '00979d80', '0097a630'}   # own class from +0: vptr/PBrain/pad move into the base
EMPTY = {'0091cdc0', '00954cd0', '00956fe0', '00957a20', '00962b80', '00970610', '00970f30', '00972830', '00982eb0', '00982f10',
         '00982f70', '0095f9a0', '00938ed0', '0094f9e0'} | LAYOUT_M
FULLWORLD = {'00985f40', '00982eb0', '00982f10', '00982f70'}                       # VC7.1 C2027 on a discarded reference call of an incomplete class
OVR = {'00938ed0': ('Val_938ed0_A', 'C', None), '0094f9e0': ('ResetHelper', 'CAIStateGroup_JackDragon_Methods', None)}
LAYINC = ('#define CAIStateGroupBase CAIStateGroupBaseLayout   // engine/CAIStateGroupBase.h flattens the vptr\n'
          '#include "engine/CAIStateGroupBase.h"\n#undef CAIStateGroupBase\n')
def prep(t, a):
    if a in LAYOUT_D or a in LAYOUT_M:
        inc = '#include "engine/CAIStateGroupBase.h"\n'
        if t.count(inc) != 1: return None, 'engine include count'
        if a in LAYOUT_M:
            c = cc(t); out = []; last = 0
            for mm in re.finditer(r'\bCAIStateGroupBase\b', c):
                out.append(t[last:mm.end()]); out.append('Layout'); last = mm.end()
            out.append(t[last:]); t = ''.join(out)
            t = t.replace('#include "engine/CAIStateGroupBaseLayout.h"\n', LAYINC)
        else:
            t = t.replace(inc, LAYINC + 'class CAIStateGroupBase : public CAIStateGroupBaseLayout { public:\n};\n')
    if a in SPLIT:
        m = re.search(r'struct\s+(\w+)\s*\{\s*\n(\s*virtual void v00\(\);\s*\n\s*CAIBrain\* PBrain;[^\n]*\n\s*unsigned char _pad08\[0x0c\];[^\n]*\n)', t)
        if not m: return None, 'split pattern'
        t = (t[:m.start()] + 'class CAIStateGroupBase { public:\n' + m.group(2) + '};\n\nstruct ' + m.group(1) + ' : CAIStateGroupBase\n{\n'
             + t[m.end():])
    if a == '0095f9a0':
        old = re.search(r'\n[ \t]*CWorld& GetWorld\(\) const;[^\n]*', t)
        t = t[:old.start()] + t[old.end():]
    if a == '00938ed0':
        t = t.replace('extern "C" void FABLE_FASTCALL Val_938ed0_A(void* stateGroup);\n', '')
        t = t.replace('struct C { char p[0x3c]; int f3c; char Valid(); };', 'struct C { char p[0x3c]; int f3c; char Valid(); void Val_938ed0_A(); };')
        t = t.replace('Val_938ed0_A(this);', 'Val_938ed0_A();')
    if a == '0094f9e0':
        t = t.replace('void ResetHelper(void);                // 0x008fcfc0 : return discarded\n', '')
        t = t.replace('    void ResetTimers();\n', '    void ResetHelper();                // 0x008fcfc0 : return discarded\n    void ResetTimers();\n')
    return t, ''
def tr(t, a):
    olds = [s for s in IX.get(a, {}).get('u', []) if s in B and s != FIN]
    if len(olds) != 1: return None, 'stand-ins %r' % olds
    t, why = prep(t, a)
    if t is None: return None, why
    m = SYM.match(olds[0])
    if a in OVR: fn, cls, X = OVR[a]; rc = 'X'
    elif not m: return None, 'not a member getter: %s' % olds[0]
    else: fn, cls, rc = m.group(1), m.group(2), m.group(5)
    rm = re.match(r'^PA([UV])(\w+)@@$', rc)
    if rm: X = rm.group(2)
    elif rc == 'AAVCWorld@@': X = 'CWorld&'
    elif rc in ('J', 'PAX', 'X'): X = None
    else: return None, 'return code %s' % rc
    c = cc(t)
    if re.search(r'\bstruct\s+CWorld\b', c): return None, 'struct CWorld model'
    if a != '0095f9a0' and re.search(r'(->|\.)\s*' + fn + r'\s*\(', c): return None, 'qualified call of ' + fn
    # declaration
    ds = list(re.finditer(r'(?m)(?:^|(?<=[;{]))([ \t]*)(?:struct\s+|class\s+)?[\w:]+\s*[*&]?\s*' + fn + r'\s*\(\s*(?:void)?\s*\)\s*(?:const\s*)?;', c))
    if a == '0095f9a0': ds = [None]
    if len(ds) != 1: return None, '%d declarations of %s' % (len(ds), fn)
    d = ds[0]
    if d is not None:
        ls = c.rfind('\n', 0, d.start()) + 1; le = c.find('\n', d.end()); le = len(c) if le < 0 else le
        line_rest = c[d.end():le].strip()
        if line_rest == '' and c[ls:d.start()].strip() == '':
            t = t[:ls] + t[le + 1:]
        else:
            t = t[:d.start()] + d.group(1) + t[d.end():]
    c = cc(t)
    # base
    bm = list(re.finditer(r'\b(class|struct)\s+CAIStateGroupBase\b\s*(:[^{;]*)?\{', c))
    if len(bm) > 1: return None, 'several CAIStateGroupBase models'
    if bm:
        b0 = bm[0].end() - 1; be = blk_end(c, b0)
        if not re.search(r'\bGetWorld\s*\(\s*\)\s*const\s*;', c[b0:be]):
            ins = 'protected:\n    ' + DECL + CMT + '\n'
            pre = t[:be]
            if not pre.endswith('\n'): ins = '\n' + ins
            t = pre + ins + t[be:]
        bpos = bm[0].start()
    else:
        if a not in EMPTY: return None, 'no local CAIStateGroupBase'
        cm = list(re.finditer(r'\b(class|struct)\s+' + cls + r'\b\s*(:\s*([^{;]*))?\{', c))
        if len(cm) != 1: return None, 'declaring class %s: %d' % (cls, len(cm))
        k = cm[0]
        base_txt = 'class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only\n    ' + DECL + CMT + '\n};\n'
        if k.group(2):
            nb = k.group(1) + ' ' + cls + ' : public CAIStateGroupBase, ' + k.group(3).strip() + ' {'
        else:
            nb = k.group(1) + ' ' + cls + ' : public CAIStateGroupBase {'
        ls2 = t.rfind('\n', 0, k.start()) + 1
        t = t[:ls2] + base_txt + t[ls2:k.start()] + nb + t[k.end():]
        bpos = ls2
    c = cc(t)
    if a in FULLWORLD:
        if t.count('class CWorld;\n') == 1: t = t.replace('class CWorld;\n', 'class CWorld { public:\n};\n')
        elif re.search(r'\bclass\s+CWorld\b', c): return None, 'CWorld declaration'
        else:
            ls3 = t.rfind('\n', 0, bpos) + 1
            t = t[:ls3] + 'class CWorld { public:\n};\n' + t[ls3:]
    elif not re.search(r'\bclass\s+CWorld\b', c[:bpos]):
        ls3 = t.rfind('\n', 0, bpos) + 1
        t = t[:ls3] + 'class CWorld;\n' + t[ls3:]
    # uses (code only; comments keep their text except the callee name next to the address)
    def subc(pat, rep, t):
        c = f2m.code_only(t); out = []; last = 0
        for mm in re.finditer(pat, c):
            out.append(t[last:mm.start()]); out.append(mm.expand(rep) if not callable(rep) else rep(mm)); last = mm.end()
        out.append(t[last:]); return ''.join(out)
    call = r'(?<![\w.>:])' + fn + r'\s*\(\s*\)'
    if a == '0095f9a0': return t, 'gw friend filter keeps group->GetWorld()'
    t = subc(r'\(\s*\(\s*CWorld\s*\*\s*\)\s*' + call + r'\s*\)\s*->', 'GetWorld().', t)
    if X in ('CWorld', 'CWorld&'):
        t = subc(call + r'\s*->', 'GetWorld().', t)
        t = subc(call + r'\s*\.', 'GetWorld().', t)
        t = subc(r'(?m)^(\s*)' + call + r'\s*;', r'\1GetWorld();', t)
        t = subc(call, '&GetWorld()', t)
    elif X:
        t = subc(call + r'\s*->', '((%s*)&GetWorld())->' % X, t)
        t = subc(r'(?m)^(\s*)' + call + r'\s*;', r'\1GetWorld();', t)
        t = subc(call, '(%s*)&GetWorld()' % X, t)
    else:
        t = subc(r'(?m)(^|[;{])(\s*)' + call + r'\s*;', r'\1\2GetWorld();', t)
        if fn != 'GetWorld' and re.search(call, f2m.code_only(t)): return None, 'non-discarded use of ' + fn
    # header comments naming the callee by the stand-in
    t = re.sub(r'(?m)^(//[^\n]*)$', lambda mm: re.sub(r'\b(?:CAIStateGroupBase::)?' + fn + r'\b(?=[^\n]*(?:8FCFC0|8fcfc0))', 'CAIStateGroupBase::GetWorld', mm.group(1)) if re.search(r'8fcfc0', mm.group(1), re.I) else mm.group(1), t)
    return t, 'gw %s %s -> GetWorld' % (cls, fn)
def chk(S0, S1, a):
    olds = [s for s in IX.get(a, {}).get('u', []) if s in B and s != FIN]
    if a in LAYOUT_M:   # a row-local inline helper typed on the engine layout follows the #define rename (defined here, not bound)
        mv = {s for s in S0 - S1 if s.replace('CAIStateGroupBase@@', 'CAIStateGroupBaseLayout@@') in S1 - S0 and s not in B}
        S0 = S0 - mv; S1 = S1 - {s.replace('CAIStateGroupBase@@', 'CAIStateGroupBaseLayout@@') for s in mv}
    if (S0 - S1) != set(olds): return 'lost %r' % sorted(S0 - S1)
    if (S1 - S0) - {FIN}: return 'gained %r' % sorted(S1 - S0)
    if FIN not in S1: return 'final not emitted'
    return None
if __name__ == '__main__':
    F.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tr, chk, tag='gw')
