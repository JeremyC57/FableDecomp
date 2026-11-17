#ifndef FABLE_LIBPK_PKCBUFFEREDREADER_H
#define FABLE_LIBPK_PKCBUFFEREDREADER_H
#ifdef FABLE_RETAIL_PKCBUFFEREDREADER_POLL_PASS23
#define FABLE_RETAIL_PKCMAILBOX_BUFFEREDREADER_PASS23
#include "libpk/PKCMailbox.h"
// Layout: actual native reader00c4b160 uses mailbox+48 and outstanding+68.
// Untouched leading state remains opaque; no fields or vtable are fabricated.
// Class stem is the original PKCBufferedReader.cpp assertion; field and Poll
// names are descriptive, with unavailable original libpk symbols documented.
#if defined(FABLE_RETAIL_CSEGMENTFILE_PASS23) || defined(FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23)
// Native C3AB00 parameter-value constructor and C4B300 config reads.
struct PKCBufferedReaderParams {
 unsigned long Granularity,ExtraBuffers,BaseBuffers;
 PKCBufferedReaderParams(unsigned long g,unsigned long e,unsigned long b):Granularity(g),ExtraBuffers(e),BaseBuffers(b){}
};
#endif
class PKCBufferedReader {
public:
#if defined(FABLE_RETAIL_CSEGMENTFILE_PASS23) || defined(FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23)
 PKCBufferedReader(void* source,const PKCBufferedReaderParams& params);
#endif
#ifdef FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23
 // Native C4B270 is the real destructor called by the Ogg member owner.
 ~PKCBufferedReader();
#endif
#ifdef FABLE_RETAIL_PKCBUFFEREDREADER_WAIT_PASS23
 unsigned char _unknown00[4];unsigned char* m_slotOwners;unsigned char _unknown08[0x0c];unsigned long m_slotCount;unsigned char _unknown18[0x30];
#else
 unsigned char _unknown00[0x48];
#endif
 PKCMailbox m_mailbox;
 long m_outstanding;
#if defined(FABLE_RETAIL_CSEGMENTFILE_PASS23) || defined(FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23)
 // Native C4B300 initializes the final word+6c; range-wrapper fields start+70.
 long m_completedReadOffset;
#endif
#ifdef FABLE_RETAIL_PKCBUFFEREDREADER_WAIT_PASS23
 bool WaitForSlot(unsigned long slot);
#endif
 void ReturnBuffer(bool* more);
 #ifdef FABLE_RETAIL_PKCBUFFEREDREADER_GETSIZE_SEEK
 // NF56 opt-in (row 0x00C3AA50 CBufferedRangeReader::SetRange): the size getter (returns +0x20) and the absolute seek, out of line at
 // 0x00C4AA30 / 0x00C4AA40. No libpk PDB or SDK symbol exists: descriptive names, the spellings the landed binders already use
 // (?GetSize@PKCBufferedReader@@QAEKXZ from 0x00C89C10, ?Seek@PKCBufferedReader@@QAE_NK@Z from 0x00C4AFD0). Declarations only.
 unsigned long GetSize();
 bool Seek(unsigned long offset);
 #endif
 #ifdef FABLE_RETAIL_PKCBUFFEREDREADER_RETURNALLBUFFERS
 // Opt-in (NF42 round 5, row 0x00C4B010): ReturnBuffer(0) until m_outstanding (+0x68) is zero. No libpk PDB or SDK symbol exists:
 // descriptive name, the landed row's method name (was on the address-named class CBufferOwner_00c4b010). Declaration only.
 void ReturnAllBuffers();
 #endif
 void Refill();
 bool PollReadCompletion();
};
#endif
#endif
