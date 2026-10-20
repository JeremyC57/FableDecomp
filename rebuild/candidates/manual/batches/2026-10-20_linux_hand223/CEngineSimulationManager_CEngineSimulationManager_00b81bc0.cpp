// CEngineSimulationManager::CEngineSimulationManager() -- CBase and CEngineComponent bases, the three empty simulation-info lists, Perlin noise seeded with 0, and the default wind / gravity / cloth / radius settings
// Leaf: ??0CEngineSimulationManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B81BC0, length 294 bytes (+ int3 padding), plain `ret`.
// Label correction: catalogued as CGuiControlNumberInit (propagated); the vtables it stores, 0x012A3984 at +0 and
//   0x012A394C at +4, are CEngineSimulationManager's (RTTI .?AVCEngineSimulationManager@@) -- the landed destructor
//   0x00B81CF0 stores the same pair.
// Callees: CBase::CBase @ 0x0099A2F0, CEngineComponent::CEngineComponent @ 0x00B59710, malloc @ 0x00BFEA0E (each STLport
//   list's head node), CPerlinNoise::CPerlinNoise(unsigned long seed) @ 0x00F36BE0 (catalog label is propagated).
// Layout: Ego_r.pdb CEngineSimulationManager with the retail 4-byte STLport lists (as the landed destructor):
//   lists +0x10/+0x14/+0x18, NextSimStepOffset +0x1c, CurrentSimStep +0x1d, MaxIterations +0x20, MaxDistanceError +0x24,
//   PerlinNoise +0x28 (0x1000), WindNoiseTime +0x1028 .. ClothGravityFactor +0x1044, GravityVector +0x1048, WindVector
//   +0x1054, SimRadius +0x1060, SimFadeBegin +0x1064, Zoom +0x1068, the three *StepsPending +0x106c..+0x1074.
// Shape: STLport list ctor (node word zeroed, node linked to itself through a local, then stored); the two vectors are
//   assigned from C3DVector temporaries (their dead frame stores survive at the end, as retail); the body order is
//   the retail store order with the five immediate stores moved ahead of the vectors (which pins the gravity temp's
//   dead X/Y stores after them).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};
struct CEngineComponent {                          // 0x0c bytes
    virtual void v01();
    void* m_pOwner;
    long  m_8;
    CEngineComponent();                            // 0x00B59710
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CPerlinNoise {
    unsigned long IndexTable[256];                 // +0x000
    C3DVector     VectorTable[256];                // +0x400
    CPerlinNoise(unsigned long seed);              // 0x00F36BE0
};

struct CEnginePrimitiveSimulationInfo;
template <class T> struct CIVCountedPointer { T* Object; };
template <class T>
struct CRetailList {                               // STLport list<T>: the head node pointer
    struct node { node* _M_next; node* _M_prev; T _M_data; };
    node* _M_node;
    CRetailList() : _M_node(0)
    {
        node* head = (node*)malloc(sizeof(node));
        head->_M_next = head;
        head->_M_prev = head;
        _M_node = head;
    }
};

struct CEngineSimulationManager : CBase, CEngineComponent {   // vtables 0x012A3984 / 0x012A394C
    CRetailList<CIVCountedPointer<CEnginePrimitiveSimulationInfo> > FullUpdateOnlySimulationInfoList;   // +0x10
    CRetailList<CIVCountedPointer<CEnginePrimitiveSimulationInfo> > AlwaysUpdateSimulationInfoList;     // +0x14
    CRetailList<CIVCountedPointer<CEnginePrimitiveSimulationInfo> > PausedSimulationInfoList;           // +0x18
    unsigned char NextSimStepOffset;               // +0x1c
    unsigned char CurrentSimStep;                  // +0x1d
    long          MaxIterations;                   // +0x20
    float         MaxDistanceError;                // +0x24
    CPerlinNoise  PerlinNoise;                     // +0x28
    float         WindNoiseTime;                   // +0x1028
    float         WindNoiseOffset;                 // +0x102c
    float         WindNoiseSpeed;                  // +0x1030
    float         WindNoisePerpendicular;          // +0x1034
    float         MinWindMultiplier;               // +0x1038
    float         MaxWindMultiplier;               // +0x103c
    float         ClothWindFactor;                 // +0x1040
    float         ClothGravityFactor;              // +0x1044
    C3DVector     GravityVector;                   // +0x1048
    C3DVector     WindVector;                      // +0x1054
    float         SimRadius;                       // +0x1060
    float         SimFadeBegin;                    // +0x1064
    float         Zoom;                            // +0x1068
    unsigned long FullUpdateOnlyStepsPending;      // +0x106c
    unsigned long AlwaysUpdateStepsPending;        // +0x1070
    unsigned long PausedStepsPending;              // +0x1074
    virtual void v00();
    virtual void v01();
    CEngineSimulationManager();
};

CEngineSimulationManager::CEngineSimulationManager()
    : PerlinNoise(0)
{
    MaxDistanceError = 5.0f;
    SimRadius = 5.0f;
    MaxWindMultiplier = 1.0f;
    ClothWindFactor = 1.0f;
    ClothGravityFactor = 1.0f;
    Zoom = 1.0f;
    MaxIterations = 5;
    SimFadeBegin = 0.85f;
    WindNoiseSpeed = 0.2f;
    WindNoisePerpendicular = 0.75f;
    MinWindMultiplier = -0.5f;
    GravityVector = C3DVector(0.0f, 0.0f, -982.0f);
    WindVector = C3DVector(0.0f, 0.0f, 0.0f);
    NextSimStepOffset = 0;
    CurrentSimStep = 0;
    FullUpdateOnlyStepsPending = 0;
    AlwaysUpdateStepsPending = 0;
    PausedStepsPending = 0;
}
