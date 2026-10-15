// Behaviour test for the C3DAnimationStats constructor @ 0x00A4EB00: empty STLport-style tree, cleared flags/duration/vector.
#include <cstdio>
#include <cstdlib>

struct CCharString { void* m_pStringData; };
struct C3DVector { float X, Y, Z; void Clear() { X = Y = Z = 0.0f; } };
struct SRbTreeNodeBase { bool _M_color; SRbTreeNodeBase* _M_parent; SRbTreeNodeBase* _M_left; SRbTreeNodeBase* _M_right; };
struct STimingEventNode : SRbTreeNodeBase { CCharString _M_key; float _M_value; };
struct CTimingEventMultimap {
    SRbTreeNodeBase* _M_header; unsigned long _M_node_count;
    CTimingEventMultimap() : _M_header(0)
    {
        _M_header = static_cast<SRbTreeNodeBase*>(std::malloc(sizeof(STimingEventNode)));
        _M_node_count = 0;
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    ~CTimingEventMultimap() { std::free(_M_header); }
};
static int g_baseCtors = 0;
struct CBaseObject { void* m_pFirstPointer; CBaseObject() : m_pFirstPointer(0) { ++g_baseCtors; } virtual ~CBaseObject() {} };
struct C3DAnimationStats : CBaseObject {
    long AnimType; float Duration; bool LoopFlag; bool MovementFlag; C3DVector MovementVector; CTimingEventMultimap TimingEvents;
    C3DAnimationStats()
    {
        LoopFlag = false; Duration = 0.0f; MovementFlag = false;
        MovementVector.Clear();
    }
};

int main()
{
    C3DAnimationStats s;
    SRbTreeNodeBase* h = s.TimingEvents._M_header;
    if (g_baseCtors != 1 || h == 0 || h->_M_color || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h || s.TimingEvents._M_node_count != 0 ||
        s.LoopFlag || s.MovementFlag || s.Duration != 0.0f || s.MovementVector.X != 0.0f || s.MovementVector.Y != 0.0f || s.MovementVector.Z != 0.0f) {
        std::printf("00a4eb00_TEST FAIL\n"); return 1;
    }
    std::printf("00a4eb00_TEST PASS\n");
    return 0;
}