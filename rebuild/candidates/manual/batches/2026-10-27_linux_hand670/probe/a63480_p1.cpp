// CFontDataBankEntry::~CFontDataBankEntry() -- font data-bank entry destructor: restore the vtable, Unload (release the Font counted pointer), then the member destructor and the CBaseClassNonCopyable base
// Leaf: ??1CFontDataBankEntry@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
class CFontBank;
class CFontDataBank;
class CFontDataBankEntry : public CBaseClassNonCopyable {
public:
    CCountedPointer<CFontBank> Font;                        // +0x04
    CFontDataBank* Bank;                                    // +0x0c
    unsigned long Index;                                    // +0x10
    unsigned long DataType;                                 // +0x14
    virtual ~CFontDataBankEntry();
    virtual bool IsLoaded();
    virtual bool CanUnload();
    virtual void Unload();
};
CFontDataBankEntry::~CFontDataBankEntry()
{
    Font.ReleaseData();
}
