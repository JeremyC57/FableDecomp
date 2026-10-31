// Grade: DIFFER(377v385) 58 difflines (was 65): animation CCharString as a call-argument temporary and the start time divided in place; left: child in edi (retail esi), frame 0x34 v 0x30, literal still selected into eax before push -1 (retry wave 2 sec2b)
// Was: DIFFER(378v385) -- the FP distance math matches; left: retail keeps the child in esi (ours edi), reuses the start/run argument slots for the school-time local and the animation string, and pushes the ST_RUN/ST_WALK literal inside the branches after push -1 (ours selects into eax first)
// CTCVillage::IsTimeToGoToSchool(CThingAICreature* child, const CWorld& world, bool run) -- true when the child's live school building is far enough away (capped) that, at its walk/run animation speed, it must set off now to arrive before its village's school start time
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A8EE, length 383 bytes (`leave`, `ret 0xc`; PDB
//   `?IsTimeToGoToSchool@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_N@Z`): frame-pointer size TU,
//   `#pragma optimize("y", off)` + `#pragma optimize("s", on)` as the landed siblings (the argument slots are reused for
//   locals).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice, on the child's work building), IS_THING_ALIVE<CThing*>
//   (CThing* const&) 0x0065D624 (`__fastcall`), CThing::GetPosition 0x004C73D0 (twice), std::bitset<>::test 0x00410DE0,
//   CVectorMap LowerBound 0x0040F020 (inlined GetTC on the village-member interface 0x23), CTCVillageMember::GetPVillage
//   0x007EE540, CTCVillage::GetSchoolTimes(float&, float&) const 0x00658ACB (landed), CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, the CTCBase world getter 0x00686A70, NCreatureMode::
//   GetMovementSpeedFromAnimation(const CCharString&, const CThingCreatureBase&, const CMeshDataBank&) 0x00861C90
//   (`__fastcall`). Data: "ST_RUN" @ 0x0125A998, "ST_WALK" @ 0x0125A990, 5.0f @ 0x012462A8, 70.0f @ 0x0125A98C,
//   g_framesPerSecond @ 0x01375550.
// Layout: retail CThingAICreature WorkBuilding (CIntelligentPointer<CThingBuilding>) +0x170 (PDB +0x198; the landed
//   sibling BuildingContainsCorrectVillagers 0x00658EC3 has HomeBuilding at +0x168, PDB +0x190); Ego_r.pdb CWorld
//   PGameTimeManager +0x1c, PMainMeshBank +0x3c (unshifted); CGameTimeManager TimeStep +0x10.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <math.h>
#pragma intrinsic(sqrt)
struct CTCBase;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x0065D624

template <class T> struct CIntelligentPointer {   // 8 bytes: vptr + item
    void* _vfptr;
    T*    m_pItem;
    T* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};
struct CThingBuilding : CThing {
};
struct CThingCreatureBase : CThing {
};
struct CThingAICreature : CThingCreatureBase {
    unsigned char                       _pad4c[0x170 - 0x4c];
    CIntelligentPointer<CThingBuilding> WorkBuilding;   // +0x170
};

struct CGameTimeManager {
    unsigned char _pad00[0x10];
    float         TimeStep;                     // +0x10
};
struct CMeshDataBank;
struct CWorld {
    unsigned char     _pad00[0x1c];
    CGameTimeManager* PGameTimeManager;         // +0x1c scoped_ptr<CGameTimeManager>
    unsigned char     _pad20[0x1c];
    CMeshDataBank*    PMainMeshBank;            // +0x3c CCountedPointer<CMeshDataBank>::Data
};
extern long g_framesPerSecond;                  // 0x01375550

namespace NCreatureMode {
    float __fastcall GetMovementSpeedFromAnimation(const CCharString& anim, const CThingCreatureBase& creature,
                                                   const CMeshDataBank& bank);   // 0x00861C90
}

struct CTCVillage;
struct CTCVillageMember {
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
struct CTCVillage {
    bool GetSchoolTimes(float& start, float& end) const;   // 0x00658ACB
    CWorld* GetWorld();                         // 0x00686A70
    bool IsTimeToGoToSchool(CThingAICreature* child, const CWorld& world, bool run);
};

bool CTCVillage::IsTimeToGoToSchool(CThingAICreature* child, const CWorld& world, bool run)
{
    if (!IS_THING_ALIVE(child->WorkBuilding.GetPItem()))
        return false;
    C3DVector position = child->GetPosition();
    C3DVector school = child->WorkBuilding.GetPItem()->GetPosition();
    float distance = (position - school).GetLength();
    if (!child->HasTC(0x23))
        return false;
    CTCVillage* village = child->GetTC<CTCVillageMember>(0x23)->GetPVillage();
    if (!village)
        return false;
    float start;
    float end;
    if (!village->GetSchoolTimes(start, end))
        return false;
    start /= world.PGameTimeManager->TimeStep;
    float speed = NCreatureMode::GetMovementSpeedFromAnimation(CCharString(run ? "ST_RUN" : "ST_WALK"), *child, *GetWorld()->PMainMeshBank);
    float frameSpeed = g_framesPerSecond * speed;
    float travel = distance * 5.0f;
    if (travel > 70.0f)
        travel = 70.0f;
    return start <= travel / frameSpeed;
}
