// CWADFile::GetPathname() -- the bank's symbol name of this WAD entry, converted to a wide string (returned by value)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39C40, length 45 bytes (+ int3 padding). CWADFile vtable 0x0129CF8C slot 11.
// Leaf: GetPathname
// Callees: CBankFile::GetEntrySymbol(unsigned long) @ 0x009CC1D0 (CCharString by hidden pointer), CCharString::ToWideString
// @ 0x0099E480 (CWideString by hidden pointer), CCharString::~CCharString @ 0x0099EAE0 (the manifest's "_Cons_val<...>" label
// there is propagated).
// Shape: the caller's hidden result pointer is held in esi and pushed first for ToWideString; the CCharString temporary is
// homed in that pointer's dead argument slot (`lea edx,[esp+10h]`) and destroyed after the conversion.
#include "engine/CWADFile.h"

class CWideString { public:
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};

class CCharString { public:
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
    CWideString ToWideString() const;       // 0x0099E480
};

class CBankFileAsync { public:
    CCharString GetEntrySymbol(unsigned long index);   // 0x009CC1D0
};

struct CWADFile_Methods : CWADFile {
    CWideString GetPathname();
};

CWideString CWADFile_Methods::GetPathname()
{
    return Bank->GetEntrySymbol(Index).ToWideString();
}