import os
# fam_glob18.py <ov> <target> <rows.txt> [tag]: naming wave 18, the retail-accessor respell of a global stand-in pointer that is
# really a static member / CCountedPointer singleton read through an inline static accessor (the only way retail code outside the
# class reaches a protected / private static):
#   013b86a0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static `1`, ESTABLISHED): `extern CMainGameComponent*
#            g_pMainGameComponent;` -> the protected static declared in the row's CMainGameComponent model + the public inline
#            `static CMainGameComponent* __fastcall Get()` (Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ); uses become
#            CMainGameComponent::Get().
#   013b89fc ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static
#            CCountedPointer<CQuestManager>, two raw-PDB readings): `extern CQuestManager* g_pQuestManager;` -> the singleton
#            template modelled with the private static PClass and the public inline `static T& __fastcall Get()` (Ego_r / FableWin
#            ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ returns *PClass); `p->` becomes
#            `CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().`, other uses `&...::Get()`.
#   0143e8f8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED, the SetDSTGame writer): `extern CGameScriptInterface*
#            g_pGameScriptInterface;` -> `extern CGameScriptInterfaceBase* pDSTGame;` (the virtual slots are the base's; a row-local
#            `class CGameScriptInterface` model is renamed to CGameScriptInterfaceBase when the row has no other definition of it).
# A stand-in class name (CEngine / QuestManagerNative ...) that the row defines once and uses only for this global is renamed to
# the real class first (RENAME_CLASS). Name-only: famlib2 keeps a row only when it compiles with exactly the old symbol gone and the
# new one named; the recheck proves the bytes.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F
ov, T, rf = sys.argv[1], sys.argv[2].lower().replace('0x', '').zfill(8), sys.argv[3]; TAG = sys.argv[4] if len(sys.argv) > 4 else 'g' + T
SPEC = {
 '013b86a0': dict(cls='CMainGameComponent', fin='?PMainGameComponent@CMainGameComponent@@1PAV1@A',
                  olds={'g_pMainGameComponent': None, 'g_MainGameComponent_013b86a0': None, 'g_PMainGameComponent': None, 'CUnprovenGlobal_013b86a0': None,
                        'g_pEngine': 'CEngine', 'g_pEngineGlobals': 'CEngineGlobals', 'g_pEngineGlobals_013b86a0': 'CEngineGlobals', 'g_pGame': 'CGame', 'g_pWorld': 'CWorld',
                        'g_object_013b86a0': 'CGlobalObject', 'Game_013b86a0': 'Game', 'g_GameText': 'CGameTextManager', 'g_MainGameComponent': 'MainGameSoundView'},
                  insert='protected:\n    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)\npublic:\n    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)\n',
                  arrow='CMainGameComponent::Get()->', value='CMainGameComponent::Get()', model=None,
                  note='// Naming wave 18: the global stand-in %s (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the\n//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).\n'),
 '013b89fc': dict(cls='CQuestManager', fin='?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A',
                  olds={'g_pQuestManager': None, 'g_QuestManager': None, 'g_questMgr': None, 'FableGlobalQuestManager': None, 'DAT_013b89fc': None, 'QuestManager': 'QuestManagerNative', 'g_pQuestMgr': 'CQuestMgr'},
                  insert=None, arrow='CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().', value='(&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get())',
                  model='''class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
''',
                  note='// Naming wave 18: the global stand-in %s (0x013B89FC) is the private static CCountedPointer<CQuestManager>\n//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).\n'),
 '0143e8f8': dict(cls='CGameScriptInterfaceBase', fin='?pDSTGame@@3PAVCGameScriptInterfaceBase@@A',
                  olds={'g_pGameScriptInterface': None}, insert=None, arrow='pDSTGame->', value='pDSTGame', model=None, extern='extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)',
                  note='// Naming wave 18: the global stand-in %s (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the\n//   virtual slots are the base interface\'s) -- name-only (the bytes are unchanged).\n'),
 '013b8790': dict(cls='CPlayerGui', fin='?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A',
                  olds={'g_pPlayerGui': None, 'g_PlayerGui': None, 'CUnprovenGlobal_013b8790': None, 'g_pGameUI': 'CGameUI', 'g_pGameUI_013b8790': 'CGameUI_Retail'},
                  insert=None, arrow='CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().', value='(&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get())',
                  model='''class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
''',
                  note='// Naming wave 18: the global stand-in %s (0x013B8790) is the private static CCountedPointer<CPlayerGui>\n//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).\n'),
 '013b8a1c': dict(cls='CThing', fin='?PCurrentThingManager@CThing@@0PAVCThingManager@@A', ptrtype='CThingManager',
                  olds={'g_pThingManager': None, 'g_pCurrentThingManager': None},
                  insert='private:\n    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)\npublic:\n    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)\n',
                  model='class CThingManager;\nclass CThing {\n    static CThingManager* PCurrentThingManager;      // 0x013B8A1C ?PCurrentThingManager@CThing@@0PAVCThingManager@@A (private static)\npublic:\n    static CThingManager* __fastcall GetPCurrentThingManager() { return PCurrentThingManager; }   // Ego_r / FableWin ?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ (inline)\n};\n',
                  arrow='CThing::GetPCurrentThingManager()->', value='CThing::GetPCurrentThingManager()',
                  note='// Naming wave 18: the global stand-in %s (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline\n//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).\n'),
 '01375550': dict(cls='CMainGameComponent', fin='?ConstantFPS@CMainGameComponent@@1JA', ptr=False,
                  olds={'g_framesPerSecond': None, 'g_FramesPerSecond': None, 'g_01375550': None, 'g_GameFramesPerSecond': None, 'g_FrameRate': None, 'FramesPerSecond': None,
                        'g_WorldFramesPerSecond': None, 'g_FramesPerSecond_01375550': None, 'g_logicFramesPerSecond': None, 'g_TicksPerSecond': None, 'g_frameRate': None, 'g_framesPerSecond_01375550': None},
                  insert='protected:\n    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)\npublic:\n    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)\n',
                  model='class CMainGameComponent {\nprotected:\n    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)\npublic:\n    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)\n};\n',
                  arrow=None, value='CMainGameComponent::GetConstantFPS()',
                  note='// Naming wave 18: the global stand-in %s (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the\n//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).\n'),
}
SP = SPEC[T]; CLS = SP['cls']; PTR = SP.get('ptr', True); RN = SP.get('ptrtype', CLS)
# the per-row stand-in spellings bound to the target (idx of HEAD): `?name@@3PAU<cls>@@A` -> rename <cls>; `?name@@3[JH]A` -> value global
if T in ('013b8790', '013b8a1c', '01375550'):
    import json
    S = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    _bt = set(l.split('\t')[1].strip() for l in open(S + '/BINDINGS_HEAD.tsv') if l.split('\t')[0] == T) if os.path.exists(S + '/BINDINGS_HEAD.tsv') else set()
    for _s in _bt:
        m = re.match(r'^\?([A-Za-z_]\w*)@@3PA[UV](\w+)@@A$', _s)
        if m and PTR and m.group(1) not in SP['olds']: SP['olds'][m.group(1)] = None if m.group(2) == RN else m.group(2)
        m = re.match(r'^\?([A-Za-z_]\w*)@@3[JH]A$', _s)
        if m and not PTR and m.group(1) not in SP['olds'] and m.group(1) != 'ConstantFPS': SP['olds'][m.group(1)] = None
