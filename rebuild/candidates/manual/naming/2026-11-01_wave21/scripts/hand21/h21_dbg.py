# h21_dbg.py: naming wave 21, family dbg -- DECISIONS_W20_RESOLVED item "CDebugStream for the libpk error-macro stream (0x00C29FC0 and its users)":
# 0x00C29FC0 is the landed PKCSemaphore::PKCSemaphore(long, long) (own row, file string .\PKCSemaphore.cpp; dtor 0x00C29FB0 ??1PKCSemaphore@@QAE@XZ);
# its callers bind stand-ins (CMember_00c29fc0 in the eight CScriptEntry ctors 0x00C39290 / 0x00C39450 / 0x00C39610 / 0x00C397D0 / 0x00C39990 /
# 0x00C48800 / 0x00C49B30 / 0x00C4BC20, CTimer_ in 0x00C26C10 / 0x00C27070, CEventPart_ in 0x00C355E0): respelled PKCSemaphore, name-only.
# The error-macro stream of 0x00C355E0 (CString_00c355e0 Append/AppendNumber + CTrace_) is respelled to the landed own rows' class: CString
# (??6CString@@QAIAAV0@PBD@Z 0x00C0DEC0, ??6CString@@QAIAAV0@J@Z 0x00C0DF00, ??1CString@@QAE@XZ 0x00C0BA40) and CDebugBreak (??0CDebugBreak@@QAE@XZ
# 0x00C16750), exactly as the landed PKCDefaultFileManager ctor 0x00C1AC40 spells the same macro. The decision's "CDebugStream" is the spelling of
# two rows (0x00C29FC0, 0x00C09920) against the own rows' CString (the class also holds the parameter names of CAudioParamBlock 0x00C0B200: a string,
# not a stream): DECISIONS_W21 item -- the own rows' Leaf is authoritative, so the users follow CString here.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_dbg.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def note(a, txt, k='src'):
    if a in _X: return
    p, rel, t = he._rw(a, k)
    if txt in t: print('already', a, k); return
    m = re.search(r'^// Leaf: [^\n]*\n', t, re.M)
    i = m.end() if m else (t.index('\n') + 1)
    he._w(p, t[:i] + txt + '\n' + t[i:]); print('ok', a, k)
SN = '// Naming (naming wave 21, DECISIONS_W20_RESOLVED): the member built out of line @ 0x00C29FC0 is the landed PKCSemaphore (PKCSemaphore::PKCSemaphore(long initialCount, long maximumCount), file .\\PKCSemaphore.cpp; dtor 0x00C29FB0); the stand-in spelling is gone.'
for a in ('00c39290', '00c39450', '00c39610', '00c397d0', '00c39990', '00c48800', '00c49b30', '00c4bc20'):
    if a in _X: continue
    he.edit(a, 'src', 'struct CMember_00c29fc0 {\n    void* m_data;\n    CMember_00c29fc0(long zero, void* value);   // 0x00C29FC0\n};', 'class PKCSemaphore { public:\n    void* m_hSemaphore;                    // the Win32 semaphore handle\n    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0\n};')
    he.edit(a, 'src', '    CMember_00c29fc0 m_member;          // +0x1c', '    PKCSemaphore m_member;              // +0x1c')
    he.edit(a, 'src', '    , m_member(0, second)', '    , m_member(0, (long)second)')
    note(a, SN)
for a in ('00c26c10', '00c27070'):
    if a in _X: continue
    he.resub(a, 'src', r'struct CTimer_%s \{                   // \+0x28, one word\n    long m_0;\n    CTimer_%s\(long start, long period\);   // 0x00C29FC0\n    ~CTimer_%s\(\);\n\};' % (a, a, a),
             'class PKCSemaphore { public:               // +0x28, one word: the Win32 semaphore handle\n    void* m_hSemaphore;\n    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0\n    ~PKCSemaphore();                       // 0x00C29FB0\n};', 1)
    he.resub(a, 'src', r'\bCTimer_%s\b' % a, 'PKCSemaphore')
    he.resub(a, 'tst', r'\bCTimer_%s\b' % a, 'PKCSemaphore')
    note(a, SN)
a = '00c355e0'
if a not in _X:
    he.edit(a, 'src', '''struct CString_00c355e0 {                  // MFC-style string: one word, the shared empty data for the default constructor
    char* m_pchData;
    CString_00c355e0() : m_pchData(g_pchNil_0139c900) { }
    ~CString_00c355e0();                       // 0x00C0BA40
    CString_00c355e0* __fastcall Append(const char* text);   // 0x00C0DEC0
    CString_00c355e0* __fastcall AppendNumber(long value);   // 0x00C0DF00
};

struct CTrace_00c355e0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c355e0();                         // 0x00C16750
};
''', '''class CString {                            // the libpk MFC-style string: one word, the shared empty data for the default constructor (the landed own rows 0x00C0BA40 / 0x00C0DEC0 / 0x00C0DF00)
public:
    char* m_pchData;
    CString() : m_pchData(g_pchNil_0139c900) { }
    ~CString();                                // 0x00C0BA40
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(long value);         // 0x00C0DF00
};

struct CDebugBreak {                       // empty temporary whose constructor breaks into the debugger (lands on a dead frame byte)
    CDebugBreak();                             // 0x00C16750
};
''')
    he.edit(a, 'src', '''struct CEventPart_00c355e0 {               // +8
    unsigned char _body[4];
    CEventPart_00c355e0(long a, long b);   // 0x00C29FC0
    ~CEventPart_00c355e0();                // 0x00C29FB0
};''', '''class PKCSemaphore { public:               // +8: the Win32 semaphore handle
    void* m_hSemaphore;
    PKCSemaphore(long initialCount, long maximumCount);   // 0x00C29FC0
    ~PKCSemaphore();                       // 0x00C29FB0
};''')
    he.edit(a, 'src', '    CEventPart_00c355e0 m_event;           // +8', '    PKCSemaphore m_event;                  // +8')
    he.edit(a, 'src', '''            CString_00c355e0 line;
            line.Append(".\\\\PKCThreadManager.cpp")->Append("(")->AppendNumber(23)->Append(") : ")->Append("Problems starting thread")->Append("\\n");''', '''            CString line;
            line << ".\\\\PKCThreadManager.cpp" << "(" << (long)23 << ") : " << "Problems starting thread" << "\\n";''')
    he.edit(a, 'src', '        CTrace_00c355e0();', '        CDebugBreak();')
    note(a, SN + '\n// Naming (naming wave 21): the error-macro stream is the libpk CString of the landed own rows 0x00C0BA40 / 0x00C0DEC0 / 0x00C0DF00 and the break the landed CDebugBreak 0x00C16750 (as the landed PKCDefaultFileManager ctor 0x00C1AC40 spells the same macro).')

# the three rows' Leaf lines named no symbol (junk leaves from the first landing): the real constructor symbols
for a in ('00c26c10', '00c27070'):
    if a not in _X: he.edit(a, 'src', '// Leaf: NScript_CreatePredicate_IsThingAlive_Chain\n', '// Leaf: ??0CPredicateChain_%s@@QAE@PAX@Z\n' % a)
if '00c355e0' not in _X:
    p, rel, t = he._rw('00c355e0', 'src')
    if '// Leaf: ' in t: print('already', '00c355e0', 'src')
    else:
        i = t.index('\n') + 1; he._w(p, t[:i] + '// Leaf: ??0CWorkerThread_00c355e0@@QAE@PAXPBUSThreadParams_00c355e0@@@Z\n' + t[i:]); print('ok', '00c355e0', 'src')
