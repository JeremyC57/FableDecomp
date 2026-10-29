#!/usr/bin/env python3
"""Call the retail frame getter as `CWorld::GetFrame()` (0x0049D870 ?GetFrame@CWorld@@SIJXZ) in the rows wave 2A left.

Retail: static long __fastcall CWorld::GetFrame() (Ego_r.pdb + FableWin.pdb, DECISIONS.tsv proven; it reads the frame
counter global 0x013B89BC and ignores any object). The rows below call it through a stand-in; ROWS names the
stand-in per row (from the gf_base recheck: the symbol each row binds to 0x0049D870) and its shape:
  standin  a single-member stand-in class `X { static long M(); }` (CWorldFrame, CWorldF, CThingWorld, CWorldClock,
           CDragonWorld, MazeWorld): X is CWorld itself (it is GetPWorld()'s pointee), so every code token X becomes
           CWorld and M becomes GetFrame; X's body becomes `class CWorld { public: static long __fastcall GetFrame(); }`
           or, when the file already defines CWorld, X's body is deleted and the member is added to CWorld when missing.
  free     a file/namespace-scope free stand-in `R N(...)` called as `N()` or `N(arg)` (arg: an identifier or `this`):
           its declaration goes, each call becomes `CWorld::GetFrame()` (cast to R when R is not int / long).
  member   a non-static member stand-in `R N() [const];` of the row's class, called as `N()` / `this->N()`: as free.
CWorld gets the static member: inside an existing local `class CWorld` body, or a new
`class CWorld { public: static long __fastcall GetFrame(); };` where the stand-in was declared (a forward
declaration is completed). Blockers: engine/CWorld.h renamed (`#define CWorld`), a CWorld that already has a
non-static GetFrame, any other use of the stand-in name. Idempotent (the stand-in name is gone after one run).
usage: fam_getframe_other.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, prev_sig, prev_word, run, del_span, innermost, statements, row_of, \
    global_classes, comment_spans, drop_nested

ROWS = {
    # standin: (class, member)
    '00922750': ('standin', 'CWorldFrame', 'GetWorldFrame'), '0094e6b0': ('standin', 'CWorldFrame', 'GetWorldFrame'),
    '00956b00': ('standin', 'CWorldFrame', 'GetWorldFrame'), '009613a0': ('standin', 'CWorldFrame', 'GetWorldFrame'),
    '007846b0': ('standin', 'CWorldFrame', 'GetWorldFrame'),
    '0094dac0': ('standin', 'CWorldF', 'GetWorldFrame'), '0095d200': ('standin', 'CWorldF', 'GetWorldFrame'),
    '00956fe0': ('standin', 'CThingWorld', 'GetCurrentFrame'), '00957a20': ('standin', 'CThingWorld', 'GetCurrentFrame'),
    '0094f730': ('standin', 'CWorldClock', 'GetCurrentFrame'), '0094feb0': ('standin', 'CDragonWorld', 'GetWorldFrame'),
    '00954b50': ('standin', 'MazeWorld', 'GetWorldFrame'), '00954e30': ('standin', 'MazeWorld', 'GetWorldFrame'),
    # free: name
    '00416e78': ('free', 'GFGetPreMainMemoryUsed'), '004c6060': ('free', 'sub_49d870'), '006585ee': ('free', 'helper_get'),
    '00697710': ('free', 'GetEngineTimeMs'), '00699560': ('free', 'GetQuakeFrame'), '006c04b0': ('free', 'GetNewThing'),
    '006cb8f0': ('free', 'helperB'), '007bed10': ('free', 'GetCurrentGameTime'), '00811450': ('free', 'helper_49d870'),
    '00851e50': ('free', 'GetElapsed'), '008af850': ('free', 'GetWorldFrame'), '00906fa0': ('free', 'GetCurrentValue'),
    '00935160': ('free', 'GFGetCurrentFrame'), '00938ed0': ('free', 'Val_938ed0_B'), '00940bd0': ('free', 'Act_940bd0_B'),
    '009580e0': ('free', 'GetWorldFrame'), '00784500': ('free', 'GFGetCurrentFrame'),
    '006cec80': ('free', 'imp_helper'), '006fecf0': ('free', 'lr_helper'), '006fedb0': ('free', 'Helper_LR'),
    '007f76e0': ('free', 'helper_49d870'), '008023a0': ('free', 'helper_49d870'), '00906fc0': ('free', 're_call'),
    '009076a0': ('free', 'iv_helper'), '00919a30': ('free', 're_call'), '0091ccb0': ('free', 're_call'),
    '009340f0': ('free', 're_call'),
    # member: name
    '0047cfee': ('member', 'Helper'), '004fb280': ('member', 'Impl'), '006bf7b0': ('member', 'ResolvePurchase'),
    '006c0330': ('member', 'GetWorldFrame'),
}
MEMBER = 'static long __fastcall GetFrame();'
NEWCLS = 'class CWorld { public: static long __fastcall GetFrame(); };'
CMT = '   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ'
CC = r'(?:__fastcall|__cdecl|__stdcall|FABLE_FASTCALL|FABLE_NATIVE_THISCALL)'


def cworld_plan(text, m, sc, at):
    """Edits that give CWorld the static member; `at` = (start, end) of the stand-in declaration to replace."""
    defs = [s for s in sc if s.kind == 'class' and s.name == 'CWorld']
    if len(defs) > 1:
        return None, 'MULTI CWorld'
    if defs:
        c = defs[0]
        body = m[c.lb:c.rb]
        g = re.search(r'\bGetFrame\s*\(', body)
        if g:
            if not re.search(r'\bstatic\s+long\s+__fastcall\s+GetFrame\s*\(\s*\)\s*;', body):
                return None, 'CWorld has another GetFrame'
            return [del_span(text, m, *at)], None
        k = c.rb
        acc = '' if re.search(r'public\s*:', body) and not re.search(r'(private|protected)\s*:', body) else 'public: '
        if c.key == 'struct' and not re.search(r'(private|protected)\s*:', body): acc = ''
        nl = '\n' if '\n' in body else ' '
        return [(k, k, '%s%s%s%s' % (' ' if nl == ' ' else '    ', acc, MEMBER, (CMT + '\n') if nl == '\n' else ' ')),
                del_span(text, m, *at)], None
    eol = text.find('\n', at[1]); eol = len(text) if eol < 0 else eol
    has_cmt = text[at[1]:eol].strip() != ''
    top = innermost(sc, at[0])
    while top is not None and top.parent is not None and top.parent.kind != 'linkage':
        top = top.parent
    if top is None or top.kind == 'linkage':
        return [(at[0], at[1], NEWCLS + ('' if has_cmt else CMT))], None
    # the stand-in sits in a class or namespace: CWorld goes to file scope, on its own line before that scope
    hs = top.lb
    while hs > 0 and m[hs - 1] not in ';{}': hs -= 1
    while hs < top.lb and m[hs] in ' \t\r\n': hs += 1
    k = text.rfind('\n', 0, hs) + 1
    return [(k, k, NEWCLS + CMT + '\n'), del_span(text, m, *at)], None


def call_sites(m, name, skip):
    out = []
    for p in tokens(m, name):
        if any(a <= p < b for a, b in skip): continue
        out.append(p)
    return out


TESTROWS = ('004c6060', '00697710', '006c04b0', '006fedb0', '00811450')   # tests that stub the stand-in by name


def test_transform(text, a):
    """A test that links the row's object and defines the free stand-in `R N(...) { body }` as its stub: the stub
    becomes `long __fastcall CWorld::GetFrame() { body }` (CWorld declared before it unless the test has one), and the
    test's own declarations of N go. Blockers: the body uses a parameter, any other use of N."""
    if a not in TESTROWS or ROWS[a][0] != 'free':
        return text, 0, []
    N = ROWS[a][1]
    m = mask(text)
    sc = scopes(m)
    hd = re.compile(r'(?:\bextern\s+(?:"C"\s+)?)?(?P<ret>\b(?:unsigned\s+|signed\s+)?(?:int|long))\s*(?:%s\s+)?%s\s*\((?P<prm>[^()]*)\)\s*(?P<end>[;{])' % (CC, re.escape(N)))
    defs, decls = [], []
    for d in hd.finditer(text):             # on the text: `extern "C"` keeps its literal
        if m[d.start('ret'):d.end()] != text[d.start('ret'):d.end()]:
            continue                           # inside a comment
        s = innermost(sc, d.start())
        if s is not None and s.kind not in ('ns', 'linkage'):
            continue
        (defs if d.group('end') == '{' else decls).append(d)
    if len(defs) != 1:
        return text, 0, ['TEST_DEFS %d' % len(defs)] if defs else []
    d = defs[0]
    lb = d.end() - 1
    from famlib import match_brace
    rb = match_brace(m, lb)
    for pn in re.findall(r'(\w+)\s*(?:,|$)', d.group('prm').strip()):
        if pn not in ('void',) and re.search(r'\b%s\b' % pn, m[lb:rb]) and not re.fullmatch(r'\w+', d.group('prm').strip()):
            return text, 0, ['TEST_BODY_USES %s' % pn]
    spans = [(x.start(), x.end()) for x in decls] + [(d.start(), d.end())]
    for p in tokens(m, N):
        if not any(x <= p < y for x, y in spans):
            return text, 0, ['TEST_OTHER_USE line %d' % (text.count('\n', 0, p) + 1)]
    edits = [del_span(text, m, x.start(), x.end()) for x in decls]
    rep = 'long __fastcall CWorld::GetFrame() {'
    if not [s for s in sc if s.kind == 'class' and s.name == 'CWorld']:
        rep = NEWCLS + CMT + '\n' + rep
    edits.append((d.start(), d.end(), rep))
    return apply_edits(text, drop_nested(edits)), len(edits), []


