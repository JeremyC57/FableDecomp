#include <cstdio>
#include <cstdlib>

// Model (CTCUnholyPower::PerEffectInfo): push_back copies in place while capacity remains and otherwise takes the overflow path
// (stand-in: grow by doubling and copy), the Effect pointer is copied through GetPItem/SetPItem.
struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    void* GetPItem() const { return m_pItem; }
    void SetPItem(void* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct PerEffectInfo {
    C3DVector           Position;
    CIntelligentPointer Effect;
    PerEffectInfo() { }
    PerEffectInfo(const PerEffectInfo& o) : Position(o.Position), Effect(o.Effect) { }
};
struct __false_type { __false_type() { } };
inline void* operator new(size_t, void* where) { return where; }

static int g_overflows = 0;

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool)
    {
        ++g_overflows;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int len = size ? 2 * size : 1;
        T* start = (T*)std::malloc(len * sizeof(T));
        T* cur = start;
        for (T* p = _M_start; p != position; ++p, ++cur)
            new (cur) T(*p);
        for (unsigned int i = 0; i < fill_len; ++i, ++cur)
            new (cur) T(x);
        std::free(_M_start);
        _M_start = start;
        _M_finish = cur;
        _M_end_of_storage = start + len;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

int main()
{
    vector<PerEffectInfo> v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    int things[5];
    for (int i = 0; i < 5; ++i) {
        PerEffectInfo e;
        e.Position.X = (float)i;
        e.Position.Y = 2.0f * i;
        e.Position.Z = -1.0f;
        e.Effect.SetPItem(&things[i]);
        v.push_back(e);
    }
    bool ok = v._M_finish - v._M_start == 5 && g_overflows == 4;     // capacities 1, 2, 4, 8
    for (int i = 0; i < 5; ++i)
        ok = ok && v._M_start[i].Position.X == (float)i && v._M_start[i].Position.Y == 2.0f * i
             && v._M_start[i].Effect.GetPItem() == &things[i];
    std::free(v._M_start);
    if (!ok) {
        std::printf("00798410_TEST FAIL\n");
        return 1;
    }
    std::printf("00798410_TEST PASS\n");
    return 0;
}