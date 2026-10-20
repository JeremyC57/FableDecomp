// CTCWife::Murdered() -- a married wife's murder unmarries her, posts opinion deed 0x3b (wife murdered) at her position to the living hero's deed log and sets his marriage state to 7 (widowed by murder)
// Grade: DIFFER(ours v retail) 217v217 -- the final SetMarriageState argument is loaded before the component (`mov ecx,[ebx+4] ; push 7 ; push ecx ; mov ecx,[eax+4]`) where retail loads the component into ecx first and the parent into eax
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D00A0, 217 bytes (+7 int3). __fastcall, `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; CPlayerManager::GetMainPlayer()
//          0x00449970; CPlayer::GetHeroThing() 0x00487DC0; CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020;
//          CThing::GetPosition() 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed(...) 0x006C6990;
//          CTCHeroStats::SetMarriageState(CThingAICreature*, EMarriageState) 0x0057BC6E. Global g_pMainGameComponent 0x013B86A0.
// Shape: the same guard as Died/OnGreet (FrameGotMarriedToThePlayer must not be in the future); the player manager is read
//        from the main game component's +0x1C as in Died; two out-parameter GetInterface lookups (0x51 = CTCHeroOpinionDeedLog,
//        bitset word 2 bit 17, a named local kept in edi across GetPosition; 4 = CTCHeroStats, byte +0x20 bit 4, the HasTC +
//        GetTC(...)->Call() form that lands the component straight in ecx) share the single `push ecx` key word; the
//        position is fetched on the parent after the constant pushes (natural right-to-left evaluation).
// Layout: CTCWife ParentThing +0x04, FrameGotMarriedToThePlayer +0x20; CMainGameComponent PlayerManager +0x1C; retail CThing
//         bitset +0x20, interface map +0x44, Dead +0x91.
struct C3DVector { float X, Y, Z; };
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const unsigned long& type);   // 0x0040F020 (the other TU copy is 0x004365B0; the key by address)
    __forceinline CTCBase* Find(long type)      // the inlined map `find`: lower bound on a key temporary, end when absent
    {
        SEntry* it = LowerBound((unsigned long)type);
        if (it == last || type < it->type)
            it = last;
        return it->component;
    }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)m_interfaces.Find(interfaceType);
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* GetHeroThing() const;               // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CMainGameComponent {
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;               // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
struct CFaction;
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deedType, const C3DVector* position, CThing* thing, CThing* creature, long postType,
                  bool b1, bool b2, CFaction* faction, bool b3, long value);   // 0x006C6990
};
struct CTCHeroStats {
    void SetMarriageState(CThing* spouse, long state);   // 0x0057BC6E
};
__forceinline bool HasTC(CThing* thing, long type)
{
    if (thing->m_presentBits[type >> 5] & (1UL << (type & 31)))
        return true;
    return false;
}
template <class T> __forceinline T* GetTC(CThing* thing, long type)
{
    return (T*)thing->m_interfaces.Find(type);
}
struct CTCWife {
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;             // +0x20
    void Unmarry(bool notify);                   // 0x006CFF10
    void Murdered();
};
void CTCWife::Murdered()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetWorldFrame())
        return;
    Unmarry(true);
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
    if (!hero || (hero->Flags91 & 1))
        return;
    CTCHeroOpinionDeedLog* deedLog;
    if (hero->GetInterface(&deedLog, 0x51))
        deedLog->PostDeed(0x3b, &ParentThing->GetPosition(), 0, 0, 7, false, false, 0, false, 0);
    if (HasTC(hero, 4))
        GetTC<CTCHeroStats>(hero, 4)->SetMarriageState(ParentThing, 7);
}
