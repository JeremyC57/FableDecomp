// CAIStateGroup_JackOfBladesBase::RushToPos(const C3DVector&) -- make the creature non-solid to its target thing (adding a CTCSolidToThing when it has none), cast the assassin rush toward the position (normalised direction and distance) with a RUSH_OUTOF follow-up animation, then make it solid to the target again; returns the action result
// Grade: DIFFER(684v690) nd=113 (was 670v690 nd=128) -- lever: the three squared components as named floats summed y, z, x (retail spills each square to its own word and reuses them for the distance sqrt); tried: x,y,z sum (664 nd=126); engine/CCountedPointer.h NOT applied: the move drops the follow-up release block (604v690), the local CCountedPointer/SCountBlock model stays; remaining: lengthSquared/sqrt spill slots and the first squared component's fld st(2) form
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009735D0, length 690 bytes, `ret 4`.
// Callees: CThing::GetPosition @ 0x004C73D0, CVectorMap::LowerBound @ 0x0040F020, CCharString ctor @ 0x0099EBF0 / dtor
// @ 0x0099EAE0, CThing::AddTC @ 0x004C9D60, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCSolidToThing::
// AddThingNotSolidTo @ 0x00770FA0 / RemoveThingNotSolidTo @ 0x00770F20, operator new @ 0x00BFEA1A,
// CCreatureAction_PlayCombatAnimation ctor @ 0x008B7FF0, counted-pointer deleter @ 0x00668610, operator delete
// @ 0x00BFE9BC, CCreatureAction_CastAssassinRush ctor @ 0x008CE1B0, ~CCombatActionBase @ 0x0062E900,
// AIAction_PerformActionAndWait @ 0x0097DB70.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CAIStateGroup_JackOfBladesBase PTargetThing +0x24.
#include <math.h>
#pragma intrinsic(sqrt)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return Y * Y + Z * Z + X * X; }
    float GetLength() const { return sqrt(GetLengthSquared()); }
    __forceinline void Normalise()
    {
        float lengthSquared = GetLengthSquared();
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CTCBase;
struct CTCParamsBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    bool HasTC(long type) const { return (m_presentBits[type >> 5] >> (type & 31)) & 1; }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing { };
struct CTCSolidToThing {                        // interface 0x4d
    void AddThingNotSolidTo(CThing* thing);     // 0x00770FA0
    void RemoveThingNotSolidTo(CThing* thing);  // 0x00770F20
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
void __fastcall CountedDestroy(void* object);   // 0x00668610
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(CountedDestroy), m_pObject(object) { }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
template<class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer(T* object)
    {
        m_pObject = object;
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& o) : m_pObject(o.m_pObject), m_pCount(o.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
};
struct CCreatureAction_PlayCombatAnimation : CCreatureActionBase {
    unsigned char _anim[0x118 - 0xa8];
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& anim, bool b);   // 0x008B7FF0
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_CastAssassinRush : CCombatActionBase {
    unsigned char _rush[0x12c - 0x114];
    CCountedPointer<CCreatureActionBase> FollowUp;   // +0x12c
    unsigned char _tail[4];
    CCreatureAction_CastAssassinRush(CThingCreatureBase* creature, const C3DVector& dir, float distance, bool a, bool b,
                                     CCountedPointer<CCreatureActionBase> followUp);   // 0x008CE1B0
    ~CCreatureAction_CastAssassinRush() { FollowUp.m_pObject = 0; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    unsigned char _pad14[0x24 - 0x14];
    CBaseIntelligentPointer PTargetThing;       // +0x24
    long RushToPos(const C3DVector& pos);
};

long CAIStateGroup_JackOfBladesBase::RushToPos(const C3DVector& pos)
{
    CTCSolidToThing* solid = 0;
    C3DVector offset = pos - GetCreature()->GetPosition();
    float yy = offset.Y * offset.Y;
    float zz = offset.Z * offset.Z;
    float xx = offset.X * offset.X;
    float lengthSquared = yy;
    lengthSquared += zz;
    lengthSquared += xx;
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    r = (3.0f - r * r * lengthSquared) * r * 0.5f;
    C3DVector dir;
    dir.X = offset.X * r;
    dir.Y = offset.Y * r;
    dir.Z = offset.Z * r;
    float distance = (float)sqrt(xx + yy + zz);
    if (GetCreature()->HasTC(0x4d))
        solid = GetCreature()->GetTC<CTCSolidToThing>(0x4d);
    else
        solid = (CTCSolidToThing*)GetCreature()->AddTC(CCharString("CTCSolidToThing"), false, 0);
    if (solid)
        solid->AddThingNotSolidTo(PTargetThing.GetPItem());
    CCountedPointer<CCreatureActionBase> followUp(new CCreatureAction_PlayCombatAnimation(GetCreature(), CCharString("RUSH_OUTOF"), true));
    long result = AIAction_PerformActionAndWait(&CCreatureAction_CastAssassinRush(GetCreature(), dir, distance, false, false, followUp), false);
    if (IS_THING_ALIVE(PTargetThing) && solid)
        solid->RemoveThingNotSolidTo(PTargetThing.GetPItem());
    return result;
}
