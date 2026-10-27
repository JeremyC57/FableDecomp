// CTCVillage::IsNightTime() const -- whether the time of day lies between the village def's SleepAdultStartTime and SleepAdultEndTime
// Grade: DIFFER(86v86) -- residue: retail loads the world's PGameTimeManager (+0x1c) into ecx straight after PeekWorld and keeps it there across the two def-pointer copies; VC7.1 here loads it after the copies (86v86, world local) or keeps it in eax and moves it (88v86, manager local); 11 spellings tried (manager/world/reference locals, const, split args, result local, /Os+/Oy-, speed, non-forceinline GetDef)
// Leaf: ?IsNightTime@CTCVillage@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658D07, 86 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?IsNightTime@CTCVillage@@QBE_NXZ; it reads CVillageDef::SleepAdultStartTime +0xB4 / SleepAdultEndTime +0xB8 (Ego_r.pdb
//   +212 / +216, retail 0x20 lower: the eight 16-byte PDB vectors before them are 12 bytes); the only start/end-pair Is*Time of the class left (Story/School/Work/Cook/Eat/Tavern/Gaming read start-time vectors, Sociable/Curfew are 0x00658D5D / 0x00658DB3); its callers are CGameScriptInterface::IsSleepingTime 0x00896210 and a minion follow state's OnReplace.
//   The manifest's "CTCVillage::IsCurfewTime" is propagated across byte-identical bodies.
// Callees: CTCBase::PeekWorld() const 0x00686A80, CGameTimeManager::IsTimeOfDayInRange(long, long) 0x006BB390.
// Shape (family M180, representative 0x00658D07): `world = PeekWorld(); return world->PGameTimeManager->IsTimeOfDayInRange(
//   GetDef()->SleepAdultStartTime, GetDef()->SleepAdultEndTime);` -- the world fetched first, then two by-value CDefPointer temporaries (the end's first), released in
//   reverse without null tests (both dereferenced). `optimize("s", on)`.
#pragma optimize("s", on)
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                          // slot 1 (+4)
    mutable unsigned long IVRefCount;                     // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
class CVillageDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0xB4 - 0x08];
    long          SleepAdultStartTime;                                  // +0xB4 (PDB +212)
    long          SleepAdultEndTime;                                  // +0xB8 (PDB +216)
};
class CGameTimeManager {
public:
    bool IsTimeOfDayInRange(long start, long end);        // 0x006BB390
};
class CWorld {
public:
    unsigned char     _pad00[0x1c];
    CGameTimeManager* PGameTimeManager;                   // +0x1c
};
class CTCVillage {
public:
    unsigned char _pad00[0x94];
    CDefPointer<const CVillageDef> PDef;                  // +0x94
    CWorld* PeekWorld() const;                            // 0x00686A80 (CTCBase)
    __forceinline CDefPointer<const CVillageDef> GetDef() const { return PDef; }
    bool IsNightTime() const;
};
bool CTCVillage::IsNightTime() const
{
    CWorld* world = PeekWorld();
    return world->PGameTimeManager->IsTimeOfDayInRange(GetDef()->SleepAdultStartTime, GetDef()->SleepAdultEndTime);
}
