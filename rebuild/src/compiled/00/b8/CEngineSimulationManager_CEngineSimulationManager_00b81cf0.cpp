// CEngineSimulationManager::~CEngineSimulationManager() -- store both vtables, clear and free the three simulation-info lists in reverse order, then the CEngineComponent base destructor and the CBase destructor (tail jump)
// Leaf: ??1CEngineSimulationManager
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