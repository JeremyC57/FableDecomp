// Behaviour test for NPlayerGui::CDrawQuestInfo::GetElementByIndex (0x00644FC5), a portable model of the row: the first element in
// list order whose ID matches, else null.
#include <stdio.h>
#include <list>
struct Elem { long ID; };
static const Elem* GetElementByIndex(const std::list<const Elem*>& elements, long index)
{
    for (std::list<const Elem*>::const_iterator it = elements.begin(); it != elements.end(); ++it)
        if ((*it)->ID == index)
            return *it;
    return 0;
}
int main()
{
    int bad = 0;
    Elem a = { 3 }, b = { 7 }, c = { 7 };
    std::list<const Elem*> l; l.push_back(&a); l.push_back(&b); l.push_back(&c);
    if (GetElementByIndex(l, 3) != &a) ++bad;
    if (GetElementByIndex(l, 7) != &b) ++bad;          // first match wins
    if (GetElementByIndex(l, 9) != 0) ++bad;
    if (GetElementByIndex(std::list<const Elem*>(), 3) != 0) ++bad;
    printf(bad ? "00644fc5_TEST FAIL\n" : "00644fc5_TEST PASS\n");
    return bad ? 1 : 0;
}
