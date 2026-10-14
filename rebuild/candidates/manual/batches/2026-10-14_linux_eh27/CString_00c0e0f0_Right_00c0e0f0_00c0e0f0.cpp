// CString_00c0e0f0::Right(count) -- EH-framed MFC-style `CString::Right` (the manifest's "CShaderResource_Assign_Range" is propagated): a negative count is clamped to 0; the data header is fetched through the inlined null-object check (log sink at 0x0143BB2C called with the literal, then the empty trace temporary @ 0x00C16750 in the dead count slot) and a count at or past the length returns a copy of `*this` (@ 0x00C0DAB0); otherwise an empty string on the frame (state 1) receives the tail through `AllocCopy` (@ 0x00C0BF90: count, length - count, 0; the header fetched again with its own check) and is copied into the hidden result (flag at +0xc) before its destructor (@ 0x00C0BA40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E0F0, oracle length 245 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,8 ; push esi ; push edi ; mov dword ptr [esp+0Ch],0 ; mov edi,[esp+24h] ; test edi,edi ; mov esi,ecx ; jge a ; xor edi,edi ; a: cmp dword ptr [esi],0 ; jne b ; mov eax,[g_pfnLogSink] ; test eax,eax ; je c ; mov ecx,offset literal ; call eax ; c: lea ecx,[esp+24h] ; call CTrace::CTrace ; b: mov eax,[esi] ; cmp edi,[eax-8] ; jl tail ; push esi ; mov esi,[esp+24h] ; mov ecx,esi ; call CString::CString(const CString&) ; ... mov eax,esi ; ... ret 8 ; tail: mov ecx,[g_pchNil] ; mov [esp+8],ecx ; test eax,eax ; mov dword ptr [esp+18h],1 ; jne d ; ... (null-object check) ; d: mov edx,[esi] ; mov eax,[edx-8] ; push 0 ; sub eax,edi ; push eax ; push edi ; lea ecx,[esp+14h] ; push ecx ; mov ecx,esi ; call AllocCopy ; mov esi,[esp+20h] ; lea edx,[esp+8] ; push edx ; mov ecx,esi ; call CString::CString(const CString&) ; lea ecx,[esp+8] ; mov dword ptr [esp+0Ch],1 ; mov byte ptr [esp+18h],0 ; call ~CString ; ... mov eax,esi ; ... ret 8
// The sibling `Left` (0x00C0E1F0) homes its temporary in the count slot and is parked. The class is named by address.
extern char* g_pchNil_0139c900;            // 0x0139C900
typedef void (__fastcall* PFNLOGSINK_00c0e0f0)(const char* text);
extern PFNLOGSINK_00c0e0f0 g_pfnLogSink_0143bb2c;   // 0x0143BB2C

struct CTrace_00c0e0f0 {                   // empty temporary (a debug-break helper): lands on a dead frame byte
    CTrace_00c0e0f0();                     // 0x00C16750
};

struct CStringData_00c0e0f0 {              // the 12-byte header in front of the characters
    long nRefs;
    long nDataLength;
    long nAllocLength;
};

struct CString_00c0e0f0 {                  // one word: the character data
    char* m_pchData;
    CString_00c0e0f0() : m_pchData(g_pchNil_0139c900) { }
    CString_00c0e0f0(const CString_00c0e0f0& other);   // 0x00C0DAB0
    ~CString_00c0e0f0();                   // 0x00C0BA40
    void AllocCopy(CString_00c0e0f0& dest, long copyLen, long copyIndex, long extraLen) const;   // 0x00C0BF90
    CStringData_00c0e0f0* GetData() const
    {
        if (m_pchData == 0) {              // the inlined null-object check
            if (g_pfnLogSink_0143bb2c)
                g_pfnLogSink_0143bb2c("Null object pointer");
            CTrace_00c0e0f0();
        }
        return ((CStringData_00c0e0f0*)m_pchData) - 1;
    }
    CString_00c0e0f0 Right(long count) const;
};

CString_00c0e0f0 CString_00c0e0f0::Right(long count) const
{
    if (count < 0)
        count = 0;
    if (count >= GetData()->nDataLength)
        return *this;
    CString_00c0e0f0 dest;
    AllocCopy(dest, count, GetData()->nDataLength - count, 0);
    return dest;
}
