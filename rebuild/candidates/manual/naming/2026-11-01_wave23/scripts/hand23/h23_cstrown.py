# h23_cstrown.py: naming wave 23, family cstrown -- DECISIONS_W22_RESOLVED item 3: the four libpk CString own rows still on stand-in classes
# (CString_00c0e0f0 / CString_00c0e360 / CString_00c0ad10) re-authored on the library `class CString` of the own rows ~CString 0x00C0BA40 /
# CString(const CString&) 0x00C0DAB0 / operator<< 0x00C0DEC0 / 0x00C0DEA0 (MFC interface): Mid / Right / Left / AllocCopy take int (the landed
# binder 0x00C0E440 emits ?Mid@CString@@QBE?AV1@HH@Z); the callees take the spellings their own rows / bound callers emit (operator=
# ??4CString@@QAEABV0@ABV0@@Z, CString(const char*) ??0CString@@QAE@PBD@Z, ReverseFind / TrimLeft / TrimRight(char) (own rows 0x00C0BC20 /
# 0x00C0D970 / 0x00C0D7C0 -- 0x00C0E360 called the last one `Remove`), SafeStrlen ?SafeStrlen@CString@@SGHPBD@Z 0x00C0BCC0, CDebugBreak
# 0x00C16750, the assertion hook g_pfnAssertHook 0x0143BB2C and _EmptyString 0x0139C900 of the ~CString / copy-ctor own rows).
# DescribeBitset_00c0ad10 keeps its stand-in function name (no evidence names it); its Append chains are the << chains of wave 22.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he

def body_after(t, marker):
    i = t.index(marker); return t[:i]

N = '// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 3): on the library CString (was the stand-in %s); int counts as the MFC interface and the bound 0x00C0E440 spelling.'

COMMON = '''typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

struct CStringData {                            // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
};
'''

def head(t, old_title, new_title, leaf, standin):
    L = t.split('\n')
    assert L[0].startswith('// ' + old_title), L[0][:80]
    L[0] = '// ' + new_title + L[0][len('// ' + old_title):]
    out = [L[0], '// Leaf: ' + leaf, N % standin]
    for l in L[1:]:
        if l.startswith('// Leaf: '): continue
        if not l.startswith('//'): break
        out.append(l)
    return '\n'.join(out) + '\n'

# ---- 0x00C0DF50 Mid
a = '00c0df50'
p, rel, t = he._rw(a, 'src')
if '?Mid@CString@@QBE?AV1@HH@Z' in t: print('already', a)
else:
    h = head(t, 'CString_00c0e0f0::Mid(long nFirst, long nCount)', 'CString::Mid(int nFirst, int nCount)', '?Mid@CString@@QBE?AV1@HH@Z', 'CString_00c0e0f0')
    h = h.replace('// Callees: CString(const CString&) 0x00C0DAB0, ~CString 0x00C0BA40, AllocCopy 0x00C0BF90, the debug-break\n//          temporary 0x00C16750,',
                  '// Callees: CString(const CString&) 0x00C0DAB0, ~CString 0x00C0BA40, AllocCopy 0x00C0BF90, CDebugBreak::CDebugBreak\n//          0x00C16750,')
    h = h.replace('// Model: the landed sibling CString_00c0e0f0::Right (0x00C0E0F0).', '// Model: the landed sibling CString::Right (0x00C0E0F0).')
    code = COMMON + '''
#define LIB_ASSERT(cond) \\
    if (!(cond)) { if (g_pfnAssertHook) g_pfnAssertHook(#cond "\\n"); CDebugBreak(); }

class CString {
public:
    char* m_pchData;                            // +0x00
    CString() { Init(); }
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    void Init();
    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;   // 0x00C0BF90
    CStringData* GetData() const
    {
        LIB_ASSERT(m_pchData != NULL)
        return ((CStringData*)m_pchData) - 1;
    }
    CString Mid(int nFirst, int nCount) const;
};
extern CString _EmptyString;                    // 0x0139C900 (its m_pchData)

inline void CString::Init()
{
    m_pchData = _EmptyString.m_pchData;
}

CString CString::Mid(int nFirst, int nCount) const
{
    if (nFirst < 0)
        nFirst = 0;
    if (nCount < 0)
        nCount = 0;
    if (nFirst + nCount > GetData()->nDataLength)
        nCount = GetData()->nDataLength - nFirst;
    if (nFirst > GetData()->nDataLength)
        nCount = 0;
    LIB_ASSERT(nFirst >= 0)
    LIB_ASSERT(nFirst + nCount <= GetData()->nDataLength)
    if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
        return *this;
    {
        CString dest;
        AllocCopy(dest, nCount, nFirst, 0);
        return dest;
    }
}'''
    he._w(p, h + '#include <stddef.h>\n' + code); print('ok', a)

