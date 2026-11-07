#ifndef FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
#define FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
// CBankFileEntryUpdateData (new, chunk 849, row 0x009CF3A0 CBankFile::WriteSymbolHeaderFile): Ego_r.pdb layout (struct_layouts_egor.tsv,
// 32 bytes): StateBlockCRC +0, InfoSize +4, InfoBuffer +8, Filenames +0xC (CSmallVector<CCharString,8>, 8 bytes, no shared header:
// opaque), RequiresUpdate +0x14 (retail `mov cl,[edx+14h]` at 0x009CF53B), Exists +0x15, StateBlock +0x18
// (CCountedPointer<CBankStateBlock>). Private with CBankFile as friend (Ego_r: CBankFile reads RequiresUpdate).
#include "engine/CCountedPointer.h"
class CBankStateBlock;
class CBankFileEntryUpdateData {
    friend class CBankFile;
private:
    unsigned long StateBlockCRC;                                    // +0x00
    unsigned long InfoSize;                                         // +0x04
    char* InfoBuffer;                                               // +0x08
    unsigned char Filenames[0x8];                                   // +0x0C CSmallVector<CCharString,8>
    bool RequiresUpdate;                                            // +0x14
    bool Exists;                                                    // +0x15
    CCountedPointer<CBankStateBlock> StateBlock;                    // +0x18
};
#endif
