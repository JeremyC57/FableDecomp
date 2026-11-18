// Behaviour test for vector<pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus> >::push_back 0x00749620
// (STLport body copied as plain C++; the intelligent-pointer base and _M_insert_overflow are stand-ins that record their use).
// Verifier fix (e16 audit): one case has _M_finish past _M_end_of_storage, so the `!=` test (retail `cmp esi,eax ; je`) is
// pinned against an ordered `<` comparison.
#include <cstdio>
#include <cstring>
inline void* operator new(unsigned int, void* where, int) { return where; }

class CBaseObject { public: int Id; };
class CThing : public CBaseObject { };
static int g_baseCtors, g_gets, g_sets;
class CBaseIntelligentPointer {
public:
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_baseCtors; }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { ++g_gets; return PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& item) { SetPointer(item.GetPointer()); }
    T* GetPointer() const { return static_cast<T*>(GetPItem()); }
    void SetPointer(T* pointer) { SetPItem((CBaseObject*)pointer); }
};
namespace NStealth {
enum EDangerStatus { DS_NONE, DS_A, DS_B };
class CDangerStatus { public: EDangerStatus DangerStatus; long Timer; };
}
struct TPair { CIntelligentPointer<const CThing> first; NStealth::CDangerStatus second; };
struct __false_type { };

static int g_overflows; static TPair* g_ovPos; static const TPair* g_ovX; static unsigned g_ovN; static bool g_ovAtEnd;
struct TVector {
    TPair* _M_start; TPair* _M_finish; TPair* _M_end_of_storage;
    void _M_insert_overflow(TPair* pos, const TPair& x, const __false_type&, unsigned n, bool atend)
    { ++g_overflows; g_ovPos = pos; g_ovX = &x; g_ovN = n; g_ovAtEnd = atend; }
    void push_back(const TPair& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) new (_M_finish, 0) TPair(__x);     // _Construct: placement new tests the pointer
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};

static void reset() { g_baseCtors = g_gets = g_sets = 0; g_overflows = 0; g_ovPos = 0; g_ovX = 0; g_ovN = 0; g_ovAtEnd = false; }

int main()
{
    bool ok = true;
    CThing thing; thing.Id = 7;
    TPair x; x.first.SetPItem(&thing); x.second.DangerStatus = NStealth::DS_B; x.second.Timer = 0x12345;
    static unsigned char storage[3 * sizeof(TPair)];
    TPair* buf = (TPair*)storage; std::memset(storage, 0xCD, sizeof(storage));
    TVector v; v._M_start = buf; v._M_finish = buf + 1; v._M_end_of_storage = buf + 3;
    // room: construct at _M_finish (slot 1), bump by one element
    reset(); v.push_back(x);
    ok = ok && v._M_finish == buf + 2 && v._M_start == buf && v._M_end_of_storage == buf + 3 && g_overflows == 0 &&
         buf[1].first.PData == &thing && buf[1].second.DangerStatus == NStealth::DS_B && buf[1].second.Timer == 0x12345 &&
         g_baseCtors == 1 && g_gets == 1 && g_sets == 1 && storage[0] == 0xCD && x.first.PData == &thing;
    // last free slot (finish + 1 == end of storage) still constructs
    reset(); v.push_back(x);
    ok = ok && v._M_finish == buf + 3 && g_overflows == 0 && buf[2].second.Timer == 0x12345 && g_baseCtors == 1;
    // full: _M_insert_overflow(finish, x, tag, 1, true); push_back itself does not move finish
    reset(); v.push_back(x);
    ok = ok && v._M_finish == buf + 3 && g_overflows == 1 && g_ovPos == buf + 3 && g_ovX == &x && g_ovN == 1 && g_ovAtEnd &&
         g_baseCtors == 0 && g_gets == 0;
    // null finish with spare capacity: nothing constructed, finish still advances
    TVector n; n._M_start = 0; n._M_finish = 0; n._M_end_of_storage = buf + 1;
    reset(); n.push_back(x);
    ok = ok && n._M_finish == (TPair*)0 + 1 && g_baseCtors == 0 && g_overflows == 0;
    // empty-but-allocated vector (start == finish != end)
    std::memset(storage, 0, sizeof(storage));
    TVector e; e._M_start = buf; e._M_finish = buf; e._M_end_of_storage = buf + 2;
    reset(); e.push_back(x);
    ok = ok && e._M_finish == buf + 1 && buf[0].first.PData == &thing && buf[0].second.DangerStatus == NStealth::DS_B && g_overflows == 0;
    // finish past end of storage: retail compares for inequality only, so it still constructs in place and bumps finish
    static unsigned char storage2[5 * sizeof(TPair)];
    TPair* buf2 = (TPair*)storage2; std::memset(storage2, 0, sizeof(storage2));
    TVector g; g._M_start = buf2; g._M_finish = buf2 + 3; g._M_end_of_storage = buf2 + 2;
    reset(); g.push_back(x);
    ok = ok && g._M_finish == buf2 + 4 && g._M_end_of_storage == buf2 + 2 && g_overflows == 0 && g_baseCtors == 1 &&
         buf2[3].first.PData == &thing && buf2[3].second.Timer == 0x12345 && buf2[2].second.Timer == 0;
    std::printf(ok ? "00749620_TEST PASS\n" : "00749620_TEST FAIL\n");
    return ok ? 0 : 1;
}