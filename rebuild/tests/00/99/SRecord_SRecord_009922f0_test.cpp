#include <cstdio>
static int g_stringCopies = 0;
class CCharString { public:
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    CCharString(const CCharString& other) : Text(other.Text) { ++g_stringCopies; }
};
struct SCountBlock { long Count; };
template <class T> class CCountedPointer { public:
    T* Data; SCountBlock* Info;
    CCountedPointer(T* d, SCountBlock* i) : Data(d), Info(i) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->Count; }
};
class CThing { public: long Id; };
// The candidate logic: the record's memberwise copy constructor.
struct SRecord {
    CCountedPointer<CThing> A; CCountedPointer<CThing> B;
    CCharString S0, S1, S2, S3;
    SRecord(CThing* a, SCountBlock* ai) : A(a, ai), B(0, 0), S0("s0"), S1("s1"), S2("s2"), S3("s3") {}
    SRecord(const SRecord& o);
};
SRecord::SRecord(const SRecord& o) : A(o.A), B(o.B), S0(o.S0), S1(o.S1), S2(o.S2), S3(o.S3) {}
int main()
{
    CThing thing = { 9 };
    SCountBlock block = { 1 };
    SRecord original(&thing, &block);
    SRecord copy(original);
    bool ok = copy.A.Data == &thing && copy.A.Info == &block && block.Count == 2 && copy.B.Info == 0 &&
              copy.S0.Text[1] == '0' && copy.S3.Text[1] == '3' && g_stringCopies == 4;
    if (!ok) { std::printf("009922f0_TEST FAIL\n"); return 1; }
    std::printf("009922f0_TEST PASS\n");
    return 0;
}