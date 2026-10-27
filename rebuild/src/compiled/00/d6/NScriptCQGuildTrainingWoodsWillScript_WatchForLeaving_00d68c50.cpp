// NScript::CQ_GuildTrainingWoodsWillScript::WatchForLeaving() -- script thread: while the hero lives and the mission is still open run frames; then (unless terminated or already succeeded) fail the mission
// Leaf: ?WatchForLeaving@CQ_GuildTrainingWoodsWillScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d68c50, 114 bytes, thiscall, plain `ret`.
// Identity (probable): CQ_GuildTrainingWoodsWillScript's DoMission 0x00D68AE0 ("first objective", "GuildWoods"; it sits after the landed CWillWhisper::Main 0x00D68810, whose catalogue span the manifest stretches over it) spawns it as the thread "WatchForLeaving" (functor store at 0x00D68BE5); the flags are Ego_r.pdb CQ_GuildTrainingWoodsWillScript::MissionSucceeded / MissionFailed (PDB +0x5C / +0x5D, retail +0x48 / +0x49).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the game script interface (+0x40): GetHero (slot 70,
//   +0x118, const CScriptThing&) and NewScriptFrame (slot 7, +0x1c); CScriptThing::IsAlive (slot 75, +0x12c).
// Shape (family L143): the `while` is rotated (the hero test before the loop and at its bottom, the flag tests at its
//   head); the closing `if` re-tests the thread and MissionSucceeded.
class CScriptThing {                                // 12 bytes, vtable 0x01238C8C
public:
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
    virtual bool IsAlive() const;                    // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void NewScriptFrame() const;             // slot 7 (+0x1c)
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
    virtual const CScriptThing& GetHero() const;     // slot 70 (+0x118)
};
class CScriptBase {                                 // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                      // +0x04 .. +0x40
    bool IsActiveThreadTerminating();               // 0x00CB7940
};
namespace NScript {
class CQ_GuildTrainingWoodsWillScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;             // +0x40
    void*                 m_pOwner;                 // +0x44
    bool                  MissionSucceeded;         // +0x48
    bool                  MissionFailed;            // +0x49
    void WatchForLeaving();
};
void CQ_GuildTrainingWoodsWillScript::WatchForLeaving()
{
    while (m_pInterface->GetHero().IsAlive() && !MissionFailed && !MissionSucceeded) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}