#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::AddBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) -- create a bar element with the two symbols' sprites, append it through AddElement and return its new ID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064F7FC, length 137 bytes.
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) @ 0x0064A37B (twice), operator new @ 0x00BFEA1A,
// CQuestInfoElementBar::CQuestInfoElementBar(float, float, const CRGBColour&, const CRGBColour&, long, long, float) @ 0x00656B1B
// (vtable 0x0125A444), CCountedPointer<CQuestInfoElementBar>::CCountedPointer(T*) @ 0x0065707E, the converting copy
// CCountedPointer<CQuestInfoElement>(const CCountedPointer<CQuestInfoElementBar>&) @ 0x00654B36,
// CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement>) @ 0x0064A3ED, ~CCountedPointer<CQuestInfoElementBar> @ 0x00654B0A.
// Layout: CQuestInfoElementBar PDB 88 bytes (retail allocation 0x58): Value +0x1c, MaxValue +0x20, MinValue +0x24, Colour +0x28,
// FadeColour +0x2c, Graphic1 +0x30, Graphic2 +0x34 (retail offsets).
// Shape: as the timer/counter adders (0x0064F93F / 0x0064F9A7): frame-pointer size TU, both sprite lookups in named locals
// (edi, ebx) before the allocation, null-checked placement constructor with three float arguments copied through the x87,
// by-value base pointer converted in the argument area, the ID read through the local before its destructor; `ret 1Ch`.
struct CCharString { char* m_pData; };            // one pointer

extern void* __cdecl operator_new_shim(unsigned int);   // (unused; operator new @ 0x00BFEA1A is the global one)

namespace NPlayerGui {

template <class T> struct CCountedPointer {        // object + count block (8 bytes); bodies out of line in other TUs
    T*    m_pObject;
    void* m_pBlock;
    explicit CCountedPointer(T* object);
    template <class U> CCountedPointer(const CCountedPointer<U>& other);
    ~CCountedPointer();
    T* operator->() const { return m_pObject; }
};

struct CQuestInfoElement {                         // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (retail 0x1c bytes)
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    void* PrimitiveHandles[3];                     // +0x10 vector<CEnginePrimitiveHandle>
};

struct CRGBColour { unsigned char R, G, B, A; };

struct CQuestInfoElementBar : CQuestInfoElement {  // retail 0x58 bytes, vtable 0x0125A444
    float      Value;                              // +0x1c
    float      MaxValue;                           // +0x20
    float      MinValue;                           // +0x24
    CRGBColour Colour;                             // +0x28
    CRGBColour FadeColour;                         // +0x2c
    long       Graphic1;                           // +0x30
    long       Graphic2;                           // +0x34
    unsigned char _rest[0x58 - 0x38];              // ThingValid, PThing, ThingUID, ThingMapNumber
    CQuestInfoElementBar(float value, float maxValue, const CRGBColour& colour, const CRGBColour& fadeColour,
                         long graphic1, long graphic2, float scale);   // 0x00656B1B
};

struct CDrawQuestInfo {
    long GetScriptSpriteBySymbol(const CCharString& symbol);           // 0x0064A37B
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddBar(float value, float maxValue, const CRGBColour& colour, const CRGBColour& fadeColour,
                const CCharString& symbol1, const CCharString& symbol2, float scale);
};

long CDrawQuestInfo::AddBar(float value, float maxValue, const CRGBColour& colour, const CRGBColour& fadeColour,
                            const CCharString& symbol1, const CCharString& symbol2, float scale)
{
    long graphic1 = GetScriptSpriteBySymbol(symbol1);
    long graphic2 = GetScriptSpriteBySymbol(symbol2);
    CCountedPointer<CQuestInfoElementBar> element(new CQuestInfoElementBar(value, maxValue, colour, fadeColour, graphic1, graphic2, scale));
    AddElement(element);
    return element->ID;
}

}
