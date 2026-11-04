// CStreamingFontDataBank::~CStreamingFontDataBank() -- streaming font bank destructor: restore the vtable, CloseFonts(), then the members (streaming entry counted pointer, file name, font array, state block) and the CBankFileAsync base
// Leaf: ??1CStreamingFontDataBank@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5F3A0, 126 bytes, thiscall, tail `jmp ??1CBankFileAsync@@UAE@XZ` (0x009D6490).
// Identity (proven): stores vtable 0x0129D5CC first (RTTI .?AVCStreamingFontDataBank@@ : CBankFileAsync : CBankFile :
//   CBaseClassNonCopyable : CBaseClass); sole caller is that vtable's slot-0 scalar deleting destructor 0x00A5FA30 (call at
//   0x00A5FA33); Ego_r.pdb ??1CStreamingFontDataBank@@UAE@XZ; the first callee is the landed ?CloseFonts@CStreamingFontDataBank@@AAEXXZ
//   0x00A5F2F0 and the tail 0x009D6490 stores CBankFileAsync's vtable 0x0129B8D4. The catalog label CChunkedFile::~CChunkedFile is
//   propagated from a masked twin.
// Callees: CloseFonts 0x00A5F2F0, the CCPPointerInfo deleter (fastcall), operator delete 0x00BFE9BC, ~CCharString 0x0099EAE0,
//   free 0x00BFEA14, ~CBankFileAsync 0x009D6490.
// Layout: as the landed constructor 0x00A5F290 / CloseFonts 0x00A5F2F0 (Ego_r names; the retail CBankFileAsync base is 0x164
//   bytes, engine/CStreamingFontDataBank.h is the flattened donor layout at the Ego_r +0x1D8): StateBlock
//   (CStreamingFontBankStateBlock : CBankStateBlock, RTTI 0x0129D594) +0x164, Fonts CArray<CStreamingFontDataBankEntry*> +0x178,
//   Filename +0x184, streamingEntry CCountedPointer<CBankFileAsyncEntry> +0x188. CBankFile / CBankFileAsync are sized stand-ins
//   over the real CBaseClassNonCopyable base (engine/CBankFile.h is flattened, retail sizeof 0x110).
// Shape: the user destructor calls the private CloseFonts() (out of line, defined earlier in retail's file); the members'
//   implicit destructors follow in reverse order: engine/CCountedPointer.h (immediate zero stores, scheduled after Filename's
//   `lea ecx`), ~CCharString, the STLport ~vector of pointers (`if (_M_start) free`), and the state block's implicit
//   ~CStreamingFontBankStateBlock (no store) expanding ~CBankStateBlock's user `{}` (its vtable store at +0x164).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#include "engine/CArray.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};
class CBankFile : public CBaseClassNonCopyable {             // RTTI CBankFile : CBaseClassNonCopyable : CBaseClass; retail sizeof 0x110 (engine/CBankFile.h)
public:
    unsigned char _body[0x110 - 0x04];                       // +0x04 .. +0x10f (engine/CBankFile.h members, flattened there)
};
class CBankFileAsync : public CBankFile {                    // RTTI CBankFileAsync : CBankFile; retail sizeof 0x164
public:
    CBankFileAsync();                                        // 0x009D5F80 ??0CBankFileAsync@@QAE@XZ
    virtual ~CBankFileAsync();                               // 0x009D6490 ??1CBankFileAsync@@UAE@XZ
    unsigned char _body[0x164 - 0x110];                      // +0x110 .. +0x163 (ThreadedFile, LoadingMemoryPool, ... CriticalSection)
};
class CBankStateBlock {
public:
    CBankStateBlock() { }
    virtual ~CBankStateBlock() { }                           // ??1CBankStateBlock@@UAE@XZ (vtable 0x0129C82C store)
    virtual void GetDefault(CBankStateBlock& block) = 0;     // slot 1
    virtual unsigned long CreateCRC() = 0;                   // slot 2
protected:
    virtual void Set(const CBankStateBlock* block) = 0;      // slot 3
};
class CStreamingFontBankStateBlock : public CBankStateBlock {
public:
    struct CStateBlock {
        long FontHeight;                                     // +0x00
        long FontWeight;                                     // +0x04
        long MinSpacing;                                     // +0x08
        bool Italics;                                        // +0x0c
    };
    CStreamingFontBankStateBlock();
    virtual void GetDefault(CBankStateBlock& block);
    virtual unsigned long CreateCRC();
    CStateBlock State;                                       // +0x04
protected:
    virtual void Set(const CBankStateBlock* block);
};
class CStreamingFontDataBankEntry;
class CBankFileAsyncEntry;
class CStreamingFontDataBank : public CBankFileAsync {
public:
    virtual ~CStreamingFontDataBank();
    CStreamingFontBankStateBlock         StateBlock;         // +0x164
    CArray<CStreamingFontDataBankEntry*> Fonts;              // +0x178
    CCharString                          Filename;           // +0x184
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;     // +0x188
private:
    void CloseFonts();                                       // 0x00A5F2F0
};
CStreamingFontDataBank::~CStreamingFontDataBank()
{
    CloseFonts();
}