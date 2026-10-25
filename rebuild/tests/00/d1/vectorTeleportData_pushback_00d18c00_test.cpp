// Behaviour test for vector<TeleportData>::push_back (0x00D18C00): copy-construct at the finish pointer while capacity
// remains, otherwise hand the element to the overflow insert at the finish position.
#include <cstdio>
#include <new>

static int g_copies = 0;
struct CTeleportData {
    int value;
    CTeleportData(int v) : value(v) { }
    CTeleportData(const CTeleportData& o) : value(o.value) { ++g_copies; }
};
template <class T1, class T2> void Construct(T1* p, const T2& value) { new (p) T1(value); }
struct CElementVector {
    CTeleportData* _M_start;
    CTeleportData* _M_finish;
    CTeleportData* _M_end_of_storage;
    int overflows;
    int overflowValue;
    void _M_insert_overflow(CTeleportData* position, const CTeleportData& x, unsigned int fill_len, bool atend)
    {
        if (position == _M_finish && fill_len == 1 && atend) { ++overflows; overflowValue = x.value; }
    }
    void push_back(const CTeleportData& x)
    {
        if (_M_finish != _M_end_of_storage) {
            Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, 1, true);
    }
};

int main()
{
    static double storage[8];
    CElementVector v;
    v._M_start = (CTeleportData*)storage;
    v._M_finish = v._M_start;
    v._M_end_of_storage = v._M_start + 2;
    v.overflows = 0; v.overflowValue = 0;
    v.push_back(CTeleportData(11));
    v.push_back(CTeleportData(22));
    v.push_back(CTeleportData(33));
    if (v._M_finish - v._M_start != 2 || v._M_start[0].value != 11 || v._M_start[1].value != 22 || g_copies != 2
        || v.overflows != 1 || v.overflowValue != 33) {
        std::printf("00d18c00_TEST FAIL\n");
        return 1;
    }
    std::printf("00d18c00_TEST PASS\n");
    return 0;
}