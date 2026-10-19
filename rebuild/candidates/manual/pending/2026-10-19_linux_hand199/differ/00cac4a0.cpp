// CMediaRenderer_00cac4a0::OnRenderEffect(const AM_MEDIA_TYPE* type, long a2, long a3, long a4, long a5, double rate) -- copy the media type into a CMediaType, check it, then forward the remaining arguments to virtual slot 22; the first failure HRESULT is returned, else S_OK
// Flags: /EHsc /GS
// Grade: DIFFER(194v194) -- one register: retail builds the CheckMediaType argument with `lea ecx,[esp+8] ; push ecx`, ours with eax (pointer, reference and ternary forms alike)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC4A0, length 208 bytes (ret 1Ch; EH frame + /GS cookie).
// Label note: the manifest's "CEngineScreenEffectRenderer::OnRenderEffect" is a propagation into the DirectShow base-class code
// linked into the exe; the class is named by address. The local is a DirectShow CMediaType (the 0x48-byte AM_MEDIA_TYPE:
// pUnk +0x3C, cbFormat +0x40, pbFormat +0x44; its GUIDs' byte arrays make it a /GS buffer).
// Callees: CMediaType::CMediaType(const AM_MEDIA_TYPE&, HRESULT*) 0x00CB1D60, CMediaType::~CMediaType 0x00CB1CC0 (landed as
// "CComHolder_00cb1cc0::Release": it is the destructor -- EH state 0 covers it), the media-type check 0x00CAB990 on `this`
// (landed as "CAudioBank_00cab990::LoadAsync"), the forwarding target virtual slot 22 (+0x58); __security_check_cookie
// 0x00BFE9F9 against the cookie 0x0139C8A8.
// Shape: the CMediaType is a function-scope local (EH state 0 after construction, -1 before each explicit destructor
// call on the two return paths); the check result rides in edi, the forwarded result in esi.
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct IUnknown;
struct AM_MEDIA_TYPE {
    GUID          majortype;                    // +0x00
    GUID          subtype;                      // +0x10
    int           bFixedSizeSamples;            // +0x20
    int           bTemporalCompression;         // +0x24
    unsigned long lSampleSize;                  // +0x28
    GUID          formattype;                   // +0x2c
    IUnknown*     pUnk;                         // +0x3c
    unsigned long cbFormat;                     // +0x40
    unsigned char* pbFormat;                    // +0x44
};
class CMediaType : public AM_MEDIA_TYPE {
public:
    CMediaType(const AM_MEDIA_TYPE& type, long* result = 0);   // 0x00CB1D60
    ~CMediaType();                                             // 0x00CB1CC0
};

class CMediaRenderer_00cac4a0 {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual long Forward(long a2, long a3, long a4, long a5, double rate);   // slot 22 (+0x58)
    long CheckMediaType(const CMediaType* type);   // 0x00CAB990
    long OnRenderEffect(const AM_MEDIA_TYPE* type, long a2, long a3, long a4, long a5, double rate);
};

long CMediaRenderer_00cac4a0::OnRenderEffect(const AM_MEDIA_TYPE* type, long a2, long a3, long a4, long a5, double rate)
{
    CMediaType mediaType(*type, 0);
    long result = CheckMediaType(&mediaType);
    if (result < 0)
        return result;
    result = Forward(a2, a3, a4, a5, rate);
    if (result < 0)
        return result;
    return 0;
}
