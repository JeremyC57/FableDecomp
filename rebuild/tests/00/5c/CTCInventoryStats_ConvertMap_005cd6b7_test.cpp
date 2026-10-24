// behaviour test for 005cd6b7: CTCInventoryStats::ConvertMap -- each (creature group, count) of the killed map becomes a
// (display name, count) list entry; groups whose display names compare equal are summed into one entry, new names are
// appended in map order, and every name string made along the way is destroyed
#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CWideString {
    const char* PStringData;
    explicit CWideString(const char* s) : PStringData(s) { ++g_live; }
    CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CWideString() { --g_live; }
    bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData) return true;
        if (PStringData == 0 || other.PStringData == 0) return false;
        return std::strcmp(PStringData, other.PStringData) == 0;
    }
};
struct Pair { CWideString first; long second; Pair(const CWideString& a, long b) : first(a), second(b) {} };
struct Node { Node* next; Node* prev; Pair* data; };
struct List {
    Node sentinel; Node pool[16]; int used;
    List() : used(0) { sentinel.next = sentinel.prev = &sentinel; }
    ~List() { for (Node* n = sentinel.next; n != &sentinel; n = n->next) delete n->data; }
    void push_back(const Pair& p)
    {
        Node* n = &pool[used++]; n->data = new Pair(p.first, p.second);
        n->prev = sentinel.prev; n->next = &sentinel; sentinel.prev->next = n; sentinel.prev = n;
    }
};
struct MapEntry { unsigned long key; long value; };
static char g_wolfA[] = "Wolf", g_wolfB[] = "Wolf", g_hobbe[] = "Hobbe";
static CWideString GetCreatureGroupAsString(unsigned long group)
{
    if (group == 1) return CWideString(g_wolfA);
    if (group == 2) return CWideString(g_hobbe);
    if (group == 3) return CWideString(g_wolfB);     /* a different buffer, equal text */
    return CWideString(0);
}
static void ConvertMap(const MapEntry* first, const MapEntry* last, List& out)
{
    for (const MapEntry* it = first; it != last; ++it) {
        CWideString name = GetCreatureGroupAsString(it->key);
        Node* end = &out.sentinel;
        bool found = false;
        for (Node* li = out.sentinel.next; li != end; li = li->next) {
            if (li->data->first == name) {
                li->data->second += it->value;
                found = true;
                break;
            }
        }
        if (!found)
            out.push_back(Pair(name, it->value));
    }
}
int main()
{
    int bad = 0;
    {
        MapEntry killed[5] = { { 1, 3 }, { 2, 5 }, { 3, 4 }, { 7, 1 }, { 9, 2 } };
        List out;
        ConvertMap(killed, killed + 5, out);
        Node* n = out.sentinel.next;
        if (n == &out.sentinel || n->data->first.PStringData != g_wolfA || n->data->second != 7) ++bad;   /* 3 + 4 */
        n = n->next;
        if (n == &out.sentinel || n->data->first.PStringData != g_hobbe || n->data->second != 5) ++bad;
        n = n->next;
        if (n == &out.sentinel || n->data->first.PStringData != 0 || n->data->second != 3) ++bad;          /* two null names */
        if (n->next != &out.sentinel || out.used != 3) ++bad;
        if (g_live != 3) ++bad;                 /* only the three list entries' names are alive */
        ConvertMap(killed, killed, out);        /* empty map: nothing changes */
        if (out.used != 3) ++bad;
    }
    if (g_live != 0) ++bad;
    if (bad) { std::printf("005cd6b7_TEST FAIL\n"); return 1; }
    std::printf("005cd6b7_TEST PASS\n");
    return 0;
}