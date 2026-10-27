// NPlayerGui::CDrawQuestInfo::AddCounter(const CCharString& sprite, long count, float scale) -- add a CQuestInfoElementCounter for the script sprite to the quest-info panel and return its element ID
// Leaf: ?AddCounter@CDrawQuestInfo@NPlayerGui@@QAEJABVCCharString@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064FA0F, 104 bytes, thiscall, `ret 0Ch`, ebp frame.
// Identity (proven): Ego_r.pdb ?AddCounter@CDrawQuestInfo@NPlayerGui@@QAEJABVCCharString@@JM@Z; it news a 0x2C-byte CQuestInfoElementCounter
//   (ctor 0x00656BD8, Ego_r.pdb ??0CQuestInfoElementCounter@CDrawQuestInfo@NPlayerGui@@QAE@JJM@Z) and holds it in CCountedPointer<CQuestInfoElementCounter>
//   (0x006570FA); its caller is CGameScriptInterface::AddQuestInfoCounter 0x00891B40 (tail jump). The manifest's "CDrawQuestInfo::AddCounter" is propagated across byte-identical bodies.
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) 0x0064A37B, operator new 0x00BFEA1A, the element ctor
//   0x00656BD8, CCountedPointer<CQuestInfoElementCounter>(CQuestInfoElementCounter*) 0x006570FA, the converting CCountedPointer<CQuestInfoElement>(const
//   CCountedPointer<CQuestInfoElementCounter>&) 0x00654BC0 (built in the by-value argument slot), CDrawQuestInfo::AddElement 0x0064A3ED,
//   ~CCountedPointer<CQuestInfoElementCounter> 0x00654B94.
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
    class CQuestInfoElementCounter : public CQuestInfoElement {
    public:
        CQuestInfoElementCounter(long sprite, long count, float scale);   // 0x00656BD8
        unsigned char _body[0x2C - 0x1c];
    };
    long GetScriptSpriteBySymbol(const CCharString& symbol);   // 0x0064A37B
    long AddCounter(const CCharString& sprite, long count, float scale);
private:
    void AddElement(CCountedPointer<CQuestInfoElement> element);   // 0x0064A3ED
};
long CDrawQuestInfo::AddCounter(const CCharString& sprite, long count, float scale)
{
    long spriteIndex = GetScriptSpriteBySymbol(sprite);
    CCountedPointer<CQuestInfoElementCounter> element(new CQuestInfoElementCounter(spriteIndex, count, scale));
    AddElement(element);
    return element->ID;
}
}