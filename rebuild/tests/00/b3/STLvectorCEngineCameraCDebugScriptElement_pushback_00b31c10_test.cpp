// Behaviour test for 0x00B31C10 std::vector<CDebugScriptElement>::push_back: with room the element is
// copied in place at _M_finish and _M_finish advances by one element; when full, _M_insert_overflow gets
// (_M_finish, x, tag, 1, true).
#include <cstdio>
#include <new>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CDebugScriptElement {
    C3DVector WorldPos;
    C3DVector LookVector;
    float     FOV;
    float     TimeToNextElement;
};

struct __false_type { __false_type() {} };

struct CDebugScriptVector {
    CDebugScriptElement* _M_start;
    CDebugScriptElement* _M_finish;
    CDebugScriptElement* _M_end_of_storage;
    void _M_insert_overflow(CDebugScriptElement* position, const CDebugScriptElement& x, const __false_type&, unsigned int fill_len, bool atend);
    void push_back(const CDebugScriptElement& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CDebugScriptElement(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};

static CDebugScriptElement* g_where = 0;
static const CDebugScriptElement* g_value = 0;
static unsigned int g_count = 0;
static bool g_atend = false;
static int g_calls = 0;

void CDebugScriptVector::_M_insert_overflow(CDebugScriptElement* position, const CDebugScriptElement& x, const __false_type&, unsigned int fill_len, bool atend)
{
    g_where = position; g_value = &x; g_count = fill_len; g_atend = atend; ++g_calls;
}

int main()
{
    CDebugScriptElement buf[2];
    CDebugScriptElement v;
    v.WorldPos.X = 1.0f; v.WorldPos.Y = 2.0f; v.WorldPos.Z = 3.0f;
    v.LookVector.X = 0.0f; v.LookVector.Y = 0.6f; v.LookVector.Z = 0.8f;
    v.FOV = 7.5f; v.TimeToNextElement = 2.0f;
    CDebugScriptVector vec;
    vec._M_start = buf; vec._M_finish = buf; vec._M_end_of_storage = buf + 1;
    vec.push_back(v);
    if (vec._M_finish != buf + 1 || g_calls != 0 || buf[0].WorldPos.X != 1.0f || buf[0].WorldPos.Z != 3.0f || buf[0].LookVector.Y != 0.6f
        || buf[0].LookVector.Z != 0.8f || buf[0].FOV != 7.5f || buf[0].TimeToNextElement != 2.0f) {
        std::printf("00b31c10_TEST FAIL\n");
        return 1;
    }
    vec.push_back(v);
    if (g_calls != 1 || g_where != buf + 1 || g_value != &v || g_count != 1 || !g_atend || vec._M_finish != buf + 1) {
        std::printf("00b31c10_TEST FAIL\n");
        return 1;
    }
    std::printf("00b31c10_TEST PASS\n");
    return 0;
}
