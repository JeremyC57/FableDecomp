// NPlayerGui::CDrawQuestInfo::ClearPrimitivesIfNecessary() -- when not drawing this frame, tell every quest-info element to release its primitives and clear the oracle-symbol handles; then the base drawer's clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644D0E, 70 bytes. Virtual slot 0 of NPlayerGui::CDrawQuestInfo's vtable
// 0x0125A688; FableWin.pdb lists ?ClearPrimitivesIfNecessary@CDrawQuestInfo@NPlayerGui@@UAEXXZ.
// Callees: CQuestInfoElement::Clear (virtual slot 2, +0x08), SGI for_each<CEnginePrimitiveHandle*, mem_fun_ref_t> @ 0x0043F360
// with CEnginePrimitiveHandle::Clear @ 0x0041BE70, NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary @ 0x00644627 (called
// non-virtually). Same for_each / base spelling as the landed CDrawQuickAccessIcons::ClearPrimitivesIfNecessary 0x0064D62F.
// Layout: as the landed CDrawQuestInfo::UpdateBar 0x00644D91: Drawing +0x18, OraclePrimitiveHandles +0x44, Elements +0x58
// (VC6 list<CCountedPointer<CQuestInfoElement> >: one sentinel pointer; the node value's object pointer at +8).
// Shape: /Os TU without a frame pointer (`optimize("s",on)`): the list walk re-reads the sentinel from the member at every
// test, each element null-tested before its virtual Clear; the for_each result sits in the `push ecx` frame word.
#pragma optimize("s", on)
class CEnginePrimitiveHandle { public:                    // 8 bytes: vptr + counted primitive pointer
    void* _vtable;
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

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* begin() { return m_pFirst; }
    CEnginePrimitiveHandle* end() { return m_pLast; }
};

class CDrawBase { public:
    virtual void ClearPrimitivesIfNecessary();     // slot 0: 0x00644627
    unsigned char       _base04[0x18 - 4];         // PlayerGui .. Bank
    bool                Drawing;                   // +0x18
    CRetailHandleVector PrimitiveHandles;          // +0x1c
};

class CDrawQuestInfo : public CDrawBase { public:                // retail members 4 lower than the PDB from GuiMode on
    class CQuestInfoElement { public:                     // retail 0x1c bytes
        virtual void v00();                        // slot 0
        virtual bool IsActive();                   // slot 1
        virtual void Clear();                      // slot 2
        virtual long GetType();                    // slot 3
        long                ID;                    // +0x04
        long                ScriptID;              // +0x08
        float               Scale;                 // +0x0c
        CRetailHandleVector PrimitiveHandles;      // +0x10
    };
    struct CCountedElementPointer {                // CCountedPointer<CQuestInfoElement>: object + count block
        CQuestInfoElement* m_pObject;
        void*              m_pBlock;
    };
    struct CRetailElementList {                    // VC6 Dinkumware list: one sentinel pointer
        struct node { node* next; node* prev; CCountedElementPointer value; };
        struct iterator {
            node* m_pNode;
            explicit iterator(node* n) : m_pNode(n) { }
            iterator(const iterator& o) : m_pNode(o.m_pNode) { }
            CCountedElementPointer& operator*() const { return m_pNode->value; }
            iterator& operator++() { m_pNode = m_pNode->next; return *this; }
            bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
        };
        node* head;
        iterator begin() { return iterator(head->next); }
        iterator end() { return iterator(head); }
    };
    long                GuiMode;                   // +0x28
    float               FadeInTimer;               // +0x2c
    float               MiniModeTimer;             // +0x30
    bool                MiniMode;                  // +0x34
    long                FramesUntilMiniMode;       // +0x38
    long                MiniGameElementIndex;      // +0x3c
    void*               OracleSymbols;             // +0x40
    CRetailHandleVector OraclePrimitiveHandles;    // +0x44
    void*               Name;                      // +0x50
    void*               Text;                      // +0x54
    CRetailElementList  Elements;                  // +0x58
    virtual void ClearPrimitivesIfNecessary();
};

void CDrawQuestInfo::ClearPrimitivesIfNecessary()
{
    if (!Drawing) {
        for (CRetailElementList::iterator it = Elements.begin(); it != Elements.end(); ++it) {
            CQuestInfoElement* element = (*it).m_pObject;
            if (element)
                element->Clear();
        }
        for_each(OraclePrimitiveHandles.begin(), OraclePrimitiveHandles.end(), mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    }
    CDrawBase::ClearPrimitivesIfNecessary();
}

}