// Grade: DIFFER(751v735) -- the x87 offset math and the action loops match apart from: frame 0x50 v 0x4c (retail reuses the CCharString slot for the marker pointer), the first cross-product term's operand order, and the move loop's head (retail falls into it unaligned; ours jumps to an aligned head, +16 bytes); 3 grades
// CAIStateGroup_MinionListenToStory::Update() -- find the nearest story place marker, walk to a random spot 3.5..5 in front of it (within the configured angle), face the marker, sit down and idle while the village says it is story time; terminate when anything fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009101C0, length 736 bytes (+ int3 padding), plain `ret`, frame 0x4c.
// Callees: CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, CGameDefinitionManager::Get @ 0x0044C6B0, GetDefGlobalIndexFromName
// @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, PeekThingSearchTools @ 0x008FD000, GetPBestThing<HasDefGlobalIndex, Nearest>
// @ 0x009107B0, the marker's +0x60 object virtual slot 72 (+0x120, its right-handed set), GetPWorldSeed @ 0x008FCFD0, GFFloatRandom
// @ 0x004978A0 (__fastcall seed), AIAction_MoveToPosProximity @ 0x0097E890, AIAction_LookAtPos @ 0x0097D580, SitDown @ 0x0090F830,
// AIAction_DoNothing @ 0x0097E200, CVectorMap::LowerBound @ 0x0040F020 (0x23 = CTCVillageMember), CTCVillageMember::GetPVillage
// @ 0x007EE540, CTCVillage story-time test @ 0x00658A80, Terminate @ 0x008FCF40.
// Layout: local model (PBrain +4; marker +0x60 object; the angle-range and proximity globals @ 0x0138C464 / 0x0138C460).
// Shape: the offset is Forward*cos + (Forward x Up)*sin scaled by the random radius.
extern "C" double __cdecl sin(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sin, cos)
extern float g_StoryAngleRange;                 // 0x0138C464
extern float g_StoryProximity;                  // 0x0138C460
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0
class CCharString {
public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
private:
    void* PStringData;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - b.Z * a.X, b.Y * a.X - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CPlacementObject;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x14];
    CPlacementObject* PPlacement;               // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CThingAICreature : CThingCreatureBase { };
struct CTCVillage {
    bool IsStoryTime();                         // 0x00658A80
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetPVillage();                  // 0x007EE540
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager* Get();       // 0x0044C6B0
};
static __forceinline long GetDefGlobalIndex(const char* defName)
{
    CCharString name(defName);
    return CGameDefinitionManager::Get()->GetDefGlobalIndexFromName(name);
}
struct CThingFilter_HasDefGlobalIndex {
    long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(long index) : DefGlobalIndex(index) { }
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
struct CThingSearchTools {
    bool GetPBestThing(const CThingFilter_HasDefGlobalIndex& filter, const CThingCompare_Nearest& compare, CThing** best);   // 0x009107B0
};
struct CPlacementVirtuals {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d();
    virtual void v1e(); virtual void v1f(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e(); virtual void v2f();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v3a(); virtual void v3b();
    virtual void v3c(); virtual void v3d(); virtual void v3e(); virtual void v3f(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
};
struct CPlacementObject : CPlacementVirtuals { };
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* arrived, long movement,
                                     bool a, bool b, bool c, const CThing* ignore, bool d, float e);   // 0x0097E890
    long AIAction_LookAtPos(long repeat, C3DVector pos, long frames, float speed);   // 0x0097D580
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_MinionListenToStory : CAIStateGroupBase {
    bool SitDown();                             // 0x0090F830
    void Update();
};

void CAIStateGroup_MinionListenToStory::Update()
{
    long markerIndex = GetDefGlobalIndex("STORY_PLACE_MARKER");
    CThing* marker = 0;
    PeekThingSearchTools()->GetPBestThing(CThingFilter_HasDefGlobalIndex(markerIndex),
                                          CThingCompare_Nearest(GetCreature()->GetPosition()), &marker);
    if (marker) {
        C3DVector markerPos = marker->GetPosition();
        CRightHandedSet rhset = marker->PPlacement->GetRHSet();
        float radius = (float)(GFFloatRandom(GetPWorldSeed(), 1.5f) + 3.5);
        float angle = GFFloatRandom(GetPWorldSeed(), g_StoryAngleRange * 6.2831855f) - g_StoryAngleRange * 3.1415927f;
        C3DVector right = CrossProduct(rhset.Forward, rhset.Up);
        C3DVector target = markerPos + rhset.Forward * ((float)cos(angle) * radius) + right * ((float)sin(angle) * radius);
        bool arrived;
        long result;
        for (;;) {
            result = AIAction_MoveToPosProximity(2, target, g_StoryProximity, &arrived, 5, true, false, true, 0, true, 0.0f);
            if (result != 4)
                break;
        }
        if (result == 2) {
            do {
                result = AIAction_LookAtPos(2, markerPos, 0x11, 0.0027777f);
            } while (result == 4);
            if (SitDown()) {
                CTCVillageMember* member;
                while (AIAction_DoNothing() == 4) {
                    GetCreature()->GetInterface(&member, 0x23);
                    if (!member->GetPVillage()->IsStoryTime())
                        break;
                }
            }
        }
    }
    Terminate();
}
