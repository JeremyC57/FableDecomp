// CTCHeroCentre::~CTCHeroCentre() -- hero-centre thing component destructor: frees the quest-card position array, destroys and frees the quest-card intelligent pointers, releases the hero-centre def, then the CTCBase destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077EE70, length 97 bytes (tail jump). Called by the scalar deleting destructor
// @ 0x0077EE50, which is slot 0 of vtable 0x0126AF94 (rtti-vtables.tsv: CTCHeroCentre).
// Label: the manifest's "CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase" is propagated and wrong.
// Callees: free @ 0x00BFEA14 (both vectors' storage), each CIntelligentPointer's virtual destructor (slot 0 with 0), the def's
// virtual slot 1 (Destroy at zero count), CTCBase::~CTCBase @ 0x00686830 (out of line: stores the CTCBase vtable 0x0125BE0C, tail
// jumps to the base; manifest "CChunkedFileChunk").
// Layout (PDB CTCHeroCentre, 60 bytes; retail 12-byte STLport vectors): PDef CDefPointer<const CHeroCentreDef> +0x0C, CurrentGUIMode
// +0x10, FrameQuestCardListFrom +0x14, QuestCards vector<CIntelligentPointer<CThing>> +0x18 (8-byte elements: vptr + item),
// QuestCardPositions vector<C3DVector> +0x24, WaitingForTutorialToFinish +0x30. engine/CTCHeroCentre.h keeps the containers opaque, so
// the class is modelled locally with real member types.
// Shape: no vtable store of its own -- the model has no virtual function (the CTCBase vptr is a plain word, its destructor out of
// line); the element loop is STLport's `_Destroy(first, last)` taking both ends by value (finish cached in ebx), the storage
// release `if (start) free(start)`, and the def release `if (p) { p->Release(); p = 0; }` (all `__forceinline`).
// Leaf: ??1CTCHeroCentre
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CThing;

class CIntelligentPointer {                        // CIntelligentPointer<CThing>: 8 bytes (vptr + item)
public:
    virtual ~CIntelligentPointer();
    void* m_pItem;
};

struct C3DVector {
    float X, Y, Z;
};

template <class T>
inline void DestroyRange(T* first, T* last)        // STLport _Destroy(first, last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T>
struct CRetailVector {                             // STLport vector: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    __forceinline ~CRetailVector()
    {
        DestroyRange(m_pStart, m_pFinish);
        if (m_pStart)
            free(m_pStart);
    }
};

template <>
struct CRetailVector<C3DVector> {
    C3DVector* m_pStart;
    C3DVector* m_pFinish;
    C3DVector* m_pEndOfStorage;
    __forceinline ~CRetailVector()
    {
        if (m_pStart)
            free(m_pStart);
    }
};

class CHeroCentreDef {
public:
    virtual void v000();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

struct CDefPointer {                               // CDefPointer<const CHeroCentreDef>
    CHeroCentreDef* m_pDef;
    __forceinline ~CDefPointer()
    {
        if (m_pDef) {
            m_pDef->Release();
            m_pDef = 0;
        }
    }
};

class CTCBase {                                    // engine/CTCBase.h: vptr, ParentThing +4, flags +8
public:
    void*   m_vtable;
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    ~CTCBase();                                    // 0x00686830
};

class CTCHeroCentre : public CTCBase {
public:
    CDefPointer                        PDef;                       // +0x0c
    long                               CurrentGUIMode;             // +0x10 EGUIMode
    unsigned long                      FrameQuestCardListFrom;     // +0x14
    CRetailVector<CIntelligentPointer> QuestCards;                 // +0x18
    CRetailVector<C3DVector>           QuestCardPositions;         // +0x24
    bool                               WaitingForTutorialToFinish; // +0x30
    ~CTCHeroCentre();
};

CTCHeroCentre::~CTCHeroCentre()
{
}