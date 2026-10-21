#include <cstdio>
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
class C2DVector {
public:
    float X;
    float Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
namespace NUISystem {
class CList {
public:
    virtual void _v00(); virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05();
    virtual void _v06(); virtual void _v07(); virtual void _v08(); virtual void _v09(); virtual void _v10(); virtual void _v11();
    virtual void _v12(); virtual void _v13(); virtual void _v14(); virtual void _v15(); virtual void _v16(); virtual void _v17();
    virtual void _v18(); virtual void _v19(); virtual void _v20(); virtual void _v21(); virtual void _v22(); virtual void _v23();
    virtual void _v24(); virtual void _v25(); virtual void _v26(); virtual void _v27(); virtual void _v28(); virtual void _v29();
    virtual void _v30(); virtual void _v31(); virtual void _v32();
    virtual void ChangePositionDelta(const C2DVector& delta, float time, bool flag);   // slot 33 (+0x84)
    void SetSelectedChild(long child);
    unsigned char     _pad004[0x15c - 4];
    long              SelectedChild;                     // +0x15c
    unsigned char     _pad160[0x1c8 - 0x160];
    std::vector<bool> ScrollingChildrenVisibility;       // +0x1c8
};
void CList::SetSelectedChild(long child)
{
    SelectedChild = child;
    if (child >= 0 && child < ScrollingChildrenVisibility.size() && !ScrollingChildrenVisibility[child])
    {
        long first = -1;
        long last = -1;
        for (long i = 0; i < ScrollingChildrenVisibility.size(); ++i)
        {
            if (first == -1 && ScrollingChildrenVisibility[i])
                first = i;
            if (first != -1 && !ScrollingChildrenVisibility[i])
            {
                last = i - 1;
                break;
            }
        }
        if (last == -1)
            last = ScrollingChildrenVisibility.size() - 1;
        if (child > last)
            ChangePositionDelta(C2DVector(0.0f, (child - last) * -25.0f), 0.0f, false);
        else if (first != -1)
            ChangePositionDelta(C2DVector(0.0f, (first - child) * 25.0f), 0.0f, false);
    }
}
}

#include <stdio.h>
void NUISystem::CList::_v00() {}
void NUISystem::CList::_v01() {}
void NUISystem::CList::_v02() {}
void NUISystem::CList::_v03() {}
void NUISystem::CList::_v04() {}
void NUISystem::CList::_v05() {}
void NUISystem::CList::_v06() {}
void NUISystem::CList::_v07() {}
void NUISystem::CList::_v08() {}
void NUISystem::CList::_v09() {}
void NUISystem::CList::_v10() {}
void NUISystem::CList::_v11() {}
void NUISystem::CList::_v12() {}
void NUISystem::CList::_v13() {}
void NUISystem::CList::_v14() {}
void NUISystem::CList::_v15() {}
void NUISystem::CList::_v16() {}
void NUISystem::CList::_v17() {}
void NUISystem::CList::_v18() {}
void NUISystem::CList::_v19() {}
void NUISystem::CList::_v20() {}
void NUISystem::CList::_v21() {}
void NUISystem::CList::_v22() {}
void NUISystem::CList::_v23() {}
void NUISystem::CList::_v24() {}
void NUISystem::CList::_v25() {}
void NUISystem::CList::_v26() {}
void NUISystem::CList::_v27() {}
void NUISystem::CList::_v28() {}
void NUISystem::CList::_v29() {}
void NUISystem::CList::_v30() {}
void NUISystem::CList::_v31() {}
void NUISystem::CList::_v32() {}
static int g_calls; static float g_dx, g_dy, g_t; static int g_flag = -1;
void NUISystem::CList::ChangePositionDelta(const C2DVector& d, float t, bool f) { ++g_calls; g_dx = d.X; g_dy = d.Y; g_t = t; g_flag = f ? 1 : 0; }
static NUISystem::CList g_list;
static bool g_vis[16];
static void Vis(const char* s) { int n = 0; while (s[n]) { g_vis[n] = s[n] == '1'; ++n; } g_list.ScrollingChildrenVisibility._M_start = g_vis; g_list.ScrollingChildrenVisibility._M_finish = g_vis + n; g_list.ScrollingChildrenVisibility._M_end_of_storage = g_vis + n; }
int main()
{
    Vis("0011100");
    g_list.SetSelectedChild(-1);
    bool ok = g_list.SelectedChild == -1 && g_calls == 0;
    g_list.SetSelectedChild(9);              // out of range
    ok = ok && g_list.SelectedChild == 9 && g_calls == 0;
    g_list.SetSelectedChild(3);              // visible: nothing
    ok = ok && g_calls == 0;
    g_list.SetSelectedChild(6);              // below the run (last = 4): (6-4) * -25
    ok = ok && g_calls == 1 && g_dx == 0.0f && g_dy == -50.0f && g_t == 0.0f && g_flag == 0;
    g_list.SetSelectedChild(0);              // above the run (first = 2): (2-0) * 25
    ok = ok && g_calls == 2 && g_dy == 50.0f;
    Vis("00111");
    g_list.SetSelectedChild(1);              // run reaches the end: last = 4, first = 2 -> 25
    ok = ok && g_calls == 3 && g_dy == 25.0f;
    Vis("000");
    g_list.SetSelectedChild(1);              // nothing visible: last = 2, child <= last, first == -1 -> nothing
    ok = ok && g_calls == 3;
    printf(ok ? "005360b1_TEST PASS\n" : "005360b1_TEST FAIL\n");
    return ok ? 0 : 1;
}