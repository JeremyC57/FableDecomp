#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00678250"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static LONG32 g_written[16]; static int g_count = 0;
class CDataOutputStream { public: void WriteLong(LONG32 v) { g_written[g_count++] = v; } };
class CReactionMatch {
public:
    LONG32 type, payload;
    virtual ~CReactionMatch() { }
    virtual LONG32 GetMatchType() const { return type; }
    virtual void TransferBinaryOut(CDataOutputStream& out) const { out.WriteLong(payload); }
};
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct list {
    _List_node<T>* _M_node;
    unsigned long size() const { unsigned long n = 0; for (_List_node<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
};
void TransferBinaryOut(CDataOutputStream& out, const list<CReactionMatch*>& value)
{
    out.WriteLong((LONG32)value.size());
    for (_List_node<CReactionMatch*>* it = value._M_node->_M_next; it != value._M_node; it = it->_M_next) {
        CReactionMatch* match = it->_M_data;
        out.WriteLong(match->GetMatchType());
        match->TransferBinaryOut(out);
    }
}
int main()
{
    CReactionMatch a, b; a.type = 4; a.payload = 40; b.type = 2; b.payload = 20;
    _List_node<CReactionMatch*> head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head; n1._M_data = &a; n2._M_data = &b;
    list<CReactionMatch*> l; l._M_node = &head;
    CDataOutputStream out;
    TransferBinaryOut(out, l);
    CHECK(g_count == 5 && g_written[0] == 2 && g_written[1] == 4 && g_written[2] == 40 && g_written[3] == 2 && g_written[4] == 20);
    head._M_next = &head; g_count = 0;
    TransferBinaryOut(out, l);
    CHECK(g_count == 1 && g_written[0] == 0);
    std::printf("00678250_TEST PASS\n");
    return 0;
}