// Behaviour test for 00c13840: vector::_Xlen -- always throws length_error("vector<T> too long"), catchable as its logic_error base; the vector itself is untouched
#include <cstdio>
#include <cstring>

class logic_error {
public:
    explicit logic_error(const char* message) : m_message(message) { }
    virtual ~logic_error() { }
    virtual const char* what() const { return m_message; }
private:
    const char* m_message;
};
class length_error : public logic_error {
public:
    explicit length_error(const char* message) : logic_error(message) { }
};

struct Request { int m_id; };
template<class T> struct Vector {
    void* m_alval; T* m_first; T* m_last; T* m_end;
    void Xlen() const { throw length_error("vector<T> too long"); }
    unsigned int max_size() const { return 0x3FFFFFFF; }
    void reserve(unsigned int n) { if (max_size() < n) Xlen(); }
};

int main()
{
    Vector<Request*> v = { 0, 0, 0, 0 };
    int caught = 0;
    try {
        v.reserve(0x40000000);
    } catch (const length_error& e) {
        caught = std::strcmp(e.what(), "vector<T> too long") == 0 ? 1 : 2;
    }
    if (caught != 1 || v.m_first != 0) { std::printf("00c13840_TEST FAIL\n"); return 1; }
    caught = 0;
    try {
        v.Xlen();
    } catch (const logic_error& e) {
        caught = std::strcmp(e.what(), "vector<T> too long") == 0 ? 1 : 2;
    }
    if (caught != 1) { std::printf("00c13840_TEST FAIL\n"); return 1; }
    std::printf("00c13840_TEST PASS\n");
    return 0;
}