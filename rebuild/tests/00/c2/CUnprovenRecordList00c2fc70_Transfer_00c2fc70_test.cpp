#include <stdio.h>
#include <string.h>
// CUnprovenRecordList_00c2fc70::Transfer(PKSerialiseCParser* archive) -- Load owned records or transfer the existing collection through its visitor.
// Leaf: ?Transfer@CUnprovenRecordList_00c2fc70@@QAE_NPAVPKSerialiseCParser@@@Z
// Flags: /EHsc
// Identity: own CBankData caller00c2ee80 uses three corresponding collections; record ctor00c3b690/dtor00c3b6b0/Transfer00C3B7A0, actual PKCAutoDelete Release00c2f2b0 and visitor ctor00c2ee60. Descriptive record/collection names.
// Layout: no active shared headers. Own record allocation12; pointer guard4, abstract visitor base4 plus Archive4/Failures8. Collection uses observed virtual slots5 Clear/9 Add/10 ForEach/14 Reserve/15 Count only; unknown class/base size not claimed. PKSerialiseCParser name from existing helper bindings/assertions.
#include <new>
class PKSerialiseCParser{public:void* Reader;void* Writer;bool IsReading();bool TransferSize(unsigned long*);};
class CUnprovenTransferRecord_00C3B7A0{public:unsigned char Unmodeled[12];CUnprovenTransferRecord_00C3B7A0();~CUnprovenTransferRecord_00C3B7A0();bool Transfer_00C3B7A0(PKSerialiseCParser*);};
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
template<> __declspec(noinline) CUnprovenTransferRecord_00C3B7A0* PKCAutoDelete<CUnprovenTransferRecord_00C3B7A0>::Release(){if(!m_pObject){{CString text;text << "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoDelete.h" << "(" << (long)42 << ") : " << "Shouls have checked first..." << "\n";if(g_pfnAssertHook)g_pfnAssertHook(text.m_pchData);}CDebugBreak();}CUnprovenTransferRecord_00C3B7A0* p=m_pObject;m_pObject=0;return p;}
class CUnprovenTransferVisitorBase_00C2EDF0{public:virtual void Visit(CUnprovenTransferRecord_00C3B7A0*)=0;virtual ~CUnprovenTransferVisitorBase_00C2EDF0(){}};class CUnprovenTransferVisitor_00C2EDF0:public CUnprovenTransferVisitorBase_00C2EDF0{public:virtual void Visit(CUnprovenTransferRecord_00C3B7A0*);PKSerialiseCParser* Archive;unsigned long Failures;CUnprovenTransferVisitor_00C2EDF0(PKSerialiseCParser*);};
class CUnprovenRecordList_00c2fc70{public:
virtual void UnusedSlot0(){}
virtual void UnusedSlot1(){}
virtual void UnusedSlot2(){}
virtual void UnusedSlot3(){}
virtual void UnusedSlot4(){}
virtual void Clear()=0;
virtual void UnusedSlot6(){}
virtual void UnusedSlot7(){}
virtual void UnusedSlot8(){}
virtual void Add(CUnprovenTransferRecord_00C3B7A0*)=0;
virtual void ForEach(CUnprovenTransferVisitorBase_00C2EDF0*)=0;
virtual void UnusedSlot11(){}
virtual void UnusedSlot12(){}
virtual void UnusedSlot13(){}
virtual void Reserve(unsigned long)=0;
virtual unsigned long Count()=0;
bool Transfer(PKSerialiseCParser*);};
bool CUnprovenRecordList_00c2fc70::Transfer(PKSerialiseCParser* archive){if(archive->IsReading()){unsigned long count;if(!archive->TransferSize(&count))return false;Clear();Reserve(count);while(count){PKCAutoDelete<CUnprovenTransferRecord_00C3B7A0> item(new CUnprovenTransferRecord_00C3B7A0);if(!item.m_pObject->Transfer_00C3B7A0(archive))return false;Add(item.Release());--count;}return true;}else{unsigned long count=Count();if(!archive->TransferSize(&count))return false;CUnprovenTransferVisitor_00C2EDF0 visitor(archive);ForEach(&visitor);return visitor.Failures==0;}}

