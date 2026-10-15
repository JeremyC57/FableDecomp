#include <cstdio>
#include <cstdlib>
static int g_unloads = 0, g_notified = 0, g_deleted = 0;
struct Tree { long lastUnload; void OnNavMapUnload(long i) { lastUnload = i; ++g_unloads; } };
struct Info { unsigned int RefCount; };
struct Map {
    Tree* Data; Info* PInfo; long Index;
    ~Map() { if (PInfo) { --PInfo->RefCount; if (PInfo->RefCount == 0) { ++g_deleted; delete PInfo; } } Data = 0; PInfo = 0; }
};
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct MapNode : NodeBase { Map data; };
struct Nav { void OnNavigationMapRemoved() { ++g_notified; } };
struct NavNode : NodeBase { Nav* data; };
struct Manager {
    NodeBase* navs; NodeBase* maps;
    void RemoveNavigationMap(long index)
    {
        NodeBase* found = maps;
        for (NodeBase* it = maps->next; it != maps; it = it->next) {
            Map& m = ((MapNode*)it)->data;
            if (m.Index == index) found = it; else m.Data->OnNavMapUnload(index);
        }
        if (found != maps) {
            NodeBase* next = found->next; NodeBase* prev = found->prev;
            prev->next = next; next->prev = prev;
            ((MapNode*)found)->data.~Map();
            std::free(found);
        }
        for (NodeBase* n = navs->next; n != navs; n = n->next) ((NavNode*)n)->data->OnNavigationMapRemoved();
    }
};
static void Link(NodeBase* head, NodeBase* n) { n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; }
int main()
{
    NodeBase mh, nh; mh.next = mh.prev = &mh; nh.next = nh.prev = &nh;
    Tree trees[3]; long idx[3] = { 4, 7, 9 };
    for (int i = 0; i < 3; ++i) {
        MapNode* n = (MapNode*)std::malloc(sizeof(MapNode)); n->data.Data = &trees[i]; n->data.PInfo = new Info; n->data.PInfo->RefCount = 1; n->data.Index = idx[i];
        trees[i].lastUnload = 0; Link(&mh, n);
    }
    Nav nv[2]; NavNode nn[2]; for (int i = 0; i < 2; ++i) { nn[i].data = &nv[i]; Link(&nh, &nn[i]); }
    Manager m; m.navs = &nh; m.maps = &mh;
    m.RemoveNavigationMap(7);
    int count = 0; for (NodeBase* n = mh.next; n != &mh; n = n->next) ++count;
    bool fail = count != 2 || g_unloads != 2 || trees[0].lastUnload != 7 || trees[2].lastUnload != 7 || trees[1].lastUnload != 0 || g_deleted != 1 || g_notified != 2;
    m.RemoveNavigationMap(55);
    count = 0; for (NodeBase* n = mh.next; n != &mh; n = n->next) ++count;
    if (count != 2 || g_unloads != 4 || g_notified != 4) fail = true;
    if (fail) { std::printf("00a155d0_TEST FAIL\n"); return 1; }
    std::printf("00a155d0_TEST PASS\n");
    return 0;
}
