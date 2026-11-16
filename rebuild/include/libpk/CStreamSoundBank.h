#ifndef FABLE_LIBPK_CSTREAMSOUNDBANK_H
#define FABLE_LIBPK_CSTREAMSOUNDBANK_H
#include "libpk/PKCTRList.h"
#include "libpk/CStreamBlockReader.h"
// Descriptive keyed lookup, native FindByKey slot17,20-byte native prefix.
class CStreamBankLookup:public PKCTRList<void> {
public:
 CStreamBankLookup();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(CStreamBankLookup*);
 virtual void* FindByKey(const void*);
 unsigned char Unrecovered08[12];
};
// Native accessed prefix: block-reader4, sample lookup38,allocation key4c,driver lookup50.
class CStreamSoundBank {
public:
 unsigned char Unrecovered00[4];
 CStreamBlockReader* m_pStream;
 unsigned char Unrecovered08[0x30];
 CStreamBankLookup m_samples;
 long m_padding;
 CStreamBankLookup m_drivers;
};
#endif
