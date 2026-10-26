#include <cstdio>
struct CDefClassBase_Retail {
    virtual void v00() { }
    unsigned char _body[0x24];
    CDefClassBase_Retail() { _body[0] = 0; }
    CDefClassBase_Retail(const CDefClassBase_Retail& other) { _body[0] = other._body[0]; }
};
static int g_vectorCopies = 0;
struct CRetailVector_CCharString {
    void* first; void* last; void* end;
    CRetailVector_CCharString() : first(0), last(0), end(0) { }
    CRetailVector_CCharString(const CRetailVector_CCharString& other) : first(other.first), last(other.last), end(other.end) { ++g_vectorCopies; }
};
class CConversation : public CDefClassBase_Retail { public:
    long Lines; CRetailVector_CCharString Speaker; CRetailVector_CCharString Dialogue; CRetailVector_CCharString Animation; CRetailVector_CCharString AnimLoop;
    virtual void v00() { }
    CConversation() : Lines(0) { }
    CConversation(const CConversation& other);
};
CConversation::CConversation(const CConversation& other)
    : CDefClassBase_Retail(other), Lines(other.Lines), Speaker(other.Speaker), Dialogue(other.Dialogue), Animation(other.Animation), AnimLoop(other.AnimLoop)
{
}
int main()
{
    int a, b;
    CConversation c; c._body[0] = 7; c.Lines = 3; c.Speaker.first = &a; c.AnimLoop.last = &b;
    g_vectorCopies = 0;
    CConversation d(c);
    if (d._body[0] != 7 || d.Lines != 3 || d.Speaker.first != &a || d.AnimLoop.last != &b || g_vectorCopies != 4 || sizeof(CConversation) != 0x5c) { std::printf("00e54ca0_TEST FAIL\n"); return 1; }
    std::printf("00e54ca0_TEST PASS\n");
    return 0;
}