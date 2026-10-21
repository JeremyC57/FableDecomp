// Behaviour test for 0048a210 (CPlayer::PlayerInit): the null mode lands at the front of the mode list, the init block
// is copied, flags are reset, and only a local player gets its interface and joystick device (main player -> main device).
#include <cstdio>
#include <cstdlib>

struct ModeNode { ModeNode* next; ModeNode* prev; int mode; };
struct Init { int component, world, defs, number, defType; bool local; int manager, iface; bool character; };
struct Player {
    bool valid, character, local, initialModeSet, aggressive;
    int component, world, number, manager, iface, defType, device, respawn, controlled, guiBuilt;
    ModeNode head;
    Player() { head.next = &head; head.prev = &head; head.mode = -1; valid = false; device = 99; guiBuilt = 0; controlled = 7; aggressive = true; }
    void pushFront(int mode)
    {
        ModeNode* position = head.next;
        ModeNode* node = (ModeNode*)std::malloc(sizeof(ModeNode));
        node->mode = mode;
        ModeNode* prev = position->prev;
        node->next = position; node->prev = prev; prev->next = node; position->prev = node;
    }
    void initDevice(int mainPlayer, int mainDevice)
    {
        if (number == mainPlayer) device = mainDevice;
        else if (number == mainDevice) device = 0;
        else device = number;
    }
    bool playerInit(const Init* in, int mainPlayer, int mainDevice)
    {
        valid = true;
        pushFront(0);
        character = in->character; component = in->component; world = in->world; number = in->number;
        local = in->local; manager = in->manager; defType = in->defType; device = -1;
        controlled = 0; aggressive = false; respawn = 0; initialModeSet = true; iface = 0;
        if (local) { iface = in->iface; initDevice(mainPlayer, mainDevice); guiBuilt = 1; }
        return true;
    }
};
int main()
{
    int ok = 1;
    Init a = { 1, 2, 3, 0, 5, true, 6, 7, true };
    Player p; p.pushFront(4);
    if (!p.playerInit(&a, 0, 2)) ok = 0;
    if (p.head.next->mode != 0 || p.head.next->next->mode != 4 || p.head.prev->mode != 4) ok = 0;
    if (!p.valid || !p.character || p.iface != 7 || p.device != 2 || !p.guiBuilt || p.controlled || p.aggressive) ok = 0;
    Init b = { 1, 2, 3, 2, 5, true, 6, 9, false };
    Player q; q.playerInit(&b, 0, 2);
    if (q.device != 0) ok = 0;
    Init c = { 1, 2, 3, 3, 5, false, 6, 9, false };
    Player r; r.playerInit(&c, 0, 2);
    if (r.device != -1 || r.iface != 0 || r.guiBuilt || !r.initialModeSet) ok = 0;
    if (!ok) { std::printf("0048a210_TEST FAIL\n"); return 1; }
    std::printf("0048a210_TEST PASS\n");
    return 0;
}
