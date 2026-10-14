// CString_00c0e360::GetFileNameWithoutExtension() -- EH-framed MFC-style string body (hidden return pointer, `ret 4`): the text after the last backslash (@ 0x00C0BC20 reverse find, clamped to the start) becomes a stack string (@ 0x00C0BFE0, state 1) with leading backslashes trimmed (@ 0x00C0D970); when a dot is found its left part (by value @ 0x00C0E1F0, state 2) is assigned back (@ 0x00C0C0C0), released and the remaining dots removed (@ 0x00C0D7C0); the string is copied to the hidden result (@ 0x00C0DAB0, flag word set to 1) and released (@ 0x00C0BA40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E360, oracle length 209 bytes. Built under /EHsc.
// Retail bytes:  ... push 5Ch ; mov esi,ecx ; mov dword ptr [esp+10h],0 ; call ReverseFind ; test eax,eax ; jg pos ; xor eax,eax ; pos: mov ecx,[esi] ; add ecx,eax ; push ecx ; lea ecx,[esp+8] ; call CString::CString(const char*) ; push 5Ch ; lea ecx,[esp+8] ; mov dword ptr [esp+1Ch],1 ; call TrimLeft ; push 2Eh ; lea ecx,[esp+8] ; call ReverseFind ; test eax,eax ; jl nodot ; push eax ; lea edx,[esp+0Ch] ; push edx ; lea ecx,[esp+0Ch] ; call Left ; push eax ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],2 ; call operator= ; lea ecx,[esp+8] ; mov byte ptr [esp+18h],1 ; call ~CString ; nodot: push 2Eh ; lea ecx,[esp+8] ; call Remove ; mov esi,[esp+20h] ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call CString::CString(const CString&) ; lea ecx,[esp+4] ; mov dword ptr [esp+0Ch],1 ; mov byte ptr [esp+18h],0 ; call ~CString ; ...
// The 0x00C0xxxx library module (MFC CString). Named from the manifest (CCharString there).
struct CString_00c0e360 {
    char* m_pchData;
    CString_00c0e360(const char* text);    // 0x00C0BFE0
    CString_00c0e360(const CString_00c0e360& other);   // 0x00C0DAB0
    ~CString_00c0e360();                   // 0x00C0BA40
    const CString_00c0e360& operator=(const CString_00c0e360& other);   // 0x00C0C0C0
    int ReverseFind(char ch) const;        // 0x00C0BC20
    void TrimLeft(char ch);                // 0x00C0D970
    void Remove(char ch);                  // 0x00C0D7C0
    CString_00c0e360 Left(int count) const;   // 0x00C0E1F0
    CString_00c0e360 GetFileNameWithoutExtension() const;
};

CString_00c0e360 CString_00c0e360::GetFileNameWithoutExtension() const
{
    int pos = ReverseFind('\\');
    if (pos <= 0)
        pos = 0;
    CString_00c0e360 name(m_pchData + pos);
    name.TrimLeft('\\');
    int dot = name.ReverseFind('.');
    if (dot >= 0) {
        name = name.Left(dot);
        name.Remove('.');
    }
    return name;
}