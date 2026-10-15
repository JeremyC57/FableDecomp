// NCreatureMode::CModeScorpionKingExhausted::Recover() -- the creature (+0x78) sent underground (virtual slot 28 with true) and given a CCreatureAction_GoUnderground (0x008E4770, a 0x114-byte temporary on the frame) as its current action (SetCurrentAction @ 0x006644F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DF640, length 58 bytes.
// Callees: the creature's virtual slot 28 (+0x70), CCreatureAction_GoUnderground::CCreatureAction_GoUnderground(CThingCreatureBase*) @ 0x008E4770, CThingCreatureBase::SetCurrentAction @ 0x006644F0 (the action by reference: the explicit temporary reuses the constructor's returned `this` in eax), CCreatureAction_GoUnderground::~CCreatureAction_GoUnderground @ 0x0062E900 (labelled ~CTCLightningOrb, propagated). Shape: a 0x114 frame, `this` in esi, the creature re-read from +0x78 for every use, `ret`.
namespace NCreatureMode {
struct CCreatureActionBase;
struct CThingCreatureBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetUnderground(bool underground); // slot 28 (+0x70)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CCreatureActionBase {
    virtual void v00();
};
struct CCreatureAction_GoUnderground : CCreatureActionBase {   // 0x114 bytes
    unsigned char _body[0x110];
    CCreatureAction_GoUnderground(CThingCreatureBase* creature);   // 0x008E4770
    ~CCreatureAction_GoUnderground();              // 0x0062E900
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    void* m_pThing;                                // +0x04
};

struct CModeScorpionKingExhausted : CModeBase {
    unsigned char       _pad08[0x70];              // +0x08 .. +0x78
    CThingCreatureBase* m_pCreature;               // +0x78
    void Recover();
};

void CModeScorpionKingExhausted::Recover()
{
    m_pCreature->SetUnderground(true);
    m_pCreature->SetCurrentAction(CCreatureAction_GoUnderground(m_pCreature));
}
}
