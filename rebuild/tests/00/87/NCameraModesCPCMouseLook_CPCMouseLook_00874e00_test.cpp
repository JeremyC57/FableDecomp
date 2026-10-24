// Behaviour test for 0x00874E00: CPCMouseLook's constructor runs the base constructor, leaves an empty self-linked
// mouse-input list and sets the current state's recentre timer to -1.
#include <cstdio>
#include <cstdlib>

class CGameCameraManager { public: int id; };
class CCameraModeDef;
template <class T> class CDefPointer { public: T* p; };

class C2DVector { public: float X, Y; };
class C3DVector { public: float X, Y, Z; C3DVector() { } };

template <class T> struct SListNode { SListNode* _M_next; SListNode* _M_prev; T _M_data; };
template <class T> class CRetailList
{
public:
    SListNode<T>* _M_node;
    CRetailList() : _M_node(0)
    {
        SListNode<T>* node = (SListNode<T>*)std::malloc(sizeof(SListNode<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};

namespace NCameraModes
{
    class CBase
    {
    public:
        CGameCameraManager* m_manager;
        CBase(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>&) : m_manager(&manager) { }
        virtual ~CBase() { }
    };

    class CPCMouseLook : public CBase
    {
    public:
        class CRandomPacketOfVariablesDefiningTheCameraState
        {
        public:
            float AngleXY, AngleUpDown, DollyLength, DeltaAngleXY;
            long RecentreTimer;
            C3DVector ViewPosOffset;
            CRandomPacketOfVariablesDefiningTheCameraState() { }
        };
        CRetailList<C2DVector> MouseInputs;
        C3DVector AxisX, AxisY, AxisZ;
        CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;
        CRandomPacketOfVariablesDefiningTheCameraState StateLast;
        CPCMouseLook(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def);
    };
}

NCameraModes::CPCMouseLook::CPCMouseLook(CGameCameraManager& manager, const CDefPointer<const CCameraModeDef>& def)
    : CBase(manager, def)
{
    StateCurrent.RecentreTimer = -1;
}

int main()
{
    CGameCameraManager manager; manager.id = 3;
    CDefPointer<const CCameraModeDef> def; def.p = 0;
    NCameraModes::CPCMouseLook look(manager, def);
    SListNode<C2DVector>* s = look.MouseInputs._M_node;
    bool ok = look.m_manager == &manager && look.StateCurrent.RecentreTimer == -1
           && s != 0 && s->_M_next == s && s->_M_prev == s;
    std::free(s);
    if (!ok) { std::printf("00874e00_TEST FAIL\n"); return 1; }
    std::printf("00874e00_TEST PASS\n");
    return 0;
}