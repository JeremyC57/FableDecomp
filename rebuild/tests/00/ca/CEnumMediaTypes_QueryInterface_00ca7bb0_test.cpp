#include <stdio.h>
// CEnumMediaTypes::QueryInterface(const IID& riid,void** ppv) -- Return the supported enumerator or unknown interface with an owned reference.
// Leaf: ?QueryInterface@CEnumMediaTypes@@UAGJABU_GUID@@PAPAX@Z
// Identity: proven. DirectShow amfilter CEnumMediaTypes::QueryInterface; own first GUID at012a9ae4 =89c31040-846b-11ce-97d3-00aa0055595a(IEnumMediaTypes), second012c06c4 exact IID_IUnknown. Own caller/table and established clone/ctor confirm enumeration interface. Correct GetInterface00cacbe0 adds reference and returns same primary interface.
// Layout: original IUnknown/enum inheritance and full enumerator fields; CEnumPins carries real CGenericList/CBaseList cache24. No applicable engine header.
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
HRESULT __stdcall CEnumMediaTypes::QueryInterface(const IID& riid,void** ppv){if(!ppv)return (HRESULT)0x80004003L;if(riid==IID_IEnumMediaTypes||riid==IID_IUnknown)return GetInterface((IEnumMediaTypes*)this,ppv);*ppv=0;return (HRESULT)0x80004002L;}
const IID IID_IUnknown={0,0,0,{0xc0,0,0,0,0,0,0,0x46}};const IID IID_IEnumPins={0x56a86892,0x0ad4,0x11ce,{0xb0,0x3a,0,0x20,0xaf,0x0b,0xa7,0x70}};const IID IID_IEnumMediaTypes={0x89c31040,0x846b,0x11ce,{0x97,0xd3,0,0xaa,0,0x55,0x59,0x5a}};
static int calls,refs;CBaseList::~CBaseList(){}extern "C" HRESULT __stdcall GetInterface(IUnknown* p,void** out){++calls;*out=p;p->AddRef();return 0;}
CEnumMediaTypes::CEnumMediaTypes(CBasePin* p,CEnumMediaTypes*):m_Position(0),m_pPin(p),m_Version(0),m_cRef(1){}CEnumMediaTypes::~CEnumMediaTypes(){}
ULONG __stdcall CEnumMediaTypes::AddRef(){++refs;return ++m_cRef;}ULONG __stdcall CEnumMediaTypes::Release(){return --m_cRef;}HRESULT __stdcall CEnumMediaTypes::Next(ULONG,AM_MEDIA_TYPE**,ULONG*){return 0;}HRESULT __stdcall CEnumMediaTypes::Skip(ULONG){return 0;}HRESULT __stdcall CEnumMediaTypes::Reset(){return 0;}HRESULT __stdcall CEnumMediaTypes::Clone(IEnumMediaTypes**){return 0;}
int main(){CEnumMediaTypes e(0,0);bool ok=e.QueryInterface(IID_IEnumMediaTypes,0)==(HRESULT)0x80004003L&&!calls&&!refs;void* p=0;ok=ok&&e.QueryInterface(IID_IEnumMediaTypes,&p)==0&&p==(IEnumMediaTypes*)&e&&calls==1&&refs==1;ok=ok&&e.QueryInterface(IID_IUnknown,&p)==0&&p==(IEnumMediaTypes*)&e&&calls==2&&refs==2;for(int i=0;i<16;++i){IID bad=IID_IEnumMediaTypes;((unsigned char*)&bad)[i]^=0x80;p=&e;ok=ok&&e.QueryInterface(bad,&p)==(HRESULT)0x80004002L&&!p;}ok=ok&&calls==2&&refs==2;puts(ok?"00ca7bb0_TEST PASS":"00ca7bb0_TEST FAIL");return ok?0:1;}