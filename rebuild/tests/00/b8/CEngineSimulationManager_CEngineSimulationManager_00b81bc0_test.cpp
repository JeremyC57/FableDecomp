#include <cstdio>
#include <cstdlib>

static int g_bases, g_components, g_seed = -1, g_nodes;
struct CBase { virtual void v00() {} CBase() { ++g_bases; } };
class CEngineComponent { public: virtual void v01() {} void* m_pOwner; long m_8; CEngineComponent() : m_pOwner(0), m_8(0) { ++g_components; } };
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CPerlinNoise { unsigned long IndexTable[256]; C3DVector VectorTable[256]; CPerlinNoise(unsigned long seed) { g_seed = (int)seed; } };
template <class T> struct CRetailList {
    struct node { node* _M_next; node* _M_prev; T _M_data; };
    node* _M_node;
    CRetailList() : _M_node(0)
    {
        node* head = (node*)std::malloc(sizeof(node)); ++g_nodes;
        head->_M_next = head; head->_M_prev = head; _M_node = head;
    }
    ~CRetailList() { std::free(_M_node); }
};
class CEngineSimulationManager : public CBase, public CEngineComponent { public:
    CRetailList<void*> FullUpdateOnlySimulationInfoList, AlwaysUpdateSimulationInfoList, PausedSimulationInfoList;
    unsigned char NextSimStepOffset, CurrentSimStep; long MaxIterations; float MaxDistanceError;
    CPerlinNoise PerlinNoise;
    float WindNoiseTime, WindNoiseOffset, WindNoiseSpeed, WindNoisePerpendicular, MinWindMultiplier, MaxWindMultiplier, ClothWindFactor, ClothGravityFactor;
    C3DVector GravityVector, WindVector;
    float SimRadius, SimFadeBegin, Zoom;
    unsigned long FullUpdateOnlyStepsPending, AlwaysUpdateStepsPending, PausedStepsPending;
    CEngineSimulationManager()
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
};
static bool EmptyList(const CRetailList<void*>& l) { return l._M_node && l._M_node->_M_next == l._M_node && l._M_node->_M_prev == l._M_node; }
int main()
{
    CEngineSimulationManager* m = new CEngineSimulationManager;
    bool ok = g_bases == 1 && g_components == 1 && g_seed == 0 && g_nodes == 3
        && EmptyList(m->FullUpdateOnlySimulationInfoList) && EmptyList(m->AlwaysUpdateSimulationInfoList) && EmptyList(m->PausedSimulationInfoList)
        && m->FullUpdateOnlySimulationInfoList._M_node != m->PausedSimulationInfoList._M_node
        && m->NextSimStepOffset == 0 && m->CurrentSimStep == 0 && m->MaxIterations == 5 && m->MaxDistanceError == 5.0f
        && m->WindNoiseSpeed == 0.2f && m->WindNoisePerpendicular == 0.75f && m->MinWindMultiplier == -0.5f
        && m->MaxWindMultiplier == 1.0f && m->ClothWindFactor == 1.0f && m->ClothGravityFactor == 1.0f
        && m->GravityVector.X == 0.0f && m->GravityVector.Y == 0.0f && m->GravityVector.Z == -982.0f
        && m->WindVector.X == 0.0f && m->WindVector.Y == 0.0f && m->WindVector.Z == 0.0f
        && m->SimRadius == 5.0f && m->SimFadeBegin == 0.85f && m->Zoom == 1.0f
        && m->FullUpdateOnlyStepsPending == 0 && m->AlwaysUpdateStepsPending == 0 && m->PausedStepsPending == 0;
    delete m;
    std::printf(ok ? "00b81bc0_TEST PASS\n" : "00b81bc0_TEST FAIL\n");
    return ok ? 0 : 1;
}