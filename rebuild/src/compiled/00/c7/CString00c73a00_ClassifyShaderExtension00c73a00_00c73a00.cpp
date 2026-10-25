// CString_00c73a00::ClassifyShaderExtension() -- EH-framed MFC-style string body: 2 while the global byte @ 0x0143CCD8 is set; 3 when the text is shorter than 4 (`__stdcall` safe length @ 0x00C0BCC0); else the last four characters (by value @ 0x00C0E0F0, state 0) compare (@ 0x00C0BD60) against `.ogg` (1) and `.wma` (0), 3 otherwise; the temporary is released (@ 0x00C0BA40) on each path
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C73A00, oracle length 225 bytes. Built under /EHsc.
// Retail bytes:  ... mov al,[g_flag] ; test al,al ; push esi ; mov esi,ecx ; je len ; mov eax,2 ; ... ret ; len: mov eax,[esi] ; push eax ; call SafeStrlen ; cmp eax,4 ; jl three ; push 4 ; lea ecx,[esp+8] ; push ecx ; mov ecx,esi ; call Right ; push offset ext1 ; lea ecx,[esp+8] ; mov dword ptr [esp+14h],0 ; call Compare ; test eax,eax ; lea ecx,[esp+4] ; jne second ; mov dword ptr [esp+10h],-1 ; call ~CString ; mov eax,1 ; ... ret ; second: push offset ext2 ; call Compare ; test eax,eax ; lea ecx,[esp+4] ; mov dword ptr [esp+10h],-1 ; jne other ; call ~CString ; xor eax,eax ; ... ret ; other: call ~CString ; three: ... mov eax,3 ... ret
// Leaf: ?ClassifyShaderExtension@CString_00c73a00@@
// The 0x00C0xxxx library module (MFC CString). The manifest label is a description; named by the shape. `this` is a
//   CString: the only caller (0x00C73AF0) builds a CString temporary (ctor 0x00C0BFE0) and passes its address in ecx.
// Fix (owner item 1g, sweep2): the string members are the landed libpk CString ones -- the static `__stdcall`
//   CString::SafeStrlen 0x00C0BCC0 (was a free function `SafeStrlen_00c0bcc0`), CString::CompareNoCase 0x00C0BD60 (was
//   `Compare` on a private copy of the class), ~CString 0x00C0BA40, and Right 0x00C0E0F0 (catalog
//   "CShaderResource_Assign_Range", propagated). The row's own descriptive name and module are kept: whether it is a
//   CString member or a libpk helper taking the string in ecx cannot be read from the bytes.
extern bool g_flag_0143ccd8;               // 0x0143CCD8

class CString {                            // libpk MFC-style string: one word
public:
    char* m_pchData;
    ~CString();                                            // 0x00C0BA40
    CString Right(int count) const;                        // 0x00C0E0F0
    int CompareNoCase(const char* lpsz) const;             // 0x00C0BD60
    static int __stdcall SafeStrlen(const char* lpsz);     // 0x00C0BCC0
};

struct CString_00c73a00 : CString {
    long ClassifyShaderExtension() const;
};

long CString_00c73a00::ClassifyShaderExtension() const
{
    if (g_flag_0143ccd8)
        return 2;
    if (SafeStrlen(m_pchData) < 4)
        return 3;
    CString tail = Right(4);
    if (tail.CompareNoCase(".ogg") == 0)
        return 1;
    if (tail.CompareNoCase(".wma") == 0)
        return 0;
    return 3;
}