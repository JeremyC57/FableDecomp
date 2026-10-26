// CAIStateGroup_MinionSitAtSchool::Update() -- read the village school times, pick a randomly staggered end time, walk (seat within 10 units) or run to the seat and sit; then idle while the time of day stays in the school window (leaving at a 1-in-5-seconds random once it is over), raising a hand at random intervals while the village has a lesson on; finally get up and Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FDB0, length 848 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0x23 = CTCVillageMember, bit 3 of +0x24, unchecked),
// CTCVillageMember::GetPVillage @ 0x007EE540, CTCVillage::GetSchoolTimes @ 0x00658ACB, GetPThingWorld @ 0x008FCFA0,
// GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (__fastcall), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThing::GetPosition @ 0x004C73D0, CAIStateGroup_UsesSeat::MoveToSeat @ 0x009385F0 / SitInSeat @ 0x00938380 / the get-up
// @ 0x00938A10, CWorld::GetWorldFrame @ 0x0049D870, AIAction_DoNothing @ 0x0097E200, CCharString(const char*, long) @ 0x0099EBF0
// / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0,
// AIAction_PerformActionAndWait @ 0x0097DB70, Terminate @ 0x008FCF40.
// Data: 2^32 @ 0x0122DCB4, 1.0f @ 0x0122DED8, 10.0f @ 0x01237F0C, g_framesPerSecond @ 0x01375550,
// "ST_SEATED_CHAIR_RAISE_HAND" @ 0x0128F5D4.
// Layout: local model (PBrain +4; brain PCreature +0x18; PSeat +0x14; thing world +0x1c time of day: now +0x08, stagger
// step +0x10; village lesson flag +0xBC; CCreatureAction_PlayAnimation 0xb4 bytes).
// Shape: the world-seed random is inline (seed*0x24A1+0x24DF, GFROR13, `range ? r % range : 0`) through a __forceinline
// RandomFrames(seconds) helper that copies g_framesPerSecond into a local BEFORE fetching the seed and forms the range
// before the seed update: retail loads fps into a callee-saved register ahead of GetPWorldSeed and multiplies (x40/x5)
// between the seed arithmetic and GFROR13; the leave test's fps load stays inside the `over` branch this way.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    __forceinline float GetMagnitude() const;
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
__forceinline float C3DVector::GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
class CThingCreatureBase : public CThing { public: };
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
    unsigned char _pad0c[4];
    float StaggerStep;                          // +0x10
};
struct CThingWorld {
    unsigned char _pad[0x1c];
    CTimeOfDay* PTimeOfDay;                     // +0x1c
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];                  // +0xa8..+0xb3
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CThingWorld* GetPThingWorld();              // 0x008FCFA0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    __forceinline unsigned long RandomFrames(unsigned long seconds)   // random frame count below seconds * fps
    {
        long fps = g_framesPerSecond;
        unsigned long* seed = GetPWorldSeed();
        unsigned long range = fps * seconds;
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return range ? *seed % range : 0;
    }
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PSeat;              // +0x14
    bool MoveToSeat(long movementType);         // 0x009385F0
    bool SitInSeat();                           // 0x00938380
    void GetUpFromSeat();                       // 0x00938A10
};
struct CAIStateGroup_MinionSitAtSchool : CAIStateGroup_UsesSeat {
    virtual void Update();
};

void CAIStateGroup_MinionSitAtSchool::Update()
{
    CTCVillageMember* member;
    GetCreature()->GetInterface(&member, 0x23);
    float startTime;
    float endTime;
    member->GetPVillage()->GetSchoolTimes(&startTime, &endTime);
    float now = GetPThingWorld()->PTimeOfDay->Now;
    unsigned long* seed = GetPWorldSeed();
    *seed = *seed * 0x24A1U + 0x24DFU;
    *seed = GFROR13(*seed);
    unsigned long stagger = *seed & 3;
    endTime = (float)stagger * GetPThingWorld()->PTimeOfDay->StaggerStep + endTime;
    if (endTime > 1.0f)
        endTime = endTime - 1.0f;
    long movement = 5;
    CThing* seat = PSeat.GetPItem();
    if (seat && !(seat->m_flags91 & 1)) {
        const CThing* creature = GetCreature();
        const CThing* item = PSeat.GetPItem();
        if ((item->GetPosition() - creature->GetPosition()).GetMagnitude() < 10.0f)
            movement = 3;
    }
    if (MoveToSeat(movement) && SitInSeat()) {
        long nextHand = CWorld::GetWorldFrame() + RandomFrames(40);
        while (AIAction_DoNothing() == 4) {
            float time = GetPThingWorld()->PTimeOfDay->Now;
            bool over;
            if (endTime > now)
                over = time > endTime || time < now;
            else
                over = time > endTime && time < now;
            if (over && RandomFrames(5) == 0)
                break;
            if (CWorld::GetWorldFrame() > nextHand && member->GetPVillage()->LessonOn) {
                AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_SEATED_CHAIR_RAISE_HAND", CSoundPair(),
                    false, true, false, 0, 0), false);
                nextHand = CWorld::GetWorldFrame() + RandomFrames(40);
            }
        }
        GetUpFromSeat();
    }
    Terminate();
}