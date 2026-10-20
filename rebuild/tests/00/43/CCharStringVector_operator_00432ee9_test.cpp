// Behaviour test for 0x00432EE9 STLport vector<CCharString>::operator= (three-way capacity/size branch).
#include <cstdio>
#include <cstring>
#include <new>
#include <cstdlib>

static int g_live = 0;
class CCharString {
public:
    CCharString(const char* s) { std::strcpy(m_text, s); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_live; }
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
    ~CCharString() { --g_live; }
    const char* c_str() const { return m_text; }
private:
    char m_text[16];
};
static void DestroyRange(CCharString* first, CCharString* last) { for (; first != last; ++first) first->~CCharString(); }
static CCharString* CopyRange(const CCharString* first, const CCharString* last, CCharString* out)
{ for (int n = int(last - first); n > 0; --n, ++first, ++out) *out = *first; return out; }
static CCharString* UninitializedCopy(const CCharString* first, const CCharString* last, CCharString* out)
{ CCharString* current = out; for (; first != last; ++first, ++current) new (current) CCharString(*first); return current; }

struct CCharStringVector {
    CCharString* Start; CCharString* Finish; CCharString* End;
    CCharStringVector() : Start(0), Finish(0), End(0) { }
    unsigned int size() const { return Finish - Start; }
    unsigned int capacity() const { return End - Start; }
    CCharString* AllocateAndCopy(unsigned int n, const CCharString* first, const CCharString* last)
    { CCharString* p = static_cast<CCharString*>(std::malloc(n * sizeof(CCharString))); UninitializedCopy(first, last, p); return p; }
    void Clear() { DestroyRange(Start, Finish); std::free(Start); Start = Finish = End = 0; }
    void Reserve(unsigned int n) { Clear(); Start = Finish = static_cast<CCharString*>(std::malloc(n * sizeof(CCharString))); End = Start + n; }
    void Push(const char* s) { new (Finish) CCharString(s); ++Finish; }
    CCharStringVector& operator=(const CCharStringVector& other);
};
CCharStringVector& CCharStringVector::operator=(const CCharStringVector& other)
{
    if (&other != this) {
        unsigned int n = other.size();
        if (n > capacity()) {
            CCharString* fresh = AllocateAndCopy(n, other.Start, other.Finish);
            Clear(); Start = fresh; End = Start + n;
        } else if (size() >= n) {
            CCharString* i = CopyRange(other.Start, other.Finish, Start);
            DestroyRange(i, Finish);
        } else {
            CopyRange(other.Start, other.Start + size(), Start);
            UninitializedCopy(other.Start + size(), other.Finish, Finish);
        }
        Finish = Start + n;
    }
    return *this;
}
static bool Same(const CCharStringVector& a, const CCharStringVector& b)
{
    if (a.size() != b.size()) return false;
    for (unsigned int i = 0; i < a.size(); ++i) if (std::strcmp(a.Start[i].c_str(), b.Start[i].c_str()) != 0) return false;
    return true;
}
int main()
{
    int ok = 1;
    {
        CCharStringVector src; src.Reserve(4); src.Push("a"); src.Push("b"); src.Push("c");
        CCharStringVector grow; grow.Reserve(1); grow.Push("x");            // n > capacity
        grow = src; if (!Same(grow, src) || grow.capacity() != 3) ok = 0;
        CCharStringVector shrink; shrink.Reserve(5); shrink.Push("1"); shrink.Push("2"); shrink.Push("3"); shrink.Push("4");
        shrink = src; if (!Same(shrink, src) || shrink.capacity() != 5) ok = 0;   // size >= n
        CCharStringVector within; within.Reserve(6); within.Push("q");
        within = src; if (!Same(within, src) || within.capacity() != 6) ok = 0;   // grow inside capacity
        CCharStringVector& self = (src = src); if (&self != &src || src.size() != 3) ok = 0;
        if (g_live != 12) ok = 0;
        src.Clear(); grow.Clear(); shrink.Clear(); within.Clear();
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00432ee9_TEST FAIL\n"); return 1; }
    std::printf("00432ee9_TEST PASS\n");
    return 0;
}