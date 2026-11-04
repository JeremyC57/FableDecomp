// CMasterObject_00c4c8b0::Reset() -- bump the string counter under the debug-string lock
// Vendor naming V2 (plan phase 1, header-first): PKCProtectionInstance / PKCCriticalSection come from the shared libpk/PKCProtectionInstance.h (was a local copy).
// Vendor naming wave V1 (one address, one name): 0x00C4C330 ?Reset@CMasterObject_00c4c8b0@@QAEXXZ (was ?AddString@CDebugString@@QAEXXZ)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4C330, length 40 bytes.
// Callees: PKCProtectionInstance::PKCProtectionInstance(CProfileSection*) @ 0x00C0FFC0, PKCProtectionInstance::~PKCProtectionInstance @ 0x00C0FFE0.
// Shape: an 8-byte scoped lock (section pointer + entry tick, see wave 23f) built in the frame over the
// section at +0x10, the counter at +8 incremented through eax between the constructor and destructor calls.
// PDB CMasterObject_00c4c8b0 (28 bytes): +8 vector<CWideString> Strings, +0x18 long NoStrings; retail reads +8 as the
// counter and locks at +0x10, so the layout is modelled locally (the retail vector is 12 bytes).
#include "libpk/PKCProtectionInstance.h"
struct CProfileSection { unsigned char _opaque[4]; };   // only its address is taken


class CMasterObject_00c4c8b0 { public:
    unsigned char   _pad[8];           // +0x00
    long            m_count;           // +0x08
    unsigned char   _pad2[4];          // +0x0c
    CProfileSection m_section;         // +0x10 (opaque; only its address is taken)
    void Reset();
};

void CMasterObject_00c4c8b0::Reset()
{
    PKCProtectionInstance lock((PKCCriticalSection*)&m_section);
    ++m_count;
}