#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::GetMiniGameType() -- the mini-game type of the element whose ID is the current mini-game element index, if that index is set and the element is a mini-game bar (type 6); else 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644D54, length 61 bytes.
// Callees: CQuestInfoElement::GetType (slot 3, virtual).
// Layout: PDB CDrawQuestInfo (116 bytes): GuiMode +0x2c, FadeInTimer +0x30, MiniModeTimer +0x34, MiniMode +0x38, FramesUntilMiniMode +0x3c,
// MiniGameElementIndex +0x40, OracleSymbols +0x44, OraclePrimitiveHandles +0x50, Name +0x60, Text +0x64, Elements +0x68 -- retail 4 lower up to
// the oracle list and 0x10 lower for Elements (+0x58): the CDrawBase vector, the list and the vector are 12/4/12 bytes in the shipped build.
// The element list holds CCountedPointer<CQuestInfoElement> values (object at node +8, block at +0xc); the elements are polymorphic with
// GetType() in slot 3. Derived layouts (PDB, retail 4 lower after the base's handle vector): Bar Colour/FadeColour +0x28/+0x2c, ThingValid +0x38,
// PThing +0x39; Counter Value/MaxValue +0x24/+0x28; CounterList Value/MaxValue +0x20/+0x24; Tick Ticked +0x24; MiniGameBar Type +0x1c, Colour +0x24, Updatable +0x2c.
// Shape: the index test guards the whole walk; the node is compared with the head register on entry and re-read from the member after the virtual call; `e->ID == MiniGameElementIndex` reads the ID first (source operand order); the type is returned from the derived member at +0x1c through a shared `pop` tail.
struct CRGBColour { unsigned char R, G, B, A; };

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words (12 bytes in the shipped build)
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
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

struct CDrawQuestInfo {                            // PDB 116 bytes; retail members 4 lower from GuiMode on (the CDrawBase vector is 12 bytes), see the header comment
    unsigned char       _pad[0x28];                // CDrawBase
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
    void DisplayQuestInfo(bool display);           // 0x00643F13
    void RemoveElement(long id);                   // 0x00647357
    long GetMiniGameType();
};
struct CQuestInfoElementMiniGameBar : CQuestInfoElement {
    long       Type;                               // +0x1c (EMiniGameType)
    float      Percentage;                         // +0x20
    CRGBColour Colour;                             // +0x24
    CRGBColour FadeColour;                         // +0x28
    bool       Updatable;                          // +0x2c
};

long CDrawQuestInfo::GetMiniGameType()
{
    if (MiniGameElementIndex > 0) {
        for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) {
            CQuestInfoElement* element = (*it).m_pObject;
            if (element->ID == MiniGameElementIndex && element->GetType() == 6)
                return static_cast<CQuestInfoElementMiniGameBar*>(element)->Type;
        }
    }
    return 0;

}

}