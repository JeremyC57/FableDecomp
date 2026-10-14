// NTCScriptedControl::CActionLeadCreature::OnActivate() -- on first activation remembers the creature's movement-lock state and clears it, then removes any movement-blocking modes if the creature cannot move
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007122D0, length 78 bytes.
// Callees: virtual slots 83 (`call [edx+0x14c]`, byte result) and 84 (`call [edx+0x150]`, one argument)
// of the creature's sub-object at +0x60; IsAbleToMove @ 0x0085F3E0 (__fastcall(CThing*), bool);
// RemoveMovementBlockingModes @ 0x0085F600 (__fastcall(CThing*), void; retail tail-jumps to it).
// Layout: +4 the owning scripted control (its +4 is the creature), +0x24 the "state saved" flag,
// +0x25 the saved byte. The creature pointer is re-read through the control for every use (no
// named local), which is what the retail loads show.
struct CThing;

struct CMovementController {
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
    virtual unsigned char GetMovementLocked();   // slot 83 (+0x14c)
    virtual void SetMovementLocked(long locked);   // slot 84 (+0x150)
};

struct CThing {
    unsigned char        _pad[0x60];
    CMovementController* m_pMovement;                   // +0x60
};

struct CTCScriptedControl {
    void*   __vftable;
    CThing* m_pCreature;                                // +0x04
};

bool __fastcall IsAbleToMove(CThing* thing);                   // 0x0085F3E0
void __fastcall RemoveMovementBlockingModes(CThing* thing);    // 0x0085F600

struct CActionLeadCreature {
    virtual void v00();
    CTCScriptedControl* m_pControl;                     // +0x04
    unsigned char       _pad[0x24 - 0x08];
    bool                m_stateSaved;                   // +0x24
    unsigned char       m_savedMovementLocked;          // +0x25
    void OnActivate();
};

void CActionLeadCreature::OnActivate()
{
    if (!m_stateSaved) {
        m_savedMovementLocked = m_pControl->m_pCreature->m_pMovement->GetMovementLocked();
        m_pControl->m_pCreature->m_pMovement->SetMovementLocked(0);
    }
    if (!IsAbleToMove(m_pControl->m_pCreature))
        RemoveMovementBlockingModes(m_pControl->m_pCreature);
}