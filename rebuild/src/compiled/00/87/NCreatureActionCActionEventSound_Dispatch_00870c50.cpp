// NCreatureAction::CActionEventSound::Dispatch(const CActionEventContext& context) -- the sound action event's dispatch: in the retail build it only builds and destroys two empty CCharString locals (the playback that used them is compiled out)
// Leaf: Dispatch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00870C50, length 43 bytes (+ int3 padding), `ret 4`. Identity: vtable
// 0x0125C11C (NCreatureAction::CActionEventSound, rtti-vtables.tsv) slot 1 -- the Dispatch(const CActionEventContext&) slot
// of the CActionEvent family (landed CActionEventCameraShake::Dispatch 0x00871250, Explosion 0x008717A0, AddObject
// 0x008718B0, RemoveObject 0x00871A10). The manifest label `_Dest_val<allocator<CMacro>,CMacro>` is propagated and wrong.
// Callees: CCharString::CCharString() @ 0x0099E4B0 (x2), CCharString::~CCharString() @ 0x0099EAE0 (x2).
// Shape: `this` and the context are unused; two locals in an 8-byte frame (`sub esp,8`), the first declared at [esp+4],
// constructed in order and destroyed in reverse.
class CCharString {
public:
    void* PStringData;
    CCharString();                                           // 0x0099E4B0
    ~CCharString();                                          // 0x0099EAE0
};

namespace NCreatureAction {
struct CActionEventContext;

class CActionEventSound {                                    // vtable 0x0125C11C
public:
    virtual void v00();
    void Dispatch(const CActionEventContext& context);       // slot 1
};

void CActionEventSound::Dispatch(const CActionEventContext& /*context*/)
{
    CCharString soundName;
    CCharString boneName;
}
}