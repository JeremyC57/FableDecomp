#ifndef FABLE_ENGINE_CBANKFILEASYNCENTRY_H
#define FABLE_ENGINE_CBANKFILEASYNCENTRY_H
#ifdef FABLE_RETAIL_CBANKFILEASYNCENTRY_WATER
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CBankFileAsync.h"
#include "engine/EThreadedFilePriority.h"
class CThreadedFile;
class CBankFileAsyncData;
// Ego_d TPI 0xA2CF records the CBaseClassNonCopyable base, bank reference at +4,
// Index at +8 and PaddedSize at +0xC. Native 0x009D5260 initializes the same layout.
class CBankFileAsyncEntry : public CBaseClassNonCopyable {
    friend class CBankFileAsync;
public:
    virtual ~CBankFileAsyncEntry();
    CCountedPointer<CBankFileAsyncData> BeginReadData(unsigned long, void*, unsigned long, unsigned long, EThreadedFilePriority);
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long, unsigned long, EThreadedFilePriority);
    void ReadDataNonAsync(unsigned long, void*, unsigned long);
    unsigned long GetDataType() const;
    unsigned long GetDataSize() const { return BankFile.GetEntryDataSize(Index); }
private:
    CBankFileAsyncEntry(CBankFileAsync&, unsigned long);
    unsigned long GetDataOffset() const;
    CThreadedFile& GetThreadedFile() const;
    CBankFileAsync& BankFile;
    unsigned long Index;
    unsigned long PaddedSize;
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsyncEntry) == 0x10);
#endif
#endif
