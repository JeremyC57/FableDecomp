# wave 6 family: engine/CDataOutputStream.h now declares the scalar / vector / colour / string writers (generator
# HEADER_MEMBERS, naming wave 6; PDB spellings, retail 0x00993E30..0x00995AE0). Rows that included the data-only header
# and carried a lane "method carrier" `struct CDataOutputStream_Methods : CDataOutputStream { ... }` (no such class in
# either PDB) call the real members instead.
# Transform (idempotent): the carrier struct is deleted when every member it declares maps onto a header member
#   WriteLong(long) -> WriteSLONG, WriteULong(unsigned long) -> WriteULONG, WriteUInt64(unsigned __int64) -> WriteUHUGE,
#   WriteBool(bool) -> WriteEBOOL, WriteEBOOL / WriteFloat / WriteColour / Write3DVector / Write2DVector /
#   WriteNullTerminatedString / WritePresizedString / WriteStringNewLine unchanged;
# `CDataOutputStream_Methods` becomes `CDataOutputStream`, and calls `.X(` / `->X(` of a renamed member are respelled.
# Skips (prints `skip <reason>`): a carrier with virtual members or a member the header lacks, no header include.
import re, sys
REN = {'WriteLong': 'WriteSLONG', 'WriteULong': 'WriteULONG', 'WriteUInt64': 'WriteUHUGE', 'WriteBool': 'WriteEBOOL'}
KEEP = {'WriteEBOOL', 'WriteFloat', 'WriteColour', 'Write3DVector', 'Write2DVector', 'WriteNullTerminatedString',
        'WritePresizedString', 'WriteStringNewLine', 'WriteSLONG', 'WriteULONG', 'WriteUHUGE'}
CARRIER = re.compile(r'^struct CDataOutputStream_Methods[ \t]*:[ \t]*(?:public[ \t]+)?CDataOutputStream[ \t]*\{[^\n]*\n((?:(?!\};)[^\n]*\n)*?)\};[ \t]*\n', re.M)
MEMBER = re.compile(r'^[ \t]*void[ \t]+(\w+)[ \t]*\([^;]*\)[ \t]*;[ \t]*(//[^\n]*)?$')
HDR = {'WriteEBOOL': 'bool', 'WriteUWORD': 'unsigned short', 'WriteSWORD': 'short', 'WriteULONG': 'unsigned long',
       'WriteSLONG': 'long', 'WriteUHUGE': 'unsigned __int64', 'WriteFloat': 'float',
       'WriteNullTerminatedWString': 'const CWideString&', 'WriteColour': 'const CRGBColour&',
       'Write2DVector': 'const C2DVector&', 'Write3DVector': 'const C3DVector&', 'WriteStringNewLine': 'const CCharString&',
       'WritePresizedString': 'const CCharString&', 'WriteNullTerminatedString': 'const CCharString&'}
TRICK = re.compile(r'^#define CDataOutputStream CDataOutputStreamLayout[ \t]*\n(#include "engine/CDataOutputStream.h"[ \t]*\n)'
                   r'#undef CDataOutputStream[ \t]*\n(?:[ \t]*\n)*class CDataOutputStream[ \t]*:[ \t]*public[ \t]+CDataOutputStreamLayout[ \t]*\n?\{[ \t]*\n?'
                   r'[ \t]*public:[ \t]*\n((?:[ \t]*void[^\n]*\n)*)\};[ \t]*\n', re.M)
OWN = re.compile(r'^[ \t]*void[ \t]+(\w+)[ \t]*\(([^)]*)\)[ \t]*;[ \t]*(//[^\n]*)?$')
def ptype(s):
    s = re.sub(r'\s+', ' ', s).strip()
    s = re.sub(r'\s*&\s*', '& ', s).strip()
    toks = s.split(' ')
    if len(toks) >= 2 and re.match(r'^[A-Za-z_]\w*$', toks[-1]) and toks[-1] not in ('long', 'short', 'bool', 'float', '__int64'):
        toks = toks[:-1]
    return ' '.join(toks).replace('& ', '&').strip()
def untrick(t):
    # the row renamed the data-only header class to CDataOutputStreamLayout and derived its own CDataOutputStream to
    # declare the writers; with the members in the header the plain include is the same class
    m = TRICK.search(t)
    if not m: return t, None
    for line in m.group(2).split('\n'):
        if not line.strip(): continue
        mm = OWN.match(line)
        if not mm or HDR.get(mm.group(1)) != ptype(mm.group(2)):
            return None, 'own CDataOutputStream member not in header: ' + line.strip()[:60]
    return t[:m.start()] + m.group(1) + t[m.end():], 'untrick'
def fix_test(t):
    # a self-contained test's spy `struct CDataOutputStream_Methods {` that the row's source object links against
    if not re.search(r'^struct CDataOutputStream_Methods[ \t]*\{', t, re.M): return None, 'no spy'
    t = re.sub(r'^struct CDataOutputStream_Methods[ \t]*\{', 'class CDataOutputStream { public:', t, flags=re.M)
    t = t.replace('CDataOutputStream_Methods', 'CDataOutputStream')
    for o, n in REN.items():
        t = re.sub(r'(\bvoid[ \t]+|::|\.|->)' + o + r'[ \t]*\(', r'\g<1>' + n + '(', t)
    return t, 'ok test spy'
def fix(t):
    if 'CDataOutputStream_Methods' not in t: return None, 'no carrier'
    if '#include "engine/CDataOutputStream.h"' not in t: return None, 'no header include'
    t, why = untrick(t)
    if t is None: return None, why
    m = CARRIER.search(t)
    if not m: return None, 'carrier shape'
    names = []
    for line in m.group(1).split('\n'):
        if not line.strip(): continue
        mm = MEMBER.match(line)
        if not mm: return None, 'carrier member: ' + line.strip()[:60]
        names.append(mm.group(1))
    bad = [n for n in names if n not in REN and n not in KEEP]
    if bad: return None, 'header lacks ' + ','.join(bad)
    t = t[:m.start()] + t[m.end():]
    t = t.replace('CDataOutputStream_Methods', 'CDataOutputStream')
    for o, n in REN.items():
        if o in names: t = re.sub(r'(\.|->)' + o + r'[ \t]*\(', r'\g<1>' + n + '(', t)
    return t, 'ok ' + ','.join(names)
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = (fix_test if p.endswith('_test.cpp') else fix)(t)
        if new is None: print('skip', p, why); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why)
