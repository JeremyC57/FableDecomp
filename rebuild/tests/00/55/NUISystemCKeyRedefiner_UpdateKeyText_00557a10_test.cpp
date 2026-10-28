// Behaviour test for NUISystem::CKeyRedefiner::UpdateKeyText (0x00557A10): the undefined tag in red, or the upper-cased
// key name in white, handed to the key text (nothing without a key text).
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
static const unsigned short g_tag[] = { 'T','E','X','T','_','G','U','I','_','K','E','Y','_','U','N','D','E','F','I','N','E','D',0 };
static int g_live = 0;
class CWideString { public:
    unsigned short m_text[40];
    CWideString() { m_text[0] = 0; ++g_live; }
    CWideString(const unsigned short* text) { int i = 0; while ((m_text[i] = text[i]) != 0) ++i; ++g_live; }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const CWideString& other) { int i = 0; while ((m_text[i] = other.m_text[i]) != 0) ++i; return *this; }
    operator unsigned short*() const { return (unsigned short*)m_text; }
};
static unsigned short* _wcsupr_(unsigned short* text) { for (unsigned short* p = text; *p; ++p) if (*p >= 'a' && *p <= 'z') *p = (unsigned short)(*p - 32); return text; }
#define _wcsupr _wcsupr_
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CActionInputControl { public:                       // 28 bytes
    long  GameAction;                              // +0x00
    long  ControllerType;                          // +0x04
    long  KeyboardKey;                             // +0x08
    long  XboxButton;                              // +0x0c
    long  MouseButton;                             // +0x10
    float ControlDirection[2];                     // +0x14
};
bool __fastcall GetActionName(const CActionInputControl& control, CWideString& name, bool shortName)
{
    if (!control.KeyboardKey || shortName) return false;
    name.m_text[0] = (unsigned short)control.KeyboardKey; name.m_text[1] = 'x'; name.m_text[2] = 0; return true;
}

namespace NUISystem {
class CText { public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {}
    virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {}
    virtual void v010() {} virtual void v011() {} virtual void v012() {}
    unsigned char m_r, m_g, m_b, m_a; unsigned short m_shown[40]; bool m_isTag, m_b2; int m_calls;
    virtual void SetColour(const CRGBColour& colour) { m_r = colour.R; m_g = colour.G; m_b = colour.B; m_a = colour.A; }
    virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {}
    virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {}
    virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {}
    virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {}
    virtual void v038() {} virtual void v039() {} virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {}
    virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {} virtual void v049() {}
    virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {}
    virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {}
    virtual void v062() {} virtual void v063() {} virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {}
    virtual void v068() {} virtual void v069() {} virtual void v070() {} virtual void v071() {} virtual void v072() {} virtual void v073() {}
    virtual void v074() {} virtual void v075() {} virtual void v076() {} virtual void v077() {} virtual void v078() {} virtual void v079() {}
    virtual void v080() {} virtual void v081() {} virtual void v082() {} virtual void v083() {} virtual void v084() {} virtual void v085() {}
    virtual void v086() {} virtual void v087() {} virtual void v088() {} virtual void v089() {} virtual void v090() {} virtual void v091() {}
    virtual void v092() {} virtual void v093() {} virtual void v094() {} virtual void v095() {} virtual void v096() {} virtual void v097() {}
    virtual void v098() {} virtual void v099() {} virtual void v100() {} virtual void v101() {} virtual void v102() {} virtual void v103() {}
    virtual void v104() {} virtual void v105() {} virtual void v106() {} virtual void v107() {} virtual void v108() {} virtual void v109() {}
    virtual void v110() {} virtual void v111() {} virtual void v112() {} virtual void v113() {} virtual void v114() {} virtual void v115() {}
    virtual void v116() {} virtual void v117() {} virtual void v118() {} virtual void v119() {} virtual void v120() {} virtual void v121() {}
    virtual void v122() {} virtual void v123() {} virtual void v124() {} virtual void v125() {} virtual void v126() {} virtual void v127() {}
    virtual void v128() {} virtual void v129() {} virtual void v130() {} virtual void v131() {} virtual void v132() {} virtual void v133() {}
    virtual void v134() {} virtual void v135() {} virtual void v136() {} virtual void v137() {} virtual void v138() {} virtual void v139() {}
    virtual void v140() {} virtual void v141() {} virtual void v142() {} virtual void v143() {}
    virtual void SetText(const CWideString& text, bool isTag, bool b) { int i = 0; while ((m_shown[i] = text.m_text[i]) != 0) ++i; m_isTag = isTag; m_b2 = b; ++m_calls; }
};
class CKeyRedefiner { public:
    virtual void v00() {}
    unsigned char       _sprite04[0x194];          // +0x04 the CSprite / control bases
    CText*              PKeyText;                  // +0x198
    void*               Actions;                   // +0x19c list<EGameAction> (sentinel)
    long                SubType;                   // +0x1a0
    CActionInputControl Definition;                // +0x1a4
    void UpdateKeyText();
};

void CKeyRedefiner::UpdateKeyText()
{
    bool isTag = true;
    CWideString text(g_tag);
    CRGBColour colour(0xff, 0, 0, 0xff);
    if (Definition.ControllerType) {
        CWideString keyName;
        if (GetActionName(Definition, keyName, false)) {
            isTag = false;
            _wcsupr(keyName);
            text = keyName;
            colour = CRGBColour(0xff, 0xff, 0xff, 0xff);
        }
    }
    if (PKeyText) {
        PKeyText->SetText(text, isTag, true);
        PKeyText->SetColour(colour);
    }
}
}

int main()
{
    using namespace NUISystem;
    static CText txt; txt.m_calls = 0;
    static CKeyRedefiner k; memset(&k.Definition, 0, sizeof(k.Definition)); k.PKeyText = &txt;
    int bad = 0;
    k.UpdateKeyText();
    if (txt.m_calls != 1 || !txt.m_isTag || !txt.m_b2 || txt.m_shown[0] != 'T' || txt.m_shown[21] != 'D' || txt.m_r != 0xff || txt.m_g != 0 || txt.m_b != 0 || txt.m_a != 0xff) bad = 1;
    k.Definition.ControllerType = 1; k.Definition.KeyboardKey = 'q';
    k.UpdateKeyText();
    if (txt.m_calls != 2 || txt.m_isTag || txt.m_shown[0] != 'Q' || txt.m_shown[1] != 'X' || txt.m_shown[2] != 0 || txt.m_g != 0xff || txt.m_b != 0xff) bad = 2;
    k.Definition.KeyboardKey = 0;                    // bound but nameless: the tag again
    k.UpdateKeyText();
    if (!txt.m_isTag || txt.m_shown[0] != 'T' || txt.m_g != 0) bad = 3;
    k.PKeyText = 0; k.UpdateKeyText();
    if (txt.m_calls != 3 || g_live != 0) bad = 4;
    if (bad) { std::printf("00557a10_TEST FAIL\n"); return 1; }
    std::printf("00557a10_TEST PASS\n");
    return 0;
}