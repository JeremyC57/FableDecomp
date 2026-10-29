# fam_pgd8.py <file>...: 0x00438570 is the static CPlayerGui::PeekGuiDef returning `const CPlayerGuiDef&` (Ego_r.pdb + FableWin.pdb
#   ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ; already bound by 0066e742). The free `CPlayerGuiDef* __fastcall PeekGuiDef();`
#   declaration becomes a static member of the file's `class CPlayerGui` (or a one-member CPlayerGui when the file has none);
#   `X* d = PeekGuiDef();` -> `const X* d = &CPlayerGui::PeekGuiDef();`, `PeekGuiDef()->` -> `CPlayerGui::PeekGuiDef().`;
#   a stand-in definition `CPlayerGuiDef* __fastcall PeekGuiDef() { ...return &g; }` becomes the member's. The def is now read
#   through const, so the file's local inline element accessors (operator[], front, begin) get the STL const overload. Idempotent.
import sys, re
MEMBER = 'static const CPlayerGuiDef& __fastcall PeekGuiDef();'
FREE = re.compile(r'^[ \t]*(?:extern\s+)?CPlayerGuiDef\s*\*\s*__fastcall\s+PeekGuiDef\(\s*\)\s*;([^\n]*)\n', re.M)
DEF = re.compile(r'^([ \t]*)CPlayerGuiDef\s*\*\s*__fastcall\s+PeekGuiDef\(\s*\)\s*\{([^{}]*?)return\s+(&\s*)?(\w+)\s*;\s*\}', re.M)
CG = re.compile(r'^(class|struct)\s+CPlayerGui\s*(?::[^{;]*)?\{[ \t]*(public:)?[^\n]*\n', re.M)
ACC = re.compile(r'^([ \t]*)([\w:<>]+)\s*([&*])\s*(operator\[\]|front|begin)\(([^)]*)\)\s*\{\s*return\s+([^;{}]+);\s*\}[^\n]*\n', re.M)
def add_member(t, after):
    cg = CG.search(t)
    k = cg.end()
    if cg.group(1) == 'class' and not cg.group(2):
        nxt = t[k:]
        mm = re.match(r'\s*public:[^\n]*\n', nxt)
        if mm: k += mm.end(); ins = '    %s%s\n' % (MEMBER, after)
        else: ins = 'public:\n    %s%s\n' % (MEMBER, after)
    else: ins = '    %s%s\n' % (MEMBER, after)
    t = t[:k] + ins + t[k:]
    if not re.search(r'\bclass\s+CPlayerGuiDef\b', t[:cg.start()]):
        t = t[:cg.start()] + 'class CPlayerGuiDef;\n' + t[cg.start():]
    return t
# deferred: the const def reaches the map find 0x0042B467 whose landed stand-in spellings are non-const (needs the map's real const
#   find spelling first): 005beb57 0082ba00 0082be60
SKIP = ('_005beb57', '_0082ba00', '_0082be60')
for p in sys.argv[1:]:
    if any(s in p for s in SKIP): print('skip', p, 'deferred (const map find)'); continue
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read(); o = t
    m = FREE.search(t); d = DEF.search(t)
    if not m and not d: print('skip', p, 'no free PeekGuiDef'); continue
    if m:
        after = m.group(1); t = t[:m.start()] + t[m.end():]
        cg = CG.search(t)
        if cg: t = add_member(t, after)
        else: t = t[:m.start()] + 'class CPlayerGui { public: %s };%s\n' % (MEMBER, after) + t[m.start():]
    d = DEF.search(t)
    if d:
        if not re.search(r'static\s+const\s+CPlayerGuiDef\s*&\s*__fastcall\s+PeekGuiDef\(\)', t):
            if CG.search(t): t = add_member(t, ''); d = DEF.search(t)
            else: t = t[:d.start()] + 'class CPlayerGui { public: %s };\n' % MEMBER + t[d.start():]; d = DEF.search(t)
        t = DEF.sub(lambda q: '%sconst CPlayerGuiDef& __fastcall CPlayerGui::PeekGuiDef() {%sreturn %s%s; }'
                    % (q.group(1), q.group(2), '' if q.group(3) else '*', q.group(4)), t)
    t = re.sub(r'(?<!const )\b(CPlayerGuiDef\s*\*\s*\w+\s*=\s*)PeekGuiDef\(\s*\)', r'const \1&CPlayerGui::PeekGuiDef()', t)
    t = re.sub(r'\b(const\s+CPlayerGuiDef\s*\*\s*\w+\s*=\s*)PeekGuiDef\(\s*\)', r'\1&CPlayerGui::PeekGuiDef()', t)
    t = re.sub(r'(?<![\w:&])PeekGuiDef\(\s*\)\s*->', 'CPlayerGui::PeekGuiDef().', t)
    if t != o:
        def acc(q):
            ind, ty, ref, nm, args, ex = q.groups()
            if re.search(r'^[ \t]*const\s+%s\s*\%s\s*%s\(' % (re.escape(ty), ref, re.escape(nm)), t[q.end():q.end() + 400], re.M) or 'const' in ty:
                return q.group(0)
            return q.group(0) + '%sconst %s%s %s(%s) const { return %s; }\n' % (ind, ty, ref, nm, args, ex)
        t = ACC.sub(acc, t)
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t); print('edited', p)
    else: print('skip', p, 'nothing to respell')
