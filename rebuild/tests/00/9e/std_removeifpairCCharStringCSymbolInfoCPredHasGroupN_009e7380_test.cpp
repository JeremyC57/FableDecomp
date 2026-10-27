// Test for 0x009E7380 std::remove_if<pair<CCharString, CSymbolInfo>*, CPred_HasGroupName>: removes every symbol of the named
// group, keeps the others in order, returns the new end; no match returns last.
#include <stdio.h>
#include <string.h>
struct CSymbolInfo { int Value; const char* GroupName; };
struct Entry { const char* first; CSymbolInfo second; };
class CPred_HasGroupName {
public:
    const char* Name;
    bool operator()(const Entry& e) const { return strcmp(e.second.GroupName, Name) == 0; }
};
static Entry* find_if(Entry* f, Entry* l, CPred_HasGroupName p) { while (f != l && !p(*f)) ++f; return f; }
static Entry* remove_copy_if(Entry* f, Entry* l, Entry* r, CPred_HasGroupName p) { for (; f != l; ++f) if (!p(*f)) *r++ = *f; return r; }
static Entry* remove_if(Entry* first, Entry* last, CPred_HasGroupName pred)
{
    first = find_if(first, last, pred);
    if (first == last) return first;
    Entry* next = first;
    return remove_copy_if(++next, last, first, pred);
}
int main()
{
    Entry e[5] = { { "A", { 1, "G1" } }, { "B", { 2, "G2" } }, { "C", { 3, "G1" } }, { "D", { 4, "G3" } }, { "E", { 5, "G1" } } };
    CPred_HasGroupName p; p.Name = "G1";
    Entry* end = remove_if(e, e + 5, p);
    bool ok = end == e + 2 && strcmp(e[0].first, "B") == 0 && strcmp(e[1].first, "D") == 0;
    p.Name = "none";
    if (remove_if(e, e + 2, p) != e + 2) ok = false;
    puts(ok ? "009e7380_TEST PASS" : "009e7380_TEST FAIL");
    return ok ? 0 : 1;
}