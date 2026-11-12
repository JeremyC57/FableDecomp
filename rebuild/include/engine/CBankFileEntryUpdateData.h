#ifndef FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
#define FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
// CBankFileEntryUpdateData (new, chunk 849, row 0x009CF3A0 CBankFile::WriteSymbolHeaderFile): Ego_r.pdb layout (struct_layouts_egor.tsv,
// 32 bytes): StateBlockCRC +0, InfoSize +4, InfoBuffer +8, Filenames +0xC (CSmallVector<CCharString,8>, 8 bytes, no shared header:
// opaque), RequiresUpdate +0x14 (retail `mov cl,[edx+14h]` at 0x009CF53B), Exists +0x15, StateBlock +0x18
// (CCountedPointer<CBankStateBlock>). Private with CBankFile as friend (Ego_r: CBankFile reads RequiresUpdate).
#include "engine/CCountedPointer.h"
class CBankStateBlock;
#ifdef FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
// Retail opt-in (wf18, row 0x009CE5E0 CBankFile::AddEntryMultipleFiles): the retail (TLC) record is 0x18 bytes -- `operator new(18h)`
// at 0x009CE7D9 and the landed dtor 0x009CC6C0 -- so the Ego_r StateBlock member (+0x18) is absent. Filenames is typed as the Ego_r
// CSmallVector<CCharString,8> (Start +0 T*, Size +4 signed char; no shared header, minimal model). Adds the private inline ctor
// (Ego_r ??0CBankFileEntryUpdateData@@AAE@XZ, retail out-of-line copy 0x009CC6A0, expanded in place by that row), Equals 0x009CC750
// and SetFilenames 0x009CC840 (Ego_r private ABE / AAE).
#include "engine/CArray.h"
#include "fable_string.h"
#ifndef FABLE_CSMALLVECTOR_MINIMAL
#define FABLE_CSMALLVECTOR_MINIMAL
template <class T, int N> class CSmallVector {
public:
    CSmallVector() : Start(0), Size(0) { }
    T* Start;                                                       // +0x00
    signed char Size;                                               // +0x04
};
#endif
#endif
class CBankFileEntryUpdateData {
    friend class CBankFile;
private:
    unsigned long StateBlockCRC;                                    // +0x00
    unsigned long InfoSize;                                         // +0x04
    char* InfoBuffer;                                               // +0x08
#ifdef FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
    CSmallVector<CCharString,8> Filenames;                          // +0x0C
#else
    unsigned char Filenames[0x8];                                   // +0x0C CSmallVector<CCharString,8>
#endif
    bool RequiresUpdate;                                            // +0x14
    bool Exists;                                                    // +0x15
#ifdef FABLE_RETAIL_CBANKFILEENTRYUPDATEDATA_RETAIL
    CBankFileEntryUpdateData() : StateBlockCRC(0), InfoSize(0), InfoBuffer(0), RequiresUpdate(false), Exists(true) { }   // 0x009CC6A0
    bool Equals(const CArray<CCharString>& filenames, CCountedPointer<CBankStateBlock> stateBlock) const;   // 0x009CC750
    void SetFilenames(const CArray<CCharString>& filenames);                                               // 0x009CC840
#else
    CCountedPointer<CBankStateBlock> StateBlock;                    // +0x18
#endif
};
#endif
