// CMediaSample::SetSyncPoint(BOOL enabled) -- Set or clear the sample property and return success.
// Leaf: ?SetSyncPoint@CMediaSample@@UAGJH@Z
// Identity: original DirectShow IMediaSample2 implementation; own QueryInterface00ca9000 compares IID_IMediaSample56a8689a and IID_IMediaSample2 36b73884, actual table012bf6e0 slots8/16/10 and SDK flags1/4/2.
// Layout: no engine owner header. Real IUnknown/IMediaSample/IMediaSample2 inheritance and original SDK CMediaSample fields; m_dwFlags+4 and completePCsize72 corroborated by own AddRef+44 and SetMediaType+3c.
#ifndef _MSC_VER
#define __stdcall
#define __declspec(x)
typedef long LONGLONG;
#else
typedef __int64 LONGLONG;
#endif
typedef long HRESULT;typedef unsigned long ULONG;typedef unsigned long DWORD;typedef int BOOL;typedef long LONG;typedef unsigned char BYTE;typedef LONGLONG REFERENCE_TIME;
struct _GUID{unsigned long Data1;unsigned short Data2,Data3;unsigned char Data4[8];};typedef _GUID IID;struct _AMMediaType;typedef _AMMediaType AM_MEDIA_TYPE;
struct IUnknown{virtual HRESULT __stdcall QueryInterface(const IID&,void**)=0;virtual ULONG __stdcall AddRef()=0;virtual ULONG __stdcall Release()=0;};
struct __declspec(novtable) IMediaSample : IUnknown {
    virtual HRESULT __stdcall GetPointer(BYTE** ppBuffer) = 0;                                   // 3
    virtual long __stdcall GetSize() = 0;                                                        // 4
    virtual HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0; // 5
    virtual HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd) = 0; // 6
    virtual HRESULT __stdcall IsSyncPoint() = 0;                                                 // 7
    virtual HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint) = 0;                               // 8
    virtual HRESULT __stdcall IsPreroll() = 0;                                                   // 9
    virtual HRESULT __stdcall SetPreroll(BOOL bIsPreroll) = 0;                                   // 10
    virtual long __stdcall GetActualDataLength() = 0;                                            // 11
    virtual HRESULT __stdcall SetActualDataLength(long lLen) = 0;                                // 12
    virtual HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType) = 0;                     // 13
    virtual HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType) = 0;                       // 14 (+0x38)
    virtual HRESULT __stdcall IsDiscontinuity() = 0;                                             // 15
    virtual HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity) = 0;                         // 16
    virtual HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 17
    virtual HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd) = 0;        // 18
};
struct __declspec(novtable) IMediaSample2 : IMediaSample {
    virtual HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties) = 0;         // 19
    virtual HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties) = 0;   // 20
};
class CBaseAllocator;
class CMediaSample : public IMediaSample2 {      // amfilter.h
protected:
    friend class CBaseAllocator;
    enum { Sample_SyncPoint = 0x01, Sample_Preroll = 0x02, Sample_Discontinuity = 0x04, Sample_TypeChanged = 0x08,
           Sample_TimeValid = 0x10, Sample_MediaTimeValid = 0x20, Sample_TimeDiscontinuity = 0x40, Sample_StopValid = 0x100,
           Sample_ValidFlags = 0x1FF };
    DWORD m_dwFlags;                              // +0x04
    DWORD m_dwTypeSpecificFlags;                  // +0x08
    BYTE* m_pBuffer;                              // +0x0C
    LONG m_lActual;                               // +0x10
    LONG m_cbBuffer;                              // +0x14
    CBaseAllocator* m_pAllocator;                 // +0x18
    CMediaSample* m_pNext;                        // +0x1C
    REFERENCE_TIME m_Start;                       // +0x20
    REFERENCE_TIME m_End;                         // +0x28
    LONGLONG m_MediaStart;                        // +0x30
    LONG m_MediaEnd;                              // +0x38
    AM_MEDIA_TYPE* m_pMediaType;                  // +0x3C
    DWORD m_dwStreamId;                           // +0x40
public:
    LONG m_cRef;                                  // +0x44
    virtual ~CMediaSample();
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppv);
    ULONG __stdcall AddRef();
    ULONG __stdcall Release();
    HRESULT SetPointer(BYTE* ptr, LONG cBytes);
    HRESULT __stdcall GetPointer(BYTE** ppBuffer);
    long __stdcall GetSize();
    HRESULT __stdcall GetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd);
    HRESULT __stdcall SetTime(REFERENCE_TIME* pTimeStart, REFERENCE_TIME* pTimeEnd);
    HRESULT __stdcall IsSyncPoint();
    HRESULT __stdcall SetSyncPoint(BOOL bIsSyncPoint);
    HRESULT __stdcall IsPreroll();
    HRESULT __stdcall SetPreroll(BOOL bIsPreroll);
    long __stdcall GetActualDataLength();
    HRESULT __stdcall SetActualDataLength(long lActual);
    HRESULT __stdcall GetMediaType(AM_MEDIA_TYPE** ppMediaType);
    HRESULT __stdcall SetMediaType(AM_MEDIA_TYPE* pMediaType);
    HRESULT __stdcall IsDiscontinuity();
    HRESULT __stdcall SetDiscontinuity(BOOL bDiscontinuity);
    HRESULT __stdcall GetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd);
    HRESULT __stdcall SetMediaTime(LONGLONG* pTimeStart, LONGLONG* pTimeEnd);
    HRESULT __stdcall GetProperties(DWORD cbProperties, BYTE* pbProperties);
    HRESULT __stdcall SetProperties(DWORD cbProperties, const BYTE* pbProperties);
};
HRESULT __stdcall CMediaSample::SetSyncPoint(BOOL enabled){if(enabled)m_dwFlags|=Sample_SyncPoint;else m_dwFlags&=~Sample_SyncPoint;return 0;}