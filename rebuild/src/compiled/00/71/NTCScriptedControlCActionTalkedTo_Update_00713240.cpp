// NTCScriptedControl::CActionTalkedTo::Update() -- keeps the talker's script thing resolved, then finishes the action when the talker is no longer talking or the target intelligent pointer has dropped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713240, length 65 bytes.
// Callees: CScriptThing virtual slots 77 (`call [eax+0x134]`, byte), 71 (`call [edx+0x11c]`) and 76
// (`call [eax+0x130]`, byte) on the member at +0x14; CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on
// the member at +0xc. The finished flag is the byte at +9 of the CAction base.
// Shape: the script-thing address is formed once (`lea esi,[edi+0x14]`) and reused for the three
// virtual calls; the second condition is an `||` (slot 76 true, or a null target) whose true paths
// both reach the flag store.
class CBaseObject;

class CScriptThing { public:
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
    virtual void Resolve();   // slot 71 (+0x11c)
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual unsigned char HasFinishedTalking();   // slot 76 (+0x130)
    virtual unsigned char IsResolved();   // slot 77 (+0x134)
    void* PImp_Data;                                    // +0x04
    void* PImp_Info;                                    // +0x08
};

struct CIntelligentPointer {
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem();                            // 0x00A01B50
};

struct CActionTalkedTo {
    virtual void v00();
    void*               m_pThing;                       // +0x04
    bool                m_active;                       // +0x08
    bool                m_finished;                     // +0x09
    unsigned char       _pad[0x2];
    CIntelligentPointer m_target;                       // +0x0c
    CScriptThing        m_talker;                       // +0x14
    void Update();
};

void CActionTalkedTo::Update()
{
    if (!m_talker.IsResolved())
        m_talker.Resolve();
    if (m_talker.HasFinishedTalking() || m_target.GetPItem() == 0)
        m_finished = true;
}