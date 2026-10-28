// CEnumMediaTypes::Clone(IEnumMediaTypes** ppEnum) -- Clone an enumerator while its owner version remains current.
// Leaf: ?Clone@CEnumMediaTypes@@UAGJPAPAUIEnumMediaTypes@@@Z
// Flags: /EHsc
// Identity: DirectShow amfilter CEnumMediaTypes::Clone, same own enumeration vtable/constructor as QueryInterface. Scalar allocation20 bytes via00bfea1a; ctor00cabf70; GetMediaTypeVersion primary slot4. Standard E_POINTER/VFW_E_ENUM_OUT_OF_SYNC/E_OUTOFMEMORY and owned cloned enumerator.
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
HRESULT __stdcall CEnumMediaTypes::Clone(IEnumMediaTypes** ppEnum){if(!ppEnum)return (HRESULT)0x80004003L;HRESULT hr=0;if(m_pPin->GetMediaTypeVersion()!=m_Version){*ppEnum=0;hr=(HRESULT)0x80040203L;}else{*ppEnum=new CEnumMediaTypes(m_pPin,this);if(*ppEnum==0)hr=(HRESULT)0x8007000eL;}return hr;}
#ifdef _MSC_VER
typedef char EnumSize[sizeof(CEnumMediaTypes)==20?1:-1];
#endif