#pragma optimize("s", on)
// CTCVillage::IsTimeToGoToSchool(CThingAICreature* child, const CWorld& world, bool run) -- true when the child's live school building is far enough away (capped) that, at its walk/run animation speed, it must set off now to arrive before its village's school start time
// Leaf: ?IsTimeToGoToSchool@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065A8EE, 385 bytes (`leave ; ret 0Ch`); size TU (unaligned entry, ebp frame, argument
//   slots reused for locals): `optimize("s", on)` as the landed CTCVillage siblings (0x0065AC0E, 0x00658ACB).
// Identity: Ego_r.pdb + FableWin.pdb ?IsTimeToGoToSchool@CTCVillage@@QAE_NPAVCThingAICreature@@ABVCWorld@@_N@Z.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice, the child's WorkBuilding), IS_THING_ALIVE<CThingBuilding*>
//   0x0065D624 (`__fastcall`, Ego_r.pdb ??$IS_THING_ALIVE@PAVCThingBuilding@@@@...; same-TU body), CThing::GetPos 0x004C73D0
//   (twice), std::bitset<274>::test 0x00410DE0 and CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inside the
//   inline CThing::GetTC<CTCVillageMember>(T**), FableWin.pdb ??$GetTC@VCTCVillageMember@@@CThing@@QAE_NPAPAV..@Z),
//   CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::GetSchoolTimes(float&, float&) const 0x00658ACB,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CTCBase::GetWorld 0x00686A70 (protected const),
//   NCreatureMode::GetMovementSpeedFromAnimation(const CCharString&, const CThingCreatureBase&, const CMeshDataBank&)
//   0x00861C90 (`__fastcall`). Data: CMainGameComponent::ConstantFPS 0x01375550 (through the static inline
//   GetConstantFPS, FableWin.pdb ?GetConstantFPS@CMainGameComponent@@SIJXZ), "ST_RUN"/"ST_WALK", 5.0f, 70.0f.
// Shape (what each retail detail needed):
//   - The IS_THING_ALIVE temporary, the GetTC key and `start` all share the dead `child` argument slot [ebp+8]: VC7.1 packs
//     address-taken locals by scope, so start/end live in an inner block (function-scope start/end overlap the temporary
//     and push it into its own slot: frame 0x34 v 0x30, child in edi).
//   - The component pick `mov eax,[eax+4] ; mov ecx,eax` is the PDB's out-parameter GetTC<T>(T**) (T::StatGetInterfaceType,
//     HasTC through the out-of-line bitset test); a pointer-returning GetTC loads straight into ecx.
//   - `push -1 ; je ; push "ST_RUN" ; jmp ; push "ST_WALK" ; call ctor` is two block-scope CCharString locals in an
//     if/else whose identical tails VC7.1 merges (one `?:` argument selects the literal into a register first; an
//     argument temporary is built after the GetWorld call); the string dies before the FPS multiply (speed spilled).
//   - `if (start > ratio) return false; return true;` gives retail's `test ah,41h ; jne` with the shared `xor al,al` block on
//     the fall-through (`return start <= ratio` emits the exact-NaN `jp` form).
//   - C3DVector operator- / GetLength are `__forceinline`: in this size TU plain inline members stay calls (375v385).
// Layout: retail CThing UsingComponents (bitset<274>) +0x20, Components +0x44, Dead bit +0x91 (Ego_r +0x20/+0x48/+0xA1);
//   retail CThingAICreature WorkBuilding (CIntelligentPointer<CThingBuilding>) +0x170 (Ego_r +0x198; header debt, the
//   generated engine/CThingAICreature.h carries the Ego_r offsets); engine/CWorld.h PGameTimeManager +0x1C, PMainMeshBank
//   +0x3C; CGameTimeManager TimeStep +0x10.
#include "rebuild_abi.h"
#include "engine/CWorld.h"
#include <math.h>
#pragma intrinsic(sqrt)

class CTCBase;
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
enum ETCInterfaceType { TCI_VILLAGE_MEMBER = 0x23 };   // descriptive enumerator name (value from GetInterfaceType)
namespace std {
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <unsigned int _Nb> class bitset {                   // STLport 4.6 <bitset>
public:
    bool test(unsigned int __pos) const;                     // 0x00410DE0 (bitset<274>, out of line in this size TU)
    unsigned long _M_w[(_Nb + 31) / 32];
};
}
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char      _pad00[0x20];
    std::bitset<274>   UsingComponents;         // +0x20
    CInterfaceMap      Components;              // +0x44 (first/last; the rest of the 0x10-byte map unused here)
    unsigned char      _pad4c[0x91 - 0x4c];
    unsigned char      Dead : 1;                // +0x91 bit 0 (Ego_r.pdb CThing::Dead +0xA1)
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> __forceinline bool GetTC(T** out)
    {
        long type = T::StatGetInterfaceType();
        if (HasTC((ETCInterfaceType)type)) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;            // 0x004C73D0
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0065D624 (T = CThingBuilding*)
{
    return thing != 0 && !thing->Dead;
}
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;              // 0x00A01B50
private:
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThingBuilding : public CThing { };
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {
public:
    unsigned char                       _pad94[0x170 - 0x94];
    CIntelligentPointer<CThingBuilding> WorkBuilding;   // +0x170
};
class CGameTimeManager {
public:
    unsigned char _pad00[0x10];
    float         TimeStep;                     // +0x10
};
class CMainGameComponent {
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }
protected:
    static long ConstantFPS;                    // 0x01375550
};
namespace NCreatureMode {
float __fastcall GetMovementSpeedFromAnimation(const CCharString& name, const CThingCreatureBase& creature,
                                               const CMeshDataBank& bank);   // 0x00861C90
}
class CTCVillage;
class CTCVillageMember {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_VILLAGE_MEMBER; }
    CTCVillage* GetPVillage() const;            // 0x007EE540
};
class CTCBase {
public:
    virtual ~CTCBase();
protected:
    CWorld& GetWorld() const;                   // 0x00686A70
};
class CTCVillage : public CTCBase {
public:
    bool GetSchoolTimes(float& start, float& end) const;   // 0x00658ACB
    bool IsTimeToGoToSchool(CThingAICreature* child, const CWorld& world, bool run);
};

bool CTCVillage::IsTimeToGoToSchool(CThingAICreature* child, const CWorld& world, bool run)
{
    if (!IS_THING_ALIVE(child->WorkBuilding.GetPItem()))
        return false;
    C3DVector position = child->GetPos();
    C3DVector school = child->WorkBuilding.GetPItem()->GetPos();
    float distance = (position - school).GetLength();
    CTCVillageMember* member;
    if (!child->GetTC(&member))
        return false;
    CTCVillage* village = member->GetPVillage();
    if (!village)
        return false;
    {
        float start;
        float end;
        if (!village->GetSchoolTimes(start, end))
            return false;
        start /= world.PGameTimeManager_ptr->TimeStep;
        float speed;
        if (run) {
            CCharString animation("ST_RUN");
            speed = NCreatureMode::GetMovementSpeedFromAnimation(animation, *child, *GetWorld().PMainMeshBank_Data);
        }
        else {
            CCharString animation("ST_WALK");
            speed = NCreatureMode::GetMovementSpeedFromAnimation(animation, *child, *GetWorld().PMainMeshBank_Data);
        }
        float frameSpeed = CMainGameComponent::GetConstantFPS() * speed;
        float travel = distance * 5.0f;
        if (travel > 70.0f)
            travel = 70.0f;
        if (start > travel / frameSpeed)
            return false;
        return true;
    }
}