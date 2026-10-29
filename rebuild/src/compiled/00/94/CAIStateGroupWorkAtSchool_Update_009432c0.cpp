// CAIStateGroup_WorkAtSchool::Update() -- teacher loop: walk to the blackboard or a desk at random, mark the lesson on, wait for a pupil seat, then while inside school hours talk to up to four pupils and finally go to a seat and crouch-talk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009432C0, length 1198 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (school pointer
// vtable 0x01244344), LowerBound @ 0x0040F020 (interface 0x23 CTCVillageMember), GetPVillage @ 0x007EE540, GetSchoolTimes @
// 0x00658ACB, GetPThingWorld @ 0x008FCFA0, GetDummyObject @ 0x004C08C0, GetDummyObjectRecursive @ 0x00942B30 (__fastcall),
// GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, GFRandom @ 0x004CDAE0, GetRandomFreeSeatInBuilding @ 0x009431E0,
// AIAction_DoNothing @ 0x0097E200, AIAction_LookAtPos @ 0x0097D580, MoveAndFace @ 0x009425F0, talk helpers @ 0x009428A0 /
// 0x00942C70 / 0x00942820, GetPos @ 0x004C73D0, Terminate @ 0x008FCF40. Data: 2^32 @ 0x0122DCB4, 3.0f @ 0x01292A20.
// Layout: local model (PBrain +4; brain PCreature +0x18; creature workplace pointer +0x170; thing graphic appearance +0x64;
// village lesson flag +0xBC; time of day now +0x08).
// Shape: the time-of-day pointer goes through a named local (fixes the eax/ecx rotation of every following lea); the dummy
// names are scoped named CCharStrings with the appearance read into a local after the ctor; the coin flip is
// `long side = r & 1; bool b = side == 1;` (cmp/sete); MoveAndFace is written in each arm (the common arg-push prefix is
// hoisted by VC7.1) and GFRandom is called inside each talk arm (hoisted too); the look speed is 0.0027777f (0x3B360A13).
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CRightHandedSet { public:
    C3DVector Forward;
    C3DVector Up;
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set, float scale);   // 0x004C08C0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;   // +0x64
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool found = (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) != 0;
        if (found) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
        }
        return found;
    }
};
bool __fastcall GetDummyObjectRecursive(CThing* thing, const CCharString& name, long index, unsigned long flags, C3DVector* pos, CRightHandedSet* set);   // 0x00942B30
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad91[0x170 - 0x68];
    CBaseIntelligentPointer Workplace;          // +0x170
};
struct CVillage {
    unsigned char _pad[0xbc];
    bool LessonOn;                              // +0xBC
    bool GetSchoolTimes(float* start, float* end);   // 0x00658ACB
};
class CTCVillageMember { public:
    CVillage* GetPVillage();                    // 0x007EE540
};
struct CTimeOfDay {
    unsigned char _pad[0x08];
    float Now;                                  // +0x08
};
class CWorld { public:
    unsigned char _pad[0x1c];
    CTimeOfDay* PTimeOfDay;                     // +0x1c
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CThing* GetRandomFreeSeatInBuilding(CThing* building);   // 0x009431E0
};
struct CAIStateGroup_WorkAtSchool : CAIStateGroup_UsesSeat {
    bool MoveAndFace(C3DVector pos, float speed, C3DVector facing);   // 0x009425F0
    bool CrouchAndTalk(float seconds);          // 0x009428A0
    bool StandAndTalk(float seconds);           // 0x00942C70
    bool SitAndTalk(float seconds);             // 0x00942820
    virtual void Update();
};

void CAIStateGroup_WorkAtSchool::Update()
{
    CIntelligentPointerThing school(((CThing*)GetCreature()->Workplace.GetPItem()));
    if (!((CThing*)school.GetPItem())) {
        Terminate();
        return;
    }
    CTCVillageMember* member;
    ((CThing*)school.GetPItem())->GetInterface(&member, 0x23);
    float startTime;
    float endTime;
    member->GetPVillage()->GetSchoolTimes(&startTime, &endTime);
    CTimeOfDay* tod = PeekWorld().PTimeOfDay;
    float now = tod->Now;
    C3DVector boardPos;
    CRightHandedSet boardSet;
    {
        CCharString name("ACTION_POINT_BLACKBOARD");
        CTCGraphicAppearance* app = ((CThing*)school.GetPItem())->PGraphicAppearance;
        app->GetDummyObject(name, 0, 0, &boardPos, &boardSet, 1.0f);
    }
    C3DVector deskPos;
    CRightHandedSet deskSet;
    {
        CCharString name("ACTION_POINT_DESK");
        GetDummyObjectRecursive(((CThing*)school.GetPItem()), name, 0, 0, &deskPos, &deskSet);
    }
    C3DVector boardLook = boardPos + boardSet.Up;
    C3DVector deskLook = deskPos + deskSet.Up;
    for (;;) {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        long side = *seed & 1;
        bool atBoard = side == 1;
        bool moved;
        if (atBoard)
            moved = MoveAndFace(boardPos, 0.0f, boardLook);
        else
            moved = MoveAndFace(deskPos, 0.0f, deskLook);
        if (!moved)
            break;
        CTCVillageMember* self;
        if (GetCreature()->GetInterface(&self, 0x23) && self->GetPVillage())
            self->GetPVillage()->LessonOn = true;
        while (!GetRandomFreeSeatInBuilding(((CThing*)school.GetPItem()))) {
            if (AIAction_DoNothing() != 4) {
                Terminate();
                return;
            }
        }
        CTimeOfDay* tod2 = PeekWorld().PTimeOfDay;
        float time = tod2->Now;
        if (!((CThing*)school.GetPItem()))
            break;
        bool over;
        if (endTime > now)
            over = time > endTime || time < now;
        else
            over = time > endTime && time < now;
        if (over)
            break;
        for (long i = 0; i < 4; ++i) {
            CThing* pupil = GetRandomFreeSeatInBuilding(((CThing*)school.GetPItem()));
            if (!pupil)
                break;
            while (AIAction_LookAtPos(2, pupil->GetPos(), 0x11, 0.0027777f) == 4)
                ;
            bool ok;
            if (atBoard)
                ok = CrouchAndTalk((float)GFRandom(4, GetPWorldSeed()) + 3.0f);
            else
                ok = StandAndTalk((float)GFRandom(4, GetPWorldSeed()) + 3.0f);
            if (!ok) {
                Terminate();
                return;
            }
        }
        CThing* seat = GetRandomFreeSeatInBuilding(((CThing*)school.GetPItem()));
        if (!seat)
            continue;
        if (!MoveAndFace(seat->GetPos(), 1.0f, seat->GetPos()))
            break;
        unsigned long* seed2 = GetPWorldSeed();
        *seed2 = *seed2 * 0x24A1U + 0x24DFU;
        *seed2 = GFROR13(*seed2);
        if (!SitAndTalk((float)(*seed2 % 3 + 2)))
            break;
    }
    Terminate();
}