// CTCNamedUserActionPoints::IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& orientation, long index) const -- place the named action point (its dummy object on the owner's mesh, returned in position/orientation) and report whether a creature other than the owner and the current user stands within 0.5 of it
// Leaf: IsActionPointIndexBlocked
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FF0C0, 199 bytes (`ret 10h`; PDB ?IsActionPointIndexBlocked@CTCNamedUserActionPoints@@UBE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@J@Z; `thing` is unused).
// Callees: CTCGraphicAppearance::GetDummyObject(const CCharString&, long, unsigned long, C3DVector*, CRightHandedSet*, float)
//   @ 0x004C08C0 on ParentThing->PTCGraphicAppearance (+0x64), __ftol2 @ 0x00BFEA70 (x then y), GetPItem @ 0x00A01B50,
//   CTCBase::GetWorld() const @ 0x00686A80 -> PThingSearchTools (+0x20), CThingSearchTools::GetPFirstThingInRoughArea<
//   CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_In2DRange>, CThingCreatureBase> @ 0x007FE730 (the PDB
//   instantiation name). 1.0f is an immediate push, 0.5f an immediate store.
// Layout: Ego_r.pdb CTCUserActionPointBase with 12-byte retail vectors (FreeActionPointIndexes +0x0c, BlockedActionPointIndexes
//   +0x18, NumberOfActionPoints +0x24, FrameOfNextCheck +0x28, PThing +0x2c) and CTCNamedUserActionPoints Names +0x34 (PDB +0x3c);
//   filters CThingFilter_IsNot {Thing}, CThingFilter_In2DRange {Pos, Range}. engine/CTCNamedUserActionPoints.h is a flattened
//   POD with the PDB offsets, so the hierarchy is modelled locally.
// Shape: the box is built from two truncated longs (x, y, x, y); the three filters and the Bind3 are named locals (the IsNot
//   filters and the result land in the dead parameter slots); the search tools are read into a named local before the pushes
//   (the chained form loads them after); `? true : false` gives the `test al,al ; setne al` tail.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct CCharString { void* PStringData; };
class CThing;
struct CThingCreatureBase;
struct CThingFilter_IsNot {
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float     Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
template <class A, class B, class C> struct CFilter_Bind3 {
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
struct CThingSearchTools {
    template <class TFilter, class T>
    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const TFilter& filter, T** out) const;   // 0x007FE730
};
struct CWorld {
    unsigned char _pad00[0x20];
    CThingSearchTools* PThingSearchTools;               // +0x20 scoped_ptr
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64 (retail)
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void* PData;
};
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CTCUserActionPointBase {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    vector<long>  FreeActionPointIndexes;               // +0x0c
    vector<long>  BlockedActionPointIndexes;            // +0x18
    long          NumberOfActionPoints;                 // +0x24
    long          FrameOfNextCheck;                     // +0x28
    CBaseIntelligentPointer PThing;                     // +0x2c CIntelligentPointer<CThing>
    const CWorld* GetWorld() const;                     // 0x00686A80 (CTCBase)
};
struct CTCNamedUserActionPoints : CTCUserActionPointBase {
    vector<CCharString> Names;                          // +0x34
    virtual bool IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& orientation, long index) const;
};

bool CTCNamedUserActionPoints::IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& orientation, long index) const
{
    ParentThing->PTCGraphicAppearance->GetDummyObject(Names[index], 0, 0, &position, &orientation, 1.0f);
    long x = (long)position.X;
    long y = (long)position.Y;
    C2DBoxI box(x, y, x, y);
    CThingFilter_In2DRange inRange(position, 0.5f);
    CThingFilter_IsNot notUser(PThing.GetPItem());
    CThingFilter_IsNot notSelf(ParentThing);
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_In2DRange> filter(notSelf, notUser, inRange);
    const CThingSearchTools* tools = GetWorld()->PThingSearchTools;
    CThingCreatureBase* blocker;
    return tools->GetPFirstThingInRoughArea(box, filter, &blocker) ? true : false;
}
