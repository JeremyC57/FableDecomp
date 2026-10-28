// CEnumPins::QueryInterface(const IID& riid,void** ppv) -- Return the supported enumerator or unknown interface with an owned reference.
// Leaf: ?QueryInterface@CEnumPins@@UAGJABU_GUID@@PAPAX@Z
// Identity: proven. DirectShow amfilter CEnumPins::QueryInterface; own first GUID at012a9af4 =56a86892-0ad4-11ce-b03a-0020af0ba770(IEnumPins), second012c06c4 exact IID_IUnknown. Own caller/table and established clone/ctor confirm enumeration interface. Correct GetInterface00cacbe0 adds reference and returns same primary interface.
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
HRESULT __stdcall CEnumPins::QueryInterface(const IID& riid,void** ppv){if(!ppv)return (HRESULT)0x80004003L;if(riid==IID_IEnumPins||riid==IID_IUnknown)return GetInterface((IEnumPins*)this,ppv);*ppv=0;return (HRESULT)0x80004002L;}