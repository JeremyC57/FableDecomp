# h18_own.py: naming wave 18, family own --
#  0x00994B40: the landed CCreatureActionBase_GetFramesRemaining_00994b40.cpp body (`mov eax,[ecx+0xc]; sub eax,[ecx+8]; ret`) is
#    ?GetLength@CMemoryDataOutputStream@@QBEKXZ (DECISIONS_W17: data-stream block, the CMemoryDataOutputStream callers respelled in
#    wave 17, Ego_r symbol): Data.Size (+0xc) - Data.Start (+0x8) of the CArray<char> Data member (Ego_r CMemoryDataOutputStream +8).
#    File rename (RENAMES.tsv) + ps1; new test.
#  0x0074EE90: g_pMainGameComponent -> the protected static CMainGameComponent::PMainGameComponent (ESTABLISHED 0x013B86A0,
#    ?PMainGameComponent@CMainGameComponent@@1PAV1@A) read through the inline public static accessor Get() (Ego_r / FableWin
#    ?Get@CMainGameComponent@@SIPAV1@XZ), the retail way into a protected static member. Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import put, edit, resub
put('00994b40', 'src', '''// CMemoryDataOutputStream::GetLength() const -- the number of bytes written: Data.Size - Data.Start of the CArray<char> buffer
// Leaf: ?GetLength@CMemoryDataOutputStream@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994B40, 7 bytes: mov eax, [ecx+0xc] ; sub eax, [ecx+8] ; ret.
// Identity (DECISIONS_W17_RESOLVED 0x00994B40, chunk c681): the data-stream block, Ego_r.pdb ?GetLength@CMemoryDataOutputStream@@QBEKXZ;
//   the CMemoryDataOutputStream callers (0x004FBDE0, 0x00523B40, 0x006F92A0, 0x009B08C0, 0x009D4F90, 0x00A382E0, 0x00AAD150,
//   0x00AC3B00, 0x00ADFDC0 ...) bind it. Layout: Ego_r CMemoryDataOutputStream {CDataOutputStream base 8, CArray<char> Data +0x8
//   (Start +0x8, Size +0xc, ...), CurrentPosition +0x18}.
// Naming wave 18: landed as CCreatureActionBase::GetFramesRemaining (a shape label); respelled on the PDB member (name-only: the
//   bytes are unchanged; the body is an ICF-shared shape, this address is the stream's).
class CDataOutputStream { public:
    virtual ~CDataOutputStream();                    // slot 0 (vtable +0)
    unsigned char _pad04[4];                         // +0x04
};
template <class T> class CArray { public:
    T*            Start;                             // +0x00
    unsigned long Size;                              // +0x04
    unsigned char _pad08[4];                         // +0x08
};
class CMemoryDataOutputStream : public CDataOutputStream { public:
    CArray<char>  Data;                              // +0x08
    unsigned long CurrentPosition;                   // +0x18
    unsigned long GetLength() const;
};

unsigned long CMemoryDataOutputStream::GetLength() const
{
    return Data.Size - (unsigned long)Data.Start;
}
''')
put('00994b40', 'tst', '''#include <cstdio>
// Behaviour test for CMemoryDataOutputStream::GetLength() const (0x00994B40): Data.Size - Data.Start.
class CDataOutputStream { public:
    virtual ~CDataOutputStream() { }
    unsigned char _pad04[4];
};
template <class T> class CArray { public:
    T*            Start;
    unsigned long Size;
    unsigned char _pad08[4];
};
class CMemoryDataOutputStream : public CDataOutputStream { public:
    CArray<char>  Data;
    unsigned long CurrentPosition;
    unsigned long GetLength() const { return Data.Size - (unsigned long)Data.Start; }
};
int main()
{
    CMemoryDataOutputStream s; s.Data.Start = (char*)2; s.Data.Size = 7;
    if (s.GetLength() != 5) { std::printf("00994b40_TEST FAIL\\n"); return 1; }

    std::printf("00994b40_TEST PASS\\n");
    return 0;
}
''')
a = '0074ee90'
edit(a, 'src', '''class CMainGameComponent { public:
    unsigned char _pad00[0x14];''', '''class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
    unsigned char _pad00[0x14];''')
edit(a, 'src', 'extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0\n', '')
edit(a, 'src', 'CWideString text = g_pMainGameComponent->GetTextBank().GetTextBySymbol("TXT_SIGN_NOTHING");', 'CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TXT_SIGN_NOTHING");')
edit(a, 'src', '// Globals: g_pMainGameComponent 0x013B86A0 (GOTCHAS wave 25), g_pPlayerGui 0x013B8790 (majority spelling; descriptive).',
     '// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0 (ESTABLISHED_SPELLINGS; read through the inline static Get(), naming wave 18),\n//   g_pPlayerGui 0x013B8790 (majority spelling; descriptive).')
