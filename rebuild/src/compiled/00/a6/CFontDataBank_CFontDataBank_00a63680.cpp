// CFontDataBank::~CFontDataBank() -- font bank dtor: CloseFonts(), then ~Filename, ~Fonts, the state block's inline ~CBankStateBlock and ~CBankFile
// Leaf: ??1CFontDataBank@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A63680.
// Identity: RTTI vtable 0x0129DA84 (CFontDataBank : CBankFile); Ego_r.pdb ??1CFontDataBank@@UAE@XZ; StateBlock +0x110,
//   Fonts +0x120, Filename +0x12c.
// Callees: CFontDataBank::CloseFonts 0x00A633A0 (private), ~CWideString, ~CArray, ~CBankFile.
// Header debt: the flat engine/CBankFile.h cannot be a polymorphic base, so CBankFile is a local opaque class (as landed 0x00A5F3A0).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "fable_string.h"

class CBankFile : public CBaseClassNonCopyable {             // RTTI CBankFile : CBaseClassNonCopyable : CBaseClass; retail sizeof 0x110 (engine/CBankFile.h)
public:
    CBankFile();
    virtual ~CBankFile();                                    // 0x009CE180 ??1CBankFile@@UAE@XZ
    unsigned char _body[0x110 - 0x04];                       // +0x04 .. +0x10f (engine/CBankFile.h members, flattened there)
};
class CBankStateBlock {                                      // RTTI .?AVCBankStateBlock@@ (no bases), vtable 0x0129C82C
public:
    CBankStateBlock() { }
    virtual ~CBankStateBlock() { }
    virtual void GetDefault(CBankStateBlock& block) = 0;     // slot 1
    virtual unsigned long CreateCRC() = 0;                   // slot 2
protected:
    virtual void Set(const CBankStateBlock* block) = 0;      // slot 3
};
class CFontBankStateBlock : public CBankStateBlock {         // RTTI CFontBankStateBlock : CBankStateBlock (0x10 bytes)
public:
    struct CStateBlock { unsigned char _body[0xc]; };       // +0x04 (engine/CFontBankStateBlock.h: 12 bytes)
    CFontBankStateBlock();
    virtual void GetDefault(CBankStateBlock& block);
    virtual unsigned long CreateCRC();
    CStateBlock State;                                       // +0x04
protected:
    virtual void Set(const CBankStateBlock* block);
};
class CFontDataBankEntry;
class CFontDataBank : public CBankFile {                     // vtable 0x0129DA84
public:
    CFontDataBank();
    virtual ~CFontDataBank();
private:
    void CloseFonts();                                       // 0x00A633A0 ?CloseFonts@CFontDataBank@@AAEXXZ
    CFontBankStateBlock         StateBlock;                  // +0x110
    CArray<CFontDataBankEntry*> Fonts;                       // +0x120
    CCharString                 Filename;                    // +0x12c
};
FABLE_STATIC_ASSERT(sizeof(CFontDataBank) == 0x130);

CFontDataBank::~CFontDataBank()
{
    CloseFonts();
}