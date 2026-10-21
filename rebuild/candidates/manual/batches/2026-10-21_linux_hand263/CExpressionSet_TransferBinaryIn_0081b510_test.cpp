// behaviour test for 0081b510 CExpressionSet::TransferBinaryIn: the count sizes the entry vector with default entries,
// the entries are read as one raw block, a dirty map is re-sorted by type and cleaned, and the trailing dword is consumed
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
struct Entry { int Type; int Data[9]; };
static bool Less(const Entry& a, const Entry& b) { return a.Type < b.Type; }
struct Stream { const unsigned char* p; unsigned int pos; void Read(void* d, int n) { std::memcpy(d, p + pos, n); pos += n; } };
struct Set { std::vector<Entry> v; bool dirty; };
static void TransferBinaryIn(Set& s, Stream& in)
{
    int count; in.Read(&count, 4);
    Entry def; std::memset(&def, 0, sizeof(def)); def.Data[3] = -1;
    s.v.resize(count, def);
    int bytes = (int)(s.v.size() * sizeof(Entry));
    if (bytes > 0) in.Read(&s.v[0], bytes);
    if (s.dirty) { std::sort(s.v.begin(), s.v.end(), Less); s.dirty = false; }
    in.Read(&count, 4);
}
int main()
{
    unsigned char buf[256]; std::memset(buf, 0, sizeof(buf));
    int count = 2; std::memcpy(buf, &count, 4);
    Entry a; std::memset(&a, 0, sizeof(a)); a.Type = 5; Entry b; std::memset(&b, 0, sizeof(b)); b.Type = 2;
    std::memcpy(buf + 4, &a, sizeof(a)); std::memcpy(buf + 4 + sizeof(a), &b, sizeof(b));
    Stream in = { buf, 0 }; Set s; s.dirty = true;
    TransferBinaryIn(s, in);
    if (s.v.size() != 2 || s.v[0].Type != 2 || s.v[1].Type != 5 || s.dirty || in.pos != 4 + 2 * sizeof(Entry) + 4) {
        std::printf("0081b510_TEST FAIL\n"); return 1;
    }
    std::printf("0081b510_TEST PASS\n");
    return 0;
}
