// CQuestInfoElementBar::CQuestInfoElementBar(float maxValue, float minValue, const CRGBColour& colour, const CRGBColour& fadeColour, long graphic1, long graphic2, float scale) -- a quest-info bar with no thing attached: value starts full, colours/graphics from the arguments, thing pointer/UID/map cleared
// Leaf: ??0CQuestInfoElementBar
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00656B1B, length 135 bytes, `ret 1Ch`.
// Callees: CQuestInfoElement::CQuestInfoElement(float scale) @ 0x00656795 (catalog label "CVsyncCallbackTimerInfo" is
// propagated), CIntelligentPointer<const CThingPhysical>::CIntelligentPointer(CThing*) @ 0x00655145 (with 0).
// Vtable 0x0125A444. Layout: PDB CQuestInfoElementBar less 4 (the retail STLport vector of the base is 12 bytes); PThing
// is byte-aligned at +0x39 (its own class is packed) while the 64-bit ThingUID keeps its 8-byte alignment (+0x48).
// Shape: size TU (`optimize("s", on)` + `("y", off)`: ebp frame, `fld ; push ecx ; fstp [esp]` for the float base
// argument); `Value(maxValue), MaxValue(maxValue)` gives `fst ; fstp`; each colour is the CRGBColour copy ctor calling
// an inline Set(r, g, b, a) (all four bytes loaded before any store); one zero register for the null thing.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CRGBColour { public:                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class CThing;
class CThingPhysical { public:
    unsigned char      _pad00[0x18];
    unsigned __int64   m_UID;                      // +0x18 (the thing's 64-bit UID)
    unsigned char      _pad20[0x8e - 0x20];
    short              m_MapNumber;                // +0x8e
};
#pragma pack(push, 1)
struct CIntelligentPointer_CThingPhysical {        // CIntelligentPointer<const CThingPhysical> (8 bytes, byte-aligned in retail)
    void* __vftable;
    void* m_pItem;
    CIntelligentPointer_CThingPhysical(CThing* thing);   // 0x00655145
};
#pragma pack(pop)
struct CEnginePrimitiveHandleVector {             // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
};
struct CQuestInfoElement {                         // 0x1c bytes in retail
    virtual void v00();
    long  ID;                                      // +0x04
    long  ScriptID;                                // +0x08
    float Scale;                                   // +0x0c
    CEnginePrimitiveHandleVector PrimitiveHandles; // +0x10
    CQuestInfoElement(float scale);                // 0x00656795 (catalog label "CVsyncCallbackTimerInfo" is propagated)
};
struct CQuestInfoElementBar : CQuestInfoElement {  // vtable 0x0125A444
    float         Value;                           // +0x1c
    float         MaxValue;                        // +0x20
    float         MinValue;                        // +0x24
    CRGBColour    Colour;                          // +0x28
    CRGBColour    FadeColour;                      // +0x2c
    long          Graphic1;                        // +0x30
    long          Graphic2;                        // +0x34
    bool          ThingValid;                      // +0x38
    CIntelligentPointer_CThingPhysical PThing;     // +0x39
    unsigned __int64 ThingUID;                     // +0x48 (8-aligned)
    long          ThingMapNumber;                  // +0x50
    CQuestInfoElementBar(CThingPhysical* thing, const CRGBColour& colour, long graphic, float scale);
    CQuestInfoElementBar(float maxValue, float minValue, const CRGBColour& colour, const CRGBColour& fadeColour,
                         long graphic1, long graphic2, float scale);
};
CQuestInfoElementBar::CQuestInfoElementBar(float maxValue, float minValue, const CRGBColour& colour, const CRGBColour& fadeColour,
                                           long graphic1, long graphic2, float scale)
    : CQuestInfoElement(scale), Value(maxValue), MaxValue(maxValue), MinValue(minValue), Colour(colour), FadeColour(fadeColour),
      Graphic1(graphic1), Graphic2(graphic2), ThingValid(false), PThing(0), ThingUID(0), ThingMapNumber(0)
{
}