def transform(text, path):
    a, is_test = row_of(path)
    if a not in ROWS:
        return text, 0, []
    if is_test:
        return test_transform(text, a)
    spec = ROWS[a]
    m = mask(text)
    if re.search(r'#\s*define\s+CWorld\b', m):
        return text, 0, ['RENAMED CWorld']
    sc = scopes(m)
    edits = []
    if spec[0] == 'standin':
        _, X, M = spec
        cl = [s for s in sc if s.kind == 'class' and s.name == X]
        if not cl:
            return text, 0, []
        if len(cl) != 1:
            return text, 0, ['MULTI %s' % X]
        c = cl[0]
        st = statements(m, c.lb, c.rb)
        if len(st) != 1 or not re.match(r'^\s*static\s+long\s+(?:%s\s+)?%s\s*\(\s*(?:void)?\s*\)\s*;\s*$' % (CC, M), m[st[0][0]:st[0][1]]):
            return text, 0, ['STANDIN_BODY %s' % X]
        hs = m.rfind(c.key, 0, c.lb)
        en = c.rb + 1
        k = en
        while k < len(m) and m[k] in ' \t': k += 1
        if k < len(m) and m[k] == ';': en = k + 1
        plan, why = cworld_plan(text, m, sc, (hs, en))
        if plan is None:
            return text, 0, [why]
        edits += plan
        for p in tokens(m, X):
            if hs <= p < en: continue
            edits.append((p, p + len(X), 'CWorld'))
        for p in tokens(m, M):
            if c.lb < p < c.rb: continue
            j = prev_sig(m, p)
            if not (m[j] in '.>' or (m[j] == ':' and prev_word(m, j - 1) == X)):
                return text, 0, ['OTHER_USE %s line %d' % (M, text.count('\n', 0, p) + 1)]
            edits.append((p, p + len(M), 'GetFrame'))
    else:
        N = spec[1]
        if spec[0] == 'free':
            dre = re.compile(r'(?:\bextern\s+(?:"C"\s+)?)?(?P<ret>\b(?:unsigned\s+|signed\s+)?(?:int|long|void\s*\*|char|short))\s*(?:%s\s+)?%s\s*\((?P<prm>[^()]*)\)\s*;' % (CC, re.escape(N)))
        else:
            dre = re.compile(r'(?P<ret>\b(?:unsigned\s+|signed\s+)?(?:int|long))\s*(?:%s\s+)?%s\s*\((?P<prm>\s*(?:void)?\s*)\)\s*(?:const\s*)?;' % (CC, re.escape(N)))
        decls = []
        for d in dre.finditer(text):           # on the text: `extern "C"` keeps its literal
            if m[d.start('ret'):d.end()] != text[d.start('ret'):d.end()]:
                continue                       # inside a comment
            s = innermost(sc, d.start())
            if spec[0] == 'free' and s is not None and s.kind not in ('ns', 'linkage'):
                continue
            if spec[0] == 'member' and (s is None or s.kind != 'class'):
                continue
            decls.append(d)
        if len(decls) != 1:
            return text, 0, ['DECLS %d' % len(decls)] if decls else []
        d = decls[0]
        ret = re.sub(r'\s+', ' ', d.group('ret')).strip()
        prm = d.group('prm').strip()
        if spec[0] == 'free' and prm not in ('', 'void') and (',' in prm or '(' in prm):
            return text, 0, ['PARAMS %s' % prm]
        st = d.start()
        plan, why = cworld_plan(text, m, sc, (st, d.end()))
        if plan is None:
            return text, 0, [why]
        edits += plan
        rep = 'CWorld::GetFrame()' if ret in ('int', 'long') else '((%s)CWorld::GetFrame())' % ret
        for p in call_sites(m, N, [(d.start(), d.end())]):
            j = prev_sig(m, p)
            b = p
            if m[j] == '>' and m[j - 1] == '-' and m[j - 5:j - 1] == 'this':
                b = j - 5
            elif m[j] in '.>:':
                return text, 0, ['QUALIFIED_USE line %d' % (text.count('\n', 0, p) + 1)]
            r = re.match(r'\s*\(\s*(?:this|[A-Za-z_]\w*)?\s*\)', m[p + len(N):])
            if not r:
                return text, 0, ['CALL_SHAPE line %d' % (text.count('\n', 0, p) + 1)]
            edits.append((b, p + len(N) + r.end(), rep))
    # comments that name the stand-in next to the address
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
