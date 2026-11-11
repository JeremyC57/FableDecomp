// NEntityEvents::CEventCutSceneAnimEvent::GetType() const -- returns (EEventType)0x24
// Leaf: ?GetType@CEventCutSceneAnimEvent@NEntityEvents@@UBE?AW4EEventType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00692370, thiscall, `ret`.
// Identity: slot 1 of NEntityEvents::CEventCutSceneAnimEvent's vtable 0x0125C1CC (defined here: the slot holds this body in 1 vtable(s), all of NEntityEvents::CEventCutSceneAnimEvent or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetType(void) at this slot; Ego_r/FableWin name set: ?GetType@CEventCutSceneAnimEvent@NEntityEvents@@UBE?AW4EEventType@2@XZ; `ret` matches the signature.
namespace NEntityEvents { enum EEventType { }; }
namespace NEntityEvents {
class CEventCutSceneAnimEvent {
public:
    virtual NEntityEvents::EEventType GetType() const;
};
NEntityEvents::EEventType CEventCutSceneAnimEvent::GetType() const
{
    return (EEventType)0x24;
}
}
