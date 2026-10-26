// Behaviour test for 0x006DC550 GFSerialiseVectorBinaryOut<CAttackHistoryCombo>: the count is written first, then every
// element (16-byte stride) is handed to the traits' TransferBinaryOut in order.
#include <cstdio>

class CAttackHistoryCombo { public: unsigned int ID; int Words[2]; float Multiplier; };

struct Writer {
    int log[32]; int n;
    Writer() : n(0) {}
    void WriteLong(long v) { log[n++] = (int)v; }
};

template <class T> struct Vec {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int i) { return *(begin() + i); }
};

template <class T> struct Traits {
    T DefaultVal;
    void TransferBinaryOut(Writer& s, const T* v) const { s.WriteLong((long)v->ID); }
};

template <class T> void SerialiseOut(Writer& stream, Vec<T>& value)
{
    Traits<T> traits;
    long count = value.size();
    stream.WriteLong(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

int main()
{
    CAttackHistoryCombo e[3];
    for (int i = 0; i < 3; ++i) { e[i].ID = 100 + i; e[i].Words[0] = e[i].Words[1] = 0; e[i].Multiplier = 1.0f; }
    Vec<CAttackHistoryCombo> v; v._M_start = e; v._M_finish = e + 3; v._M_end_of_storage = e + 3;
    Writer w;
    SerialiseOut(w, v);
    if (sizeof(CAttackHistoryCombo) != 16 || w.n != 4 || w.log[0] != 3 || w.log[1] != 100 || w.log[3] != 102) {
        std::printf("006dc550_TEST FAIL\n"); return 1;
    }
    Vec<CAttackHistoryCombo> empty; empty._M_start = empty._M_finish = empty._M_end_of_storage = e;
    Writer w2;
    SerialiseOut(w2, empty);
    if (w2.n != 1 || w2.log[0] != 0) { std::printf("006dc550_TEST FAIL\n"); return 1; }
    std::printf("006dc550_TEST PASS\n");
    return 0;
}