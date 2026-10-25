// Behaviour test for vector<CHandleInfo>::_Xlen @ 0x00C28DC0: the thrower raises a length_error carrying "vector<T> too long"
// and never returns (modelled with a local logic_error / length_error pair over a fixed message buffer).
#include <cstdio>
#include <cstring>

class logic_error_model {
public:
    explicit logic_error_model(const char* message) { std::strncpy(m_text, message, sizeof(m_text) - 1); m_text[sizeof(m_text) - 1] = 0; }
    virtual ~logic_error_model() { }
    virtual const char* what() const { return m_text; }
private:
    char m_text[32];
};
class length_error_model : public logic_error_model {
public:
    explicit length_error_model(const char* message) : logic_error_model(message) { }
};

struct vector_model {
    int m_reached;
    void _Xlen() { throw length_error_model("vector<T> too long"); }
    void Buy(unsigned long count) { if (count > 0x3fffffffUL) _Xlen(); m_reached = 1; }
};

int main()
{
    vector_model v;
    v.m_reached = 0;
    v.Buy(16);
    if (v.m_reached != 1) { std::printf("00c28dc0_TEST FAIL\n"); return 1; }
    v.m_reached = 0;
    int caught = 0;
    try {
        v.Buy(0x40000000UL);
    } catch (const logic_error_model& e) {
        caught = (std::strcmp(e.what(), "vector<T> too long") == 0) ? 1 : 2;
    }
    if (caught != 1 || v.m_reached != 0) { std::printf("00c28dc0_TEST FAIL\n"); return 1; }
    std::printf("00c28dc0_TEST PASS\n");
    return 0;
}