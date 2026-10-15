#include <cstdio>
#include <cstdlib>
template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; }
};
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* p) : ParentThing(p) { } virtual ~CTCBase() { } };
struct CWound { unsigned char _body[8]; };
struct CTCWound : CTCBase {
    unsigned long LastFrameChanged;
    CRetailList<CWound> Wounds;
    CTCWound(CThing* parent) : CTCBase(parent) , LastFrameChanged(0) { }
};
int main()
{
    CThing thing; thing.id = 2;
    CTCWound o(&thing);
    bool ok = o.ParentThing == &thing && o.Wounds.head != 0 && o.Wounds.head->next == o.Wounds.head && o.Wounds.head->prev == o.Wounds.head;
    ok = ok && o.LastFrameChanged == 0;
    std::free(o.Wounds.head);
    if (!ok) { std::printf("00757640_TEST FAIL\n"); return 1; }
    std::printf("00757640_TEST PASS\n");
    return 0;
}
