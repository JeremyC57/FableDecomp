// CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(std::vector<CThing*>& fire_points) -- collect the preferred fire points that are not occupied in the 60x60 box around the creature
// Grade: DIFFER(130v130) -- one lea: the CThingFilter_IsPreferredFirePoint temporary is homed in the dead argument slot (lea ecx,[esp+34h]) where retail keeps both filter operands in the frame (+6/+7)
// Leaf: ?GetFirePointList@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D4C0, 130 bytes, ret 4. Ego_r + FableWin .pdb
//   ?GetFirePointList@CAIStateGroup_RangedCombatUsePreferredFirePoint@@IAEXAAV?$vector@PAVCThing@@...@Z (protected; vector,
//   not list). FableWin names the whole expression: GFFilter_Bind2<CThingFilter_IsPreferredFirePoint,
//   std::unary_negate<CThingFilter_IsOccupiableOccupied> > (0x02C21580), std::not1<CThingFilter_IsOccupiableOccupied>,
//   C2DBoxF(const C2DVector&, float), C2DVector(const C3DVector&), C2DBoxF::ToC2DBoxI() const.
// Callees: CThing::GetPos() const 0x004C73D0, C2DBoxF::ToC2DBoxI() const 0x00A55A30, CAIStateGroupBase::PeekThingSearchTools()
//   const 0x008FD000, CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, CFilter_Bind2<...> > 0x0095DB90
//   (Ego_r donor 0x00982220). The half-size 30.0f is the shared float constant 0x01295318.
// Tried (this session): everything in the call (filter pointers stored before GetPos), named box (this file), named
//   preferred + named unary_negate (frame 0x30), named preferred only (+4 and the dead slot). Earlier: named filter locals,
//   free template operator&&.
// Next: three named one-byte locals -- CThingFilter_IsOccupiableOccupied occupied; std::unary_negate<...> unoccupied(occupied);
//   CThingFilter_IsPreferredFirePoint preferred; -- retail's frame has four one-byte slots at +4..+7 with +6/+7 bound.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template <class _Arg, class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template <class _Predicate>
class unary_negate : public unary_function<typename _Predicate::argument_type, bool> {
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& __x) : _M_pred(__x) { }
};
template <class _Predicate>
inline unary_negate<_Predicate> not1(const _Predicate& __pred) { return unary_negate<_Predicate>(__pred); }
}

class C3DVector { public: float X, Y, Z; };
class C2DVector {
public:
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& centre, float radius)
        : TLX(centre.X - radius), TLY(centre.Y - radius), BRX(centre.X + radius), BRY(centre.Y + radius) { }
    C2DBoxI ToC2DBoxI() const;                         // 0x00A55A30
};

class CThing {
public:
    const C3DVector& GetPos() const;                   // 0x004C73D0
};

class CThingFilter_IsPreferredFirePoint {
public:
    CThingFilter_IsPreferredFirePoint() { }
};
class CThingFilter_IsOccupiableOccupied : public std::unary_function<CThing, bool> {
public:
    CThingFilter_IsOccupiableOccupied() { }
};
template <class A, class B> class CFilter_Bind2 {
public:
    const A* F1;
    const B* F2;
    CFilter_Bind2(const A& a, const B& b) : F1(&a), F2(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> __fastcall GFFilter_Bind2(const A& a, const B& b)
{
    return CFilter_Bind2<A, B>(a, b);
}

class CThingSearchTools {
public:
    template <class T, class TVector, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& things) const;   // 0x0095DB90
};

class CAIBrain {
public:
    unsigned char _pad[0x18];
    CThing* PCreature;                                 // +0x18
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                  // +0x04
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000
};
class CAIStateGroup_RangedCombatUsePreferredFirePoint : public CAIStateGroupBase {
protected:
    void GetFirePointList(std::vector<CThing*>& fire_points);
};

void CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList(std::vector<CThing*>& fire_points)
{
    C2DBoxF area(C2DVector(PBrain->PCreature->GetPos()), 30.0f);
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(
        area.ToC2DBoxI(),
        GFFilter_Bind2(CThingFilter_IsPreferredFirePoint(), std::not1(CThingFilter_IsOccupiableOccupied())),
        fire_points);
}
