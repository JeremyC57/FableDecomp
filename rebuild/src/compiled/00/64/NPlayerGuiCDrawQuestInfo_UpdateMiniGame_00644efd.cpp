#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::UpdateMiniGame(float, long, long) -- clamp the percentage to 1, find the active mini-game element and, if it is an updatable mini-game bar, store the percentage and forward the counter values to its single extra element
// Leaf: ?UpdateMiniGame@CDrawQuestInfo@NPlayerGui@@QAEXMJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644EFD, length 136 bytes.
// Callees: CQuestInfoElement::GetType (slot 3, virtual), CDrawQuestInfo::UpdateCounter(long, long, long) @ 0x00644E3C.
// The sibling overload @ 0x00644F85 takes a colour.
// Layout: PDB CDrawQuestInfo, retail 4 lower from GuiMode on: MiniGameElementIndex +0x3c, Elements +0x58 (see the landed
// UpdateCounter 0x00644E3C). CQuestInfoElementMiniGameBar (PDB 72 bytes, retail 4 lower after the base's handle vector):
// Type +0x1c, Percentage +0x20, Colour +0x24, FadeColour +0x28, Updatable +0x2c, Graphic +0x30, ExtraElements +0x34
// (vector<long>, three words).
// Shape: frame-pointer size TU; the clamp is the Dinkumware `min(percentage, 1.0f)` through const references (the 1.0f
// temporary lives in the frame slot, `fld1 ; fcomp [arg] ; test ah,5 ; jnp` selects the address, the result is written back
// into the argument slot); the index is cached in edx for the walk, the found element leaves the loop; `size() == 1` is
// folded by the compiler into `(last - first) & ~3 == 4`; the single extra element is read as `*(begin() + 0)`.
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
    void UpdateCounter(long id, long value, long maxValue);   // 0x00644E3C
    void UpdateMiniGame(float percentage, long value, long maxValue);
};

struct CRetailLongVector {                         // VC6 Dinkumware vector<long>: three words
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    long* begin() { return m_pFirst; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    long& operator[](unsigned long n) { return *(begin() + n); }
};

struct CQuestInfoElementMiniGameBar : CQuestInfoElement {
    long              Type;                        // +0x1c (EMiniGameType)
    float             Percentage;                  // +0x20
    CRGBColour        Colour;                      // +0x24
    CRGBColour        FadeColour;                  // +0x28
    bool              Updatable;                   // +0x2c
    long              Graphic;                     // +0x30
    CRetailLongVector ExtraElements;               // +0x34
};

template <class T> inline const T& Min(const T& left, const T& right)
{
    return right < left ? right : left;
}

void CDrawQuestInfo::UpdateMiniGame(float percentage, long value, long maxValue)
{
    long index = MiniGameElementIndex;
    if (index > 0) {
        percentage = Min(percentage, 1.0f);
        for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) {
            CQuestInfoElement* element = (*it).m_pObject;
            if (element->ID == index) {
                if (element->GetType() == 6) {
                    CQuestInfoElementMiniGameBar* bar = static_cast<CQuestInfoElementMiniGameBar*>(element);
                    if (bar->Updatable) {
                        bar->Percentage = percentage;
                        if (bar->Type == 6 && bar->ExtraElements.size() == 1)
                            UpdateCounter(bar->ExtraElements[0], value, maxValue);
                    }
                }
                return;
            }
        }
    }
}

}