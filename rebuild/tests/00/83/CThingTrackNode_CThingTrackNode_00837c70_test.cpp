// Behaviour test for CThingTrackNode::CThingTrackNode (0x00837C70): type-10 base, empty name, cleared links and flags.
#include <cstdio>
#include <cstring>
#include <new>
static long g_type = -1;
struct CBaseIntelligentPointer { void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) {} };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() : CBaseIntelligentPointer() {} };
struct CCharString { const char* text; CCharString() : text("") {} };
struct CThing { long type; CThing(long t) : type(t) { g_type = t; } };
struct CThingGameObject : CThing {
    CIntelligentPointer m_owner;
    CThingGameObject(long type) : CThing(type), m_owner() {}
};
struct CThingTrackNode : CThingGameObject {
    CThingTrackNode* PNextNode;
    CThingTrackNode* PPreviousNode;
    unsigned long long NextNodeUID;
    unsigned long long PreviousNodeUID;
    CCharString      TrackName;
    bool             IsStart;
    bool             IsEnd;
    CThingTrackNode() : CThingGameObject(10)
    {
        PNextNode = 0; PPreviousNode = 0; NextNodeUID = 0; PreviousNodeUID = 0; IsStart = false; IsEnd = false;
    }
};
int main()
{
    static unsigned char raw[sizeof(CThingTrackNode)];
    std::memset(raw, 0xAB, sizeof(raw));
    CThingTrackNode* n = new (raw) CThingTrackNode;
    if (g_type != 10 || n->type != 10 || n->PNextNode || n->PPreviousNode || n->NextNodeUID || n->PreviousNodeUID || n->IsStart || n->IsEnd || n->TrackName.text[0] || n->m_owner.m_pItem) { std::printf("00837c70_TEST FAIL\n"); return 1; }
    std::printf("00837c70_TEST PASS\n");
    return 0;
}