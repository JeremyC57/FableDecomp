#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::AddTimer(const CTimer*, const CCharString&, float) -- create a timer element with the symbol's sprite, append it through AddElement and return its new ID
// Leaf: AddTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064F93F, length 104 bytes (catalog label "AddCounter": the body builds a
// 0x24-byte CQuestInfoElementTimer, vtable 0x0125A458, so this is the timer adder; the counter twin is 0x0064F9A7).
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) @ 0x0064A37B, operator new @ 0x00BFEA1A,
// CQuestInfoElementTimer::CQuestInfoElementTimer(const CTimer*, long, float) @ 0x0064A4E9,
// CCountedPointer<CQuestInfoElementTimer>::CCountedPointer(T*) @ 0x006570BC, the converting copy
// CCountedPointer<CQuestInfoElement>(const CCountedPointer<CQuestInfoElementTimer>&) @ 0x00654B7B,
// CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement>) @ 0x0064A3ED, ~CCountedPointer<CQuestInfoElementTimer> @ 0x00654B4F.
// Shape: frame-pointer size TU (`optimize("y",off)` + `("s",on)`, `pop ecx` after operator new); the sprite lookup runs
// before the allocation (a named local), the null-checked placement constructor feeds the counted-pointer local at
// [ebp-8], the by-value base-class counted pointer is converted straight into the argument area (`push ecx ; push ecx ;
// mov ecx,esp`), and the element ID is read through the local before its destructor runs.
class CCharString { public: char* m_pData; };            // one pointer

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

struct CTimer;

struct CQuestInfoElementTimer : CQuestInfoElement {   // retail 0x24 bytes, vtable 0x0125A458
    long TimerIndex;                               // +0x1c
    long Graphic;                                  // +0x20
    CQuestInfoElementTimer(const CTimer* timer, long graphic, float scale);   // 0x0064A4E9
};

class CDrawQuestInfo { public:
    long GetScriptSpriteBySymbol(const CCharString& symbol);           // 0x0064A37B
    void AddElement(CCountedPointer<CQuestInfoElement> element);       // 0x0064A3ED
    long AddTimer(const CTimer* timer, const CCharString& symbol, float scale);
};

long CDrawQuestInfo::AddTimer(const CTimer* timer, const CCharString& symbol, float scale)
{
    long graphic = GetScriptSpriteBySymbol(symbol);
    CCountedPointer<CQuestInfoElementTimer> element(new CQuestInfoElementTimer(timer, graphic, scale));
    AddElement(element);
    return element->ID;
}

}