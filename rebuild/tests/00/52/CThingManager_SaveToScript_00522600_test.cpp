// Behaviour test for CThingManager::SaveToScript (0x00522600): version line appended, only things on the map that are
// not force-no-serialise and want serialising are collected, then handed to the writer in mode 2.
#include <cstdio>
#include <cstring>

class CThing { public:
    short CurrentMapNumber; bool ForceNo; bool Serialise;
    bool IsForceNoSerialise() { return ForceNo; }
    bool IsToBeSerialised() { return Serialise; }
};
struct SThingListNode { SThingListNode* _M_next; SThingListNode* _M_prev; CThing* _M_data; };
struct CThingVector {
    CThing* Items[8]; int Count;
    CThingVector() : Count(0) { }
    void push_back(CThing* t) { Items[Count++] = t; }
};
static char g_script[64];
static CThing* g_saved[8]; static int g_savedCount; static long g_mode;
class CThingManager { public:
    SThingListNode* Things; long SerialiseVersionNumber;
    void SaveThingList(char* script, long mode, CThingVector& things, void*)
    {
        (void)script; g_mode = mode; g_savedCount = things.Count;
        for (int i = 0; i < things.Count; ++i) g_saved[i] = things.Items[i];
    }
    void SaveToScript(char* script, long mapNumber, void* names)
    {
        char num[16]; std::sprintf(num, "%ld", SerialiseVersionNumber);
        std::strcat(script, "Version "); std::strcat(script, num); std::strcat(script, ";\r\n");
        CThingVector things;
        for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
            CThing* thing = node->_M_data;
            if (thing->CurrentMapNumber == mapNumber && !thing->IsForceNoSerialise() && thing->IsToBeSerialised())
                things.push_back(thing);
        }
        SaveThingList(script, 2, things, names);
    }
};

int main()
{
    CThing a = { 3, false, true }, b = { 4, false, true }, c = { 3, true, true }, d = { 3, false, false }, e = { 3, false, true };
    SThingListNode head, n[5]; CThing* items[5] = { &a, &b, &c, &d, &e };
    head._M_next = &n[0];
    for (int i = 0; i < 5; ++i) { n[i]._M_data = items[i]; n[i]._M_next = i < 4 ? &n[i + 1] : &head; }
    CThingManager m; m.Things = &head; m.SerialiseVersionNumber = 7;
    g_script[0] = 0;
    m.SaveToScript(g_script, 3, 0);
    bool ok = std::strcmp(g_script, "Version 7;\r\n") == 0 && g_mode == 2 && g_savedCount == 2 && g_saved[0] == &a && g_saved[1] == &e;
    if (!ok) { std::printf("00522600_TEST FAIL\n"); return 1; }
    std::printf("00522600_TEST PASS\n");
    return 0;
}