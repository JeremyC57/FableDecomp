// CString::Right(int nCount) -- EH-framed MFC-style `CString::Right` (the manifest's "CShaderResource_Assign_Range" is propagated): a negative count is clamped to 0; the data header is fetched through the inlined null-object check (log sink at 0x0143BB2C called with the literal, then the empty trace temporary @ 0x00C16750 in the dead count slot) and a count at or past the length returns a copy of `*this` (@ 0x00C0DAB0); otherwise an empty string on the frame (state 1) receives the tail through `AllocCopy` (@ 0x00C0BF90: count, length - count, 0; the header fetched again with its own check) and is copied into the hidden result (flag at +0xc) before its destructor (@ 0x00C0BA40)
// Leaf: ?Right@CString@@QBE?AV1@H@Z
// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 3): on the library CString (was the stand-in CString_00c0e0f0); int counts as the MFC interface and the bound 0x00C0E440 spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E0F0, oracle length 245 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push esi ; push edi ; mov dword ptr [esp+0Ch],0 ; mov edi,[esp+24h] ; test edi,edi ; mov esi,ecx ; jge a ; xor edi,edi ; a: cmp dword ptr [esi],0 ; jne b ; mov eax,[g_pfnLogSink] ; test eax,eax ; je c ; mov ecx,offset literal ; call eax ; c: lea ecx,[esp+24h] ; call CTrace::CTrace ; b: mov eax,[esi] ; cmp edi,[eax-8] ; jl tail ; push esi ; mov esi,[esp+24h] ; mov ecx,esi ; call CString::CString(const CString&) ; ... mov eax,esi ; ... ret 8 ; tail: mov ecx,[g_pchNil] ; mov [esp+8],ecx ; test eax,eax ; mov dword ptr [esp+18h],1 ; jne d ; ... (null-object check) ; d: mov edx,[esi] ; mov eax,[edx-8] ; push 0 ; sub eax,edi ; push eax ; push edi ; lea ecx,[esp+14h] ; push ecx ; mov ecx,esi ; call AllocCopy ; mov esi,[esp+20h] ; lea edx,[esp+8] ; push edx ; mov ecx,esi ; call CString::CString(const CString&) ; lea ecx,[esp+8] ; mov dword ptr [esp+0Ch],1 ; mov byte ptr [esp+18h],0 ; call ~CString ; ... mov eax,esi ; ... ret 8
// The sibling `Left` (0x00C0E1F0) homes its temporary in the count slot and is parked.
typedef void (__fastcall* PFNASSERTHOOK)(const char* message);
extern PFNASSERTHOOK g_pfnAssertHook;           // 0x0143BB2C

struct CDebugBreak {                            // empty temporary whose constructor calls DebugBreak
    CDebugBreak();                              // 0x00C16750
};

struct CStringData {                            // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
};

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
}