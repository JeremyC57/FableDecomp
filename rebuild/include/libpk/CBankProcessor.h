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
#ifdef FABLE_RETAIL_SSAMPLEHEADER_FORMAT
// NF65 overlay opt-in (row 0x00C2E190 CBankProcessor::AddSample): the rest of the 0x20-byte sample record AddSample reads (+0x08 .. +0x1C:
// length, data offset, the WAVEFORMATEX channels / format tag / sample rate, the loop points in samples, negative = none). Descriptive
// member names (libpk has no PDB). The first two members and their offsets are unchanged.
struct SSampleHeader {unsigned long m_number;CString m_fileName;
 unsigned long m_length,m_dataOffset;unsigned short m_channels,m_formatTag;unsigned long m_sampleRate;long m_loopStart,m_loopEnd;};
#else
struct SSampleHeader {unsigned long m_number;CString m_fileName;};
#endif
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
#ifdef FABLE_RETAIL_CBANKDATA_PREPARE
 // NF63 (round 7, row 0x00C2EB00): the record lists' preparation step 0x00C2ED50 (slot 5 on each list), catalog CBankData::Prepare
 // (descriptive; libpk has no PDB). Declaration only.
 bool Prepare();                                                     // 0x00C2ED50
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
#ifdef FABLE_RETAIL_CEVENTTRIGGERREF_NUMBERCTOR
 // NF65 overlay opt-in (row 0x00C2E4D0): the number-keyed constructor 0x00C1EE60 (registry, key: FindItem, else `new` a 16-byte keyed item
 // and attach it), with the landed self row's parameter list (CItemDispatcher*, long; the class is this one, NAME_FIXES.tsv).
 // Descriptive (libpk has no PDB). Needs libpk/CItem.h. Declaration only.
 CEventTriggerRef(CItemDispatcher*,long);                           // 0x00C1EE60
#endif
 unsigned long GetDriverCount();
 long InsertDriver(unsigned long,void*);
#ifdef FABLE_RETAIL_CEVENTTRIGGERREF_TYPESETTERS
 // NF65 overlay opt-in (row 0x00C1F4E0): the two type setters InsertDriver calls on a failed insert (thiscall, no arguments; 0x00C1F3C0
 // after -1, else 0x00C1F380), out of line. DESCRIPTIVE names: the landed self rows' names (CItemAction::SetFamily2 / SetVariant2), whose
 // class is this one (CItemAction 0x00C1EEE0 = this header's CEventTriggerRef ctor; NAME_FIXES.tsv). Declarations only.
 void SetVariant2();                                                 // 0x00C1F380
 void SetFamily2();                                                  // 0x00C1F3C0
#endif
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
#ifdef FABLE_RETAIL_CBANKPROCESSOR_ADDSAMPLE
 // NF65 overlay opt-in (row 0x00C2E190): the sample step (descriptive, libpk has no PDB; the landed AddSamples 0x00C2E270 calls it per
 // record) and the manager +0x04 typed as the CScriptHost whose CreateSample 0x00C0E700 it calls (needs FABLE_RETAIL_CSCRIPTHOST_CREATESAMPLE).
 // Same size and offsets. Declaration only.
 void AddSample(const SSampleHeader* header);                       // 0x00C2E190
 CBankData*m_pData;CScriptHost*m_pManager;CLoadLog*m_pLog;
#else
 CBankData*m_pData;void*m_pManager;CLoadLog*m_pLog;
#endif
 CIndexMap_00c3abd0 m_drivers;
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 CSampleIndexMap m_samples;
#else
 unsigned char UnrecoveredSampleMap3c[0x30];
#endif
#ifdef FABLE_RETAIL_CBANKPROCESSOR_NUMBERTRIGGERS
 // NF65 overlay opt-in (row 0x00C2E4D0): the number-trigger registry +0x70 typed as the id-keyed item dispatcher (libpk/CItem.h; its
 // FindItem 0x00C10140 is what AddNumberTriggers asks). Same size and offsets. Needs libpk/CItem.h.
 CNamedSet*m_pCriteriaTriggers;CItemDispatcher*m_pNumberTriggers;
#else
 CNamedSet*m_pCriteriaTriggers;void*m_pNumberTriggers;
#endif
#ifdef FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
 bool m_option74,m_option75,m_option76,m_option77,m_option78;
#else
 unsigned char UnrecoveredOptions74[5];
#endif
 bool m_bSampleNameTriggers,m_bNamedTriggers;
 void AddSampleNameTriggers();
#ifdef FABLE_RETAIL_CBANKPROCESSOR_ADDEVENTTRIGGER
 // NF65 overlay opt-in (row 0x00C2E7A0): the event's criteria-string trigger step, the spelling the landed AddEvent 0x00C2E8D0 /
 // AddSubEvent 0x00C2E960 bind (?AddEventTrigger@CBankProcessor@@QAEXABUCEventRecord@@_N@Z; descriptive, libpk has no PDB). Out of line.
 // The includer defines CEventRecord. Declaration only.
 void AddEventTrigger(const CEventRecord& record, bool subEvent);    // 0x00C2E7A0
#endif
#ifdef FABLE_RETAIL_CBANKPROCESSOR_STEPS
 // NF63 (round 7, row 0x00C2EB00 Load): the processor's steps in Load's call order, catalog names (descriptive; libpk has no PDB; the
 // landed rows' CChildListLoader spellings are an older chunk's descriptive class). Declarations only, no layout change.
 void AddSamples();                                                  // 0x00C2E270
 void AddDrivers();                                                  // 0x00C2E480
 void AddNumberTriggers();                                           // 0x00C2E4D0
 void ProcessAllDrivers();                                           // 0x00C2EAB0
 void AddEvents();                                                   // 0x00C2EA30
 void AddSubEvents();                                                // 0x00C2EA70
#endif
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
