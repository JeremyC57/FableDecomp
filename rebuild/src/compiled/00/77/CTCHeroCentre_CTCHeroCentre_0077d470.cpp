// CTCHeroCentre::CTCHeroCentre(CThing*) -- CTCBase(thing), the hero-centre vtable, a null def pointer, GUI mode / quest-card frame zeroed, two empty vectors (QuestCards, QuestCardPositions) and the tutorial-wait flag cleared
// Leaf: ??0CTCHeroCentre@@QAE@PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077D470, length 57 bytes (+ int3 padding). Vtable 0x0126AF94 = CTCHeroCentre.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800.
// Shape: one `xor eax,eax` after the base call, the vtable store, then ten dword stores +0xc..+0x2c and the byte +0x30 in
// declaration order. The def pointer must be a CDefPointer MEMBER OBJECT with an inline null constructor (like the two
// vectors): written as a plain `CDefPointeeBase*` with a `PDef_Object(0)` initialiser it is scheduled differently and the
// body grades DIFFER(57v57) (the first agent's attempts: plain pointer, plain pointer + body stores, vectors as plain records).
// Local model: engine/CTCHeroCentre.h carries the Ego_r.pdb debug-STL layout (vectors of 0x10 bytes: QuestCardPositions +0x28,
// WaitingForTutorialToFinish +0x38); the retail release vectors are 0xc bytes, so QuestCardPositions is at +0x24 and the flag
// at +0x30 (the byte store above). engine/CTCBase.h is a flattened POD, so the polymorphic CTCBase is modelled locally too.
class CThing;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase(CThing* thing);                                 // 0x00686800
};

class CDefPointeeBase;
struct CDefPointer {                                        // CDefPointer<CHeroCentreDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};
struct CThingRef;                                           // CIntelligentPointer<CThing> element (only pointers are stored here)
class C3DVector { public: float X, Y, Z; };

template<class T> struct CRetailVector {                    // std::vector<T>, VC7.1 release: first / last / end
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

enum EGUIMode { GUI_MODE_0 };

class CTCHeroCentre : public CTCBase { public:                            // vtable 0x0126AF94
    CDefPointer PDef;                                       // +0x0c
    EGUIMode CurrentGUIMode;                                // +0x10
    unsigned long FrameQuestCardListFrom;                   // +0x14
    CRetailVector<CThingRef> QuestCards;                    // +0x18
    CRetailVector<C3DVector> QuestCardPositions;            // +0x24
    bool WaitingForTutorialToFinish;                        // +0x30
    virtual void v00();
    CTCHeroCentre(CThing* thing);
};

CTCHeroCentre::CTCHeroCentre(CThing* thing)
    : CTCBase(thing), CurrentGUIMode(GUI_MODE_0), FrameQuestCardListFrom(0), WaitingForTutorialToFinish(false)
{
}