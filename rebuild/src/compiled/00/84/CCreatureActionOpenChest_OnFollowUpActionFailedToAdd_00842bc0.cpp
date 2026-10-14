// CCreatureAction_OpenChest::OnFollowUpActionFailedToAdd() -- post the "chest opened" message (type 72, radius 30, at the origin) to the world's message manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842BC0, length 61 bytes.
// Callees: CCreatureActionBase::GetWorld @ 0x00692940 (thiscall on the unchanged ecx), CMessageEventManager::AddMessage @ 0x00687540 on the world's manager at +0x60 (result dropped).
// Shape: a 12-byte frame for the zero vector built by its inline constructor; the message's eight arguments pushed right to left; no base call.
struct CThing;
struct C3DVector {
    float x, y, z;
    C3DVector() : x(0), y(0), z(0) { }
};
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long distance, C3DVector* pos, CThing* source, CThing* extra, float a, float b, long c);   // 0x00687540 (landed naming, 0x00941550)
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessages;      // +0x60
};

struct CCreatureActionBase {
    virtual void v00();
    CWorld* GetWorld();                     // 0x00692940
};
struct CCreatureAction_OpenChest : CCreatureActionBase {
    void OnFollowUpActionFailedToAdd();
};

void CCreatureAction_OpenChest::OnFollowUpActionFailedToAdd()
{
    C3DVector origin;
    CWorld* world = GetWorld();              // named: retail fetches the world before pushing the arguments
    world->m_pMessages->AddMessage(72, 30, &origin, 0, 0, 0.0f, 0.0f, 0);
}