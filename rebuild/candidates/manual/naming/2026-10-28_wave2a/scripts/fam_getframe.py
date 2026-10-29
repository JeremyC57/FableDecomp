#!/usr/bin/env python3
"""Respell the retail frame getter 0x0049D870 to its PDB form `?GetFrame@CWorld@@SIJXZ`
(static long __fastcall CWorld::GetFrame(void); Ego_r.pdb + FableWin.pdb; body reads the frame
global 0x013B89BC).

Shapes handled (all byte-neutral: no arguments, so cdecl/fastcall/static/free emit the same call;
`unsigned long`/`int` -> `long` only moves the mangled return code, the recheck proves the bytes):
  MEMBER  `static <long|unsigned long|int> [cc] N()` inside a file-scope `class|struct CWorld`,
          N in {GetWorldFrame, GetCurrentFrame, GetFrame, GetFrameNumber, GetFrameCount}
          -> `static long __fastcall GetFrame()`; every code token N -> GetFrame (N must not be
          declared anywhere else in the file), an out-of-line `<ret> CWorld::N()` gets `long`.
  FREE    a file-scope `[extern ["C"]] <long|unsigned long|int> [cc] N();` declaration (and a test's
          free stub definition), N in {GFGetCurrentFrame, GetWorldFrame, GetWorldFrame_0049d870,
          GFGetFrameCount} or any of {GetFrameCount, ReadGameTime, GetGameTick} with a 0x0049D870
          comment on the line -> the getter becomes a static member of the file's CWorld model:
          inserted at the end of a file-scope CWorld body when the file defines one, else the
          declaration becomes `class CWorld { public: static long __fastcall GetFrame(); };`
          (the class key follows an existing `struct CWorld;` forward declaration so no other
          mangled name moves); calls `N()` -> `CWorld::GetFrame()`; a stub definition
          `<ret> [cc] N() {` -> `long CWorld::GetFrame() {`.
Comments: `CWorld::N` and a bare `N`/`N()` within 40 characters of `0x0049D870` become
`CWorld::GetFrame`.
Blockers (no edit, printed as `note`): OTHER_CLASS (member of a class not named CWorld),
NESTED (CWorld not at file scope), AMBIG (N declared elsewhere in the file), HEADER (file includes
engine/CWorld.h: the generated header has no GetFrame member, see HEADERS.md), MEMBER_CALL.
Idempotent.  usage: fam_getframe.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import (mask, scopes, innermost, at_global, is_global_pos, global_classes, apply_edits,
                    tokens, prev_sig, prev_word, comment_spans, run, drop_nested, del_span)

MEMBER_NAMES = ('GetWorldFrame', 'GetCurrentFrame', 'GetFrame', 'GetFrameNumber', 'GetFrameCount')
FREE_STRONG = ('GFGetCurrentFrame', 'GetWorldFrame', 'GetWorldFrame_0049d870', 'GFGetFrameCount')
FREE_ANCHORED = ('GetFrameCount', 'ReadGameTime', 'GetGameTick')
ANCHOR = re.compile(r'0x0*49D870\b', re.I)
RET = r'(?:unsigned\s+long|unsigned\s+int|unsigned|long|int)'
CC = r'(?:__fastcall|__cdecl|__stdcall|FABLE_FASTCALL)'
NEW_DECL = 'long __fastcall GetFrame'

MEMBER_DECL = re.compile(r'\bstatic\s+(?:inline\s+)?(?P<ret>%s)\s+(?:(?P<cc>%s)\s+)?(?P<name>%s)\s*\(\s*(?:void)?\s*\)'
                         % (RET, CC, '|'.join(MEMBER_NAMES)))
FREE_DECL = re.compile(r'(?P<pre>\b(?:extern\s+(?:"\s*"\s+)?|static\s+(?:inline\s+)?|inline\s+))?(?P<ret>(?<![\w:])%s)\s+(?:(?P<cc>%s)\s+)?(?P<name>%s)\s*\(\s*(?:void)?\s*\)\s*(?P<end>[;{])'
                       % (RET, CC, '|'.join(FREE_STRONG + FREE_ANCHORED)))
DEFN_OOL = r'(?P<ret>(?<![\w:])%s)(?P<sp>\s+)(?:%s\s+)?CWorld\s*::\s*%s\s*\('


def line_of(text, pos):
    a = text.rfind('\n', 0, pos) + 1
    b = text.find('\n', pos)
    return a, (len(text) if b < 0 else b)


def decl_like(m, p, name):
    """Token at p is a declaration/definition of a function `name` (type word before it)."""
    j = prev_sig(m, p)
    if j < 0: return False
    if m[j] in '.>:': return False
    if m[j] in '*&': return True
    w = prev_word(m, p)
    if not w or w in ('return', 'new', 'delete', 'else', 'case', 'throw', 'sizeof', 'do'):
        return False
    k = p + len(name)
    while k < len(m) and m[k] in ' \t\r\n': k += 1
    return k < len(m) and m[k] == '(' and (w[0].isalpha() or w[0] == '_')


def comment_edits(text, m, name):
    if name == 'GetFrame':
        return []
    out = []
    for a, b in comment_spans(text, m):
        seg = text[a:b]
        for mm in re.finditer(r'(?:(?<![\w:])CWorld\s*::\s*)?(?<![\w:])%s(?!\w)' % re.escape(name), seg):
            st, en = a + mm.start(), a + mm.end()
            qualified = mm.group(0).startswith('CWorld')
            if qualified or ANCHOR.search(text[en:en + 40]):
                out.append((st, en, 'CWorld::GetFrame'))
    return out


def transform(text, path):
    m = mask(text)
    sc = scopes(m)
    edits, notes = [], []
    done_names = set()
    absorbed = set()
    worlds = global_classes(m, sc, 'CWorld')
    # ---- MEMBER shape
    for mm in MEMBER_DECL.finditer(m):
        s = innermost(sc, mm.start())
        name = mm.group('name')
        if s is None or s.kind != 'class':
            continue
        if s.name != 'CWorld':
            notes.append('OTHER_CLASS %s::%s' % (s.name, name)); continue
        if not at_global(s):
            notes.append('NESTED CWorld::%s' % name); continue
        ret, cc = re.sub(r'\s+', ' ', mm.group('ret')), mm.group('cc')
        if name == 'GetFrame' and ret == 'long' and cc == '__fastcall':
            continue
        # the name must be declared nowhere else
        others = [p for p in tokens(m, name) if decl_like(m, p, name) and not (s.lb < p < s.rb)]
        if others:
            notes.append('AMBIG %s declared at %s' % (name, ','.join(str(text.count('\n', 0, p) + 1) for p in others))); continue
        if name != 'GetFrame' and any(s.lb < p < s.rb for p in tokens(m, 'GetFrame')):
            notes.append('AMBIG CWorld already has GetFrame'); continue
        edits.append((mm.start('ret'), mm.end('name'), NEW_DECL))
        # an inline wrapper `{ return FREE(); }` around a free stand-in of the same getter:
        # the member itself is the retail function, so it becomes a plain declaration
        k = mm.end()
        while k < len(m) and m[k] in ' \t': k += 1
        wb = re.match(r'\{\s*return\s+(?:::\s*)?(%s)\s*\(\s*\)\s*;\s*\}' % '|'.join(FREE_STRONG + FREE_ANCHORED), m[k:])
        if wb:
            edits.append((mm.end(), k + wb.end(), ';'))
            absorbed.add(wb.group(1))
        if name != 'GetFrame':
            for p in tokens(m, name):
                if mm.start('ret') <= p < mm.end('name'): continue
                j = prev_sig(m, p)
                edits.append((p, p + len(name), 'GetFrame'))
        for od in re.finditer(DEFN_OOL % (RET, CC, re.escape(name)), m):
            if re.sub(r'\s+', ' ', od.group('ret')) != 'long':
                edits.append((od.start('ret'), od.end('ret'), 'long'))
        edits += comment_edits(text, m, name)
        done_names.add(name)
    # ---- FREE shape
    free = {}
    for mm in FREE_DECL.finditer(m):
        name = mm.group('name')
        if name in done_names: continue
        if not is_global_pos(sc, mm.start()): continue
        a, b = line_of(text, mm.start())
        if name in FREE_ANCHORED and not ANCHOR.search(text[a:b]):
            continue
        free.setdefault(name, []).append(mm)
    good = {}
    for name, ms in free.items():
        span = set()
        for x in ms: span.add((x.start('ret') if not x.group('pre') else x.start('pre'), x.end('name')))
        bad = False
        calls = []
        for p in tokens(m, name):
            if any(a <= p < b for a, b in span): continue
            j = prev_sig(m, p)
            if j >= 0 and m[j] in '.>':
                notes.append('MEMBER_CALL %s line %d' % (name, text.count('\n', 0, p) + 1)); bad = True; break
            if decl_like(m, p, name):
                notes.append('AMBIG %s line %d' % (name, text.count('\n', 0, p) + 1)); bad = True; break
            if j >= 0 and m[j] == ':' and j > 0 and m[j - 1] == ':':
                w = prev_word(m, j - 1)
                if w:
                    notes.append('QUALIFIED %s::%s' % (w, name)); bad = True; break
            calls.append(p)
        if not bad:
            good[name] = (ms, calls)
    if good:
        ok = True
        if re.search(r'#\s*include\s*[<"]engine/CWorld\.h[>"]', text):
            notes.append('HEADER engine/CWorld.h (%s)' % ','.join(sorted(good))); ok = False
        elif len(worlds) > 1:
            notes.append('MULTI CWorld bodies'); ok = False
        elif set(good) <= absorbed:
            pass
        elif worlds and any(worlds[0].lb < p < worlds[0].rb for p in tokens(m, 'GetFrame')):
            notes.append('AMBIG CWorld already has GetFrame'); ok = False
        if ok:
            allm = sorted((x for ms, _ in good.values() for x in ms), key=lambda x: x.start())
            decls = [x for x in allm if x.group('end') == ';']
            defs = [x for x in allm if x.group('end') == '{']
            def st_of(x): return x.start('pre') if x.group('pre') else x.start('ret')
            if worlds and any(x.start() < worlds[0].lb for x in defs):
                notes.append('DEF_BEFORE_CLASS (file left unedited)')
                return text, 0, notes
            fwd = re.search(r'\b(class|struct)\s+CWorld\s*;', m)
            key = fwd.group(1) if fwd else 'class'
            if set(good) <= absorbed:
                for x in decls:
                    edits.append(del_span(text, m, st_of(x), x.end()))
            elif worlds:
                w = worlds[0]
                acc = '' if (w.key == 'struct' and not re.search(r'\b(private|protected)\s*:', m[w.lb:w.rb])) else 'public:\n'
                ins = ('\n' if m[w.rb - 1] != '\n' else '') + acc + '    static long __fastcall GetFrame();   // 0x0049D870\n'
                edits.append((w.rb, w.rb, ins))
                for x in decls:
                    edits.append(del_span(text, m, st_of(x), x.end()))
            else:
                newcls = '%s CWorld { %sstatic long __fastcall GetFrame(); };' % (key, 'public: ' if key == 'class' else '')
                first = allm[0]
                if first.group('end') == ';':
                    edits.append((st_of(first), first.end(), newcls))
                else:
                    a0, _ = line_of(text, first.start())
                    edits.append((a0, a0, newcls + '   // 0x0049D870\n'))
                for x in decls:
                    if x is not first:
                        edits.append(del_span(text, m, st_of(x), x.end()))
            for x in defs:
                edits.append((st_of(x), x.end('name'), 'long CWorld::GetFrame'))
            for name, (ms, calls) in good.items():
                for p in calls:
                    edits.append((p, p + len(name), 'CWorld::GetFrame'))
                edits += comment_edits(text, m, name)
    edits = drop_nested(edits)
    if not edits:
        return text, 0, notes
    return apply_edits(text, edits), len(edits), notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
