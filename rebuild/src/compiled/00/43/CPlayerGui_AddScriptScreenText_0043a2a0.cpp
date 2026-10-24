// CPlayerGui::AddScriptScreenText(const CWideString&, const C2DVector&, const CRGBColour&, const CCharString&) -- queue a script text line: convert the position to virtual coordinates snapped to the pixel grid and append it, stamped with the world frame
// Leaf: ?AddScriptScreenText@CPlayerGui@@QAEXABVCWideString@@ABVC2DVector@@ABVCRGBColour@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0043A2A0, length 224 bytes (218 + int3 padding), `ret 0x10` (__fastcall, four references).
// Name: FableWin.pdb `?AddScriptScreenText@CPlayerGui@@QAEXABVCWideString@@ABVC2DVector@@ABVCRGBColour@@ABVCCharString@@@Z`; it sits
// in the CPlayerGui TU (Update 0x0043A080 before, InitialActivate 0x0043A380 after); single caller 0x00892837. The catalog's
// "ShowOnScreenMessage_FormatDisplay" is a label.
// Callees: GFACoordToVCoord(const C2DVector*, C2DVector*) @ 0x009E21D0 (__fastcall), GFRoundVXToNearestPixel(double) @ 0x009E1D00
// and GFRoundVYToNearestPixel(double) @ 0x009E1D50 (landed, __fastcall float), CWorld::GetFrame() @ 0x0049D870 (FableWin
// `?GetFrame@CWorld@@SIJXZ`: returns the static at 0x013B89BC; the catalog / landed name "GFGetPreMainMemoryUsed" is propagated),
// CWideString::CWideString(const CWideString&) @ 0x0099B720, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// malloc @ 0x00BFEA0E (the 0x20-byte list node), NPlayerGui::CScriptScreenText::CScriptScreenText(const CScriptScreenText&)
// @ 0x0043F490 (landed, out of line), ~CCharString @ 0x0099EAE0, ~CWideString @ 0x0099B510.
// Shape: /O2; `ScriptScreenText.push_back(CScriptScreenText(text, vpos, colour, fontName, CWorld::GetFrame()))` -- the five-argument
// constructor (FableWin `??0CScriptScreenText@NPlayerGui@@QAE@ABVCWideString@@ABVC2DVector@@ABVCRGBColour@@ABVCCharString@@K@Z`) is
// inlined into the temporary at [esp+0x10]; its arguments are evaluated first (GetFrame into edi); CRGBColour's copy constructor is
// the landed four-channel Set(R, G, B, A) (the byte permutation); STLport's push_back = insert(end(), x): end() read before the node
// allocation, the placement-new null test, the out-of-line copy constructor, then the four link stores; the temporary's inline
// destructor releases FontName then Text.
// Layout: NPlayerGui::CScriptScreenText (Ego_r.pdb, 0x18): Text +0, VPos +4, Colour +0xC (B,G,R,A bytes), FontName +0x10,
// FrameAdded +0x14 (the landed 0x0043F490 model). CPlayerGui::ScriptScreenText is at retail +0x324; engine/CPlayerGui.h places it at
// the donor's +0x340 (16-byte vectors before it), so a local model.
// Model: local -- engine/CPlayerGui.h and CWorld.h exist, but the generated header is data-only (pack(1), no member functions,
// no virtuals: the vptr and counted pointers are flattened into raw words); this body is a CPlayerGui member function and calls
// CWorld::GetFrame, so both are local (ScriptScreenText +0x324).
#include <stdlib.h>
#include <new>
class CWideString {
public:
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    void* PStringData;
};
class CCharString {
public:
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class C2DVector {
public:
    float X;
    float Y;
};
class CRGBColour {                                       // D3DCOLOR byte order
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
void __fastcall GFACoordToVCoord(const C2DVector* acoord, C2DVector* vcoord);   // 0x009E21D0
float __fastcall GFRoundVXToNearestPixel(double x);     // 0x009E1D00
float __fastcall GFRoundVYToNearestPixel(double y);     // 0x009E1D50
class CWorld {
public:
    static long __fastcall GetFrame();                   // 0x0049D870
};
namespace NPlayerGui {
class CScriptScreenText {                                // 0x18 bytes
public:
    CScriptScreenText(const CWideString& text, const C2DVector& vpos, const CRGBColour& colour, const CCharString& fontName,
                      unsigned long frameAdded)
        : Text(text), VPos(vpos), Colour(colour), FontName(fontName), FrameAdded(frameAdded) {}
    CScriptScreenText(const CScriptScreenText& other);   // 0x0043F490
    CWideString   Text;                                  // +0x00
    C2DVector     VPos;                                  // +0x04
    CRGBColour    Colour;                                // +0x0C
    CCharString   FontName;                              // +0x10
    unsigned long FrameAdded;                            // +0x14
};
}
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> class list {                          // STLport 4.x: the sentinel node pointer
public:
    typedef _List_node<T> _Node;
    _Node* _M_node;
    __forceinline _Node* _M_create_node(const T& x)
    {
        _Node* p = (_Node*)malloc(sizeof(_Node));
        new (&p->_M_data) T(x);
        return p;
    }
    __forceinline _List_node_base* insert(_List_node_base* position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    __forceinline _List_node_base* end() { return _M_node; }
    __forceinline void push_back(const T& x) { insert(end(), x); }
};
}
class CPlayerGui {
public:
    unsigned char                               _pad000[0x324];
    std::list<NPlayerGui::CScriptScreenText>    ScriptScreenText;   // +0x324
    void AddScriptScreenText(const CWideString& text, const C2DVector& pos, const CRGBColour& colour, const CCharString& fontName);
};

void CPlayerGui::AddScriptScreenText(const CWideString& text, const C2DVector& pos, const CRGBColour& colour, const CCharString& fontName)
{
    C2DVector vpos;
    GFACoordToVCoord(&pos, &vpos);
    vpos.X = GFRoundVXToNearestPixel(vpos.X);
    vpos.Y = GFRoundVYToNearestPixel(vpos.Y);
    ScriptScreenText.push_back(NPlayerGui::CScriptScreenText(text, vpos, colour, fontName, CWorld::GetFrame()));
}