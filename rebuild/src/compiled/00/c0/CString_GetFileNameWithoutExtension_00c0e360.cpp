// CString::GetFileNameWithoutExtension() -- EH-framed MFC-style string body (hidden return pointer, `ret 4`): the text after the last backslash (@ 0x00C0BC20 reverse find, clamped to the start) becomes a stack string (@ 0x00C0BFE0, state 1) with leading backslashes trimmed (@ 0x00C0D970); when a dot is found its left part (by value @ 0x00C0E1F0, state 2) is assigned back (@ 0x00C0C0C0), released and the trailing dots trimmed (TrimRight @ 0x00C0D7C0); the string is copied to the hidden result (@ 0x00C0DAB0, flag word set to 1) and released (@ 0x00C0BA40)
// Leaf: ?GetFileNameWithoutExtension@CString@@QBE?AV1@XZ
// Naming (naming wave 23, DECISIONS_W22_RESOLVED item 3): on the library CString (was the stand-in CString_00c0e360); int counts as the MFC interface and the bound 0x00C0E440 spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0E360, oracle length 209 bytes. Built under /EHsc.
// Retail bytes:  ... push 5Ch ; mov esi,ecx ; mov dword ptr [esp+10h],0 ; call ReverseFind ; test eax,eax ; jg pos ; xor eax,eax ; pos: mov ecx,[esi] ; add ecx,eax ; push ecx ; lea ecx,[esp+8] ; call CString::CString(const char*) ; push 5Ch ; lea ecx,[esp+8] ; mov dword ptr [esp+1Ch],1 ; call TrimLeft ; push 2Eh ; lea ecx,[esp+8] ; call ReverseFind ; test eax,eax ; jl nodot ; push eax ; lea edx,[esp+0Ch] ; push edx ; lea ecx,[esp+0Ch] ; call Left ; push eax ; lea ecx,[esp+8] ; mov byte ptr [esp+1Ch],2 ; call operator= ; lea ecx,[esp+8] ; mov byte ptr [esp+18h],1 ; call ~CString ; nodot: push 2Eh ; lea ecx,[esp+8] ; call Remove ; mov esi,[esp+20h] ; lea eax,[esp+4] ; push eax ; mov ecx,esi ; call CString::CString(const CString&) ; lea ecx,[esp+4] ; mov dword ptr [esp+0Ch],1 ; mov byte ptr [esp+18h],0 ; call ~CString ; ...
// The 0x00C0xxxx library module (MFC CString). Named from the manifest (CCharString there).
class CString {
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
    int pos = ReverseFind('\\');
    if (pos <= 0)
        pos = 0;
    CString name(m_pchData + pos);
    name.TrimLeft('\\');
    int dot = name.ReverseFind('.');
    if (dot >= 0) {
        name = name.Left(dot);
        name.TrimRight('.');
    }
    return name;
}