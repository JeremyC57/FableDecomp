// CUnprovenRecordList_00c2f7d0::Transfer(PKSerialiseCParser* archive) -- Load owned records or transfer the existing collection through its visitor.
// Leaf: ?Transfer@CUnprovenRecordList_00c2f7d0@@QAE_NPAVPKSerialiseCParser@@@Z
// Flags: /EHsc
// Identity: own CBankData caller00c2ee80 uses three corresponding collections; record ctor00c3b930/dtor00c3b840/Transfer00C3B850, actual PKCAutoDelete Release00c2f070 and visitor ctor00c2ee20. Descriptive record/collection names.
// Layout: no active shared headers. Own record allocation32; pointer guard4, abstract visitor base4 plus Archive4/Failures8. Collection uses observed virtual slots5 Clear/9 Add/10 ForEach/14 Reserve/15 Count only; unknown class/base size not claimed. PKSerialiseCParser name from existing helper bindings/assertions.
#include <new>
class PKSerialiseCParser{public:void* Reader;void* Writer;bool IsReading();bool TransferSize(unsigned long*);};
class CUnprovenTransferRecord_00C3B850{public:unsigned char Unmodeled[32];CUnprovenTransferRecord_00C3B850();~CUnprovenTransferRecord_00C3B850();bool Transfer_00C3B850(PKSerialiseCParser*);};
template<class T>class PKCAutoDelete{public:T* m_pObject;explicit PKCAutoDelete(T* p):m_pObject(p){}~PKCAutoDelete(){if(m_pObject){delete m_pObject;m_pObject=0;}}T* Release();};

#ifndef _MSC_VER
#define __declspec(x)
#endif
// Visible Release body retains the original PKCAutoDelete.h(42) diagnostic and ownership clear.
#ifndef _MSC_VER
#define __fastcall
#endif
extern char* _afxPchNil;typedef void(__fastcall* AssertHook)(const char*);extern AssertHook g_pfnAssertHook;
class CString{public:char* m_pchData;CString():m_pchData(_afxPchNil){}~CString();CString& __fastcall operator<<(const char*);CString& __fastcall operator<<(long);};struct CDebugBreak{CDebugBreak();};
template<> __declspec(noinline) CUnprovenTransferRecord_00C3B850* PKCAutoDelete<CUnprovenTransferRecord_00C3B850>::Release(){if(!m_pObject){{CString text;text << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << (long)42 << ") : " << "Shouls have checked first..." << "\n";if(g_pfnAssertHook)g_pfnAssertHook(text.m_pchData);}CDebugBreak();}CUnprovenTransferRecord_00C3B850* p=m_pObject;m_pObject=0;return p;}
class CUnprovenTransferVisitorBase_00C2ED90{public:virtual void Visit(CUnprovenTransferRecord_00C3B850*)=0;virtual ~CUnprovenTransferVisitorBase_00C2ED90(){}};class CUnprovenTransferVisitor_00C2ED90:public CUnprovenTransferVisitorBase_00C2ED90{public:virtual void Visit(CUnprovenTransferRecord_00C3B850*);PKSerialiseCParser* Archive;unsigned long Failures;CUnprovenTransferVisitor_00C2ED90(PKSerialiseCParser*);};
class CUnprovenRecordList_00c2f7d0{public:
virtual void UnusedSlot0(){}
virtual void UnusedSlot1(){}
virtual void UnusedSlot2(){}
virtual void UnusedSlot3(){}
virtual void UnusedSlot4(){}
virtual void Clear()=0;
virtual void UnusedSlot6(){}
virtual void UnusedSlot7(){}
virtual void UnusedSlot8(){}
virtual void Add(CUnprovenTransferRecord_00C3B850*)=0;
virtual void ForEach(CUnprovenTransferVisitorBase_00C2ED90*)=0;
virtual void UnusedSlot11(){}
virtual void UnusedSlot12(){}
virtual void UnusedSlot13(){}
virtual void Reserve(unsigned long)=0;
virtual unsigned long Count()=0;
bool Transfer(PKSerialiseCParser*);};
bool CUnprovenRecordList_00c2f7d0::Transfer(PKSerialiseCParser* archive){if(archive->IsReading()){unsigned long count;if(!archive->TransferSize(&count))return false;Clear();Reserve(count);while(count){PKCAutoDelete<CUnprovenTransferRecord_00C3B850> item(new CUnprovenTransferRecord_00C3B850);if(!item.m_pObject->Transfer_00C3B850(archive))return false;Add(item.Release());--count;}return true;}else{unsigned long count=Count();if(!archive->TransferSize(&count))return false;CUnprovenTransferVisitor_00C2ED90 visitor(archive);ForEach(&visitor);return visitor.Failures==0;}}