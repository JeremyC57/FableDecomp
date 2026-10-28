#pragma optimize("t",on)
// CVideoSys::Init(const CWideString& filename, CTexture* texture, bool render_source_pin) -- build the DirectShow graph for a movie: filter graph, the texture renderer filter, then RenderFile (.wmv/.asf) or source filter + pin connection, the control interfaces, the critical section and the render event
// Leaf: ?Init@CVideoSys@@QAEJABVCWideString@@PAVCTexture@@_N@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B9D0, 592 bytes, thiscall `ret 0Ch`, returns the HRESULT.
// Identity: Ego_r.pdb / FableWin.pdb `?Init@CVideoSys@@QAEJABVCWideString@@PAVCTexture@@_N@Z` (the manifest row agrees);
//   the body is the DirectX 9 SDK Texture3D9 sample's InitDShowTextureRenderer moved into CVideoSys (CLSID_FilterGraph,
//   the texture renderer filter, AddSourceFilter L"SOURCE", FindPin L"Output" / L"In").
// Callees: GFGetSystemManager 0x009A4EC0 (PInputManager +0x58: CInputManager::Update, RTTI vtable 0x0129C79C slot 2, then
//   CInputManager::ClearEvents 0x009F5540), CMovieSubtitles::ParseSubtitles 0x009BD460 (after the inline SetTextBank),
//   CWideString::operator const wchar_t* 0x0099AF00 and GetAsCharArray 0x0099AF10 (Ego_r donor order: the operator
//   0x00B02200 sits right before GetAsCharArray 0x00B02220), wcsstr 0x00BFEB2E, operator new 0x00BFEA1A (0x180 bytes),
//   CTextureRenderer::CTextureRenderer(IUnknown*, long*, CTexture*) 0x00A3B510, CoCreateInstance / InitializeCriticalSection
//   / CreateEventW (IAT), IGraphBuilder / IBaseFilter / IUnknown COM slots.
// Globals: two bss bools 0x013D2825 (tested first: Init does nothing when set) and 0x013D2826 (cleared here); neither
//   has any other reference in the exe, so their names stay descriptive.
// Layout: engine/CVideoSys.h (TLC offsets); Subtitles is the opaque 0x2C-byte CMovieSubtitles (retail STLport list 4
//   bytes, pTextBank +4). The renderer's IBaseFilter subobject is at +0xC (landed CTextureRenderer ctor model).
// Shape: the IBaseFilter pointer is a named local (`IBaseFilter* renderer_filter = TexRenderer;`, the null-checked
//   upcast kept in edi for AddFilter and the later FindPin); the source output pin and the renderer input pin live in
//   the dead filename / bool argument slots; /Zc:wchar_t gives the PDB's `_W` spellings.
#define CVideoSys CVideoSysLayout
#include "engine/CVideoSys.h"
#undef CVideoSys

struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern "C" const _GUID CLSID_FilterGraph;      // 0x012AB174 {E436EBB3-524F-11CE-9F53-0020AF0BA770}
extern "C" const _GUID IID_IGraphBuilder;      // 0x012A9934 {56A868A9-0AD4-11CE-B03A-0020AF0BA770}
extern "C" const _GUID IID_IMediaControl;      // 0x012AA094
extern "C" const _GUID IID_IMediaPosition;     // 0x012AA064
extern "C" const _GUID IID_IMediaSeeking;      // 0x012A9A04
extern "C" const _GUID IID_IMediaEvent;        // 0x012AA084
extern "C" const _GUID IID_IBasicAudio;        // 0x012AA054

