// CAIStateGroup_FlyingThingBase::ExecuteFlyingPath(const C3DVector&) -- switch the creature to the flying navigator, build a pretty Bezier path to the destination and fly it curve by curve in tenth steps (skipping points already within reach), restoring the default navigator when done
// Grade: DIFFER(976v954) -- whole body shaped (nav switch, path build, facing/reach, t loop, MoveToPosProximity retry, pop_front, cleanup); residue is register allocation: retail keeps this in edi and promotes path._M_start._M_cur to ebp across the loops, ours keeps this in ebp and spills the front curve, which also moves the pop_front block and the isFinal constant (mov bl,dl v mov bl,1)
#include <math.h>
#include "engine/CAIStateGroupBase.h"
extern "C" void __cdecl free(void*);               // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float Get2DLengthSquared() const { return X * X + Y * Y; }
};
struct CBezierCurve {                              // Layout: Ego_r.pdb CBezierCurve, 4 points, 0x30
    C3DVector Point1;
    C3DVector Point2;
    C3DVector Point3;
    C3DVector Point4;
    C3DVector GetPointOnCurve(float t) const;      // 0x009122A0
};
struct CBezierIterator {                           // STLport deque iterator, 2 curves per 0x60 buffer
    CBezierCurve* _M_cur;
    CBezierCurve* _M_first;
    CBezierCurve* _M_last;
    CBezierCurve** _M_node;
    CBezierIterator() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    void _M_set_node(CBezierCurve** node) { _M_node = node; _M_first = *node; _M_last = _M_first + 2; }
};
struct CBezierDeque {
    CBezierIterator _M_start;
    CBezierIterator _M_finish;                     // +0x10
    CBezierCurve** _M_map;
    unsigned long _M_map_size;
    CBezierDeque() : _M_map(0), _M_map_size(0) { _M_initialize_map(0); }
    ~CBezierDeque();                               // 0x00913CC0
    void _M_initialize_map(unsigned long count);   // 0x009137F0
    bool empty() const { return _M_finish._M_cur == _M_start._M_cur; }
    long size() const
    {
        return 2 * (long)(_M_finish._M_node - _M_start._M_node - 1) + (long)(_M_finish._M_cur - _M_finish._M_first)
               + (long)(_M_start._M_last - _M_start._M_cur);
    }
    CBezierCurve& front() { return *_M_start._M_cur; }
    void pop_front()
    {
        if (_M_start._M_cur != _M_start._M_last - 1)
            ++_M_start._M_cur;
        else {
            if (_M_start._M_first)
                free(_M_start._M_first);
            _M_start._M_set_node(_M_start._M_node + 1);
            _M_start._M_cur = _M_start._M_first;
        }
    }
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
struct CNavigationMethods {
    void SetNavigatorToType(unsigned long type);   // 0x006CBA80
};
extern unsigned long FlyingNavigatorType;          // 0x0129CB44
extern unsigned long DefaultNavigatorType;         // 0x0129CBA4
struct CTCPhysicsBase {                            // Layout: Ego_r.pdb CTCPhysicsBase, Radius +0x34
    virtual void p00(); virtual void p01(); virtual void p02(); virtual void p03(); virtual void p04();
    virtual void p05(); virtual void p06(); virtual void p07(); virtual void p08(); virtual void p09();
    virtual void p10(); virtual void p11(); virtual void p12(); virtual void p13(); virtual void p14();
    virtual void p15(); virtual void p16(); virtual void p17(); virtual void p18(); virtual void p19();
    virtual void p20(); virtual void p21(); virtual void p22(); virtual void p23(); virtual void p24();
    virtual void p25(); virtual void p26(); virtual void p27(); virtual void p28(); virtual void p29();
    virtual void p30(); virtual void p31(); virtual void p32(); virtual void p33(); virtual void p34();
    virtual void p35(); virtual void p36(); virtual void p37(); virtual void p38(); virtual void p39();
    virtual void p40(); virtual void p41(); virtual void p42(); virtual void p43(); virtual void p44();
    virtual void p45(); virtual void p46(); virtual void p47(); virtual void p48(); virtual void p49();
    virtual void p50(); virtual void p51(); virtual void p52(); virtual void p53(); virtual void p54();
    virtual void p55(); virtual void p56(); virtual void p57(); virtual void p58(); virtual void p59();
    virtual void p60(); virtual void p61(); virtual void p62(); virtual void p63(); virtual void p64();
    virtual void p65(); virtual void p66();
    virtual void SetMoving(bool moving);           // slot 67 (+0x10c)
    virtual void p68();
    virtual void p69();
    virtual C3DVector GetFacing() const;           // slot 70 (+0x118)
    unsigned char _pad04[0x34 - 4];
    float Radius;                                  // +0x34
};
struct CThingFlying {                              // Layout: CThing UsingComponents +0x20, Components +0x44, PhysicsTC +0x60
    virtual void t00(); virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04();
    virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09();
    virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14();
    virtual void t15(); virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19();
    virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23(); virtual void t24();
    virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29();
    virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34();
    virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39();
    virtual void t40(); virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44();
    virtual void t45(); virtual void t46(); virtual void t47(); virtual void t48(); virtual void t49();
    virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54();
    virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59();
    virtual void t60(); virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64();
    virtual void t65(); virtual void t66(); virtual void t67(); virtual void t68(); virtual void t69();
    virtual void t70(); virtual void t71(); virtual void t72(); virtual void t73(); virtual void t74();
    virtual void t75(); virtual void t76(); virtual void t77(); virtual void t78(); virtual void t79();
    virtual void t80(); virtual void t81(); virtual void t82();
    virtual float GetFlightAllowance(const C3DVector& pos);   // slot 83 (+0x14c)
    unsigned char _pad04[0x20 - 4];
    unsigned long UsingComponents[9];              // +0x20
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                     // +0x60
    const C3DVector& GetPosition() const;          // 0x004C73D0
    const C3DVector& GetCreaturePosition() const;  // 0x006A4D00 (CThingCreatureBase::GetPosition)
    bool TestTC(long type) const { return ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0; }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType, bool* has)
    {
        *has = TestTC(interfaceType);
        if (*has) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct CBrainUpdateZone {
    long Unknown;
    unsigned long UpdatePeriod;                    // +4
};
struct CAIBrain {                                  // Layout: retail offsets (engine/CAIBrain.h has the debug-build +0xa8)
    unsigned char _pad00[0x18];
    CThingFlying* Creature;                        // +0x18
    unsigned char _pad1c[0xa0 - 0x1c];
    const CBrainUpdateZone* PCurrentUpdateZone;    // +0xa0
};
enum EAIActionReturn { AI_ACTION_RETURN_0, AI_ACTION_STOPPED, AI_ACTION_DONE, AI_ACTION_FAILED, AI_ACTION_RUNNING };
enum EAIActionRepeat { AI_ACTION_REPEAT_0, AI_ACTION_REPEAT_1, AI_ACTION_REPEAT_2 };
enum EMovementType { MOVEMENT_FLY = 7 };
struct CThing;
extern const float EPSILON;                        // 0x0129BA3C (0.0001f)
struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {   // Layout: Ego_r.pdb, base 0x14
    float MinHeightToKeepAboveGround;              // +0x14
    float MinHeightToKeepAboveGroundInFlight;      // +0x18
    float MaxHeightAboveGround;                    // +0x1c
    float FlightRange;                             // +0x20
    CThingFlying* GetCreature() const { return PBrain->Creature; }
    bool IsStopping() const;                       // 0x008FD010
    bool CalculatePrettyPath(CBezierDeque& path, const C3DVector& destination);   // 0x00912BA0
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& flag,
        EMovementType type, bool a, bool b, bool c, const CThing* ignore, bool d, float speed);   // 0x0097E890
    EAIActionReturn ExecuteFlyingPath(const C3DVector& destination);
};

EAIActionReturn CAIStateGroup_FlyingThingBase::ExecuteFlyingPath(const C3DVector& destination)
{
    if (IsStopping())
        return AI_ACTION_STOPPED;
    CNavigationMethods* navigation;
    bool hasNavigation;
    GetCreature()->GetInterface(&navigation, 7, &hasNavigation);
    navigation->SetNavigatorToType(FlyingNavigatorType);
    CBezierDeque path;
    if (!CalculatePrettyPath(path, destination)) {
        navigation->SetNavigatorToType(DefaultNavigatorType);
        return AI_ACTION_FAILED;
    }
    long updatePeriod = 1;
    if (PBrain->PCurrentUpdateZone)
        updatePeriod = PBrain->PCurrentUpdateZone->UpdatePeriod;
    CThingFlying* creature = GetCreature();
    C3DVector ahead = creature->GetPosition() + creature->PhysicsTC->GetFacing();
    float radius = GetCreature()->PhysicsTC->Radius;
    float reach = GetCreature()->GetFlightAllowance(ahead) * updatePeriod + radius + 0.01f;
    float reachSquared = reach * reach;
    while (!path.empty()) {
        CBezierCurve& curve = path.front();
        for (float t = 0.1f; t <= 1.0f + EPSILON; t += 0.1f) {
            bool isFinal = path.size() == 1 && fabs(t - 1.0f) < EPSILON;
            C3DVector point = curve.GetPointOnCurve(t);
            if (!isFinal && !((point - GetCreature()->GetCreaturePosition()).Get2DLengthSquared() >= reachSquared))
                continue;
            bool flag = false;
            EAIActionReturn result;
            for (;;) {
                result = AIAction_MoveToPosProximity(AI_ACTION_REPEAT_2, point, 0.01f, flag, MOVEMENT_FLY, true, false, true, 0, true, 0.0f);
                bool close = isFinal ? false : (point - GetCreature()->GetCreaturePosition()).Get2DLengthSquared() <= reachSquared;
                if (result == AI_ACTION_RUNNING) {
                    if (close)
                        break;
                } else if (result == AI_ACTION_DONE)
                    break;
                else
                    return result;
            }
        }
        path.pop_front();
    }
    GetCreature()->PhysicsTC->SetMoving(false);
    navigation->SetNavigatorToType(DefaultNavigatorType);
    return AI_ACTION_DONE;
}
