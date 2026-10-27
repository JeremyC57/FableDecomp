#include <cstdio>
// Behaviour model: the visitor carries the walking object and is applied to every list item.
struct Owner;
static int visits = 0; static Owner* seen = 0;
struct Visitor { Owner* o; Visitor(Owner* p) : o(p) { } void Visit(int) { ++visits; seen = o; } };
struct List { int items[3]; void ForEach(Visitor& v) { for (int i = 0; i < 3; ++i) v.Visit(items[i]); } };
struct Owner { List l; void Walk() { Visitor v(this); l.ForEach(v); } };
int main()
{
    Owner o; o.Walk();
    if (visits != 3 || seen != &o) { std::printf("00c47210_TEST FAIL\n"); return 1; }
    std::printf("00c47210_TEST PASS\n");
    return 0;
}