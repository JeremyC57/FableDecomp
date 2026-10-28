// NAnimationEvents::CManager::AnimHasEmbeddedEvent(unsigned long animation, const CCharString& event) const -- does the animation's own (embedded) event stats contain the named event
// Leaf: ?AnimHasEmbeddedEvent@CManager@NAnimationEvents@@ABE_NKABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F90D0, 92 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb donor order AnimHasEmbeddedEvent 0x0071E750 -> GetEmbeddedEventTime 0x0071E7B0 matches retail this row ->
//   GetEmbeddedEventTime 0x006F9130 (landed, the same stats fetch); C3DAnimationStats donor order HasEvent 0x00BA4A40 -> GetEventTime
//   0x00BA4AA0 matches the retail callees 0x00A4EAC0 (this row) -> 0x00A4EAE0 (the landed GetEventTime). Private in the PDB (`ABE`);
//   the landed GetEmbeddedEventTime spells itself public `QBE` -> Review at landing.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const 0x00A242C0 and C3DAnimationInfo::GetAnimationStats() const 0x00A269C0
//   (landed spellings), C3DAnimationStats::HasEvent(const CCharString&) const 0x00A4EAC0 (Ego_r.pdb), operator delete 0x00BFE9BC (the
//   counted pointer's info block).
// Layout and the CCountedPointer model: the landed twin 0x006F9130 (MeshDataBank +4; info {RefCount, DeleteFunc, Data}).
// Shape: the stats temporary lives to the end of the full expression: HasEvent's bool is kept in bl across its release.
#include "fable_string.h"
void operator delete(void*) throw(); //00BFE9BC
class CCPPointerInfo { public:unsigned long RefCount;void(FABLE_FASTCALL* DeleteFunc)(void*);void* Data;};
template<class T>class CCountedPointer { public:
 T* Data;CCPPointerInfo* Info;
 CCountedPointer():Data(0),Info(0){}
 CCountedPointer(const CCountedPointer& o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}
 T* operator->() const { return Data; }
 ~CCountedPointer(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}}
};
class C3DAnimationStats {
public:
    bool HasEvent(const CCharString& event) const; // 0x00A4EAC0
};
class C3DAnimationInfo {
public:
    CCountedPointer<const C3DAnimationStats> GetAnimationStats() const;   // 0x00A269C0
};
class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long) const;   // 0x00A242C0
};
namespace NAnimationEvents {
class CManager {
public:
    unsigned long  NativeManagerPrefix;
    CMeshDataBank* MeshDataBank;                   // +0x04
private:
    bool AnimHasEmbeddedEvent(unsigned long animation, const CCharString& event) const;
};
}

bool NAnimationEvents::CManager::AnimHasEmbeddedEvent(unsigned long animation, const CCharString& event) const
{
    return MeshDataBank->PeekAnimation(animation)->GetAnimationStats()->HasEvent(event);
}