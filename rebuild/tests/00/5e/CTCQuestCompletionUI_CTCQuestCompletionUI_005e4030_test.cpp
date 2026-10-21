// behaviour test for 005e4030 CTCQuestCompletionUI ctor: initial member values, self-linked list sentinel and map headers, registration
#include <cstdio>
#include <cstdlib>
static int g_registered = 0;
struct CTCBase { void* parent; bool toBeRemoved; CTCBase(void* p) : parent(p), toBeRemoved(false) {} void AddToGlobalUpdateTCs() { ++g_registered; } };
struct Node { Node* next; Node* prev; char boast[0x20]; };
struct MapHeader { char colour; MapHeader* parent; MapHeader* left; MapHeader* right; long k; unsigned long v; };
struct Map {
    MapHeader* header; unsigned long count;
    Map() : header(0) { header = (MapHeader*)std::malloc(sizeof(MapHeader)); count = 0; header->colour = 0; header->parent = 0; header->left = header; header->right = header; }
};
struct CUI : CTCBase {
    bool Active, ReallyActive; void* comps[3]; bool Quitting, WaitRegion, WaitPlayer;
    unsigned long Frame; long FrameStarted; void* PCardDef; Node* head; double LastRenderTime;
    long Gold, Renown; float GoldShown; Map Exp, GoldMap, RenownMap; bool CanTeleport;
    CUI(void* p) : CTCBase(p), Active(false), ReallyActive(false), Quitting(false), WaitRegion(false), WaitPlayer(false),
        Frame(0), FrameStarted(-1), PCardDef(0), head(0), LastRenderTime(0.0), Gold(0), Renown(0), GoldShown(0), CanTeleport(true)
    {
        comps[0] = comps[1] = comps[2] = 0;
        Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; head = s;
        AddToGlobalUpdateTCs();
    }
};
int main()
{
    CUI ui((void*)&g_registered);
    if (g_registered != 1 || ui.FrameStarted != -1 || ui.Frame != 0 || !ui.CanTeleport || ui.Active) { std::printf("005e4030_TEST FAIL\n"); return 1; }
    if (ui.head->next != ui.head || ui.Exp.header->left != ui.Exp.header || ui.RenownMap.count != 0 || ui.GoldMap.header->parent != 0) { std::printf("005e4030_TEST FAIL\n"); return 1; }
    if (ui.LastRenderTime != 0.0 || ui.comps[2] != 0) { std::printf("005e4030_TEST FAIL\n"); return 1; }
    std::printf("005e4030_TEST PASS\n");
    return 0;
}