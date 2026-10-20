// CQuestManager::ResetPositionOfAllQuestCards() -- while the hero lives, move every living in-limbo quest-card thing (active and completed lists) onto the hero's position through its physics component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AFCA0, length 221 bytes, plain `ret`.
// Quest cards are real things kept in limbo; whenever the hero is moved they are reset onto him so that a card dropped
// back into the world appears at his feet.
// Callees: CPlayerManager::GetMainPlayer 0x00449970 (g_pMainGameComponent 0x013B86A0 +0x1c), CPlayer::GetHeroThing
// 0x00487DC0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0 (const C3DVector&),
// CTCPhysicsBase::ResetPosition (vtable slot 31, +0x7c; RTTI vtable 0x0125D2C4 -> 0x006B01D0).
// Layout: retail CThing (PDB -0x10): PhysicsTC +0x60, Dead +0x91 bit 0, InLimbo +0x91 bit 5; retail CQuestManager
// ActiveQuestCards +0x5c and CompletedQuestCards +0x60 (4-byte STLport lists of CIntelligentPointer<CThing>, node data +8).
// Shape: the two walks are written out one after the other (each re-reads its list head for the end test); VC7.1
// fetches the physics vtable before the GetPosition call and parks it, and the hero, in the two frame words.
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);   // slot 31 (0x006B01D0 in the base)
};
struct CThing {                                  // retail CThing (local model: offsets PDB -0x10 past the containers)
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                   // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   Dead : 1;                    // +0x91 bit 0 (PDB +0xa1)
    unsigned char   CreatedByOwnerThingCreation : 1;
    unsigned char   IncludeInThingScans : 1;
    unsigned char   Activated : 1;
    unsigned char   GloballyLoaded : 1;
    unsigned char   InLimbo : 1;                 // bit 5
    unsigned char   KillLocked : 1;
    unsigned char   CreatedBySerialisation : 1;
    const C3DVector& GetPosition() const;        // 0x004C73D0
};
struct CPlayer {
    CThing* GetHeroThing() const;                // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                    // 0x00449970
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;               // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent; // 0x013B86A0

struct CBaseIntelligentPointer {                 // 8 bytes: vptr, item block
    void* PItem;
    virtual ~CBaseIntelligentPointer();
    CThing* GetPItem() const;                    // 0x00A01B50
};
struct CCardNode {                               // STLport list<CIntelligentPointer<CThing>> node
    CCardNode*              _M_next;
    CCardNode*              _M_prev;
    CBaseIntelligentPointer _M_data;             // +0x08
};
struct CQuestManager {
    unsigned char _pad00[0x5c];
    CCardNode*    ActiveQuestCards;              // +0x5c (PDB +0x88)
    CCardNode*    CompletedQuestCards;           // +0x60 (PDB +0x94)
    void ResetPositionOfAllQuestCards();
};

void CQuestManager::ResetPositionOfAllQuestCards()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero == 0 || hero->Dead)
        return;
    CCardNode* node;
    for (node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card && !card->Dead && card->InLimbo) {
            CTCPhysicsBase* physics = card->PhysicsTC;
            if (physics)
                physics->ResetPosition(hero->GetPosition());
        }
    }
    for (node = CompletedQuestCards->_M_next; node != CompletedQuestCards; node = node->_M_next) {
        CThing* card = node->_M_data.GetPItem();
        if (card && !card->Dead && card->InLimbo) {
            CTCPhysicsBase* physics = card->PhysicsTC;
            if (physics)
                physics->ResetPosition(hero->GetPosition());
        }
    }
}
