#ifndef FABLE_RETAIL_IMEUI_SDK_H
#define FABLE_RETAIL_IMEUI_SDK_H
// Layout: original ImeUi.h and Ego_r IMEUI_APPEARANCE64 / IMEUI_VERTEX32.
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef const unsigned short* LPCTSTR;
struct tagRECT { long left,top,right,bottom; };
typedef tagRECT RECT;
struct HKL__;
typedef HKL__* HKL;
class CImeUiFont_Base
{
public:
    virtual void    SetHeight( UINT uHeight )
    {
        (void)uHeight;
    }; // for backward compatibility
    virtual void    SetColor( DWORD color ) = 0;
    virtual void    SetPosition( int x, int y ) = 0;
    virtual void    GetTextExtent( LPCTSTR szText, DWORD* puWidth, DWORD* puHeight ) = 0;
    virtual void    DrawText( LPCTSTR pszText ) = 0;
};

typedef struct
{
    // symbol (Henkan-kyu)
    DWORD symbolColor;
    DWORD symbolColorOff;
    DWORD symbolColorText;
    BYTE symbolHeight;
    BYTE symbolTranslucence;
    BYTE symbolPlacement;
    CImeUiFont_Base* symbolFont;

    // candidate list
    DWORD candColorBase;
    DWORD candColorBorder;
    DWORD candColorText;

    // composition string
    DWORD compColorInput;
    DWORD compColorTargetConv;
    DWORD compColorConverted;
    DWORD compColorTargetNotConv;
    DWORD compColorInputErr;
    BYTE compTranslucence;
    DWORD compColorText;

    // caret
    BYTE caretWidth;
    BYTE caretYMargin;
}               IMEUI_APPEARANCE;

typedef struct	// D3DTLVERTEX compatible
{
    float sx;
    float sy;
    float sz;
    float rhw;
    DWORD color;
    DWORD specular;
    float tu;
    float tv;
}               IMEUI_VERTEX;


// Original ImeUi.cpp private layout, native g_CaretInfo +0x14 is margins.
struct CompStringAttribute {
 UINT caretX,caretY;CImeUiFont_Base* pFont;DWORD colorComp,colorCand;RECT margins;
};
#define IMEUI_STATE_OFF 0
#define IMEUI_STATE_ON 1
#define LANG_KOREAN 0x12
#endif