static bool reading,transferSizeOK;static unsigned long requested,writtenCount;static int transferred,created,destroyed,failAt;static PKSerialiseCParser* expectedArchive;static int errors;
bool PKSerialiseCParser::IsReading(){return reading;}bool PKSerialiseCParser::TransferSize(unsigned long* p){if(this!=expectedArchive)++errors;if(!transferSizeOK)return false;if(reading)*p=requested;else writtenCount=*p;return true;}
CUnprovenTransferRecord_00C3B7A0::CUnprovenTransferRecord_00C3B7A0(){++created;memset(Unmodeled,0,sizeof(Unmodeled));}CUnprovenTransferRecord_00C3B7A0::~CUnprovenTransferRecord_00C3B7A0(){++destroyed;}bool CUnprovenTransferRecord_00C3B7A0::Transfer_00C3B7A0(PKSerialiseCParser* p){if(p!=expectedArchive)++errors;++transferred;return transferred!=failAt;}
CUnprovenTransferVisitor_00C2EDF0::CUnprovenTransferVisitor_00C2EDF0(PKSerialiseCParser* p):Archive(p),Failures(0){}void CUnprovenTransferVisitor_00C2EDF0::Visit(CUnprovenTransferRecord_00C3B7A0* p){if(!Failures&&!p->Transfer_00C3B7A0(Archive))++Failures;}
class Harness:public CUnprovenRecordList_00c2fc70{public:CUnprovenTransferRecord_00C3B7A0* Items[4];unsigned long Used;int Clears,Reserves,Visits;Harness():Used(0),Clears(0),Reserves(0),Visits(0){}~Harness(){for(unsigned long i=0;i<Used;++i)delete Items[i];}virtual void Clear(){++Clears;for(unsigned long i=0;i<Used;++i)delete Items[i];Used=0;}virtual void Add(CUnprovenTransferRecord_00C3B7A0* p){Items[Used++]=p;}virtual void ForEach(CUnprovenTransferVisitorBase_00C2EDF0* p){++Visits;for(unsigned long i=0;i<Used;++i)p->Visit(Items[i]);}virtual void Reserve(unsigned long n){++Reserves;if(n>4)++errors;}virtual unsigned long Count(){return Used;}};
char* _afxPchNil=0;AssertHook g_pfnAssertHook=0;CString::~CString(){}CString& __fastcall CString::operator<<(const char*){++errors;return *this;}CString& __fastcall CString::operator<<(long){++errors;return *this;}CDebugBreak::CDebugBreak(){++errors;}
int main(){PKSerialiseCParser archive;expectedArchive=&archive;bool ok=true;{Harness h;reading=true;transferSizeOK=false;ok=ok&&!h.Transfer(&archive)&&h.Clears==0&&h.Reserves==0;transferSizeOK=true;requested=0;ok=ok&&h.Transfer(&archive)&&h.Used==0&&h.Clears==1&&h.Reserves==1;requested=3;failAt=2;transferred=0;ok=ok&&!h.Transfer(&archive)&&h.Used==1&&created==2&&destroyed==1;failAt=0;transferred=0;requested=2;ok=ok&&h.Transfer(&archive)&&h.Used==2;reading=false;transferred=0;ok=ok&&h.Transfer(&archive)&&writtenCount==2&&transferred==2;failAt=1;transferred=0;ok=ok&&!h.Transfer(&archive)&&transferred==1;transferSizeOK=false;int visits=h.Visits;ok=ok&&!h.Transfer(&archive)&&h.Visits==visits;}ok=ok&&created==destroyed&&!errors;puts(ok?"00c2fc70_TEST PASS":"00c2fc70_TEST FAIL");return ok?0:1;}