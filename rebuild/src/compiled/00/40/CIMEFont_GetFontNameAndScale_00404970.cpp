// CIMEFont::GetFontNameAndScale(CCharString&,float&) const -- select Arial atlas and scale by unsigned pixel height
// Retail 00404970, 226 bytes. Native PDB Height +16; true const method (fablewin PDB018bcc20).
// Calls CCharString::operator=(const char*) 0099EFE0; narrow append 0099F100.
// Actual fable_string.h has no narrow += declaration: field-free NativeCharString adapter
// declares the missing nonvirtual member, with identical storage and zero this adjustment.
// PDB ??YCCharString@@QAEABV0@PBD@Z confirms const CCharString& result.
#include "fable_string.h"
struct NativeCharString : CCharString {
    const CCharString& Append(const char*); // 0x0099F100
};
FABLE_STATIC_ASSERT(sizeof(NativeCharString)==sizeof(CCharString));
// Native RTTI0122DCBC: CIMEFont derives CImeUiFont_Base at0 (donor size4).
// Base has exactly five genuine virtual interface slots, no virtual destructor:
// native slot0 SetHeight,1 SetColor,2 SetPosition,3 GetTextExtent,4 DrawTextW.
class CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int)=0;
    virtual void SetColor(unsigned long)=0;
    virtual void SetPosition(int,int)=0;
    virtual void GetTextExtent(const wchar_t*,unsigned long*,unsigned long*)=0;
    virtual void DrawTextW(const wchar_t*)=0;
};
namespace NUISystem {
class CIMEFont : public CImeUiFont_Base {
public:
    virtual void SetHeight(unsigned int);
    virtual void SetColor(unsigned long);
    virtual void SetPosition(int,int);
    virtual void GetTextExtent(const wchar_t*,unsigned long*,unsigned long*);
    virtual void DrawTextW(const wchar_t*);
    fable_u32 Colour;
    float Position[2];
    fable_u32 Height;
    void GetFontNameAndScale(CCharString&,float&) const;
};
void CIMEFont::GetFontNameAndScale(CCharString& name,float& scale) const {
    float height = (float)Height;
    name = "ENG_ARIAL_";
    if (Height < 16) {
        static_cast<NativeCharString&>(name).Append("12");
        scale = height * (1.0f/12.0f);
    } else if (Height < 18) {
        static_cast<NativeCharString&>(name).Append("16");
        scale = height * (1.0f/16.0f);
    } else if (Height < 24) {
        static_cast<NativeCharString&>(name).Append("18");
        scale = height * (1.0f/18.0f);
    } else if (Height < 28) {
        static_cast<NativeCharString&>(name).Append("24");
        scale = height * (1.0f/24.0f);
    } else {
        static_cast<NativeCharString&>(name).Append("28");
        scale = height * (1.0f/28.0f);
    }
}
}