def tr(t, a):
    c = F.code_only(t)
    olds = [o for o in SP['olds'] if re.search(r'(?<![\w$])' + re.escape(o) + r'(?![\w$])', c)]
    if not olds: return None, 'no stand-in identifier'
    if len(olds) > 1: return None, 'several stand-ins %s' % olds
    old = olds[0]; stand_cls = SP['olds'][old]
    EXT = r'(?m)^[ \t]*extern\s+(?:const\s+)?([\w:]+(?: (?:long|int|short|char))?)\s*' + (r'\*' if PTR else '') + r'\s*' + re.escape(old) + r'\s*;[^\n]*\n'
    ext = [m for m in re.finditer(EXT, t)]
    if len(ext) != 1: return None, '%d extern declarations of %s' % (len(ext), old)
    typ = ext[0].group(1)
    if not PTR:
        if typ not in ('long', 'int', 'unsigned long', 'unsigned int'): return None, '%s typed %s' % (old, typ)
        if re.search(r'(?<![\w$])' + re.escape(old) + r'\s*(=[^=]|\+\+|--|[-+*/]=)', c): return None, '%s written' % old
    if stand_cls and typ != stand_cls:
        # the same stand-in name on another row-local class: that class is the stand-in when it is the row's single definition
        if typ != RN and len(F.class_defs(c, typ)) == 1: stand_cls = typ
        else: return None, 'stand-in %s typed %s' % (old, typ)
    if not stand_cls and PTR and typ != RN and T != '0143e8f8':
        if len(F.class_defs(c, typ)) == 1 and not re.search(r'\b' + RN + r'\b', c): stand_cls = typ
        else: return None, '%s typed %s' % (old, typ)
    msg = []
    if stand_cls:
        # rename the stand-in class to the real one: defined once, the real class not defined / declared
        if re.search(r'\b' + RN + r'\b', c): return None, '%s already named beside stand-in class %s' % (RN, stand_cls)
        if len(F.class_defs(c, stand_cls)) != 1: return None, '%d definitions of stand-in class %s' % (len(F.class_defs(c, stand_cls)), stand_cls)
        t = F.rename_code(t, stand_cls, RN); c = F.code_only(t); msg.append('class %s -> %s' % (stand_cls, RN))
        # the PDB class key is `class` (V): a struct stand-in becomes a class with its members public
        t, nk = re.subn(r'\bstruct(\s+)' + RN + r'\b(\s*(?::[^{;]*)?)\{', r'class\1' + RN + r'\2{ public:', t)
        t = re.sub(r'\bstruct(\s+)' + RN + r'\s*;', r'class\1' + RN + ';', t)
        if nk: msg.append('struct -> class')
        c = F.code_only(t)
        ext = [m for m in re.finditer(EXT, t)]
    if T == '0143e8f8':
        if typ == 'CGameScriptInterface' and not re.search(r'\bCGameScriptInterfaceBase\b', c):
            # a row-local model: rename it (the slots are the base's); a shared-header row has the base already
            if 'engine/CGameScriptInterface.h' in t: pass
            elif len(F.class_defs(c, 'CGameScriptInterface')) == 1:
                t = F.rename_code(t, 'CGameScriptInterface', 'CGameScriptInterfaceBase'); c = F.code_only(t); msg.append('local CGameScriptInterface model -> CGameScriptInterfaceBase')
            else: return None, 'CGameScriptInterface typed without a definition or the header'
        elif typ != 'CGameScriptInterface' and typ != 'CGameScriptInterfaceBase': return None, 'typed %s' % typ
        ext = [m for m in re.finditer(EXT, t)]
        t = t[:ext[0].start()] + SP['extern'] + '\n' + t[ext[0].end():]
    else:
        # drop the extern line; model / insert
        t = t[:ext[0].start()] + t[ext[0].end():]
        c = F.code_only(t)
        d = F.class_defs(c, CLS)
        if SP['insert'] and len(d) == 1:
            kw, s0, b, e = d[0]
            acc0 = 'private:' if kw == 'class' else 'public:'
            t = t[:b + 1] + '\n' + SP['insert'] + acc0 + t[b + 1:]
        elif SP['insert'] and len(d) > 1: return None, '%d definitions of %s' % (len(d), CLS)
        elif SP['insert'] and not SP.get('model'):
            return None, 'no definition of %s' % CLS
        else:
            ins = d[0][1] if d else None
            if ins is None:
                m = re.search(r'(?m)^class ' + CLS + r';[^\n]*\n', c)
                if not m and SP['insert']: ins = ext[0].start()   # the model stands where the extern was
                elif not m: return None, 'no definition / declaration of %s' % CLS
                else: ins = m.end()
            else:
                # the model goes before the class definition's top-level start
                from memlib import top_start
                ins = top_start(c, ins)
            t = t[:ins] + SP['model'] + t[ins:]
    # uses
    c = F.code_only(t); out = []; last = 0; n = 0
    for m in re.finditer(r'(?<![\w$])' + re.escape(old) + r'(?![\w$])', c):
        out.append(t[last:m.start()]); j = m.end()
        k = j
        while k < len(c) and c[k] in ' \t': k += 1
        if SP['arrow'] and c.startswith('->', k): out.append(SP['arrow']); last = k + 2
        else: out.append(SP['value']); last = j
        n += 1
    out.append(t[last:]); t = ''.join(out)
    if n == 0: return None, 'no use of %s' % old
    # comments naming the stand-in
    t = re.sub(r'//[^\n]*', lambda mm: re.sub(r'\b' + re.escape(old) + r'\b', SP['value'] if T != '013b89fc' else 'CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass', mm.group(0)), t)
    # note after the Leaf line (or line 1)
    L = t.split('\n'); li = [i for i, x in enumerate(L) if x.startswith('// Leaf: ')]
    at = li[0] + 1 if li else (1 if L[0].startswith('//') else 0)
    L.insert(at, (SP['note'] % old).rstrip('\n')); t = '\n'.join(L)
    return t, ' '.join(msg + ['%s x%d' % (old, n)])
