#define FABLE_RETAIL_CWIDESTRING
// CFontBank::GetStringWidth(const CWideString&) const @ 0x0048DA62
// Leaf: ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z
// The shared inline definition is emitted by the 00541A00 shipping object.
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_ICONTEXT_FIT_PASS30
#include "engine/CFontBank.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static const wchar_t* requestedText;
static const CWideString* requestedObject;
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}
const wchar_t* CWideString::GetAsCharArray() const
{
    assert(this == requestedObject);
    return requestedText;
}
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CFontBank::~CFontBank() {}
bool CFontBank::Preload(const CWideString&, bool) const { assert(false); return false; }

class MetricFont : public CFontBank {
public:
    MetricFont() : calls(0) {}
    virtual C2DExtentsF GetStringExtents(const wchar_t* text, bool flag) const {
        assert(text == requestedText && !flag);
        ++calls;
        return C2DExtentsF(*text ? 13.5f : 0.0f, 77.0f);
    }
    virtual float GetFontMaxHeight() const { assert(false); return 0; }
    virtual void PrepareToRender(bool) const { assert(false); }
    virtual void Render(const CWideString&, const C3DVector&, float, const CRGBColour&, unsigned long, const CTexture*) const { assert(false); }
    mutable unsigned calls;
};
int main()
{
    MetricFont font;
    CWideString empty, filled;
    requestedText = L"";
    requestedObject = &empty;
    assert(font.GetStringWidth(empty) == 0.0f);
    requestedText = L"abc";
    requestedObject = &filled;
    assert(font.GetStringWidth(filled) == 13.5f);
    assert(font.calls == 2);
    puts("0048da62_TEST PASS");
    return 0;
}
