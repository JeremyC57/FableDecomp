// CFileInstallerEntry_009c3130::CFileInstallerEntry_009c3130(const CWideString& source, const CWideString& destination, long size, bool flagA, bool flagB) -- copies the two names, keeps the size and the two flags, clears the done flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C3130, length 57 bytes.
// Leaf: ??0CFileInstallerEntry_009c3130
// Retail bytes: mov eax,[esp+4] ; push esi ; push eax ; mov esi,ecx ; call CWideString(copy) ; mov ecx,[esp+0Ch] ; push ecx ; lea ecx,[esi+4] ; call CWideString(copy) ;
//               mov al,[esp+14h] ; mov edx,[esp+10h] ; mov cl,[esp+18h] ; mov [esi+0Dh],al ; mov [esi+8],edx ; mov byte ptr [esi+0Ch],0 ; mov [esi+0Eh],cl ;
//               mov eax,esi ; pop esi ; ret 14h
// Callee: the CWideString copy constructor @ 0x0099B720 (twice).
// The manifest label "CScreenFilterSThingByPass::CScreenFilterSThingByPass" is propagated: this 0x10-byte record is allocated
// (operator new(0x10)) and built by CFileInstaller::AddFilteredFiles (0x009C47A0, calls at 0x009C4C90 / 0x009C4DA1) from two
// wide-string locals, a dword and two flags; no PDB layout is known, so the class is named by address and the members describe
// what the installer stores. Shape: the three scalar arguments are loaded before any of the four stores; the cleared byte
// sits between the dword and the first flag in declaration order.
#include <stddef.h>

struct CWideString {
    wchar_t* m_pData;
    CWideString(const CWideString& other);     // 0x0099B720
};

struct CFileInstallerEntry_009c3130 {
    CWideString   m_source;                    // +0x00
    CWideString   m_destination;               // +0x04
    long          m_size;                      // +0x08
    bool          m_done;                      // +0x0c
    bool          m_flagA;                     // +0x0d
    bool          m_flagB;                     // +0x0e
    CFileInstallerEntry_009c3130(const CWideString& source, const CWideString& destination, long size, bool flagA, bool flagB);
};

CFileInstallerEntry_009c3130::CFileInstallerEntry_009c3130(const CWideString& source, const CWideString& destination, long size,
                                                           bool flagA, bool flagB)
    : m_source(source), m_destination(destination), m_size(size), m_done(false), m_flagA(flagA), m_flagB(flagB)
{
}