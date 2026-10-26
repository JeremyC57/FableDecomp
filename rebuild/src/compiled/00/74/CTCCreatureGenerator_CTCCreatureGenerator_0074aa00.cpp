// CTCCreatureGenerator::CTCCreatureGenerator(CThing* thing) -- construct the creature-generator component: the CTCBase part, empty creature-family array / created-creature list / triggerer and position vectors, active and ready flags set, unlimited trigger count, no def, a null random-particle pointer, then the generation defaults (radii 1.0, no limits) and the nav-check flag
// Leaf: ??0CTCCreatureGenerator
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074AA00, 248 bytes (+ int3 padding), `ret 4`.
// Label: the catalog's "CPSCOrbit::CPSCOrbit" is propagated; the body stores vtable 0x01268C24 = CTCCreatureGenerator
//   (RTTI; OnCreate 0x00749EA0 is its slot 4) and its only caller is the component factory 0x004D37A8.
// Callees: CTCBase::CTCBase(CThing*) 0x00686800, malloc 0x00BFEA0E (the list sentinel), CCharString() 0x0099E4B0
//   (GenParams.ScriptNameOfAllGeneratedCreatures), CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 (then the
//   CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout (retail, Ego_r.pdb less the container deltas: CArray/vectors 12 bytes, list 4 bytes; as the landed OnCreate):
//   CreatureFamilies +0x0c, CreatedCreatures +0x18, CreatureTriggerers +0x1c, GenParams +0x28 (GenerationRadius,
//   SelfTriggerRadius, SelfTrigger, SelfTriggerResetInterval +0x34, TriggerOnActivate +0x38, ActiveCreatureLimit +0x3c,
//   TotalGenerationLimit +0x40, NumTriggers +0x44, ScriptName +0x48), Active +0x4c .. WaitForAllCreaturesDead +0x50,
//   NumTriggersLeft +0x54, ActivationFrame +0x58, LastSpaceTest +0x5c, OpenPositions +0x60, HiddenPositions +0x6c,
//   DebugPrimitives +0x78, CreationCount +0x84, UndeadTriggered +0x88, Destroyed +0x89, PDef +0x8c, PRandomParticle +0x90,
//   FramesUntilRandomEffect +0x98, FramesUntilRandomSoundEffect +0x9c, NeedToDoNavCheck +0xa0, DoNavCheckOnGeneration +0xa1,
//   PExclusionArea +0xa4. engine/CTCCreatureGenerator.h carries the PDB offsets, so the class is modelled locally.
// Shape: STLport containers through their real bases (alloc proxies from allocator temporaries); the scalars stored after
//   the intelligent pointer are ctor-body assignments.
#include <stddef.h>
extern "C" void* __cdecl malloc(size_t size);          // 0x00BFEA0E
class CThing;
class CTCShapeManager;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
};
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item = 0) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> struct CAllocator {                  // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> { // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                 // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct SCreatureNode {                                  // _List_node<CIntelligentPointer<CThing> >
    SCreatureNode* _M_next;
    SCreatureNode* _M_prev;
    unsigned char  _M_data[8];
};
struct CCreatureList {                                  // STLport list (retail: the node pointer only)
    CAllocProxy<SCreatureNode> _M_node;
    CCreatureList() : _M_node(CAllocator<SCreatureNode>(), 0)
    {
        SCreatureNode* node = (SCreatureNode*)malloc(sizeof(SCreatureNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node.m_data = node;
    }
};
class CDefIndex { public: long Index; };
template <class T> class CArray { public:                      // retail CArray<T> (12 bytes)
    T*   Data;
    long Size;
    long Capacity;
    CArray() : Data(0), Size(0), Capacity(0) { }
};
class C3DVector { public: float X, Y, Z; };
class CEnginePrimitiveHandle { public: void* __vftable; void* Data; };
class CThingCreatureBase;
struct TGenerationParameters {
    float       GenerationRadius;                       // +0x00
    float       SelfTriggerRadius;                      // +0x04
    bool        SelfTrigger;                            // +0x08
    long        SelfTriggerResetInterval;               // +0x0c
    bool        TriggerOnActivate;                      // +0x10
    long        ActiveCreatureLimit;                    // +0x14
    long        TotalGenerationLimit;                   // +0x18
    long        NumTriggers;                            // +0x1c
    CCharString ScriptNameOfAllGeneratedCreatures;      // +0x20
};
class CCreatureGeneratorDef;
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};

class CTCCreatureGenerator : public CTCBase { public:                 // vtable 0x01268C24
    CArray<CDefIndex>                                   CreatureFamilies;      // +0x0c
    CCreatureList                                       CreatedCreatures;      // +0x18
    CRetailVector<CIntelligentPointer<CThingCreatureBase> > CreatureTriggerers;  // +0x1c
    TGenerationParameters                               GenParams;             // +0x28
    bool                                                Active;                // +0x4c
    bool                                                Initialised;           // +0x4d
    bool                                                ReadyToInitialise;     // +0x4e
    bool                                                ExternalTrigger;       // +0x4f
    bool                                                WaitForAllCreaturesDead;   // +0x50
    long                                                NumTriggersLeft;       // +0x54
    long                                                ActivationFrame;       // +0x58
    bool                                                LastSpaceTest;         // +0x5c
    CRetailVector<C3DVector>                            OpenPositions;         // +0x60
    CRetailVector<C3DVector>                            HiddenPositions;       // +0x6c
    CRetailVector<CEnginePrimitiveHandle>               DebugPrimitives;       // +0x78
    long                                                CreationCount;         // +0x84
    bool                                                UndeadTriggered;       // +0x88
    bool                                                Destroyed;             // +0x89
    CDefPointer<const CCreatureGeneratorDef>            PDef;                  // +0x8c
    CIntelligentPointer<CThing>                         PRandomParticle;       // +0x90
    long                                                FramesUntilRandomEffect;        // +0x98
    long                                                FramesUntilRandomSoundEffect;   // +0x9c
    bool                                                NeedToDoNavCheck;      // +0xa0
    bool                                                DoNavCheckOnGeneration;   // +0xa1
    CTCShapeManager*                                    PExclusionArea;        // +0xa4
    virtual void v00();
    CTCCreatureGenerator(CThing* thing);
};

CTCCreatureGenerator::CTCCreatureGenerator(CThing* thing)
    : CTCBase(thing), Active(true), Initialised(false), ReadyToInitialise(true), ExternalTrigger(false),
      WaitForAllCreaturesDead(true), NumTriggersLeft(-1), ActivationFrame(0), LastSpaceTest(false),
      CreationCount(0), UndeadTriggered(false), Destroyed(false)
{
    GenParams.GenerationRadius = 1.0f;
    GenParams.SelfTriggerRadius = 1.0f;
    FramesUntilRandomEffect = 0;
    FramesUntilRandomSoundEffect = 0;
    DoNavCheckOnGeneration = false;
    PExclusionArea = 0;
    GenParams.TriggerOnActivate = false;
    GenParams.SelfTriggerResetInterval = 0;
    GenParams.NumTriggers = -1;
    GenParams.ActiveCreatureLimit = -1;
    GenParams.TotalGenerationLimit = -1;
    NeedToDoNavCheck = true;
}