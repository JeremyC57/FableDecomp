#include <cstdio>
#include <cstdlib>
template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; }
};
struct CThing { int id; };
struct CTCBase { CThing* ParentThing; CTCBase(CThing* p) : ParentThing(p) { } virtual ~CTCBase() { } };
struct CQuestReward { unsigned char _body[8]; };
struct CTCHeroGuide : CTCBase {
    
    CRetailList<CQuestReward> QuestRewards;
    CTCHeroGuide(CThing* parent) : CTCBase(parent)  { }
};
int main()
{
    CThing thing; thing.id = 2;
    CTCHeroGuide o(&thing);
    bool ok = o.ParentThing == &thing && o.QuestRewards.head != 0 && o.QuestRewards.head->next == o.QuestRewards.head && o.QuestRewards.head->prev == o.QuestRewards.head;
    
    std::free(o.QuestRewards.head);
    if (!ok) { std::printf("00780070_TEST FAIL\n"); return 1; }
    std::printf("00780070_TEST PASS\n");
    return 0;
}