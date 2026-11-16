#ifndef FABLE_LIBPK_PKCMAILBOX_H
#define FABLE_LIBPK_PKCMAILBOX_H
#ifdef FABLE_RETAIL_PKCMAILBOX_BUFFEREDREADER_PASS23
#include "libpk/PKCCriticalSection.h"
#include "libpk/PKCSemaphore.h"
// Layout:00c4bc20 constructs the real critical section at+0,stores queue+18,
// constructs semaphore+1c; GetCount00c4be10 locks this then queries queue+18.
// Descriptive libpk naming retained from existing GetCount binding; no PDB.
class PKCMailbox {
public:
#if defined(FABLE_RETAIL_CSEGMENTFILE_PASS23) || defined(FABLE_RETAIL_LIBPK_DECODER_FACTORY_PASS23)
 // Native C4BC20: queue pointer then semaphore capacity.
 PKCMailbox(void* queue,long capacity);
#endif
 PKCCriticalSection m_criticalSection;
 void* m_pQueue;
 PKCSemaphore m_semaphore;
 long GetCount();
#ifdef FABLE_RETAIL_PKCBUFFEREDREADER_RETURN_CONTEXT_PASS23
 // Existing descriptive canonical name; nativeC4BC80 returns dequeued unsigned request id.
 unsigned long GetResult();
#endif
};
#endif
#endif
