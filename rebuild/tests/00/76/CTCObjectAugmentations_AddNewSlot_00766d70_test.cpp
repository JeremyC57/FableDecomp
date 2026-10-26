// Behaviour test for 0x00766D70 CTCObjectAugmentations::AddNewSlot: appends one empty (null) slot only while the
// slot count is below the def's MaximumNumberOfSlots, through both the in-place and the growth path.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CDefPointee { unsigned long m_count; };

struct CDefPointer {
    CDefPointee* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->m_count; }
};

struct CDefPointerVector {
    CDefPointer* _M_start;
    CDefPointer* _M_finish;
    CDefPointer* _M_end_of_storage;
    int m_overflows;
    CDefPointerVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0), m_overflows(0) { }
    ~CDefPointerVector() { std::free(_M_start); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(CDefPointer* pos, const CDefPointer& x)
    {
        ++m_overflows;
        unsigned int n = size();
        unsigned int cap = n ? n * 2 : 1;
        CDefPointer* s = (CDefPointer*)std::malloc(cap * sizeof(CDefPointer));
        CDefPointer* d = s;
        for (CDefPointer* p = _M_start; p != pos; ++p, ++d) new (d) CDefPointer(*p);
        new (d) CDefPointer(x); ++d;
        std::free(_M_start);
        _M_start = s; _M_finish = d; _M_end_of_storage = s + cap;
    }
    void push_back(const CDefPointer& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CDefPointer(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x);
    }
};

class CObjectAugmentationsDef { public: long NumberOfSlots; long MaximumNumberOfSlots; };

class CTCObjectAugmentations { public:
    CObjectAugmentationsDef* PDef;
    CDefPointerVector Augmentations;
    long GetNumberOfSlots() const { return (long)Augmentations.size(); }
    long GetMaximumNumberOfSlots() const { return PDef->MaximumNumberOfSlots; }
    void AddNewSlot()
    {
        if (GetNumberOfSlots() < GetMaximumNumberOfSlots())
            Augmentations.push_back(CDefPointer());
    }
};

int main()
{
    CObjectAugmentationsDef def = { 1, 3 };
    CTCObjectAugmentations tc;
    tc.PDef = &def;
    int ok = 1;
    for (int i = 0; i < 6; ++i)
        tc.AddNewSlot();
    if (tc.Augmentations.size() != 3u) ok = 0;            // capped at MaximumNumberOfSlots
    for (unsigned int i = 0; i < tc.Augmentations.size(); ++i)
        if (tc.Augmentations._M_start[i].Object != 0) ok = 0;
    if (tc.Augmentations.m_overflows != 3) ok = 0;        // capacities 1, 2, 4
    def.MaximumNumberOfSlots = 4;                         // one more allowed: in place (capacity 4)
    tc.AddNewSlot(); tc.AddNewSlot();
    if (tc.Augmentations.size() != 4u || tc.Augmentations.m_overflows != 3) ok = 0;
    def.MaximumNumberOfSlots = -1;                        // signed compare: never adds
    tc.AddNewSlot();
    if (tc.Augmentations.size() != 4u) ok = 0;
    if (!ok) { std::printf("00766d70_TEST FAIL\n"); return 1; }
    std::printf("00766d70_TEST PASS\n");
    return 0;
}