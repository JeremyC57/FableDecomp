#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::UpdateBar(long, float, float, float) -- find the element with the given ID and, if it is a bar (type 1), store the value and -- when not negative -- the maximum and minimum
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644D91, length 103 bytes.
// Callees: CQuestInfoElement::GetType (slot 3, virtual).
// Layout: PDB CDrawQuestInfo, retail 4 lower from GuiMode on, Elements +0x58 (see the landed UpdateCounter 0x00644E3C).
// CQuestInfoElementBar (PDB 88 bytes): Value +0x20, MaxValue +0x24, MinValue +0x28 -> retail +0x1c/+0x20/+0x24 (the base's
// handle vector is 12 bytes in the shipped build).
// Shape: as ChangeBarColour 0x00644DF8 (head kept in ecx, null test before the ID compare, the found element leaves the
// loop) but in a frame-pointer size TU (`optimize("y",off)` + `("s",on)`); the float value is copied as a dword, the two
// optional limits are stored only when `>= 0.0f` (`fcomp ; test ah,1 ; jne` skips the store when below zero).
struct CRGBColour { unsigned char R, G, B, A; };

struct CEnginePrimitiveHandle {                    // 8 bytes: id + counted primitive pointer
    long  m_id;
    void* m_pPrimitive;
    void Clear();                                  // 0x0041BE70
};

template <class R, class T> class mem_fun_ref_t {  // SGI functor holding a 4-byte pointer-to-member
public:
    explicit mem_fun_ref_t(R (T::*pfn)()) : m_pfn(pfn) {}
    R operator()(T& object) const { return (object.*m_pfn)(); }
private:
    R (T::*m_pfn)();
};

template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*pfn)())
{
    return mem_fun_ref_t<R, T>(pfn);
}

template <class It, class F> F __fastcall for_each(It first, It last, F f);   // 0x0043F360 (SGI, /Gr)

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words (12 bytes in the shipped build)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
};

struct CQuestInfoElement {                         // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (PDB 32 bytes, retail 0x1c)
    virtual void v00();                            // slot 0
    virtual bool IsActive();                       // slot 1 (+0x04): false once the element has expired (Update removes it)
    virtual void Clear();                          // slot 2 (+0x08): release the element's primitives
    virtual long GetType();                        // slot 3 (+0x0c): 1 bar, 3 counter, 4 counter list, 5 tick, 6 mini-game bar
    long                ID;                        // +0x04
    long                ScriptID;                  // +0x08
    float               Scale;                     // +0x0c
    CRetailHandleVector PrimitiveHandles;          // +0x10
};

struct CCountedElementPointer {                    // CCountedPointer<CQuestInfoElement>: object + count block
    CQuestInfoElement* m_pObject;
    void*              m_pBlock;
};

struct CRetailElementList {                        // VC6 Dinkumware list<CCountedPointer<CQuestInfoElement>>: one sentinel pointer
    struct node { node* next; node* prev; CCountedElementPointer value; };
    struct iterator {
        node* m_pNode;
        explicit iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& o) : m_pNode(o.m_pNode) { }
        CCountedElementPointer& operator*() const { return m_pNode->value; }
        iterator& operator++() { m_pNode = m_pNode->next; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
        bool operator==(const iterator& o) const { return m_pNode == o.m_pNode; }
    };
    node* head;
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    bool empty() const { return head->next == head; }
    iterator erase(iterator where);                // 0x00655EEA
};

struct CDrawQuestInfo {                            // PDB 116 bytes; retail members 4 lower from GuiMode on (the CDrawBase vector is 12 bytes)
    void*               _vtable;                   // +0x00 CDrawBase vptr
    unsigned char       _base04[0x18 - 4];         // CDrawBase: PlayerGui .. Bank
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c (CDrawBase)
    long                GuiMode;                   // +0x28 (EPlayerGuiMode)
    float               FadeInTimer;               // +0x2c
    float               MiniModeTimer;             // +0x30
    bool                MiniMode;                  // +0x34
    long                FramesUntilMiniMode;       // +0x38
    long                MiniGameElementIndex;      // +0x3c
    void*               OracleSymbols;             // +0x40 list<long>: one sentinel pointer
    CRetailHandleVector OraclePrimitiveHandles;    // +0x44
    void*               Name;                      // +0x50 CWideString
    void*               Text;                      // +0x54 CWideString
    CRetailElementList  Elements;                  // +0x58
    void UpdateBar(long id, float value, float maxValue, float minValue);
};

struct CQuestInfoElementBar : CQuestInfoElement {
    float      Value;                              // +0x1c
    float      MaxValue;                           // +0x20
    float      MinValue;                           // +0x24
    CRGBColour Colour;                             // +0x28
    CRGBColour FadeColour;                         // +0x2c
};

void CDrawQuestInfo::UpdateBar(long id, float value, float maxValue, float minValue)
{
    for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) {
        CQuestInfoElement* element = (*it).m_pObject;
        if (element != 0 && element->ID == id) {
            if (element->GetType() == 1) {
                CQuestInfoElementBar* bar = static_cast<CQuestInfoElementBar*>(element);
                bar->Value = value;
                if (maxValue >= 0.0f)
                    bar->MaxValue = maxValue;
                if (minValue >= 0.0f)
                    bar->MinValue = minValue;
            }
            return;
        }
    }
}

}