struct IUnknown {
    virtual long __stdcall QueryInterface(const _GUID& iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct IPin : IUnknown { };
struct IBaseFilter : IUnknown {
    virtual long __stdcall GetClassID(_GUID* clsid);                  // IPersist
    virtual long __stdcall Stop();                                    // IMediaFilter
    virtual long __stdcall Pause();
    virtual long __stdcall Run(__int64 start);
    virtual long __stdcall GetState(unsigned long timeout, int* state);
    virtual long __stdcall SetSyncSource(void* clock);
    virtual long __stdcall GetSyncSource(void** clock);
    virtual long __stdcall EnumPins(void** pins);                     // IBaseFilter
    virtual long __stdcall FindPin(const wchar_t* id, IPin** pin);    // slot 11 (+0x2C)
};
struct IAMovieSetup : IUnknown { };
struct IGraphBuilder : IUnknown {
    virtual long __stdcall AddFilter(IBaseFilter* filter, const wchar_t* name) = 0;           // slot 3 (+0x0C)
    virtual long __stdcall RemoveFilter(IBaseFilter* filter) = 0;
    virtual long __stdcall EnumFilters(void** filters) = 0;
    virtual long __stdcall FindFilterByName(const wchar_t* name, IBaseFilter** filter) = 0;
    virtual long __stdcall ConnectDirect(IPin* out, IPin* in, const void* mt) = 0;
    virtual long __stdcall Reconnect(IPin* pin) = 0;
    virtual long __stdcall Disconnect(IPin* pin) = 0;
    virtual long __stdcall SetDefaultSyncSource() = 0;
    virtual long __stdcall Connect(IPin* out, IPin* in) = 0;                                  // slot 11 (+0x2C)
    virtual long __stdcall Render(IPin* out) = 0;                                             // slot 12 (+0x30)
    virtual long __stdcall RenderFile(const wchar_t* file, const wchar_t* play_list) = 0;     // slot 13 (+0x34)
    virtual long __stdcall AddSourceFilter(const wchar_t* file, const wchar_t* name, IBaseFilter** filter) = 0;  // +0x38
};

struct _RTL_CRITICAL_SECTION {
    _RTL_CRITICAL_SECTION_DEBUG* DebugInfo;
    long  LockCount;
    long  RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};
extern "C" __declspec(dllimport) long __stdcall CoCreateInstance(const _GUID& clsid, IUnknown* outer,
    unsigned long context, const _GUID& iid, void** object);                                  // IAT 0x01440640
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION* section);  // 0x0143FEE4
extern "C" __declspec(dllimport) void* __stdcall CreateEventW(void* attributes, int manual_reset, int initial_state,
    const wchar_t* name);                                                                      // IAT 0x0143FDC8
extern "C" wchar_t* __cdecl wcsstr(const wchar_t* string, const wchar_t* search);              // 0x00BFEB2E
void* __cdecl operator new(unsigned int size);                                                // 0x00BFEA1A


class CWideString {
public:
    void* PStringData;
    operator const wchar_t*() const;            // 0x0099AF00
    const wchar_t* GetAsCharArray() const;      // 0x0099AF10
};

namespace NGameText { class CDataBank; }
class CMovieSubtitles {
public:
    void*                        SubTitles;     // +0x00 STLport list<CSubTitle>
    const NGameText::CDataBank*  pTextBank;     // +0x04
    void SetTextBank(const NGameText::CDataBank* bank) { pTextBank = bank; }
    void ParseSubtitles(const CWideString& filename);   // 0x009BD460
};

class CInputManager {
public:
    virtual ~CInputManager();
    virtual bool SetExclusiveMode(bool exclusive);
    virtual void Update();                      // slot 2 (+0x08)
    void ClearEvents();                         // 0x009F5540
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                       // +0x00
    CCPPointerInfo* Info;                       // +0x04
};
class CSystemManager {
public:
    unsigned char                  _pad00[0x58];
    CCountedPointer<CInputManager> PInputManager;   // +0x58
};
CSystemManager* __fastcall GFGetSystemManager();    // 0x009A4EC0

// strmbase bases of the renderer (landed CTextureRenderer model: IBaseFilter vptr +0x0C, PVideoSys +0x160).
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const _GUID& iid, void** object);
    virtual unsigned long __stdcall NonDelegatingAddRef();
    virtual unsigned long __stdcall NonDelegatingRelease();
};
class CUnknown : public INonDelegatingUnknown {
public:
    void* m_pUnknown;                           // +0x04
    long  m_cRef;                               // +0x08
};
class CBaseFilter : public CUnknown, public IBaseFilter, public IAMovieSetup {
public:
    long __stdcall QueryInterface(const _GUID& iid, void** object);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall FindPin(const wchar_t* id, IPin** pin);
    unsigned char _pad14[0x20 - 0x14];          // m_State, m_pClock
    __int64       m_tStart;                     // +0x20 CRefTime (REFERENCE_TIME): the class is 8-aligned (size 0x180)
    unsigned char _pad28[0xe0 - 0x28];
};
struct IQualProp : IUnknown { };
struct IQualityControl : IUnknown { };
class CBaseVideoRenderer : public CBaseFilter, public IQualProp, public IQualityControl {
public:
    long __stdcall QueryInterface(const _GUID& iid, void** object);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    unsigned char _pad_e8[0x160 - 0xe8];
};
class CVideoSys;
struct CTextureRenderer : public CBaseVideoRenderer {
    CVideoSys* PVideoSys;                       // +0x160
    CTexture*  Texture;                         // +0x164
    long       TextureFormat;                   // +0x168
    int        Width;                           // +0x16c
    int        Height;                          // +0x170
    int        Pitch;                           // +0x174
    bool       CreateOwnTexture;                // +0x178
    CTextureRenderer(IUnknown* unknown, long* phr, CTexture* texture);   // 0x00A3B510
};

