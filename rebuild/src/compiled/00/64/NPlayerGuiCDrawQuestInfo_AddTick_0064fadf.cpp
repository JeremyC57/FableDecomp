// NPlayerGui::CDrawQuestInfo::AddTick(const CCharString& sprite, bool ticked, float scale) -- add a CQuestInfoElementTick for the script sprite to the quest-info panel and return its element ID
// Leaf: ?AddTick@CDrawQuestInfo@NPlayerGui@@QAEJABVCCharString@@_NM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064FADF, 104 bytes, thiscall, `ret 0Ch`, ebp frame.
// Identity (proven): Ego_r.pdb ?AddTick@CDrawQuestInfo@NPlayerGui@@QAEJABVCCharString@@_NM@Z; it news a 0x28-byte CQuestInfoElementTick
//   (ctor 0x00656C3C, Ego_r.pdb ??0CQuestInfoElementTick@CDrawQuestInfo@NPlayerGui@@QAE@J_NM@Z) and holds it in CCountedPointer<CQuestInfoElementTick>
//   (0x00657176); its caller is CGameScriptInterface::AddQuestInfoTick 0x00891B60 (tail jump). The manifest's "CDrawQuestInfo::AddCounter" is propagated across byte-identical bodies.
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) 0x0064A37B, operator new 0x00BFEA1A, the element ctor
//   0x00656C3C, CCountedPointer<CQuestInfoElementTick>(CQuestInfoElementTick*) 0x00657176, the converting CCountedPointer<CQuestInfoElement>(const
//   CCountedPointer<CQuestInfoElementTick>&) 0x00654C4A (built in the by-value argument slot), CDrawQuestInfo::AddElement 0x0064A3ED,
//   ~CCountedPointer<CQuestInfoElementTick> 0x00654C1E.
// Shape (family M177, representative 0x0064FA0F): the ID is read through the counted pointer before its destructor.
//   `optimize("y", off)` + `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString;
void* __cdecl operator new(unsigned int size);            // 0x00BFEA1A
template <class T> class CCountedPointer {
public:
    CCountedPointer(T* p);                                // out of line (one per T)
    template <class U> CCountedPointer(const CCountedPointer<U>& other);   // converting copy, out of line
    ~CCountedPointer();
    T* operator->() const { return Data; }
    T* Data;                                              // +0x00
    void* Info;                                           // +0x04
};
namespace NPlayerGui {
class CDrawQuestInfo {
public:
    class CQuestInfoElement {
    public:
        virtual ~CQuestInfoElement();
        long ID;                                          // +0x04
        long ScriptID;                                    // +0x08
        float Scale;                                      // +0x0c
        unsigned long PrimitiveHandles[3];                // +0x10 vector<CEnginePrimitiveHandle>
    };
    class CQuestInfoElementTick : public CQuestInfoElement {
    public:
        CQuestInfoElementTick(long sprite, bool ticked, float scale);   // 0x00656C3C
        unsigned char _body[0x28 - 0x1c];
    };
    long GetScriptSpriteBySymbol(const CCharString& symbol);   // 0x0064A37B
    long AddTick(const CCharString& sprite, bool ticked, float scale);
private:
    void AddElement(CCountedPointer<CQuestInfoElement> element);   // 0x0064A3ED
};
long CDrawQuestInfo::AddTick(const CCharString& sprite, bool ticked, float scale)
{
    long spriteIndex = GetScriptSpriteBySymbol(sprite);
    CCountedPointer<CQuestInfoElementTick> element(new CQuestInfoElementTick(spriteIndex, ticked, scale));
    AddElement(element);
    return element->ID;
}
}