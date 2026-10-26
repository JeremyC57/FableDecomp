#include <cstdio>
#include <cstdlib>
template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; }
};
class CThing { public: int id; };
struct CBase { int tag; CBase() : tag(5) { } virtual ~CBase() { } };
struct CGraphicDataBankInfo { unsigned char _body[8]; };
class CGraphicBankManager : public CBase { public:
    
    CRetailList<CGraphicDataBankInfo> GraphicDataBanks;
    CGraphicBankManager()   { }
};
int main()
{
    CThing thing; thing.id = 2;
    CGraphicBankManager o;
    bool ok = o.tag == 5 && o.GraphicDataBanks.head != 0 && o.GraphicDataBanks.head->next == o.GraphicDataBanks.head && o.GraphicDataBanks.head->prev == o.GraphicDataBanks.head;
    
    std::free(o.GraphicDataBanks.head);
    if (!ok) { std::printf("009f8690_TEST FAIL\n"); return 1; }
    std::printf("009f8690_TEST PASS\n");
    return 0;
}