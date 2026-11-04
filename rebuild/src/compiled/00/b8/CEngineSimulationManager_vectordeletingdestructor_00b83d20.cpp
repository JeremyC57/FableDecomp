// CEngineSimulationManager::`vector deleting destructor'(unsigned int) `adjustor{4}' -- CEngineComponent-subobject vtable entry: this -= 0x4, jump to the deleting destructor 0x00B83FA0
// Leaf: ??_ECEngineSimulationManager@@W3AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B83D20, 3+5 bytes: `sub ecx,0x4 ; jmp 0x00B83FA0`. Compiler-generated adjustor thunk.
// Identity (two sources): (1) slot 0 of the CEngineComponent (+4) vtable 0x012A394C (RTTI complete-object locator offset 0x4, class
//   .?AVCEngineSimulationManager@@; the catalog's "vfunc_0"); (2) its jump target 0x00B83FA0 is slot 0 of the primary vtable 0x012A3954, the class's
//   ??_G. VC7.1 names the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the adjustor in the
//   thunk name (??_ECEngineSimulationManager@@W3AEPAXI@Z); objdump -t of this object shows the leaf.
// TU: the thunk is emitted with the vtable in any TU that defines the class's constructor or destructor (chunk 765 shape). This
//   source reuses the landed definer CEngineSimulationManager::~CEngineSimulationManager 0x00B81CF0 (landed destructor, verbatim) as that TU;
//   only the thunk is graded here. Landed definer: rebuild/src/compiled/00/b8/CEngineSimulationManager_CEngineSimulationManager_00b81cf0.cpp
// Callees: ??_GCEngineSimulationManager@@UAEPAXI@Z 0x00B83FA0 (bound as its ??_E alias).
// ---- definer TU (verbatim from the landed row, minus its Leaf/Flags lines) ----
// CEngineSimulationManager::~CEngineSimulationManager() -- store both vtables, clear and free the three simulation-info lists in reverse order, then the CEngineComponent base destructor and the CBase destructor (tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81CF0, length 112 bytes.
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); the only caller
// is the scalar deleting destructor @ 0x00B83FA0, slot 0 of CEngineSimulationManager's vtable 0x012A3984 (stored at +0;
// 0x012A394C is the same class's CEngineComponent-subobject vtable at +4).
// Callees: list<CIVCountedPointer<CEnginePrimitiveSimulationInfo>>::clear @ 0x00B836E0 (x3; catalogued as the propagated
// "CNavigatorQueueManager::FrameUpdate"), free @ 0x00BFEA14 (x3), CEngineComponent::~CEngineComponent @ 0x00B59730,
// CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300 (tail jump).
// Layout: Ego_r.pdb CEngineSimulationManager with the retail 4-byte STLport list (node pointer only):
// FullUpdateOnlySimulationInfoList +0x10, AlwaysUpdateSimulationInfoList +0x14, PausedSimulationInfoList +0x18.
// Shape: two bases (CBase at +0, CEngineComponent at +4, 0x0c bytes); each list's inline destructor is
// `clear(); if (node) free(node)`, run in reverse declaration order; then the second base's destructor as a call and the
// first base's as the tail jump.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};
class CEngineComponent { public:                          // 0x0c bytes
    virtual void v01();
    void* m_pOwner;
    long  m_8;
    virtual ~CEngineComponent();                           // 0x00B59730
};

class CEnginePrimitiveSimulationInfo;
template <class T> class CIVCountedPointer { public: T* Object; };
template <class T>
struct CRetailList {                               // STLport list<T>: the head node pointer
    struct node;
    node* _M_node;
    void clear();                                  // 0x00B836E0 for T = CIVCountedPointer<CEnginePrimitiveSimulationInfo>
    ~CRetailList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};

class CEngineSimulationManager : public CBaseClassNonCopyable, public CEngineComponent { public:   // vtables 0x012A3984 / 0x012A394C
    CRetailList<CIVCountedPointer<CEnginePrimitiveSimulationInfo> > FullUpdateOnlySimulationInfoList;   // +0x10
    CRetailList<CIVCountedPointer<CEnginePrimitiveSimulationInfo> > AlwaysUpdateSimulationInfoList;     // +0x14
    CRetailList<CIVCountedPointer<CEnginePrimitiveSimulationInfo> > PausedSimulationInfoList;           // +0x18
    virtual void v01();
    ~CEngineSimulationManager();
};

CEngineSimulationManager::~CEngineSimulationManager()
{
}