OLDSYMS = None
def ck(D0, U0, D1, U1, a):
    rem = [s for s in U0 - U1 if '?A0x' not in s]
    add = [s for s in U1 - U0 if '?A0x' not in s]
    if SP['fin'] not in U1: return 'final not named (+%s -%s)' % (add[:3], rem[:3])
    bad_rem = [s for s in rem if not re.search(r'3PA[UV]', s) and not s.endswith('@@3PAXA') and not re.search(r'@@3[JKH]A$', s)]
    bad_add = [s for s in add if s != SP['fin']]
    # the inline accessors are emitted as COMDATs beside the graded leaf (as engine/CCountedPointer.h members are): not a change of the row
    nd = [s for s in D1 - D0 if not s.startswith('??_C@') and not s.startswith('__real@') and not s.startswith(('?Get@CMainGameComponent@@SIPAV1@XZ', '?Get@?$CTBaseSingletonNoDefaultAlloc@', '??D?$CCountedPointer@', '?GetPCurrentThingManager@CThing@@SI', '?GetConstantFPS@CMainGameComponent@@SI'))]
    if bad_rem or bad_add or len(rem) != 1: return 'diff -%s +%s' % (rem[:3], add[:3])
    if nd: return 'new defined %s' % nd[:3]
    return None
if __name__ == '__main__':
    F.run(ov, F.rows_from(rf) if hasattr(F, 'rows_from') else [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
