// NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion() -- track the freedom fighters: refill CurrentBanditVector with every HTE_FreedomFighter and wait (frame by frame, refilling whenever a bandit was killed) until BanditsAtNextRegion reaches the bandit count, then flag that the bandits left the region
// Leaf: ?MonitorBanditRegion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6CCC0, 247 bytes, thiscall, plain `ret`.
// Identity (proven): CQ_HangingTreeEvilScript::SpawnParty 0x00D6C4A0 builds a CSpawnedFunc named "MonitorBanditRegion" (0x012D2374) whose member-function pointer is this address (store at 0x00D6C76D); ?MonitorBanditRegion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d6ccc0" is a gap-scan placeholder.
// Callees: std::vector<CScriptThing>::erase 0x008AC9D0 (the inline clear()), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940; interface slots 75 GetAllThingsWithScriptName, 7 NewScriptFrame.
// Data: "HTE_FreedomFighter" @ 0x012D2050.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets, 4 lower again after each Dinkumware container. BanditKilled +0x4a, BanditsLeftRegion +0x4e, BanditsAtNextRegion +0x68, CurrentBanditVector +0xb8 (Ego_r.pdb +0x5e/+0x62/+0x7c/+0xd0).
// Shape: size() is the signed /12 of the byte span compared unsigned against the long count.
class CScriptThing;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6 layout (0x0C): start, finish, end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _pad048[0x2];
    bool BanditKilled;                                  // +0x4a (Ego_r.pdb +0x5e)
    unsigned char _pad04b[0x3];
    bool BanditsLeftRegion;                             // +0x4e (Ego_r.pdb +0x62)
    unsigned char _pad04f[0x19];
    long BanditsAtNextRegion;                           // +0x68 (Ego_r.pdb +0x7c)
    unsigned char _pad06c[0x40];                        // +0x74 Executioner .. +0xac CurrentEnemyVector
    std::vector<CScriptThing> CurrentEnemyVector;       // +0xac
    std::vector<CScriptThing> CurrentBanditVector;      // +0xb8 (Ego_r.pdb +0xd0)
    void MonitorBanditRegion();
};
}

void NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion()
{
    BanditsLeftRegion = false;
    CurrentBanditVector.clear();
    Game->GetAllThingsWithScriptName("HTE_FreedomFighter", CurrentBanditVector);
    while ((unsigned int)BanditsAtNextRegion < CurrentBanditVector.size()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (BanditKilled) {
            if (IsActiveThreadTerminating()) return;
            CurrentBanditVector.clear();
            Game->GetAllThingsWithScriptName("HTE_FreedomFighter", CurrentBanditVector);
            BanditKilled = false;
        }
    }
    if (IsActiveThreadTerminating()) return;
    BanditsLeftRegion = true;
}