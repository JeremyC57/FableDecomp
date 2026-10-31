#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
// NPlayerGui::CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement>) -- tag the new element with the running quest's ID, give it the next free element ID, scale it down and append it to the element list
// Grade: DIFFER(99v99) (sec2b: moved onto engine/CCountedPointer.h, bytes unchanged) -- register assignment only: retail keeps the argument object in ebx, the maximum in edi and the loop id in esi; ours edi/esi/ebx (declaration-order and expression variants tried)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A3ED, length 99 bytes.
// Callees: list<CCountedPointer<CQuestInfoElement>>::push_back(const T&) @ 0x00656197 (manifest "ListPushFrontWrapper":
// it inserts before the head sentinel, i.e. at the end), CCountedPointer<CQuestInfoElement>::~CCountedPointer @ 0x0043E1C0
// (out of line, destroys the by-value argument). Globals: the quest manager @ 0x013B89FC; its +0x88 word is
// PQuestCurrentlyRunning (PDB +228, the retail lists are 8 bytes narrower), whose first member is the quest ID.
// Layout: PDB CDrawQuestInfo, retail Elements +0x58; CQuestInfoElement ID +4, ScriptID +8, Scale +0xc.
// Shape: size TU without a frame; the running quest is read before the argument's object pointer (ebx), the walk keeps
// the running maximum in edi (`cmp ; jle ; mov`), the new ID is `max + 1`, `Scale *= 0.7f` (literal, `fld ; fmul [pool]`),
// the push_back receives the argument by reference (`lea eax,[esp+0Ch]`) and ecx still holds &Elements from the walk.
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


struct CRetailElementList {                        // VC6 Dinkumware list<CCountedPointer<CQuestInfoElement>>: one sentinel pointer
    struct node { node* next; node* prev; CCountedPointer<CQuestInfoElement> value; };
    struct iterator {
        node* m_pNode;
        explicit iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& o) : m_pNode(o.m_pNode) { }
        CCountedPointer<CQuestInfoElement>& operator*() const { return m_pNode->value; }
        iterator& operator++() { m_pNode = m_pNode->next; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
        bool operator==(const iterator& o) const { return m_pNode == o.m_pNode; }
    };
    node* head;
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    bool empty() const { return head->next == head; }
    iterator erase(iterator where);                // 0x00655EEA
    void push_back(const CCountedPointer<CQuestInfoElement>& value);   // 0x00656197
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
    void AddElement(CCountedPointer<CQuestInfoElement> element);
};

struct CActiveQuest {                              // CQuestManager::CActiveQuest (PDB 56 bytes)
    long ID;                                       // +0x00
};

struct CQuestManager {
    unsigned char _pad[0x88];
    CActiveQuest* PQuestCurrentlyRunning;          // +0x88 (PDB +228)
};

}

extern NPlayerGui::CQuestManager* g_pQuestManager; // 0x013B89FC

namespace NPlayerGui {

void CDrawQuestInfo::AddElement(CCountedPointer<CQuestInfoElement> element)
{
    CActiveQuest* quest = g_pQuestManager->PQuestCurrentlyRunning;
    CQuestInfoElement* object = element.Data;
    if (quest != 0)
        object->ScriptID = quest->ID;
    long maxID = 0;
    for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) {
        long id = (*it).Data->ID;
        if (id > maxID)
            maxID = id;
    }
    object->ID = maxID + 1;
    object->Scale *= 0.7f;
    Elements.push_back(element);
}

}
