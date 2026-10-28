#include <stdio.h>
// CEnumPins::Clone(IEnumPins** ppEnum) -- Clone an enumerator while its owner version remains current.
// Leaf: ?Clone@CEnumPins@@UAGJPAPAUIEnumPins@@@Z
// Flags: /EHsc
// Identity: DirectShow amfilter CEnumPins::Clone, same own enumeration vtable/constructor as QueryInterface. Scalar allocation48 bytes via00bfea1a; ctor00cabe00; GetPinVersion primary slot5. Standard E_POINTER/VFW_E_ENUM_OUT_OF_SYNC/E_OUTOFMEMORY and owned cloned enumerator.
// Layout: complete enumeration fields, DirectShow real COM/base inheritance and primary owner virtual prefix. Unused owner fields and secondary interface methods omitted, no complete owner size claimed. CBaseObject is the empty DirectShow release type, not the unrelated engine class of the same spelling.
#include <string.h>
#ifndef _MSC_VER
#define __stdcall
typedef int HRESULT;typedef unsigned int ULONG;typedef int LONG;typedef unsigned int DWORD;
#else
typedef long HRESULT;typedef unsigned long ULONG;typedef long LONG;typedef unsigned long DWORD;
#pragma intrinsic(memcmp)
#endif
struct _GUID{DWORD Data1;unsigned short Data2,Data3;unsigned char Data4[8];};typedef _GUID GUID;typedef GUID IID;
inline bool operator==(const GUID& a,const GUID& b){return !memcmp(&a,&b,sizeof(GUID));}
struct IUnknown{virtual HRESULT __stdcall QueryInterface(const IID&,void**)=0;virtual ULONG __stdcall AddRef()=0;virtual ULONG __stdcall Release()=0;};
struct IPin;struct _AMMediaType;typedef _AMMediaType AM_MEDIA_TYPE;class CBasePin;class CBaseFilter;
struct IEnumPins:IUnknown{virtual HRESULT __stdcall Next(ULONG,IPin**,ULONG*)=0;virtual HRESULT __stdcall Skip(ULONG)=0;virtual HRESULT __stdcall Reset()=0;virtual HRESULT __stdcall Clone(IEnumPins**)=0;};
struct IEnumMediaTypes:IUnknown{virtual HRESULT __stdcall Next(ULONG,AM_MEDIA_TYPE**,ULONG*)=0;virtual HRESULT __stdcall Skip(ULONG)=0;virtual HRESULT __stdcall Reset()=0;virtual HRESULT __stdcall Clone(IEnumMediaTypes**)=0;};
class CBaseList{public:class CNode{CNode* m_pPrev;CNode* m_pNext;void* m_pObject;};class CNodeCache{int m_iCacheSize,m_iUsed;CNode* m_pHead;};protected:CNode* m_pFirst;CNode* m_pLast;LONG m_Count;private:CNodeCache m_Cache;public:~CBaseList();};template<class T>class CGenericList:public CBaseList{};
class CEnumPins:public IEnumPins{int m_Position,m_PinCount;CBaseFilter* m_pFilter;LONG m_Version,m_cRef;CGenericList<CBasePin> m_PinCache;public:CEnumPins(CBaseFilter*,CEnumPins*);virtual ~CEnumPins();HRESULT __stdcall QueryInterface(const IID&,void**);ULONG __stdcall AddRef();ULONG __stdcall Release();HRESULT __stdcall Next(ULONG,IPin**,ULONG*);HRESULT __stdcall Skip(ULONG);HRESULT __stdcall Reset();HRESULT __stdcall Clone(IEnumPins**);};
class CEnumMediaTypes:public IEnumMediaTypes{int m_Position;CBasePin* m_pPin;LONG m_Version,m_cRef;public:CEnumMediaTypes(CBasePin*,CEnumMediaTypes*);virtual ~CEnumMediaTypes();HRESULT __stdcall QueryInterface(const IID&,void**);ULONG __stdcall AddRef();ULONG __stdcall Release();HRESULT __stdcall Next(ULONG,AM_MEDIA_TYPE**,ULONG*);HRESULT __stdcall Skip(ULONG);HRESULT __stdcall Reset();HRESULT __stdcall Clone(IEnumMediaTypes**);};
extern "C" HRESULT __stdcall GetInterface(IUnknown*,void**);extern "C" const IID IID_IUnknown;extern "C" const IID IID_IEnumPins;extern "C" const IID IID_IEnumMediaTypes;

