// CFileInstallerSingleton::Get() -- lazily create the one CFileInstaller (0xA0 bytes) into its static counted pointer and return the object
// Leaf: ?Get@CFileInstallerSingleton@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00404440, 69 bytes (+11 int3 pad). Callees: operator new
// 0x00BFEA1A, CFileInstaller::CFileInstaller 0x009C5890, CCountedPointer<CFileInstaller>::Reset
// 0x00403DB0 (counted-pointer assignment; landed as CCountedPointer_00403630::Reset); the static
// counted pointer sits at 0x013CA818 (object +0, count block +4).
//
// Retail bytes:  mov eax,[g.object] ; test eax,eax ; jne R ; push 0xa0 ; call operator new ;
//   add esp,4 ; test eax,eax ; je Z ; mov ecx,eax ; call CFileInstaller ; push eax ;
//   mov ecx,&g ; call Reset ; mov eax,[g.object] ; ret ; Z: xor eax,eax ; push eax ; mov ecx,&g ;
//   call Reset ; mov eax,[g.object] ; ret ; R: ret
// `g = new CFileInstaller;` -- the new-expression's null test splits the flow and VC7.1
// duplicates the shared tail (Reset + reload + ret) into both arms. The result is re-read from
// the global after the assignment. An earlier lane landed this body as
// CFileInstallerSingleton_Get_00404440.cpp (fable_file_installer.h); this TU is self-contained.
class CFileInstaller { public:
    unsigned char m_body[0xA0];
    CFileInstaller();                             // 0x009C5890
};

struct SCountBlock;

struct CCountedFileInstallerPointer {
    CFileInstaller* m_pObject;                    // +0
    SCountBlock* m_pCount;                        // +4
    void Reset(CFileInstaller* object);           // 0x00403DB0
    CCountedFileInstallerPointer& operator=(CFileInstaller* object) { Reset(object); return *this; }
};

extern CCountedFileInstallerPointer g_FileInstallerSingleton;   // 0x013CA818

struct CFileInstallerSingleton {
    static CFileInstaller* __fastcall Get();
};

CFileInstaller* __fastcall CFileInstallerSingleton::Get()
{
    if (g_FileInstallerSingleton.m_pObject == 0)
        g_FileInstallerSingleton = new CFileInstaller;
    return g_FileInstallerSingleton.m_pObject;
}