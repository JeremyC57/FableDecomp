#include <cstdio>
#include <cstring>

struct CThing { int id; };
struct SContainerItem;
struct CTCBase { CThing* ParentThing; CTCBase(CThing& o) : ParentThing(&o) { } };
struct CContainerItemVector { SContainerItem* m_first; SContainerItem* m_last; SContainerItem* m_end; CContainerItemVector() : m_first(0), m_last(0), m_end(0) { } };
struct CTCContainer : CTCBase { CContainerItemVector ContainerItems; CTCContainer(CThing& o) : CTCBase(o) { } };
struct CTCContainerRewardHero : CTCContainer {
    bool Enabled, CreateObjectsThatTimeOut;
    CTCContainerRewardHero(CThing& o) : CTCContainer(o), Enabled(true), CreateObjectsThatTimeOut(true) { }
};
int main()
{
    CThing t; t.id = 1;
    CTCContainerRewardHero c(t);
    if (c.ParentThing != &t || c.ContainerItems.m_first || c.ContainerItems.m_last || c.ContainerItems.m_end || !c.Enabled || !c.CreateObjectsThatTimeOut) { std::printf("0074e6b0_TEST FAIL\n"); return 1; }
    std::printf("0074e6b0_TEST PASS\n");
    return 0;
}
