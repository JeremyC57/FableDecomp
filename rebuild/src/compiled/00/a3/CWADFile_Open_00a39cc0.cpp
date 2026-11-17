// CWADFile::Open(const CWideString&, FileAccessType, unsigned long) -- finds the named entry in the WAD bank (CRC of the narrowed name) and rewinds the file position
// Leaf: ?Open@CWADFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39CC0, 85 bytes (`ret 0Ch`). CWADFile vtable 0x0129CF8C slot 1 reaches it
// through the `jmp` at 0x00A39DC0; CWADFile::CWADFile (0x00A39D80) calls it directly.
// Callees: CWideString::ToCharString() const @ 0x0099B2C0 (CCharString through the hidden pointer),
// Calc @ 0x004014A0 (`__fastcall(seed, data, length)`: the CRC), the bank's entry lookup @ 0x009CC410 (labelled
// "CVectorMap::LookupValue"), CCharString::~CCharString @ 0x0099EAE0.
// Label: "CParticleMorphs::InsertEntry" is propagated; the vtable and the PDB layout (Bank +4, Index +8, FilePos
// +0xc) name it.
// Shape: the narrowed name is a TEMPORARY homed in the name's dead argument slot (`lea eax,[esp+8]`), read through
// the call's returned pointer (`mov eax,[eax]`) and destroyed at the end of the full expression, before the
// FilePos store; it reaches the CRC through an inline helper taking `const CCharString&`, whose arguments go
// right to left through two null-case-first accessors (length, then text with the shared empty string
// 0x0129AAF4), each testing the block again. A named local is read back from the frame, the two tests merge
// into one, and its destructor runs after the FilePos store.
// NF47 (round 7): 0x009CC410 is Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0 ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z, called on
//   the WAD bank (Ego_r CWADFile::Bank +4, a CBankFileAsync* ; CBankFileAsync : CBankFile, engine/CBankFileAsync.h); was the made-up
//   CBankFileAsync::LookupValue (in neither PDB). The local CCharString / CWideString / CCRC / CBankFileAsync models are replaced by
//   fable_string.h (inline GetCRC) and engine/CBankFileAsync.h on the polymorphic engine/CBankFile.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CBankFileAsync.h"

enum FileAccessType { FILE_ACCESS_READ };

class CWADFile { public:
    void*           m_vtable;                        // +0x00
    CBankFileAsync* Bank;                            // +0x04
    unsigned long   Index;                           // +0x08
    long            FilePos;                         // +0x0c
    void Open(const CWideString& name, FileAccessType access, unsigned long flags);
};

void CWADFile::Open(const CWideString& name, FileAccessType access, unsigned long flags)
{
    Index = Bank->FindIndexBySymbolCRC(name.ToCharString().GetCRC());
    FilePos = 0;
}