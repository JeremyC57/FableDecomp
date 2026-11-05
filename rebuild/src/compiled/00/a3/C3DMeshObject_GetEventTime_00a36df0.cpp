// C3DMeshObject::GetEventTime(long event, const SAnimationRef* animation, const SAnimationSource* source) -- looks the animation up (its id from the animation's playing object, the info from the source's bank), takes the info's counted animation stats and returns the stats' time for the event
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retry 10b: the bank comes from an inline getter (`source->GetBank()`, FIXES B-15) and the animation info is a named local, so GetInfo runs before the hidden result pointer of GetAnimationStats is pushed (retail order).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36DF0, length 113 bytes.
// Callees: the playing object's virtual slot 3 (+0x0c, returns the record whose +8 is the animation id),
// 0x00A242C0 the bank's info lookup (`m_infos[index]` at +0x380; propagated label
// CTCInventoryAbilities::GetAbilityLevel), 0x00A269C0 C3DAnimationInfo::GetAnimationStats() (counted pointer by
// hidden pointer into the 8-byte frame), 0x00A4EAE0 C3DAnimationStats::GetEventTime (propagated label
// CRegion::SetMiniMapRegionExitTextOffsetY: a find on +0x20 and the entry's float at +0x14), then the
// counted block's destroy thunk / 0x00BFE9BC operator delete when the temporary releases the last reference.
// `this` is never read (`ret 0Ch` with ecx overwritten first) and no direct caller exists, so the owner class is
// the manifest's; the argument types are descriptive models of the pointer chains the body walks.
// Shape: the float result is a named local (homed in the dead `animation` argument slot by `fst`, popped around
// the release and reloaded); the counted temporary's inline destructor runs after it.
#include "engine/CCountedPointer.h"
void __cdecl operator delete(void* p);                   // 0x00BFE9BC


class C3DAnimationStats { public:
    float GetEventTime(long event) const;                // 0x00A4EAE0
};
class C3DAnimationInfo { public:
    CCountedPointer<const C3DAnimationStats> GetAnimationStats();   // 0x00A269C0
};
class CMeshDataBank { public:
    C3DAnimationInfo const* PeekAnimation(unsigned long a0) const;   // 0x00A242C0 (Ego_r.pdb ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z)
};
struct C3DAnimationBank {
};
struct SAnimationRecord {
    unsigned char _pad00[8];
    long          AnimationID;                           // +0x08
};
struct CAnimationPlayer {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual SAnimationRecord* GetRecord();               // slot 3 (+0x0c)
};
struct SAnimationHolder {
    unsigned char     _pad00[8];
    CAnimationPlayer* Player;                            // +0x08
};
struct SAnimationRef {
    void*             _vt;
    SAnimationHolder* Holder;                            // +0x04
};
struct SAnimationSource {
    void*             _vt;
    C3DAnimationBank* Bank;                              // +0x04
    C3DAnimationBank* GetBank() const { return Bank; }
};

class C3DMeshObject { public:
    virtual void v00();
    float GetEventTime(long event, const SAnimationRef* animation, const SAnimationSource* source);
};

float C3DMeshObject::GetEventTime(long event, const SAnimationRef* animation, const SAnimationSource* source)
{
    long animationID = animation->Holder->Player->GetRecord()->AnimationID;
    C3DAnimationInfo* info = ((C3DAnimationInfo*)((CMeshDataBank*)(source->GetBank()))->PeekAnimation(animationID));
    float time = info->GetAnimationStats()->GetEventTime(event);
    return time;
}