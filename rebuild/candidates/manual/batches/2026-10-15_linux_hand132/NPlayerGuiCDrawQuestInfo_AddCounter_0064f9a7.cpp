#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::AddCounter(CCounter*, const CCharString&, float) -- create a counter element with the symbol's sprite, append it through AddElement and return its new ID
// Leaf: AddCounter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064F9A7, length 104 bytes (twin of the timer adder 0x0064F93F).
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) @ 0x0064A37B, operator new @ 0x00BFEA1A,
// CQuestInfoElementCounter::CQuestInfoElementCounter(CCounter*, long, float) @ 0x00656BA2 (vtable 0x0125A4F0),
// CCountedPointer<CQuestInfoElementCounter>::CCountedPointer(T*) @ 0x006570FA, the converting copy
// CCountedPointer<CQuestInfoElement>(const CCountedPointer<CQuestInfoElementCounter>&) @ 0x00654BC0,
// CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement>) @ 0x0064A3ED, ~CCountedPointer<CQuestInfoElementCounter> @ 0x00654B94.
// Layout: CQuestInfoElementCounter PDB 48 bytes -> retail 0x2c (PCounter +0x1c, Graphic +0x20, Value +0x24, MaxValue +0x28).
// Shape: as the timer adder: frame-pointer size TU, sprite lookup in a named local before the allocation, null-checked
// placement constructor into the counted-pointer local, by-value base pointer converted in the argument area, the ID read
// through the local before its destructor.
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

struct CCounter;

struct CQuestInfoElementCounter : CQuestInfoElement {   // retail 0x2c bytes, vtable 0x0125A4F0
    CCounter* PCounter;                            // +0x1c
    long      Graphic;                             // +0x20
    long      Value;                               // +0x24
    long      MaxValue;                            // +0x28
    CQuestInfoElementCounter(CCounter* counter, long graphic, float scale);   // 0x00656BA2
};

struct CDrawQuestInfo {
    long GetScriptSpriteBySymbol(const CCharString& symbol);           // 0x0064A37B
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddCounter(CCounter* counter, const CCharString& symbol, float scale);
};

long CDrawQuestInfo::AddCounter(CCounter* counter, const CCharString& symbol, float scale)
{
    long graphic = GetScriptSpriteBySymbol(symbol);
    CCountedPointer<CQuestInfoElementCounter> element(new CQuestInfoElementCounter(counter, graphic, scale));
    AddElement(element);
    return element->ID;
}

}