# ---- 0x00C0E0F0 Right
a = '00c0e0f0'
p, rel, t = he._rw(a, 'src')
if '?Right@CString@@QBE?AV1@H@Z' in t: print('already', a)
else:
    h = head(t, 'CString_00c0e0f0::Right(count)', 'CString::Right(int nCount)', '?Right@CString@@QBE?AV1@H@Z', 'CString_00c0e0f0')
    h = h.replace(' The class is named by address.', '')
    code = COMMON + '''
class CString {
public:
    char* m_pchData;                            // +0x00
    CString() { Init(); }
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    void Init();
    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;   // 0x00C0BF90
    CStringData* GetData() const
    {
        if (m_pchData == 0) {                   // the inlined null-object check
            if (g_pfnAssertHook)
                g_pfnAssertHook("Null object pointer");
            CDebugBreak();
        }
        return ((CStringData*)m_pchData) - 1;
    }
    CString Right(int nCount) const;
};
extern CString _EmptyString;                    // 0x0139C900 (its m_pchData)

inline void CString::Init()
{
    m_pchData = _EmptyString.m_pchData;
}

CString CString::Right(int nCount) const
{
    if (nCount < 0)
        nCount = 0;
    if (nCount >= GetData()->nDataLength)
        return *this;
    CString dest;
    AllocCopy(dest, nCount, GetData()->nDataLength - nCount, 0);
    return dest;
}'''
    he._w(p, h + code); print('ok', a)

# ---- 0x00C0E360 GetFileNameWithoutExtension
a = '00c0e360'
p, rel, t = he._rw(a, 'src')
if '?GetFileNameWithoutExtension@CString@@QBE?AV1@XZ' in t: print('already', a)
else:
    h = head(t, 'CString_00c0e360::GetFileNameWithoutExtension()', 'CString::GetFileNameWithoutExtension()', '?GetFileNameWithoutExtension@CString@@QBE?AV1@XZ', 'CString_00c0e360')
    h = h.replace('released and the remaining dots removed (@ 0x00C0D7C0)', 'released and the trailing dots trimmed (TrimRight @ 0x00C0D7C0)')
    code = '''class CString {
public:
    char* m_pchData;                            // +0x00
    CString(const char* lpsz);                  // 0x00C0BFE0
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    const CString& operator=(const CString& stringSrc);   // 0x00C0C0C0
    int ReverseFind(char ch) const;             // 0x00C0BC20
    void TrimLeft(char chTarget);               // 0x00C0D970
    void TrimRight(char chTarget);              // 0x00C0D7C0
    CString Left(int nCount) const;             // 0x00C0E1F0
    CString GetFileNameWithoutExtension() const;
};

CString CString::GetFileNameWithoutExtension() const
{
    int pos = ReverseFind('\\\\');
    if (pos <= 0)
        pos = 0;
    CString name(m_pchData + pos);
    name.TrimLeft('\\\\');
    int dot = name.ReverseFind('.');
    if (dot >= 0) {
        name = name.Left(dot);
        name.TrimRight('.');
    }
    return name;
}'''
    he._w(p, h + code); print('ok', a)

# ---- 0x00C0AD10 DescribeBitset_00c0ad10
a = '00c0ad10'
p, rel, t = he._rw(a, 'src')
if '?DescribeBitset_00c0ad10@@YI?AVCString@@PBV1@KG@Z' in t: print('already', a)
else:
    h = head(t, 'DescribeBitset_00c0ad10(names, count, bits)', 'DescribeBitset_00c0ad10(names, count, bits)', '?DescribeBitset_00c0ad10@@YI?AVCString@@PBV1@KG@Z', 'CString_00c0ad10')
    h = h.replace('(Append(const CString&) @ 0x00C0DEA0)', '(operator<<(const CString&) @ 0x00C0DEA0)').replace('(lstrlenA through the null-safe wrapper @ 0x00C0BCC0)', '(lstrlenA through CString::SafeStrlen @ 0x00C0BCC0)')
    h = h.replace('\n// The classes are named by address.', '\n// The function keeps its stand-in name (no evidence names it).')
    code = '''class CString {
public:
    char* m_pchData;                            // +0x00
    CString() { Init(); }
    CString(const CString& stringSrc);          // 0x00C0DAB0
    ~CString();                                 // 0x00C0BA40
    void Init();
    static int __stdcall SafeStrlen(const char* lpsz);   // 0x00C0BCC0
    CString& __fastcall operator<<(const char* text);   // 0x00C0DEC0
    CString& __fastcall operator<<(const CString& string);   // 0x00C0DEA0
};
extern CString _EmptyString;                    // 0x0139C900 (its m_pchData)

inline void CString::Init()
{
    m_pchData = _EmptyString.m_pchData;
}

CString __fastcall DescribeBitset_00c0ad10(const CString* names, unsigned long count, unsigned short bits)
{
    unsigned long i = 0;
    CString line;
    if (bits == 0) {
        line << "[not set]";
        return line;
    }
    const CString* name = names;
    while (bits != 0) {
        if (i >= count)
            break;
        if (bits & 1) {
            if (CString::SafeStrlen(line.m_pchData))
                line << " + ";
            line << "'" << *name << "'";
        }
        bits >>= 1;
        ++i;
        ++name;
    }
    return line;
}'''
    he._w(p, h + code); print('ok', a)
