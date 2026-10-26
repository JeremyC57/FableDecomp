#include <cstdio>
#include <cstdlib>
template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; }
};
class CThing { public: int id; };
class CTCBase { public: CThing* ParentThing; CTCBase(CThing* p) : ParentThing(p) { } virtual ~CTCBase() { } };
struct CIntelligentPointer_CThing { unsigned char _body[8]; };
class CTCRegionFollower : public CTCBase { public:
    
    CRetailList<CIntelligentPointer_CThing> Followers;
    CTCRegionFollower(CThing* parent) : CTCBase(parent)  { }
};
int main()
{
    CThing thing; thing.id = 2;
    CTCRegionFollower o(&thing);
    bool ok = o.ParentThing == &thing && o.Followers.head != 0 && o.Followers.head->next == o.Followers.head && o.Followers.head->prev == o.Followers.head;
    
    std::free(o.Followers.head);
    if (!ok) { std::printf("006aed80_TEST FAIL\n"); return 1; }
    std::printf("006aed80_TEST PASS\n");
    return 0;
}