struct INonDelegatingUnknown{virtual HRESULT __stdcall NonDelegatingQueryInterface(const IID&,void**)=0;virtual ULONG __stdcall NonDelegatingAddRef()=0;virtual ULONG __stdcall NonDelegatingRelease()=0;};
class CBaseObject{};
class CUnknown:public INonDelegatingUnknown,public CBaseObject{IUnknown* const m_pUnknown;protected:LONG m_cRef;public:CUnknown():m_pUnknown(0),m_cRef(1){}virtual ~CUnknown(){}HRESULT __stdcall NonDelegatingQueryInterface(const IID&,void**);ULONG __stdcall NonDelegatingAddRef();ULONG __stdcall NonDelegatingRelease();};
struct IPersist:IUnknown{};struct IMediaFilter:IPersist{};struct IBaseFilter:IMediaFilter{};struct IAMovieSetup:IUnknown{};struct IPin:IUnknown{};struct IQualityControl:IUnknown{};class CRefTime;
class CBaseFilter:public CUnknown,public IBaseFilter,public IAMovieSetup{public:HRESULT __stdcall QueryInterface(const IID&,void**);ULONG __stdcall AddRef();ULONG __stdcall Release();virtual HRESULT StreamTime(CRefTime&);virtual LONG GetPinVersion();};
class CBasePin:public CUnknown,public IPin,public IQualityControl{public:HRESULT __stdcall QueryInterface(const IID&,void**);ULONG __stdcall AddRef();ULONG __stdcall Release();virtual LONG GetMediaTypeVersion();};
HRESULT __stdcall CEnumPins::Clone(IEnumPins** ppEnum){if(!ppEnum)return (HRESULT)0x80004003L;HRESULT hr=0;if(m_pFilter->GetPinVersion()!=m_Version){*ppEnum=0;hr=(HRESULT)0x80040203L;}else{*ppEnum=new CEnumPins(m_pFilter,this);if(*ppEnum==0)hr=(HRESULT)0x8007000eL;}return hr;}
#ifdef _MSC_VER
typedef char EnumSize[sizeof(CEnumPins)==48?1:-1];
#endif
static LONG version=7;static int checks,clones,bad;static void* expectedSource;static void* expectedOwner;CBaseList::~CBaseList(){}HRESULT __stdcall CUnknown::NonDelegatingQueryInterface(const IID&,void**){return 0;}ULONG __stdcall CUnknown::NonDelegatingAddRef(){return ++m_cRef;}ULONG __stdcall CUnknown::NonDelegatingRelease(){return --m_cRef;}
HRESULT __stdcall CBaseFilter::QueryInterface(const IID&,void**){return 0;}ULONG __stdcall CBaseFilter::AddRef(){return 1;}ULONG __stdcall CBaseFilter::Release(){return 1;}LONG CBaseFilter::GetPinVersion(){++checks;return version;}
HRESULT CBaseFilter::StreamTime(CRefTime&){return 0;}
CEnumPins::CEnumPins(CBaseFilter* p,CEnumPins* source):m_Position(source?source->m_Position:19),m_PinCount(3),m_pFilter(p),m_Version(source?source->m_Version:version),m_cRef(1){if(source){++clones;if(source!=expectedSource||p!=expectedOwner)++bad;}}CEnumPins::~CEnumPins(){}
HRESULT __stdcall CEnumPins::QueryInterface(const IID&,void**){return 0;}ULONG __stdcall CEnumPins::AddRef(){return ++m_cRef;}ULONG __stdcall CEnumPins::Release(){return --m_cRef;}HRESULT __stdcall CEnumPins::Next(ULONG,IPin**,ULONG*){return 0;}HRESULT __stdcall CEnumPins::Skip(ULONG n){m_Position+=n;return 0;}HRESULT __stdcall CEnumPins::Reset(){return 0;}
int main(){CBaseFilter owner;CEnumPins original(&owner,0);expectedOwner=&owner;expectedSource=&original;bool ok=original.Clone(0)==(HRESULT)0x80004003L&&!checks&&!clones;IEnumPins* output=&original;version=8;ok=ok&&original.Clone(&output)==(HRESULT)0x80040203L&&!output&&checks==1&&!clones;version=7;ok=ok&&original.Clone(&output)==0&&output&&output!=&original&&checks==2&&clones==1&&!bad;if(output){CEnumPins* copied=static_cast<CEnumPins*>(output);expectedSource=copied;IEnumPins* second=0;ok=ok&&copied->Clone(&second)==0&&second&&second!=copied&&clones==2&&!bad;delete static_cast<CEnumPins*>(second);delete copied;}puts(ok?"00cabed0_TEST PASS":"00cabed0_TEST FAIL");return ok?0:1;}