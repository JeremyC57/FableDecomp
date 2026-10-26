// Behaviour test for 00a36df0: src/00a36df0.cpp copied verbatim; the animation player, bank, info and stats stubbed so
// the record's animation id selects the info, the event time comes from its stats, and the counted stats pointer is
// released (deleted at the last reference).
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// C3DMeshObject::GetEventTime(long event, const SAnimationRef* animation, const SAnimationSource* source) -- looks the animation up (its id from the animation's playing object, the info from the source's bank), takes the info's counted animation stats and returns the stats' time for the event
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
class CCPPointerInfo { public:                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
void __cdecl operator delete(void* p);                   // 0x00BFE9BC

template<class T> class CCountedPointer { public:
    T*              Data;                                // +0
    CCPPointerInfo* Info;                                // +4
    ~CCountedPointer()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
    }
    T* operator->() const { return Data; }
};

class C3DAnimationStats { public:
    float GetEventTime(long event) const;                // 0x00A4EAE0
};
class C3DAnimationInfo { public:
    CCountedPointer<const C3DAnimationStats> GetAnimationStats();   // 0x00A269C0
};
struct C3DAnimationBank {
    C3DAnimationInfo* GetInfo(long index);               // 0x00A242C0
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
    C3DAnimationInfo* info = source->GetBank()->GetInfo(animationID);
    float time = info->GetAnimationStats()->GetEventTime(event);
    return time;
}
// ---- end source ----
void CAnimationPlayer::v00() {} void CAnimationPlayer::v01() {} void CAnimationPlayer::v02() {}
void C3DMeshObject::v00() {}
static SAnimationRecord g_record;
SAnimationRecord* CAnimationPlayer::GetRecord() { return &g_record; }
static C3DAnimationInfo g_infos[4]; static long g_askedIndex = -1;
C3DAnimationInfo* C3DAnimationBank::GetInfo(long index) { g_askedIndex = index; return &g_infos[index]; }
static C3DAnimationStats g_stats; static CCPPointerInfo* g_info = 0; static int g_deleted = 0;
static void __fastcall DeleteStats(void*) { ++g_deleted; }
CCountedPointer<const C3DAnimationStats> C3DAnimationInfo::GetAnimationStats()
{
    static void* s_holder[2]; s_holder[0] = &g_stats; s_holder[1] = g_info; ++g_info->RefCount;
    return *(CCountedPointer<const C3DAnimationStats>*)s_holder;
}
static long g_event = -1;
float C3DAnimationStats::GetEventTime(long event) const { g_event = event; return 0.5f * event; }
int main()
{
    static CAnimationPlayer player; static SAnimationHolder holder; holder.Player = &player;
    SAnimationRef ref; ref._vt = 0; ref.Holder = &holder;
    static C3DAnimationBank bank; SAnimationSource src; src._vt = 0; src.Bank = &bank;
    g_record.AnimationID = 2;
    g_info = new CCPPointerInfo; g_info->RefCount = 1; g_info->DeleteFunc = DeleteStats; g_info->Data = &g_stats;
    C3DMeshObject mesh;
    float t = mesh.GetEventTime(6, &ref, &src);
    CHECK(t == 3.0f && g_askedIndex == 2 && g_event == 6 && g_info->RefCount == 1 && g_deleted == 0);
    g_info->RefCount = 0;                                // only the temporary holds it: deleted on release
    t = mesh.GetEventTime(1, &ref, &src);
    CHECK(t == 0.5f && g_deleted == 1);
    if (fails == 0) printf("00a36df0_TEST PASS\n"); else printf("00a36df0_TEST FAIL\n");
    return fails;
}