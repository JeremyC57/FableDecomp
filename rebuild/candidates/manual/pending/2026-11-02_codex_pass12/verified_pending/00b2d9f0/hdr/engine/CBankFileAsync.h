#ifdef FABLE_RETAIL_TEXTURE_CACHE
#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#include "engine/CBankFile.h"
class CBankFileAsyncData;
// Retail opt-in (Codex pass12): both original PDBs prove names/access; native
// constructor009d5f80 proves all member boundaries and the0x164 complete size.
class CBankFileAsync:public CBankFile {
public:
 CBankFileAsync();virtual ~CBankFileAsync();
 virtual bool OpenReadOnly(const CCharString&,unsigned long);
 virtual void OpenReadWrite(const CCharString&,unsigned long,unsigned long);
 virtual bool OpenFileReadOnly(const CWideString&,unsigned long);
 virtual void OpenFileReadWrite(const CWideString&,unsigned long,unsigned long);
 virtual void Close();
private:
 unsigned char ThreadedFile[8]; // CCountedPointer<CThreadedFile>, +110
 unsigned char LoadingMemoryPool[8]; // CCountedPointer<CMovableResourceMemoryPool>, +118
 unsigned char MemoryFailureHandler[8]; // CCountedPointer<CLoadingMemoryFailureHandler>, +120
 unsigned char AsyncDataInstances[12],ReadsToRestart[12]; // set<CBankFileAsyncData*>, +128/+134
 CBankFileAsyncData* LastRestartingData; // +140
 unsigned char DisposalList[4]; // list<CCountedPointer<CBankFileAsyncData> >, +144
 bool NonCached,DisableAsync; // +148/+149
 unsigned char _pad_14a[2];
 unsigned char CriticalSection[24]; // _RTL_CRITICAL_SECTION, +14c
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
#endif
#else
#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#include "engine/CBankFile.h"
// Layout: original public CBankFile base, native ctor9d5f80 and RTTI secondary base164.
class CBankFileAsync:public CBankFile {
public:CBankFileAsync();virtual ~CBankFileAsync();
private:unsigned char _pad_0x110[0x54];
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
#endif

#endif
