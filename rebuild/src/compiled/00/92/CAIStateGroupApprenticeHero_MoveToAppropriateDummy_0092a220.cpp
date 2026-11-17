// CAIStateGroup_ApprenticeHero::MoveToAppropriateDummy() -- pick the target point for the current practice action (the dummy, or a random "RangedPractice" thing) and a random proximity, then keep moving there while the dummy type stays legal; true once arrived
// Leaf: ?MoveToAppropriateDummy@CAIStateGroupApprenticeHeroMethods@@QAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092A220, length 455 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0, GetPWorldSeed @ 0x008FCFD0, GFFloatRandom @ 0x004978A0 (__fastcall: range on the
//          stack, seed in ecx), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0 ("RangedPractice" @ 0x01291A40), the dummy finder
//          @ 0x00929FC0, AIAction_MoveToPosProximity @ 0x0097E890, IsStopping @ 0x008FD010, Terminate @ 0x008FCF40; the
//          interface-0xAA lower_bound inlined with its result unused; IsDummyTypeLegal inlined (bools @ 0x013BAE18..1A).
// Model: the landed sibling GetDummy 0x0092A0C0 (same TU). Layout: engine/CAIStateGroup_ApprenticeHero.h (PDummy +0x18, Action +0x1C).
// Shape: C3DVector with a user copy ctor keeps the position in the frame (not ebx/ebp); a named CCharString in its own
//          block (address re-taken, destroyed before the null test); do/while(result == 4) keeps the loop unrotated; a
//          named `action` copy read before the dead lookup puts Action in ebx and result in ebp (proximity reloaded).
#include "fable_compiler.h"
// NF51 (round 7): the AI state-group base and the apprentice class come from the shared headers (were a local CAIStateGroupBase
// model plus a layout-renamed include of engine/CAIStateGroup_ApprenticeHero.h, and a local CCharString): the retail branch
// FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO of engine/CAIStateGroup_ApprenticeHero.h over engine/CAIStateGroupBase.h. The callee
// 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h;
// GetPWorldSeed 0x008FCFD0 / IsStopping 0x008FD010 / Terminate 0x008FCF40 are the base's members, and the finder 0x00929FC0 is
// ?GetObjectWithScriptName@CAIStateGroup_ApprenticeHero@@IAEPAVCThingGameObject@@ABVCCharString@@@Z (_SCRIPTNAME, Ego_r.pdb).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO
#define FABLE_RETAIL_CAISTATEGROUP_APPRENTICEHERO_SCRIPTNAME
#include "engine/fable_ai_action_enums.h"
#include "fable_string.h"
#include "engine/CAIStateGroup_ApprenticeHero.h"
#include "engine/C3DVector.h"
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound (inlined in this TU)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};
class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb)
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingGameObject : public CThing { };
struct CTCInformation;                          // interface 0xaa
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern bool g_dummyTypeInUse0;                  // 0x013BAE18
extern bool g_dummyTypeInUse1;                  // 0x013BAE19
extern bool g_dummyTypeInUse2;                  // 0x013BAE1A
struct CAIStateGroupApprenticeHeroMethods : CAIStateGroup_ApprenticeHero {
    static bool IsDummyTypeLegal(long dummyType)
    {
        switch (dummyType) {
        case 0: return !g_dummyTypeInUse0;
        case 1: return !g_dummyTypeInUse1;
        case 2: return !g_dummyTypeInUse2;
        }
        return false;
    }
    bool MoveToAppropriateDummy();
};
bool CAIStateGroupApprenticeHeroMethods::MoveToAppropriateDummy()
{
    C3DVector position;
    float proximity;
    switch (Action) {
    case 0:
        position = PDummy->GetPos();
        proximity = GFFloatRandom(0.5f, GetPWorldSeed()) + 1.0f;
        break;
    case 1: {
        CThingGameObject* target;
        {
            CCharString name("RangedPractice");
            target = GetObjectWithScriptName(name);
        }
        if (!target)
            return false;
        position = target->GetPos();
        proximity = 0.0f;
        break;
    }
    case 2:
        position = PDummy->GetPos();
        proximity = GFFloatRandom(1.0f, GetPWorldSeed()) + 2.0f;
        break;
    default:
        return false;
    }
    long result;
    do {
        bool reached;
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, position, proximity, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        if (IsStopping()) {
            Terminate();
            return false;
        }
        long action = Action;
        CTCInformation* information;
        PBrain->PCreature->GetInterface(&information, 0xaa);
        if (!IsDummyTypeLegal(action))
            return false;
    } while (result == 4);
    return result == 2;
}