// CFontDataBank::CFontDataBank() -- CBankFile base, this vtable, the embedded CFontBankStateBlock's vtable, an empty Fonts array, then an empty Filename
// Leaf: ??0CFontDataBank@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A63630, 75 bytes. Name: Ego_r.pdb ??0CFontDataBank@@QAE@XZ; the catalog label agrees.
// Layout: engine/CFontDataBank.h (retail 0x130: CBankFile base 0x110, StateBlock +0x110, Fonts +0x120, Filename +0x12C);
//   Ego_r.pdb CFontBankStateBlock {vptr, CStateBlock State} (State has no constructor code), Fonts = CArray<CFontDataBankEntry*>.
// Vtables: 0x0129DA84 CFontDataBank, 0x0129DA5C CFontBankStateBlock. Callees: CBankFile::CBankFile 0x009CD480 (landed),
//   CCharString::CCharString() 0x0099E4B0, CCharString::operator=(const char*) 0x0099EFE0 with "" (0x0122D70E, the pooled
//   tail of a longer literal).
// Shape: members built in declaration order with inline constructors (the STLport vector's three zero stores through one
//   zero register); `Filename = "";` in the body.
class CBankFile {
public:
    virtual ~CBankFile();
    unsigned char _body[0x10c];                         // +0x004..+0x10F (engine/CBankFile.h)
    CBankFile();                                        // 0x009CD480
};
class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString();                                      // 0x0099E4B0
    ~CCharString();
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CStateBlock { public: unsigned char _data[0xc]; };
class CFontBankStateBlock {                             // vtable 0x0129DA5C
public:
    virtual ~CFontBankStateBlock() { }
    CStateBlock State;                                  // +0x04
    CFontBankStateBlock() { }
};
class CFontDataBankEntry;
#include "engine/CArray.h"

class CFontDataBank : public CBankFile {               // vtable 0x0129DA84
public:
    CFontBankStateBlock        StateBlock;              // +0x110
    CArray<CFontDataBankEntry*> Fonts;                  // +0x120
    CCharString                Filename;                // +0x12C
    CFontDataBank();
    virtual ~CFontDataBank();
};

CFontDataBank::CFontDataBank()
{
    Filename = "";
}