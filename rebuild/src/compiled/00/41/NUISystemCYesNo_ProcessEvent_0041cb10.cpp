// NUISystem::CYesNo::ProcessEvent(EEvent event) -- yes/no dialog input: events 4 (yes) and 5 (no) reset the dialog state to 0, then run the matching yes or no callback when one is set
// Grade: RELOCATION_MATCH (70v70). Lever (sec1b flag pre-screen): the TU is size-optimised -- `#pragma optimize("s", on)`, as the
//   neighbouring landed rows 0x0041C88F / 0x0041C8C5 / 0x0041CCE4 are; at plain /O2 VC7.1 cross-jumps the two callback calls.
// Leaf: ?ProcessEvent@CYesNo
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041CB10, 70 bytes (+ int3 padding), virtual, `ret 4`.
// Vtable 0x0122F85C (NUISystem::CYesNo, the event-receiver base at +4) slot 1; Ego_r.pdb
// `virtual void CYesNo::ProcessEvent(EEvent)` (landed CMenuEntry::ProcessEvent 0x0053D200 uses the same base model).
// Callees: the primary ChangeState(unsigned long) (slot 48, +0xc0, reached through `lea ecx,[esi-4]`), and the
//   YesCallback / NoCallback function pointers (Ego_r.pdb type `void (__fastcall*)()`, SetNoCallBack's `P6IXXZ`).
// Shape: `this` is the receiver subobject (full object + 4), so the members sit 4 lower in the code; `event == 4 ||
//   event == 5` is the two-compare entry test; the yes and no blocks are separate ifs on the event kept in edi.
// Layout: retail CYesNo YesCallback +0x15c / NoCallback +0x160 (Ego_r.pdb +0x18c / +0x190; retail = PDB - 0x30).
#pragma optimize("s", on)
namespace NUISystem {

enum EEvent { EVENT_YES = 4, EVENT_NO = 5 };

typedef void (__fastcall* TCallback)();

struct CComponentVirtuals {                         // primary vtable (CComponent's slots)
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
    virtual void ChangeState(unsigned long state);  // slot 48 (+0xc0)
};
struct CEventReceiver {                             // the base subobject at +4
    virtual void v000();
    virtual void ProcessEvent(EEvent event);        // slot 1
};
class CComponent : public CComponentVirtuals, public CEventReceiver {
public:
    unsigned char _pad08[0x15c - 0x08];
};
class CYesNo : public CComponent {
public:
    TCallback YesCallback;                          // +0x15c
    TCallback NoCallback;                           // +0x160
    virtual void ProcessEvent(EEvent event);
};

void CYesNo::ProcessEvent(EEvent event)
{
    if (event == EVENT_YES || event == EVENT_NO)
        static_cast<CComponent*>(this)->ChangeState(0);
    if (event == EVENT_YES && YesCallback)
        YesCallback();
    if (event == EVENT_NO && NoCallback)
        NoCallback();
}
}