struct IMediaControl : IUnknown { };
struct IMediaPosition : IUnknown { };
struct IMediaSeeking : IUnknown { };
struct IMediaEvent : IUnknown { };
struct IBasicAudio : IUnknown { };

extern bool g_VideoSysInitDisabled_013d2825;    // 0x013D2825 (bss, read only here)
extern bool g_VideoSysInitFlag_013d2826;        // 0x013D2826 (bss, cleared only here)

class CVideoSys : public CVideoSysLayout {
public:
    long Init(const CWideString& filename, CTexture* texture, bool render_source_pin);
};

long CVideoSys::Init(const CWideString& filename, CTexture* texture, bool render_source_pin)
{
    IBaseFilter* source_filter;                 // the SDK sample's pFSrc, declared up front (own frame slot)
    if (g_VideoSysInitDisabled_013d2825)
        return 0;

    CInputManager* input_manager = GFGetSystemManager()->PInputManager.Data;
    input_manager->Update();
    input_manager->ClearEvents();
    g_VideoSysInitFlag_013d2826 = false;

    reinterpret_cast<CMovieSubtitles*>(Subtitles)->SetTextBank((const NGameText::CDataBank*)pTextBank);
    reinterpret_cast<CMovieSubtitles*>(Subtitles)->ParseSubtitles(filename);

    bool render_file = false;
    if (wcsstr(filename, L".wmv") || wcsstr(filename, L".asf"))
        render_file = true;

    long hr = CoCreateInstance(CLSID_FilterGraph, 0, 1, IID_IGraphBuilder, (void**)&GBuilder);
    if (hr < 0)
        return hr;

    if (!TexRenderer) {
        TexRenderer = new CTextureRenderer(0, &hr, texture);
        TexRenderer->PVideoSys = this;
        TexRenderer->AddRef();
    }
    IBaseFilter* renderer_filter = TexRenderer;
    hr = GBuilder->AddFilter(renderer_filter, L"Fable Texture Renderer Filter");
    if (hr < 0)
        return hr;

    if (render_file) {
        hr = GBuilder->RenderFile(filename.GetAsCharArray(), 0);
        if (hr < 0)
            return hr;
    }
    else {
        hr = GBuilder->AddSourceFilter(filename.GetAsCharArray(), L"SOURCE", &source_filter);
        if (hr < 0)
            return hr;
        IPin* source_pin_out;
        hr = source_filter->FindPin(L"Output", &source_pin_out);
        if (hr < 0)
            return hr;
        if (render_source_pin) {
            GBuilder->Render(source_pin_out);
        }
        else {
            IPin* renderer_pin_in;
            hr = renderer_filter->FindPin(L"In", &renderer_pin_in);
            if (hr < 0)
                return hr;
            hr = GBuilder->Connect(source_pin_out, renderer_pin_in);
            if (hr < 0)
                return hr;
        }
    }

    GBuilder->QueryInterface(IID_IMediaControl, (void**)&MControl);
    GBuilder->QueryInterface(IID_IMediaPosition, (void**)&MPosition);
    GBuilder->QueryInterface(IID_IMediaSeeking, (void**)&MSeeking);
    GBuilder->QueryInterface(IID_IMediaEvent, (void**)&MEvent);
    GBuilder->QueryInterface(IID_IBasicAudio, (void**)&BasicAudio);
    State = 3;
    InitializeCriticalSection((_RTL_CRITICAL_SECTION*)&CriticalSection_DebugInfo);
    RenderEvent = CreateEventW(0, 0, 0, 0);
    return 0;
}