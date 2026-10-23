// Grade: DIFFER(553v547) -- register allocation: retail keeps the default font in edi from the text position
//   to the end and spills the timer seconds to the dead `pos` argument slot ([ebp+8], later the colour); ours keeps
//   `pos` in edi through the graphic block (box `push edi` instead of `push [ebp+8]`), the seconds in edi and spills
//   the font, which also shifts the local slots (width -8 v -0xC, box -0x44 v -0x3C). The colour shape (A and R
//   hoisted, G/B per branch, one object at [ebp+8]) is reproduced by a forceinline if/return helper.
//   Tried: ternary of two colours as the argument (two temps), per-channel ternary (setcc), default-then-override,
//   copy-initialised / const-ref ternary local (extra copy), if/else assignment (extra copy), non-forceinline helper.
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info) -- draw the timer's graphic (when it has one) as a sprite sized by the bank graphic times the element scale, then the timer's remaining seconds as "mm:ss" beside it in the default font (pinkish red at ten seconds or less), vertically centred on the graphic; returns the drawn height (graphic height * scale + 2, else the font height * scale)
// Leaf: DrawAt
// Retail 0x006507D5, 547 bytes, size-optimised TU (#pragma optimize("s"): ebp frame, `leave`, unaligned entry), `ret 8`.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90, the quest-info graphic bank (g_pPlayerGui 0x013B8790
// +0x1c8 draw-quest-info +0x14; vtable slots 21/22 width/height), C2DBoxF(const C2DVector&, const C2DVector&) 0x0041A7E0,
// _ftol 0x00BFEA70, CEnginePrimitive2DSprite 0x0041BEB0 (box overload), renderer AddPrimitive slot 23, GFACoordToVCoord
// 0x009E21D0 (__fastcall), the timer source (g_pMainGameComponent 0x013B86A0 +0x24 +0x38, vtable slot 90: seconds left
// for a timer index), CWideString() 0x0099AED0, the varargs CWideString::Format 0x0099BA70 (L"%02d:%02d" 0x0125A694),
// ~CWideString 0x0099B510, CPlayerGui::Draw2DText 0x00438010.
// Pool: 2^32 0x0122DCB4, 255.0f 0x01230014, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 2.0f 0x0122DCDC.
// Layout: retail CQuestInfoElement is the PDB less 4 (12-byte handle vector): Scale +0x0c, PrimitiveHandles +0x10;
// CQuestInfoElementTimer TimerIndex +0x1c, Graphic +0x20.
#pragma optimize("s", on)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float Left, Top, Right, Bottom;
    C2DBoxF(const C2DVector& topLeft, const C2DVector& size);   // 0x0041A7E0
};
struct C2DSpriteBox {
    float Left, Top, Right, Bottom;
    C2DSpriteBox(const C2DBoxF& box) : Left(box.Left), Top(box.Top), Right(box.Right), Bottom(box.Bottom) {}
};
void __fastcall GFACoordToVCoord(const C2DVector* in, C2DVector* out);   // 0x009E21D0
class CWideString {
public:
    void* m_pStringData;
    ~CWideString();                                 // 0x0099B510
    CWideString();                                  // 0x0099AED0
    void __cdecl Format(const unsigned short* format, ...);   // 0x0099BA70
};
class CFontBank {
public:
    void* __vftable;
    long  Height;                                   // +0x04
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_QUEST_INFO = 0x4a };
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DSpriteBox& box, unsigned long layer, unsigned long bank,
                             unsigned long frame, unsigned long blend, unsigned long interpolation, float angleStart,
                             float angleEnd, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long param, const C2DVector& uvStart, const C2DVector& uvEnd);   // 0x0041BEB0
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo {
public:
    float                     FrameTime;            // +0x00
    unsigned char             _pad04[0x30];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
class CGraphicBank {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20();
    virtual unsigned long GetGraphicWidth(long graphic, long frame);    // slot 21
    virtual unsigned long GetGraphicHeight(long graphic, long frame);   // slot 22
};
struct CDrawQuestInfoBase {
    unsigned char _pad[0x14];
    CGraphicBank* Bank;                             // +0x14
};
struct CPlayerGui {
    unsigned char       _pad000[0x1c8];
    CDrawQuestInfoBase* DrawQuestInfo;              // +0x1c8
    unsigned char       _pad1cc[0x2a0 - 0x1cc];
    float               Alpha;                      // +0x2a0
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790
class CTimerSource {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual long GetTimerSeconds(long timerIndex);  // slot 90
};
struct CGameWorld {
    unsigned char _pad[0x38];
    CTimerSource* Timers;                           // +0x38
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CGameWorld*   World;                            // +0x24
    unsigned char _pad28[0x1614c - 0x28];
    CFontBank*    DefaultFont;                      // +0x1614c
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0

namespace NPlayerGui {
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    void Resize(long count);                        // 0x00443F90
};
struct CDrawQuestInfo {
    struct CQuestInfoElement {
        virtual ~CQuestInfoElement();
        virtual void v01();
        virtual void Update();                      // slot 2
        long                ID;                     // +0x04
        long                ScriptID;               // +0x08
        float               Scale;                  // +0x0c
        CRetailHandleVector PrimitiveHandles;       // +0x10
    };
    struct CQuestInfoElementTimer : CQuestInfoElement {
        long TimerIndex;                            // +0x1c
        long Graphic;                               // +0x20
        static __forceinline CRGBColour GetTimerColour(long seconds)
        {
            if (seconds < 11)
                return CRGBColour(0xff, 100, 100, 0xff);
            return CRGBColour(0xff, 0xff, 0xff, 0xff);
        }
        float DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info);
    };
};

float CDrawQuestInfo::CQuestInfoElementTimer::DrawAt(const C2DVector& pos, CEnginePrimitivePrepareInfo* info)
{
    float width = 0.0f;
    float height = 0.0f;
    if (Graphic > 0) {
        PrimitiveHandles.Resize(1);
        CGraphicBank* bank = g_pPlayerGui->DrawQuestInfo->Bank;
        width = (float)bank->GetGraphicWidth(Graphic, 0);
        height = (float)bank->GetGraphicHeight(Graphic, 0);
        C2DBoxF box(pos, C2DVector(width * Scale, height * Scale));
        CEnginePrimitive2DSprite sprite(0, box, ENGINE_2D_LAYER_QUEST_INFO, Graphic, 0, 2, 0, 0.0f, 0.0f,
                                        (unsigned char)(g_pPlayerGui->Alpha * 255.0f), 0xff, 0xff, 0xff, 2,
                                        C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info->Renderer->AddPrimitive(PrimitiveHandles.m_pFirst, sprite, 0xc0);
    }
    CFontBank* font = g_pMainGameComponent->DefaultFont;
    C2DVector textPos(pos.X + width * Scale, pos.Y);
    textPos.Y += (height - (float)font->Height) * 0.5f;
    C2DVector screen;
    GFACoordToVCoord(&textPos, &screen);
    long seconds = g_pMainGameComponent->World->Timers->GetTimerSeconds(TimerIndex);
    CWideString text;
    text.Format(L"%02d:%02d", seconds / 60, seconds % 60);
    g_pPlayerGui->Draw2DText(screen, text, font, GetTimerColour(seconds), Scale, ENGINE_2D_LAYER_QUEST_INFO, true);
    if (height > 0.0f)
        return height * Scale + 2.0f;
    return (float)font->Height * Scale;
}
}
