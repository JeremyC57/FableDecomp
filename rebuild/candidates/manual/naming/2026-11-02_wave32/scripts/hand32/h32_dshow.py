# h32_dshow.py: naming wave 32, family dshow (chunk 704 PRIORITY): the five CBaseVideoRenderer vtables 0x012BF5D8 / 598 / 584 / 560 / 54C
#  carried the real ??_7CBaseVideoRenderer@@6B{CUnknown,IBaseFilter,IAMovieSetup,IQualProp,IQualityControl}@@@ (chunk 704 ~CBaseVideoRenderer
#  0x00CA7310) and the landed ctor 0x00CA7360's ??_7CVolumeDerived_00ca7360@@6B...@@@ stand-ins: 0x00CA7360 is re-authored as the DX9 SDK
#  strmbase renbase.cpp CBaseVideoRenderer::CBaseVideoRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) on the chunk 704 hierarchy (AM_NOVTABLE
#  CUnknown / CBaseFilter, novtable MIDL interfaces), calling ??0CBaseRenderer@@QAE@ABU_GUID@@PBGPAUIUnknown@@PAJ@Z 0x00CA61D0 and the virtual
#  ResetStreamingTimes 0x00CA52A0 (slot 47 of 0x012BF5D8, between OnDirectRender 0x00CA5570 and SendQuality 0x00CA56E0: the renbase.h order).
#  Its caller CTextureRenderer::CTextureRenderer 0x00A3B510 spelled the ctor with `struct GUID` (UGUID); guiddef.h's GUID is the typedef of
#  struct _GUID (U_GUID, the spelling 86 landed symbols use), so the caller's GUID becomes that typedef.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
HIER = open(os.path.join(W, 'runs', 'dshow_hier.txt')).read()
CBVR = '''class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {
protected:
    int            m_nNormal;                               // +0xE8
    BOOL           m_bSupplierHandlingQuality;              // +0xEC
    int            m_trThrottle;                            // +0xF0
    int            m_trRenderAvg;                           // +0xF4
    int            m_trRenderLast;                          // +0xF8
    int            m_tRenderStart;                          // +0xFC
    int            m_trEarliness;                           // +0x100
    int            m_trTarget;                              // +0x104
    int            m_trWaitAvg;                             // +0x108
    int            m_trFrameAvg;                            // +0x10C
    int            m_trDuration;                            // +0x110
    REFERENCE_TIME m_trRememberStampForPerf;                // +0x118
    int            m_cFramesDropped;                        // +0x120
    int            m_cFramesDrawn;                          // +0x124
    __int64        m_iTotAcc;                               // +0x128
    __int64        m_iSumSqAcc;                             // +0x130
    REFERENCE_TIME m_trLastDraw;                            // +0x138
    __int64        m_iSumSqFrameTime;                       // +0x140
    __int64        m_iSumFrameTime;                         // +0x148
    int            m_trLate;                                // +0x150
    int            m_trFrame;                               // +0x154
    int            m_tStreamingStart;                       // +0x158
public:
    CBaseVideoRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr);
    virtual ~CBaseVideoRenderer();                          // 0x00CA7310
    virtual HRESULT ResetStreamingTimes();                  // 0x00CA52A0 (slot 47)
};
'''
he.put('00ca7360', 'src', '''// CBaseVideoRenderer::CBaseVideoRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) -- DirectShow strmbase video renderer constructor: CBaseRenderer from the four arguments, this class's five vtables, the three counters cleared, then ResetStreamingTimes()
// Leaf: ??0CBaseVideoRenderer@@QAE@ABU_GUID@@PBGPAUIUnknown@@PAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7360, 103 bytes, thiscall, `ret 10h`; no EH frame (default flags).
// Identity (proven): the DX9 SDK strmbase renbase.cpp constructor. (1) It stores the same five vtables as the chunk 704 ~CBaseVideoRenderer
//   0x00CA7310 (0x012BF5D8 +0x00, 0x012BF598 +0x0C, 0x012BF584 +0x10, 0x012BF560 +0xE0, 0x012BF54C +0xE4). (2) Its base call 0x00CA61D0
//   stores CBaseRenderer's vtables 0x012BF498 / 458 / 444 (the landed CBaseFilter::QueryInterface 0x00CA62D0 names 0x012BF458 CBaseRenderer's
//   IBaseFilter vtable) and is the SDK CBaseRenderer ctor body. (3) The body is renbase.cpp's: init list m_bSupplierHandlingQuality(FALSE)
//   +0xEC, m_cFramesDropped(0) +0x120, m_cFramesDrawn(0) +0x124 (emitted in declaration order), then ResetStreamingTimes() -- virtual
//   (slot 47 of 0x012BF5D8), called directly as in any constructor. (4) Its only caller is CTextureRenderer::CTextureRenderer 0x00A3B510
//   (CLSID 0x0129D150, name L"Texture Renderer": UNICODE strmbase, LPCTSTR = const WCHAR*). The catalog label "CSpellContainerList::
//   CSpellContainerList" and the CVolumeDerived_00ca7360 model were wrong.
// Callees: CBaseRenderer::CBaseRenderer 0x00CA61D0, CBaseVideoRenderer::ResetStreamingTimes 0x00CA52A0.
// Layout: the chunk 704 ~CBaseVideoRenderer model (CBaseRenderer to +0xE0, IQualProp +0xE0, IQualityControl +0xE4); the CBaseVideoRenderer
//   members are renbase.h's (DX9, no PERF): m_nNormal +0xE8 .. m_tStreamingStart +0x158, sizeof 0x160 (CTextureRenderer's members start at
//   +0x160, Ego_r.pdb).
// History: landed as CVolumeDerived_00ca7360 (classes named by address); re-authored on the strmbase hierarchy in naming wave 32 (dshow).
''' + HIER + '''''' + CBVR + '''
CBaseVideoRenderer::CBaseVideoRenderer(const GUID& RenderClass, const WCHAR* pName, IUnknown* pUnk, HRESULT* phr)
    : CBaseRenderer(RenderClass, pName, pUnk, phr),
      m_cFramesDropped(0),
      m_cFramesDrawn(0),
      m_bSupplierHandlingQuality(FALSE)
{
    ResetStreamingTimes();
}
''')
a = '00a3b510'
he.put(a, 'src', '''// CTextureRenderer::CTextureRenderer(LPUNKNOWN, HRESULT*, CTexture*) -- DirectShow video renderer that draws into a texture (own texture when none is given)
// Leaf: ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3B510, 113 bytes (+15 int3). thiscall, `ret 0Ch`. VIDEO/RENDER LAYER (DirectShow
//         strmbase + D3D9 texture; replacement candidate).
// Identity: Ego_r.pdb / FableWin.pdb ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z; RTTI .?AVCTextureRenderer@@.
// Callees: CBaseVideoRenderer::CBaseVideoRenderer(REFCLSID, LPCTSTR, LPUNKNOWN, HRESULT*) 0x00CA7360 with CLSID 0x0129D150
//          and name L"Texture Renderer" 0x0129D160 (UNICODE strmbase). Vtables: 0x0129D08C +0x00 (CUnknown), 0x0129D04C +0x0C
//          (IBaseFilter), 0x0129D030 +0x10 (IAMovieSetup), 0x0129D008 +0xE0 (IQualProp), 0x0129CFF0 +0xE4 (IQualityControl).
// Shape: the five vtable stores, init list PVideoSys(0), CreateOwnTexture(false) through one `xor ecx,ecx`; the body stores
//        *phr = S_OK, Texture = texture and sets CreateOwnTexture when no texture was passed.
// Layout: the strmbase classes of CBaseVideoRenderer::CBaseVideoRenderer 0x00CA7360 (the chunk 704 model; sizeof 0x160); Ego_r.pdb
//         CTextureRenderer members from +0x160: PVideoSys, Texture +0x164, TextureFormat +0x168, Width +0x16C, Height +0x170,
//         Pitch +0x174, CreateOwnTexture +0x178.
// History: landed on CUnknownPart / CBaseFilterPart stand-ins with `struct GUID` (the vtable spelled ??_7CTextureRenderer@@6BCUnknownPart@@@,
//   the base ctor ...ABUGUID@@...); re-authored on the strmbase hierarchy in naming wave 32 (family dshow).
''' + HIER + CBVR + '''struct CVideoSys;
class CTexture;
enum _D3DFORMAT;
class CTextureRenderer : public CBaseVideoRenderer {
public:
    CVideoSys*  PVideoSys;                                  // +0x160
    CTexture*   Texture;                                    // +0x164
    _D3DFORMAT  TextureFormat;                              // +0x168
    int         Width;                                      // +0x16C
    int         Height;                                     // +0x170
    int         Pitch;                                      // +0x174
    bool        CreateOwnTexture;                           // +0x178
    CTextureRenderer(IUnknown* pUnk, HRESULT* phr, CTexture* texture);
};
extern "C" const GUID CLSID_TextureRenderer;                // 0x0129D150

CTextureRenderer::CTextureRenderer(IUnknown* pUnk, HRESULT* phr, CTexture* texture)
    : CBaseVideoRenderer(CLSID_TextureRenderer, L"Texture Renderer", pUnk, phr), PVideoSys(0), CreateOwnTexture(false)
{
    *phr = 0;
    Texture = texture;
    if (!texture)
        CreateOwnTexture = true;
}
''')
