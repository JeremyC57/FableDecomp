// NScript::CQ_UndeadRisingScript::ActivateNextLamp() -- light the next graveyard lamp
// Leaf: ?ActivateNextLamp@CQ_UndeadRisingScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E0E040, 668 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): the landed CUndeadRisingZombieAttacker::OnPredicateFail 0x00E0DE90 spawns this member function (member
//   pointer store) under the name "ActivateNextLamp"; Ego_r.pdb names ?ActivateNextLamp@CQ_UndeadRisingScript@NScript@@QAEXXZ.
//   That landed row binds the non-NScript spelling (NAME_FIXES.tsv). Catalog label CScriptBase_HandleGraveyardSpawn is wrong.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; CGameScriptInterface slot 421
//   IsCameraPosOnScreen, slot 100 CreateEffect(const CCharString&, const C3DVector&, ...); CScriptThing::GetPos inlined (the
//   g_ZeroVector 0x0143E8E0 arm for a null PImp).
// Data: "GRAVEYARD_CIRCLE_LIGHT_SMALL" 0x012DFF14, "" 0x0122D70E, CScriptThing vtable 0x01238C8C.
// Layout (retail = Ego_r.pdb - 0x14): number_of_zombie_tokens_in_right_area +0x48, LightsActivated[8] +0x54, activation_points
//   +0x60 (12-byte STLport vector<CScriptThing>), boss_points +0x6c.
// Shape: two search loops (an unlit lamp on screen, else the first unlit lamp), then the point copy, a default-constructed
//   effect assigned from CreateEffect, and the token increment; all CScriptThing / CCountedPointer members inline (__forceinline).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo {
public:
    long References;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->References;
    }
    __forceinline ~CCountedPointer()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->References;
                if (Info->References == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
};
class C3DVector {
public:
    float X, Y, Z;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
__forceinline CScriptThing::CScriptThing() { }
__forceinline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
__forceinline CScriptThing::~CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other)
{
    PImp = other.PImp;
    return *this;
}
__forceinline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data)
        return g_ZeroVector;
    return PImp.Data->GetPos();
}

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
}

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_UndeadRisingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned long number_of_zombie_tokens_in_right_area;  // +0x48
    unsigned long number_of_boss_kills_in_right_area;     // +0x4c
    bool HeroReachedCentre;                             // +0x50
    bool BossCutscenePlayed;                            // +0x51
    bool MissionSucceeded;                              // +0x52
    bool MissionFailed;                                 // +0x53
    bool LightsActivated[8];                            // +0x54
    long quest_status;                                  // +0x5c
    std::vector<CScriptThing> activation_points;        // +0x60
    std::vector<CScriptThing> boss_points;              // +0x6c
    void ActivateNextLamp();
};

void CQ_UndeadRisingScript::ActivateNextLamp()
{
    if (number_of_zombie_tokens_in_right_area < activation_points.size()) {
        long lamp = -1;
        long i;
        for (i = 0; i < 8; ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (!LightsActivated[i] && Game->IsCameraPosOnScreen(activation_points[i].GetPos())) {
                if (IsActiveThreadTerminating())
                    return;
                lamp = i;
                break;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        if (lamp == -1) {
            if (IsActiveThreadTerminating())
                return;
            for (i = 0; i < 8; ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (!LightsActivated[i]) {
                    if (IsActiveThreadTerminating())
                        return;
                    lamp = i;
                    break;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        LightsActivated[lamp] = true;
        CScriptThing point = activation_points[lamp];
        CScriptThing effect;
        effect = Game->CreateEffect("GRAVEYARD_CIRCLE_LIGHT_SMALL", point.GetPos(), "", 0.0f, false, false);
        ++number_of_zombie_tokens_in_right_area;
    }
}
}   // namespace NScript