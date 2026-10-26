// CCreatureAction_OpenChest::FinishAction() -- unless the chest is already open, post the "chest opened" message (type 72, radius 50, at the origin) to the world's message manager, then run the base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842B60, length 84 bytes.
// Callees: CCreatureAction_OpenChest::IsChestOpen @ 0x006925A0 (thiscall, bool; the manifest label of that address is propagated), CCreatureActionBase::GetWorld @ 0x00692940
// (thiscall; propagated STL label in the manifest), CMessageEventManager::AddMessage @ 0x00687540 on the world's manager at +0x60 (result dropped), CCreatureActionBase::FinishAction @ 0x00694430 (non-virtual).
// Shape: a 12-byte frame for the zero vector built by its inline constructor; the message's eight arguments pushed right to left.
class CThing;
class C3DVector { public:
    float x, y, z;
    C3DVector() : x(0), y(0), z(0) { }
};
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long distance, C3DVector* pos, CThing* source, CThing* extra, float a, float b, long c);   // 0x00687540 (landed naming, 0x00941550)
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessages;      // +0x60
};

class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00694430
    CWorld* GetWorld();                     // 0x00692940
};
class CCreatureAction_OpenChest : public CCreatureActionBase { public:
    virtual void FinishAction();
    bool IsChestOpen();                     // 0x006925A0
};

void CCreatureAction_OpenChest::FinishAction()
{
    if (!IsChestOpen()) {
        C3DVector origin;
        CWorld* world = GetWorld();          // named: retail fetches the world before pushing the arguments
        world->m_pMessages->AddMessage(72, 50, &origin, 0, 0, 0.0f, 0.0f, 0);
    }
    CCreatureActionBase::FinishAction();
}