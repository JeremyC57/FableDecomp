#!/usr/bin/env python3
"""The two static singleton getters as the PDB spells them (wave-4 shapes left by wave 2A):
  0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ   static CGameDefinitionManager& __fastcall Get()
  0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ       static CUserProfileManager& __fastcall Get()
(Ego_r.pdb + FableWin.pdb name sets; DECISIONS.tsv proven.) Landed rows reach them through stand-ins: a static
`Get()` of a local class (`X::Get()`, any return type / calling convention) or a free no-argument function
(`Y* GetDefinitionManager()`, `extern "C" ... GetTutMgr()`, ...). Neither takes arguments, so every calling convention
emits the same call; a pointer and a reference result are the same register.
Edit (rows in ROWS only; the table comes from the wave-4 symbol scan, see the file list):
  * the stand-in declaration goes (a member: its line; an emptied, otherwise unused stand-in class goes too);
  * `class <Target> : public Y { public: static <Target>& __fastcall Get(); };   // 0x...` is declared after the
    complete class Y the stand-in returned (Y keeps every member the callers use: derived-to-base at offset 0 is no
    code), or, when Y is already <Target>, the static member is added to it;
  * calls become `<Target>::Get()`; for an old pointer result `->` becomes `.` and other uses take `&`;
  * a test is edited the same way only when it defines the stand-in (it links the row's object); the definition
    becomes `<Target>& __fastcall <Target>::Get()` and its `return e;` statements return `*(<Target>*)(e)` (old
    pointer) or `(<Target>&)(e)` (old reference).
Blocked: Y incomplete or defined after the first call, the stand-in inside a namespace whose block holds the calls
(the new class must be at file scope), an existing class <Target> other than Y, any other use of the stand-in name.
Idempotent (the stand-in is gone after one run).  usage: fam_getmgr4.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, run, row_of, drop_nested, del_span, innermost, prev_sig, \
    match_brace, global_classes, qual_start, ns_path

T_DEF = ('CGameDefinitionManager', '0x0044C6B0')
T_UPM = ('CUserProfileManager', '0x0040D2A0')
# row: (target, 'M' class::Get | 'F' free function, name)
ROWS = {
    '004083e0': (T_DEF, 'F', 'GetGameDefinitionManager'),
    '004166a8': (T_DEF, 'M', 'CGameDefinitionManager_Methods'),
    '0041735a': (T_DEF, 'F', 'GetDefMgr'),
    '004189c2': (T_DEF, 'M', 'CSingleton44C6B0'),
    '0041d8e7': (T_DEF, 'F', 'GetDefinitionManager'),
    '0041d98a': (T_DEF, 'F', 'GetDefinitionManager'),
    '00557d20': (T_UPM, 'M', 'CUserProfileManagerSingleton'),
    '00599e3f': (T_UPM, 'F', 'GetProfileSource'),
    '006e11f0': (T_DEF, 'M', 'CDefinitionManagerAdapter'),
    '00713510': (T_DEF, 'F', 'GetDefinitionManager'),
    '00851770': (T_UPM, 'M', 'CTBaseSingleton_CUserProfileManager'),
    '0088e930': (T_UPM, 'F', 'get_88e930'),
    '0088f3e0': (T_UPM, 'F', 'GetTutMgr'),
    '0088f890': (T_UPM, 'F', 'GetMidObj'),
    '00891220': (T_UPM, 'F', 'CGSIOverrideMusic_GetCodeSection'),
    '008912a0': (T_UPM, 'F', 'CGSIStopMusic_GetCodeSection'),
    '008918c0': (T_UPM, 'M', 'CUserProfileManager_Retail'),
    '008943c0': (T_UPM, 'M', 'CUserProfileManager_Retail'),
    '00894aa0': (T_DEF, 'M', 'CDefinitionManagerMethods'),
    '00894bf0': (T_DEF, 'M', 'CGameDefinitionManager_Retail'),
    '00894c90': (T_DEF, 'M', 'CGameDefinitionManager_Retail'),
    '00894d30': (T_DEF, 'M', 'CGameDefinitionManager_Retail'),
    '00895a10': (T_DEF, 'M', 'CGameDefinitionManager_Retail'),
    '0089a070': (T_DEF, 'F', 'Sub_GetDefinitionManager'),
    '0089a0e0': (T_DEF, 'F', 'Sub_GetDefinitionManager'),
    '0089fbb0': (T_DEF, 'F', 'GetDefinitionManager'),
    '0089fc80': (T_DEF, 'M', 'CGameDefinitionManager_Retail'),
}
CC = r'(?:__fastcall|__cdecl|__stdcall|FABLE_FASTCALL)'


def transform(text, path):
    a, is_test = row_of(path)
    if a not in ROWS:
        return text, 0, []
    (TGT, ADDR), kind, name = ROWS[a]
    m = mask(text)
    sc = scopes(m)
    edits = []
    # ---- the stand-in declaration ----------------------------------------------------------------
    if kind == 'M':
        xs = [s for s in sc if s.kind == 'class' and s.name == name]
        if not xs: return text, 0, []
        if len(xs) != 1: return text, 0, ['MULTI %s' % name]
        x = xs[0]
        dp = re.compile(r'\bstatic\s+(?:const\s+)?(?P<y>(?:\w+\s*::\s*)*\w+)\s*(?P<k>[*&])\s*(?:%s\s+)?Get\s*\(\s*(?:void)?\s*\)\s*;' % CC)
        ds = [d for d in dp.finditer(m, x.lb, x.rb) if innermost(sc, d.start()) is x]
        if len(ds) != 1: return text, 0, ['DECL %d' % len(ds)]
        d = ds[0]
        y = re.sub(r'\s+', '', d.group('y')); k = d.group('k')
        if y.split('::')[-1] == name: y = name
        edits.append(del_span(text, m, d.start(), d.end()))
        # calls `[ns::]X::Get()`
        cp = re.compile(r'(?<![\w:])(?:::\s*)?(?:\w+\s*::\s*)*%s\s*::\s*Get\s*\(\s*\)' % re.escape(name))
        defp = re.compile(r'(?:const\s+)?(?:\w+\s*::\s*)*\w+\s*[*&]\s*(?:%s\s+)?(?:\w+\s*::\s*)*%s\s*::\s*Get\s*\(\s*(?:void)?\s*\)\s*\{' % (CC, re.escape(name)))
        calls = [c for c in cp.finditer(m) if not any(dd.start() <= c.start() < dd.end() for dd in defp.finditer(m))]
        defs = list(defp.finditer(m))
        # inside the class body: bare `Get()` calls
        for p in tokens(m, 'Get'):
            if x.lb < p < x.rb and not (d.start() <= p < d.end()):
                j = prev_sig(m, p)
                if m[j] not in '.>:': return text, 0, ['BARE_GET']
        body_rest = m[x.lb + 1:d.start()] + m[d.end():x.rb]
        x_empty = not body_rest.strip() or re.fullmatch(r'\s*(?:public\s*:\s*)?', body_rest) is not None
        xhs = m.rfind(x.key, 0, x.lb)
        if is_test and not defs: return text, 0, []
        others = [p for p in tokens(m, name) if not (xhs <= p < x.rb) and not any(c.start() <= p < c.end() for c in calls)
                  and not any(dd.start() <= p < dd.end() for dd in defs)]
        if x_empty and y != name and not [p for p in others if p < xhs or p > x.rb]:
            en = x.rb + 1
            kk = en
            while kk < len(m) and m[kk] in ' \t': kk += 1
            if kk < len(m) and m[kk] == ';':
                edits = [e for e in edits if not (x.lb <= e[0] <= x.rb)]
                edits.append(del_span(text, m, xhs, kk + 1))
        stand_end = x.rb
    else:
        fp = re.compile(r'(?P<pre>(?<![\w:])(?:extern\s+(?:"\s*C?\s*"\s+)?)?(?:static\s+)?(?:inline\s+)?)(?P<y>(?:\w+\s*::\s*)*\w+)\s*(?P<k>[*&])\s*(?:%s\s+)?%s\s*\(\s*(?:void)?\s*\)\s*(?P<e>[;{])' % (CC, re.escape(name)))
        fs = [f for f in fp.finditer(m) if innermost(sc, f.start()) is None or innermost(sc, f.start()).kind in ('ns', 'linkage')]
        if not fs: return text, 0, []
        decls = [f for f in fs if f.group('e') == ';']
        defs = [f for f in fs if f.group('e') == '{']
        ys = {re.sub(r'\s+', '', f.group('y')) for f in fs}
        ks = {f.group('k') for f in fs}
        if len(ys) != 1 or len(ks) != 1: return text, 0, ['FREE_SHAPES']
        y = ys.pop(); k = ks.pop()
        for f in fs:
            s_ = innermost(sc, f.start())
            if s_ is not None and s_.kind == 'ns': return text, 0, ['FREE_NS']
        if is_test and not defs: return text, 0, []
        for f in decls: edits.append(del_span(text, m, f.start('pre') if f.group('pre') else f.start('y'), f.end()))
        allspan = [(f.start(), f.end()) for f in fs]
        calls = []
        for p in tokens(m, name):
            if any(s0 <= p < s1 for s0, s1 in allspan): continue
            r_ = re.match(r'\s*\(\s*\)', m[p + len(name):])
            if not r_: return text, 0, ['FREE_USE line %d' % (text.count('\n', 0, p) + 1)]
            j = prev_sig(m, p)
            if m[j] in '.>' or (m[j] == ':' and m[j - 1] == ':'): return text, 0, ['FREE_QUAL']
            calls.append(type('C', (), {'s': p, 'e': p + len(name) + r_.end()})())
        calls = [(c.s, c.e) for c in calls]
        stand_end = max(f.end() for f in decls) if decls else min(f.start() for f in defs)
        x = None
    if kind == 'M':
        calls = [(c.start(), c.end()) for c in calls]
    if not calls and not defs: return text, 0, ['NO_CALLS']
    # ---- the base Y ---------------------------------------------------------------------------------
    yname = y.split('::')[-1]
    first_use = min([c[0] for c in calls] + [d_.start() for d_ in defs] + [len(m)])
    ctx_ns = ns_path(x) if x is not None else []
    if '::' in y:
        want = y.split('::')[:-1]
        bs = [s_ for s_ in sc if s_.kind == 'class' and s_.name == yname and ns_path(s_) == want]
    else:
        bs = [s_ for s_ in sc if s_.kind == 'class' and s_.name == yname and ns_path(s_) is not None
              and ns_path(s_) == (ctx_ns or [])[:len(ns_path(s_))]]
        if len(bs) > 1:
            deep = max(len(ns_path(s_)) for s_ in bs)
            bs = [s_ for s_ in bs if len(ns_path(s_)) == deep]
    gt = [s_ for s_ in sc if s_.kind == 'class' and s_.name == TGT and ns_path(s_) == []]
    def ins_at(pos, what):
        """insert `what` (one line) at the line after pos, at file scope: close and reopen enclosing namespaces."""
        s_ = innermost(sc, pos)
        chain_ = []
        while s_ is not None:
            if s_.kind == 'ns': chain_.append(s_.name)
            elif s_.kind == 'class':
                pos = s_.rb
                semi = pos
                while semi + 1 < len(m) and m[semi + 1] in ' \t': semi += 1
                if semi + 1 < len(m) and m[semi + 1] == ';': pos = semi + 1
                chain_ = []
            else: return 'INS_SCOPE %s' % s_.kind
            s_ = s_.parent
        eol = m.find('\n', pos + 1)
        if eol < 0: return 'INS_EOF'
        bol = eol + 1
        for e in edits:
            if e[0] < bol < e[1]: bol = e[1]
        if bol > first_use: return 'AFTER_USE'
        if innermost(sc, bol) is not innermost(sc, pos) and innermost(sc, pos) is not None and innermost(sc, pos).kind != 'class':
            return 'INS_MOVED'
        chain_.reverse()
        if chain_:
            code, _, cmt = what.rstrip('\n').partition('   //')
            what = '} ' * len(chain_) + code + ' ' + ' '.join('namespace %s {' % n_ for n_ in chain_) + ('   //' + cmt if cmt else '') + '\n'
        edits.append((bol, bol, what))
        return None
    if bs and yname == TGT and ns_path(bs[0]) == [] and len(bs) == 1:
        tb = bs[0]
        if tb.rb > first_use: return text, 0, ['TARGET_AFTER_USE']
        if re.search(r'\bGet\b', m[tb.lb:tb.rb]): return text, 0, ['TARGET_HAS_GET']
        if tb.key != 'class':
            hs_t = m.rfind(tb.key, 0, tb.lb)
            if re.search(r'\b(private|protected)\s*:', m[tb.lb:tb.rb]): return text, 0, ['TARGET_KEY']
            edits.append((hs_t, hs_t + len(tb.key), 'class'))
            edits.append((tb.lb + 1, tb.lb + 1, ' public:'))
            acc = ''
        else:
            acc = '' if re.search(r'\bpublic\s*:', m[tb.lb:tb.rb]) and not re.search(r'\b(private|protected)\s*:', m[tb.lb:tb.rb]) else 'public:\n'
        edits.append((tb.rb, tb.rb, ('' if m[tb.rb - 1] == '\n' else '\n') + acc +
                      '    static %s& __fastcall Get();   // %s ?Get@%s@@SIAAV1@XZ\n' % (TGT, ADDR, TGT)))
    elif len(bs) == 1:
        if gt: return text, 0, ['HAS_TARGET']
        b = bs[0]
        bnp = ns_path(b)
        base = '::'.join(bnp + [yname]) if bnp else yname
        r_ = ins_at(b.rb, 'class %s : public %s { public: static %s& __fastcall Get(); };   // %s ?Get@%s@@SIAAV1@XZ\n'
                    % (TGT, base, TGT, ADDR, TGT))
        if r_: return text, 0, ['BASE ' + r_]
    elif not bs and yname == TGT and not gt and '::' not in y:
        # only forward-declared: define the class with its static member, after the stand-in / forward declaration
        if re.search(r'\bstruct\s+%s\s*;' % TGT, m): return text, 0, ['TARGET_STRUCT_FWD']
        fw = [f_.end() for f_ in re.finditer(r'\bclass\s+%s\s*;' % TGT, m) if innermost(sc, f_.start()) is None]
        pos = max([stand_end] + fw) if kind == 'M' else max(fw + [0])
        if kind == 'F' and not fw: return text, 0, ['TARGET_NO_FWD']
        r_ = ins_at(pos, 'class %s { public: static %s& __fastcall Get(); };   // %s ?Get@%s@@SIAAV1@XZ\n' % (TGT, TGT, ADDR, TGT))
        if r_: return text, 0, ['TARGET ' + r_]
    else:
        return text, 0, ['BASE_BODIES %s %d' % (y, len(bs))]
    # ---- calls --------------------------------------------------------------------------------------
    qual = '::%s::Get()' % TGT if re.search(r'\bnamespace\b', m) else '%s::Get()' % TGT
    for st, en in calls:
        b0 = qual_start(m, st) if kind == 'M' else st
        edits.append((b0, en, qual))
        if k == '*':
            t = en
            while t < len(m) and m[t] in ' \t\r\n': t += 1
            if m.startswith('->', t):
                edits.append((t, t + 2, '.'))
            else:
                edits.append((b0, b0, '&'))
    # ---- definitions (tests) ------------------------------------------------------------------------
    for dd in defs:
        s_ = innermost(sc, dd.start())
        if s_ is not None and s_.kind != 'linkage': return text, 0, ['DEF_SCOPE']
        lbr = m.index('{', dd.start() if kind == 'M' else dd.start('y'))
        hs = dd.start('pre') if kind == 'F' and dd.group('pre') else (dd.start('y') if kind == 'F' else dd.start())
        if kind == 'F' and dd.group('pre') and 'extern' in dd.group('pre'):
            hs = dd.start('pre')
        rbr = match_brace(m, lbr)
        edits.append((hs, lbr, '%s& __fastcall %s::Get() ' % (TGT, TGT)))
        for r in re.finditer(r'\breturn\b(?P<e>[^;]*);', m[lbr:rbr]):
            e0 = lbr + r.start('e'); e1 = lbr + r.end('e')
            ex = text[e0:e1].strip()
            edits.append((e0, e1, (' *(%s*)(%s)' if k == '*' else ' (%s&)(%s)') % (TGT, ex)))
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
