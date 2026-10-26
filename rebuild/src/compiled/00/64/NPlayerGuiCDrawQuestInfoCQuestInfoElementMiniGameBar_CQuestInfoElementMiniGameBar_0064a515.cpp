#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar::~CQuestInfoElementMiniGameBar() -- remove every extra element this mini-game bar added to the quest-info display, free the extra-element vector, then destroy the base element's primitive-handle vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A515, length 83 bytes.
// Leaf: ??1CQuestInfoElementMiniGameBar
// Callees: NPlayerGui::CDrawQuestInfo::RemoveElement(long) @ 0x00647357 on the player GUI's quest-info display
// (global @ 0x013B8790, +0x1c8), free @ 0x00BFEA14 (null-checked, `pop ecx`), the out-of-line
// vector<CEnginePrimitiveHandle> destructor @ 0x00440C40 (tail jump on +0x10).
// Vtable 0x0125A46C = NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar (rtti-vtables.tsv).
// Layout (PDB, retail 4 lower after the base's 12-byte handle vector, as in the landed CDrawQuestInfo models):
// base CQuestInfoElement {ID +4, ScriptID +8, Scale +0xc, PrimitiveHandles +0x10}; Type +0x1c, Percentage +0x20,
// Colour +0x24, FadeColour +0x28, Updatable +0x2c, Graphic +0x30, ExtraElements (vector<long>) +0x34.
// Shape: a size-optimised TU (unaligned entry, `pop ecx`); the loop compares an unsigned index against
// `(last - first) >> 2` re-read after every call; the retail vector destructor frees the block when non-null
// (no clearing stores); the base destructor is inline and its only work is the member vector's destructor --
// retail stores no base vtable there, so the base is modelled `__declspec(novtable)` (an inline empty base
// destructor otherwise re-stores 0x0125A2B4 before the tail jump).
extern "C" void __cdecl free(void* block);        // 0x00BFEA14

class CRGBColour { public: unsigned char B, G, R, A; };

namespace NPlayerGui {

struct CRetailHandleVector {                       // VC6 Dinkumware vector<CEnginePrimitiveHandle>: three words
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    ~CRetailHandleVector();                        // 0x00440C40
};

struct CRetailLongVector {                         // vector<long>: three words, destructor inline
    long* m_pFirst;
    long* m_pLast;
    long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    long& operator[](unsigned long index) { return m_pFirst[index]; }
    ~CRetailLongVector() { if (m_pFirst) free(m_pFirst); }
};

class CDrawQuestInfo { public:
    void RemoveElement(long id);                   // 0x00647357
};

struct CPlayerGui {
    unsigned char   _pad[0x1c8];
    CDrawQuestInfo* m_pDrawQuestInfo;              // +0x1c8
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790

struct __declspec(novtable) CQuestInfoElement {    // NPlayerGui::CDrawQuestInfo::CQuestInfoElement (retail 0x1c bytes; vtable 0x0125A2B4 stored by its constructor @ 0x00656795, not modelled)
    virtual ~CQuestInfoElement() { }
    long                ID;                        // +0x04
    long                ScriptID;                  // +0x08
    float               Scale;                     // +0x0c
    CRetailHandleVector PrimitiveHandles;          // +0x10
};

struct CQuestInfoElementMiniGameBar : CQuestInfoElement {   // vtable 0x0125A46C
    long              Type;                        // +0x1c EMiniGameType
    float             Percentage;                  // +0x20
    CRGBColour        Colour;                      // +0x24
    CRGBColour        FadeColour;                  // +0x28
    bool              Updatable;                   // +0x2c
    long              Graphic;                     // +0x30
    CRetailLongVector ExtraElements;               // +0x34
    virtual ~CQuestInfoElementMiniGameBar();
};

CQuestInfoElementMiniGameBar::~CQuestInfoElementMiniGameBar()
{
    for (unsigned long i = 0; i < ExtraElements.size(); ++i)
        g_pPlayerGui->m_pDrawQuestInfo->RemoveElement(ExtraElements[i]);
}

}