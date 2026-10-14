// CDBMinion::OnPredicateFail() -- one of the parent script's minion counters is decremented, the minion thing record is fetched from the thing manager and its timer word (+0x14) reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D25EF0, length 64 bytes.
// Callees: the thing manager global @ 0x0143E8F8, vtable slot 90 (+0x168: a record for the thing id, one argument) and
// slot 89 (+0x164: two arguments, the thing id and the address of the record's +0x14 word).
// Shape: ParentClass (+0x14, PDB CDBMinion) is re-read for every use (the virtual calls could alias it); the thing id lives in a
// 4-byte CScriptThing member at +0x94 of the parent and is read through its inline accessor -- the first read materialises
// the member's address (`add eax,0x94 ; mov eax,[eax]`) because the accessor is applied to a reference, the second is a
// plain `mov eax,[eax+0x94]`. The counter at +0x74 is a plain long. The sibling @ 0x00D25D20 uses the
// next counter/thing pair (+0x70/+0x90); the two bodies are the OnPredicateFail of two CDBMinion state classes.
struct CMinionRecord {
    unsigned char _pad[0x14];
    long m_timer;                              // +0x14
};

struct CThingManager_Calls {             // retail vtable of the thing manager global @ 0x0143E8F8
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void  ResetMinionTimer(long thingId, long* timerSlot);   // slot 89 (+0x164)
    virtual CMinionRecord* GetMinionRecord(long thingId);         // slot 90 (+0x168)
};
extern CThingManager_Calls* g_pThingManager_0143e8f8;   // 0x0143E8F8

struct CScriptThing {                          // 4 bytes: the thing id/handle
    long m_thingId;
    const long& GetThingId() const { return m_thingId; }
};

struct CQ_DragonBossFightScript {             // the parent quest script (PDB: CDBMinion::ParentClass)
    unsigned char _pad[0x74];
    long          m_minionCount;               // +0x74
    unsigned char _pad2[0x94 - 0x74 - 4];
    CScriptThing  m_minion;                    // +0x94
};

struct CDBMinion {
    unsigned char             _pad[0x14];
    CQ_DragonBossFightScript* ParentClass;     // +0x14 (PDB)
    void OnPredicateFail();
};

void CDBMinion::OnPredicateFail()
{
    --ParentClass->m_minionCount;
    const CScriptThing& minion = ParentClass->m_minion;
    CMinionRecord* record = g_pThingManager_0143e8f8->GetMinionRecord(minion.GetThingId());
    g_pThingManager_0143e8f8->ResetMinionTimer(ParentClass->m_minion.m_thingId, &record->m_timer);
}