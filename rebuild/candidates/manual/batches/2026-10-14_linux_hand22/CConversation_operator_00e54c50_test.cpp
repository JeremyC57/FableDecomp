#include <cstdio>
struct CDefClassBase_Retail {
    virtual void v00() { }
    unsigned char _body[0x24];
    CDefClassBase_Retail& operator=(const CDefClassBase_Retail& other) { _body[0] = other._body[0]; return *this; }
};
static int g_vectorAssigns = 0;
struct CRetailVector_CCharString {
    void* first; void* last; void* end;
    CRetailVector_CCharString& operator=(const CRetailVector_CCharString& other) { first = other.first; last = other.last; end = other.end; ++g_vectorAssigns; return *this; }
};
struct CConversation : CDefClassBase_Retail {
    long Lines; CRetailVector_CCharString Speaker; CRetailVector_CCharString Dialogue; CRetailVector_CCharString Animation; CRetailVector_CCharString AnimLoop;
    virtual void v00() { }
    CConversation& operator=(const CConversation& other);
};
CConversation& CConversation::operator=(const CConversation& other)
{
    CDefClassBase_Retail::operator=(other);
    Lines = other.Lines;
    Speaker = other.Speaker;
    Dialogue = other.Dialogue;
    Animation = other.Animation;
    AnimLoop = other.AnimLoop;
    return *this;
}
int main()
{
    int a, b;
    CConversation c; c._body[0] = 9; c.Lines = 4; c.Speaker.first = &a; c.Dialogue.end = &b; c.Animation.first = 0; c.AnimLoop.last = 0;
    CConversation d; d._body[0] = 0; d.Lines = 0; d.Speaker.first = 0; d.Dialogue.end = 0;
    CConversation& r = (d = c);
    if (&r != &d || d._body[0] != 9 || d.Lines != 4 || d.Speaker.first != &a || d.Dialogue.end != &b || g_vectorAssigns != 4) { std::printf("00e54c50_TEST FAIL\n"); return 1; }
    std::printf("00e54c50_TEST PASS\n");
    return 0;
}
