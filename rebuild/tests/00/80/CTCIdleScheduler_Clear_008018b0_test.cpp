// Behaviour test for 0x008018B0 CTCIdleScheduler::Clear: zero Preferred/FrameLastChecked, reset both trees to empty.
#include <cstdio>
struct SIdleNode { long _M_color; SIdleNode* _M_parent; SIdleNode* _M_left; SIdleNode* _M_right; };
static int g_erased = 0;
static void CountErase(SIdleNode* x) { if (!x) return; CountErase(x->_M_right); CountErase(x->_M_left); ++g_erased; }
class CMap { public:
    SIdleNode* _M_header; unsigned int _M_node_count; unsigned int _M_key_compare;
    void _M_erase(SIdleNode* x) { CountErase(x); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header; _M_header->_M_parent = 0; _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
};
class CTCIdleScheduler { public:
    CMap IdleStates; CMap IdleStatesWeight; long Preferred; long FrameLastChecked;
    void Clear() { FrameLastChecked = 0; Preferred = 0; IdleStates.clear(); IdleStatesWeight.clear(); }
};
int main()
{
    SIdleNode h1, h2, r, l;
    h1._M_parent = &r; h1._M_left = &l; h1._M_right = &r;
    r._M_parent = &h1; r._M_left = &l; r._M_right = 0; l._M_parent = &r; l._M_left = 0; l._M_right = 0;
    h2._M_parent = 0; h2._M_left = &h2; h2._M_right = &h2;
    CTCIdleScheduler s;
    s.IdleStates._M_header = &h1; s.IdleStates._M_node_count = 2;
    s.IdleStatesWeight._M_header = &h2; s.IdleStatesWeight._M_node_count = 0;
    s.Preferred = 5; s.FrameLastChecked = 99;
    s.Clear();
    if (g_erased != 2 || s.Preferred != 0 || s.FrameLastChecked != 0 || s.IdleStates._M_node_count != 0
        || h1._M_parent != 0 || h1._M_left != &h1 || h1._M_right != &h1 || h2._M_left != &h2) {
        std::printf("008018b0_TEST FAIL\n"); return 1;
    }
    std::printf("008018b0_TEST PASS\n");
    return 0;
}