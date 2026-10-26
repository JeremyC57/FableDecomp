// Behaviour test for 0x009A9AD0 std::pair<const CCharString, CBankFileManager::CBankInfo>::pair(const pair&): the key and the four CWideString paths are copied through their copy constructors, RegisteredBank as a word.
#include <cstdio>
static int g_copies = 0;
class CCharString { public: long id; CCharString(long i) : id(i) { } CCharString(const CCharString& o) : id(o.id) { ++g_copies; } };
class CWideString { public: long id; CWideString(long i) : id(i) { } CWideString(const CWideString& o) : id(o.id) { ++g_copies; } };
class CBankInfo {
public:
    CWideString DevBankFilePath, RetailBankFilePath, DevHeaderFilePath, RetailHeaderFilePath; long RegisteredBank;
    CBankInfo() : DevBankFilePath(1), RetailBankFilePath(2), DevHeaderFilePath(3), RetailHeaderFilePath(4), RegisteredBank(5) { }
};
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }
};
int main()
{
    pair<const CCharString, CBankInfo> a(CCharString(9), CBankInfo());
    g_copies = 0;
    pair<const CCharString, CBankInfo> b(a);
    bool ok = g_copies == 5 && b.first.id == 9 && b.second.DevBankFilePath.id == 1 && b.second.RetailHeaderFilePath.id == 4 && b.second.RegisteredBank == 5;
    if (!ok) { std::printf("009a9ad0_TEST FAIL\n"); return 1; }
    std::printf("009a9ad0_TEST PASS\n");
    return 0;
}