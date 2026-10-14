// CString_00c73a00::ClassifyShaderExtension() -- EH-framed MFC-style string body: 2 while the global byte @ 0x0143CCD8 is set; 3 when the text is shorter than 4 (`__stdcall` safe length @ 0x00C0BCC0); else the last four characters (by value @ 0x00C0E0F0, state 0) compare (@ 0x00C0BD60) against `.ogg` (1) and `.wma` (0), 3 otherwise; the temporary is released (@ 0x00C0BA40) on each path
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73A00, oracle length 225 bytes. Built under /EHsc.
// Retail bytes:  ... mov al,[g_flag] ; test al,al ; push esi ; mov esi,ecx ; je len ; mov eax,2 ; ... ret ; len: mov eax,[esi] ; push eax ; call SafeStrlen ; cmp eax,4 ; jl three ; push 4 ; lea ecx,[esp+8] ; push ecx ; mov ecx,esi ; call Right ; push offset ext1 ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call Compare ; test eax,eax ; lea ecx,[esp+4] ; jne second ; mov dword ptr [esp+10h],-1 ; call ~CString ; mov eax,1 ; ... ret ; second: push offset ext2 ; call Compare ; test eax,eax ; lea ecx,[esp+4] ; mov dword ptr [esp+10h],-1 ; jne other ; call ~CString ; xor eax,eax ; ... ret ; other: call ~CString ; three: ... mov eax,3 ... ret
// The 0x00C0xxxx library module (MFC CString). The manifest label is a description; named by
// the shape.
extern bool g_flag_0143ccd8;               // 0x0143CCD8
long __stdcall SafeStrlen_00c0bcc0(const char* text);   // 0x00C0BCC0

struct CString_00c73a00 {
    char* m_pchData;
    ~CString_00c73a00();                       // 0x00C0BA40
    CString_00c73a00 Right(int count) const;   // 0x00C0E0F0
    int Compare(const char* text) const;   // 0x00C0BD60
    long ClassifyShaderExtension() const;
};

long CString_00c73a00::ClassifyShaderExtension() const
{
    if (g_flag_0143ccd8)
        return 2;
    if (SafeStrlen_00c0bcc0(m_pchData) < 4)
        return 3;
    CString_00c73a00 tail = Right(4);
    if (tail.Compare(".ogg") == 0)
        return 1;
    if (tail.Compare(".wma") == 0)
        return 0;
    return 3;
}
