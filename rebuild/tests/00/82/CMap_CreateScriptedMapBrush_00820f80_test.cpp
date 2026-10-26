// Behaviour test for 0x00820F80 CMap::CreateScriptedMapBrush: the brush is appended to the list and registered under the map ID.
#include <cstdio>
class C2DBoxI { public: int TLX, TLY, BRX, BRY; };
struct Brush { int area; Brush(const C2DBoxI& a) : area((a.BRX - a.TLX) * (a.BRY - a.TLY)) { } };
struct Node { Node* next; Node* prev; Brush data; Node(const Brush& b) : next(0), prev(0), data(b) { } };
static int g_regID = 0; static Brush* g_regBrush = 0;
class CMap { public:
    Node* header; int worldPosX;
    Brush* CreateScriptedMapBrush(const C2DBoxI& area)
    {
        Brush temp(area);
        Node* pos = header; Node* tmp = new Node(temp); Node* prev = pos->prev;
        tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp;
        Node* last = header->prev;
        int mapID = worldPosX * 10;
        g_regID = mapID; g_regBrush = &last->data;
        return &header->prev->data;
    }
};
int main()
{
    C2DBoxI z = { 0, 0, 0, 0 };
    Node head(z); head.next = &head; head.prev = &head;
    CMap m; m.header = &head; m.worldPosX = 7;
    C2DBoxI a = { 1, 2, 4, 6 };
    Brush* b = m.CreateScriptedMapBrush(a);
    C2DBoxI c = { 0, 0, 2, 2 };
    Brush* b2 = m.CreateScriptedMapBrush(c);
    bool ok = b->area == 12 && b2->area == 4 && head.next == (Node*)((char*)b - (char*)&((Node*)0)->data) && g_regID == 70 && g_regBrush == b2 &&
              head.prev->data.area == 4 && head.next->next == head.prev;
    delete head.next; delete head.prev;
    if (!ok) { std::printf("00820f80_TEST FAIL\n"); return 1; }
    std::printf("00820f80_TEST PASS\n");
    return 0;
}