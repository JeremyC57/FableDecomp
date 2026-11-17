#ifndef FABLE_LIBPK_CBANKPROCESSOR_H
#define FABLE_LIBPK_CBANKPROCESSOR_H
#if defined(FABLE_RETAIL_BANK_SAMPLE_TRIGGERS_PASS23) || defined(FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23)
#define FABLE_RETAIL_CSEGMENTFILE_DEFERRED_BANK_PASS23
#define FABLE_RETAIL_CLOADLOG_MESSAGE_PASS23
#define FABLE_RETAIL_CSTRING_BANK_TRIGGER_PASS23
#define FABLE_RETAIL_CSTRING_MAKEUPPER
#include "libpk/CScriptHost.h"
#include "libpk/PKCTRList.h"
#include "libpk/PKCTRRedblackTree.h"
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/PKCTRMorphable.h"
#endif
// Actual consumed record prefixes; no guessed full serialized tails.
struct SSampleHeader {unsigned long m_number;CString m_fileName;};
struct SDriverRecord {unsigned long m_number,m_sample;};
struct CEventRecord;
// Native receivers0/10/20: shared16-slot typed-list prefix plus transfer/key slots.
template<class T>class CBankRecordList:public PKCTRList<T>{public:
 CBankRecordList();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(PKCTRList<T>*);
 virtual T*FindByKey(const unsigned long&);
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 PKCTRMorphable Container; // C304F0/C305E0/C306D0 embeds realC185B0 at+8.
#else
 unsigned char Unrecovered08[8];
#endif
};
struct CBankData {
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 CBankData();~CBankData();
#endif
CBankRecordList<SSampleHeader>m_samples;CBankRecordList<SDriverRecord>m_drivers;CBankRecordList<CEventRecord>m_events;};
struct SIndexNode_00c3abd0{void*m_pValue;unsigned long m_index;};
// NativeC3ABF0 installs012b5578,impl4=&Tree8,constructs actual40BTreeC194F0.
class CIndexMap_00c3abd0:public PKCTRList<SIndexNode_00c3abd0>{public:
 CIndexMap_00c3abd0();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(PKCTRList<SIndexNode_00c3abd0>*);
 virtual SIndexNode_00c3abd0*FindByKey(const unsigned long&);
 void*Get(unsigned long);
 PKCTRRedblackTree Tree;
};
class CItem;
class CNamedIndex:public PKCTRList<CItem>{public:
 CNamedIndex();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(PKCTRList<CItem>*);
 virtual CItem*FindByKey(const unsigned long&);
 unsigned char Unrecovered08[8];
};
// NativeC101D0 named-key index+10; unconsumed receiver fields stay opaque.
class CNamedSet{public:unsigned char Unrecovered00[0x10];CNamedIndex Index;CItem*Find(const CString&,long*);};
// NativeC1EEE0 two pointer words; actual217B ctor and76B insertion dispatcher.
struct CEventTriggerRef{
 CEventTriggerRef(CNamedSet*,CString,bool);
 unsigned long GetDriverCount();
 long InsertDriver(unsigned long,void*);
 CNamedSet*m_pRegistry;CItem*m_pTrigger;
};
// NativeCtorC2E0C0 and independentLoadC2EB00: data0/manager4/log8,
// driverMapc/sampleMap3c/registries6c,70/options74..7a. No invented inheritance.
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#ifdef FABLE_RETAIL_CSAMPLEINDEXMAP_ADD
struct SSampleIndexNode{void*m_pValue;unsigned long m_index;SSampleIndexNode(void*,unsigned long);}; // 8 bytes; ctor 0x00C3B0C0
#else
struct SSampleIndexNode;
#endif
class CSampleIndexMap:public PKCTRList<SSampleIndexNode>{public:
 CSampleIndexMap();virtual ~CSampleIndexMap();
 virtual int IsEqual(const void*,const void*)const;
 virtual int IsLess(const void*,const void*)const;
 virtual int IsKeyEqual(const void*,const void*)const;
 virtual int IsKeyLess(const void*,const void*)const;
 virtual void TransferAllFrom(PKCTRList<SSampleIndexNode>*);
 virtual SSampleIndexNode*FindByKey(const unsigned long&);
#ifdef FABLE_RETAIL_CSAMPLEINDEXMAP_ADD
 using PKCTRList<SSampleIndexNode>::Add;
 void Add(CLoadLog*,void*,unsigned long); // 0x00C3B0E0: log a duplicate index, else Add(new SSampleIndexNode)
#endif
 PKCTRRedblackTree Tree;
};
#endif
struct CBankProcessor{
 CBankProcessor(CBankData*,void*,CLoadLog*);~CBankProcessor();
 CBankData*m_pData;void*m_pManager;CLoadLog*m_pLog;
 CIndexMap_00c3abd0 m_drivers;
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 CSampleIndexMap m_samples;
#else
 unsigned char UnrecoveredSampleMap3c[0x30];
#endif
 CNamedSet*m_pCriteriaTriggers;void*m_pNumberTriggers;
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 bool m_option74,m_option75,m_option76,m_option77,m_option78;
#else
 unsigned char UnrecoveredOptions74[5];
#endif
 bool m_bSampleNameTriggers,m_bNamedTriggers;
 void AddSampleNameTriggers();
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 void Load(); // Native C2EB00 complete processor entry.
#endif
};
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
FABLE_STATIC_ASSERT(sizeof(CBankData)==0x30);
FABLE_STATIC_ASSERT(sizeof(CSampleIndexMap)==0x30);
FABLE_STATIC_ASSERT(sizeof(CBankProcessor)==0x7c);
FABLE_STATIC_ASSERT(offsetof(CBankProcessor,m_samples)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CBankProcessor,m_pCriteriaTriggers)==0x6c);
FABLE_STATIC_ASSERT(offsetof(CBankProcessor,m_option74)==0x74);
FABLE_STATIC_ASSERT(offsetof(CBankProcessor,m_bSampleNameTriggers)==0x79);
FABLE_STATIC_ASSERT(offsetof(CBankProcessor,m_bNamedTriggers)==0x7a);
FABLE_STATIC_ASSERT(sizeof(SBankRequest)==0x14);
FABLE_STATIC_ASSERT(offsetof(SBankRequest,SampleNameTriggers)==0x0c);
FABLE_STATIC_ASSERT(offsetof(SBankRequest,EnableBothExtra)==0x11);
#endif
#endif
#endif
