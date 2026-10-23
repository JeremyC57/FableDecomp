// CCombatManager::GetBestSequenceIndexAttackerOnDefender(CCombatantInfo* attackerInfo, CCombatantInfo* defenderInfo) -- sort the attacker's usable combat sequences into forced / type-A / type-B / special / ordinary index lists, veto attacks on a defender who is not due (or not in line of sight across a steep slope), then pick a random forced sequence or the best of the first applicable list; -1 if none
// Grade: DIFFER(1394v1387) -- 73 blocks: first pass; this/attacker-info register choice (retail edi/esi, ebx = 0 then the forced list's finish cached), frame 0xb4 v 0xbc, flags kept in registers
// Leaf: GetBestSequenceIndexAttackerOnDefender
// Retail 0x006EAE20, 1392 bytes, `ret 8`.
// Callees: sequence-position check 0x006E9700; IsDefenderDueForAttack 0x006E97F0; CCombatSequenceBase::IsZoneValid
//   0x008B0610; vector<long> insert-overflow 0x00440990 / push_back 0x00485FF3; CThing::GetPosition 0x004C73D0; __ftol2;
//   CThingSearchTools area collectors 0x006F3570 / 0x006F37B0; CTools::IsLineOfSight 0x00877840; IsKnockedDown 0x0085F010
//   (`__fastcall`); CWorld::GetWorldSeed 0x0049D810; GFROR13 0x00497890 (`__fastcall`); CBaseIntelligentPointer::GetPItem
//   0x00A01B50; PickBestSequenceFromIndexList 0x006E8730; free 0x00BFEA14.
// Layout: local model from this function's use -- CCombatManager {world +0x20, combat disabled +0x2c}; CCombatantInfo
//   {creature +0, current sequence index +0x24, sequences +0x28 (8-byte entries), attacker count +0x38, target +0x5c};
//   CWorld {search tools +0x20, physics +0x34}; physics query result {CTools +0x78}; 12-byte STLport vectors.
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl fabs(double);
extern "C" double __cdecl cos(double);
#pragma intrinsic(sqrt, fabs, cos)
extern "C" void __cdecl free(void* p);
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct CRect2DI {
    C2DCoordI Min, Max;
    CRect2DI(const C2DCoordI& a, const C2DCoordI& b) : Min(a), Max(b) { }
};
struct STrueType { STrueType() { } };
struct CLongVector {
    long* Start;
    long* Finish;
    long* End;
    CLongVector() : Start(0), Finish(0), End(0) { }
    ~CLongVector()
    {
        if (Start)
            free(Start);
    }
    bool empty() const { return Start == Finish; }
    unsigned long size() const { return Finish - Start; }
    void InsertOverflow(long* pos, const long& x, const STrueType& tag, unsigned long n, bool atEnd);   // 0x00440990
    void PushBack(const long& x);                     // 0x00485FF3
    __forceinline void push_back(const long& x)
    {
        if (Finish != End) {
            if (Finish)
                *Finish = x;
            ++Finish;
        } else
            InsertOverflow(Finish, x, STrueType(), 1, true);
    }
};
struct CThing;
struct CThingVector {
    CThing** Start;
    CThing** Finish;
    CThing** End;
    CThingVector() : Start(0), Finish(0), End(0) { }
    ~CThingVector()
    {
        if (Start)
            free(Start);
    }
};
struct CThing {
    virtual void t00();
    virtual void t01();
    virtual void t02();
    virtual void t03();
    virtual void t04();
    virtual void t05();
    virtual void t06();
    virtual void t07();
    virtual void t08();
    virtual void t09();
    virtual void t0a();
    virtual void t0b();
    virtual void t0c();
    virtual void t0d();
    virtual void t0e();
    virtual void t0f();
    virtual void t10();
    virtual void t11();
    virtual void t12();
    virtual void t13();
    virtual void t14();
    virtual void t15();
    virtual void t16();
    virtual void t17();
    virtual void t18();
    virtual void t19();
    virtual void t1a();
    virtual void t1b();
    virtual void t1c();
    virtual void t1d();
    virtual void t1e();
    virtual void t1f();
    virtual void t20();
    virtual void t21();
    virtual void t22();
    virtual void t23();
    virtual void t24();
    virtual void t25();
    virtual void t26();
    virtual void t27();
    virtual void t28();
    virtual void t29();
    virtual void t2a();
    virtual void t2b();
    virtual void t2c();
    virtual void t2d();
    virtual void t2e();
    virtual void t2f();
    virtual void t30();
    virtual void t31();
    virtual void t32();
    virtual void t33();
    virtual void t34();
    virtual void t35();
    virtual void t36();
    virtual void t37();
    virtual void t38();
    virtual void t39();
    virtual void t3a();
    virtual void t3b();
    virtual void t3c();
    virtual void t3d();
    virtual void t3e();
    virtual void t3f();
    virtual void t40();
    virtual void t41();
    virtual void t42();
    virtual void t43();
    virtual void t44();
    virtual void t45();
    virtual void t46();
    virtual void t47();
    virtual void t48();
    virtual void t49();
    virtual void t4a();
    virtual void t4b();
    virtual void t4c();
    virtual bool IsAlive();
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
bool __fastcall IsKnockedDown(CThing* thing);          // 0x0085F010
struct CThingCreatureBase : CThing { };
struct CCombatSequenceBase {
    virtual void s00();
    virtual void s01();
    virtual void s02();
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual bool IsTypeA();
    virtual bool IsTypeB();
    virtual bool IsSpecial();
    virtual void s0a();
    virtual void s0b();
    virtual bool IsAvailable();
    virtual bool IsForced();
    bool IsZoneValid();                                // 0x008B0610
};
struct SSequenceEntry { CCombatSequenceBase* Sequence; long Data; };
struct CBaseIntelligentPointer {
    void* Data;
    void* Info;
    CThing* GetPItem() const;                          // 0x00A01B50
};
struct CCombatantInfo {
    CThingCreatureBase*     Creature;                  // +0x00
    unsigned char           _pad04[0x24 - 0x04];
    long                    CurrentSequence;           // +0x24
    SSequenceEntry*         SequencesStart;            // +0x28
    SSequenceEntry*         SequencesFinish;           // +0x2c
    unsigned char           _pad30[0x38 - 0x30];
    unsigned long           AttackerCount;             // +0x38
    unsigned char           _pad3c[0x5c - 0x3c];
    CBaseIntelligentPointer Target;                    // +0x5c
};
struct CThingFilter_HasTrackName { };
struct CThingFilter_IsPotentialCustomer { };
struct CThingSearchTools {
    void Collect(const CRect2DI& area, const CThingFilter_HasTrackName& filter, CThingVector* out);          // 0x006F3570
    void Collect(const CRect2DI& area, const CThingFilter_IsPotentialCustomer& filter, CThingVector* out);   // 0x006F37B0
};
struct CTools {
    bool IsLineOfSight(CThingVector* ignore, C3DVector* from, C3DVector* to, C3DVector* hitPosition, C3DVector* hitNormal,
                       float* fraction, CThing** hitThing, long test, bool flag);   // 0x00877840
};
struct CPhysicsQuery {
    unsigned char _pad00[0x78];
    CTools        Tools;                               // +0x78
};
struct CPhysics {
    virtual void p00();
    virtual void p01();
    virtual void p02();
    virtual void p03();
    virtual void p04();
    virtual void p05();
    virtual void p06();
    virtual void p07();
    virtual void p08();
    virtual void p09();
    virtual void GetQuery(CPhysicsQuery** out);
};
struct CWorld {
    unsigned char      _pad00[0x20];
    CThingSearchTools* SearchTools;                    // +0x20
    unsigned char      _pad24[0x34 - 0x24];
    CPhysics*          Physics;                        // +0x34
    unsigned long* GetWorldSeed();                     // 0x0049D810
};
class CCombatManager {
public:
    unsigned char _pad00[0x20];
    CWorld*       World;                               // +0x20
    unsigned char _pad24[0x2c - 0x24];
    bool          CombatDisabled;                      // +0x2c
    void ValidateSequencePosition(CThingCreatureBase* attacker);   // 0x006E9700
    bool IsDefenderDueForAttack(CCombatantInfo* defender);         // 0x006E97F0
    long PickBestSequenceFromIndexList(CLongVector* list);         // 0x006E8730
    long GetBestSequenceIndexAttackerOnDefender(CCombatantInfo* attackerInfo, CCombatantInfo* defenderInfo);
};

long CCombatManager::GetBestSequenceIndexAttackerOnDefender(CCombatantInfo* attackerInfo, CCombatantInfo* defenderInfo)
{
    CThingCreatureBase* attacker = attackerInfo->Creature;
    CThingCreatureBase* defender = defenderInfo->Creature;
    CCombatSequenceBase* current = 0;
    if (attackerInfo->CurrentSequence >= 0)
        current = attackerInfo->SequencesStart[attackerInfo->CurrentSequence].Sequence;
    CLongVector forced;
    CLongVector typeA;
    CLongVector typeB;
    CLongVector special;
    CLongVector ordinary;
    ValidateSequencePosition(attacker);
    bool due = IsDefenderDueForAttack(defenderInfo);
    if (CombatDisabled)
        due = false;
    long index = 0;
    for (SSequenceEntry* it = attackerInfo->SequencesStart; it != attackerInfo->SequencesFinish; ++it, ++index) {
        CCombatSequenceBase* sequence = it->Sequence;
        bool isA = sequence->IsTypeA();
        bool isB = sequence->IsTypeB();
        if ((isA || isB) && !due)
            continue;
        if (!sequence->IsAvailable() || !sequence->IsZoneValid())
            continue;
        if (sequence->IsForced())
            forced.push_back(index);
        else if (isA)
            typeA.PushBack(index);
        else if (isB)
            typeB.PushBack(index);
        else if (sequence->IsSpecial())
            special.PushBack(index);
        else
            ordinary.PushBack(index);
    }
    if (due && defender->IsAlive() && (!typeA.empty() || !typeB.empty())) {
        const C3DVector& defenderPos = defender->GetPosition();
        const C3DVector& attackerPos = attacker->GetPosition();
        C3DVector delta(attackerPos.X - defenderPos.X, attackerPos.Y - defenderPos.Y, attackerPos.Z - defenderPos.Z);
        const C3DVector& p1 = defender->GetPosition();
        const C3DVector& p2 = attacker->GetPosition();
        long centreX = (long)((p2.X + p1.X) * 0.5f);
        long centreY = (long)((p1.Y + p2.Y) * 0.5f);
        long radius = (long)((float)sqrt(delta.X * delta.X + delta.Z * delta.Z + delta.Y * delta.Y) * 0.5f);
        CRect2DI area(C2DCoordI(centreX - radius, centreY - radius), C2DCoordI(centreX + radius, centreY + radius));
        CThingVector ignore;
        World->SearchTools->Collect(area, CThingFilter_HasTrackName(), &ignore);
        World->SearchTools->Collect(area, CThingFilter_IsPotentialCustomer(), &ignore);
        CPhysicsQuery* query = 0;
        World->Physics->GetQuery(&query);
        const C3DVector& from = attacker->GetPosition();
        C3DVector eye(from.X, from.Y, from.Z + 1.0f);
        const C3DVector& to = defender->GetPosition();
        C3DVector target(to.X, to.Y, eye.Z);
        C3DVector hitPosition;
        C3DVector hitNormal;
        float fraction;
        CThing* hitThing;
        if (!query->Tools.IsLineOfSight(&ignore, &eye, &target, &hitPosition, &hitNormal, &fraction, &hitThing, 1, false) &&
            fabs(hitNormal.Z) < cos(1.0471975511965976))
            due = false;
    }
    bool currentIsB = false;
    if (current && current->IsTypeB())
        currentIsB = true;
    bool knockedDown = IsKnockedDown(defender);
    if (current && current->IsTypeA() && defenderInfo->AttackerCount > 1)
        due = false;
    long result = -1;
    if (!forced.empty()) {
        unsigned long* seed = World->GetWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        unsigned long random = *seed = GFROR13(*seed);
        unsigned long range = forced.size() * 100;
        result = forced.Start[(long)(range ? random % range : 0) / 100];
    } else if (!special.empty() && knockedDown)
        result = PickBestSequenceFromIndexList(&special);
    else if (!typeB.empty() && defenderInfo->Target.GetPItem() != attacker && due)
        result = PickBestSequenceFromIndexList(&typeB);
    else if (!typeA.empty() && (due || currentIsB))
        result = PickBestSequenceFromIndexList(&typeA);
    else if (!ordinary.empty())
        result = PickBestSequenceFromIndexList(&ordinary);
    return result;
}
