// CTCCreatureExpression::FrameUpdate() -- every 16th frame (staggered by the thing's id) pick the creature's facial expression: expression 7 while it faces along the camera, else by the hero stats' morality (8 below 45%, 9 above 55%, scaled), else expression 1
// Leaf: FrameUpdate
// Retail 0x007294A0, 445 bytes (plain `ret`). Ego_r.pdb ?FrameUpdate@CTCCreatureExpression@@UAEXXZ.
// Callees: GFGetCurrentFrame @ 0x0049D870, CTCBase::GetWorld @ 0x00686A70, the world's +0x18 camera virtual slot 4
// (direction, by reference), the thing's +0x60 object virtual slot 70 (+0x118, a C3DVector by value), LowerBound
// @ 0x0040F020 (inlined HasTC + GetTC, CTCHeroStats = 4), CTCHeroStats::GetMoralityPercentage @ 0x0057A690,
// CTCCreatureExpression::SetExpression @ 0x00729410 (out of line at the first four sites; the last site is the inline
// copy: GetSubDef @ 0x00485815, GetPRandomSeed @ 0x0049D810, FindRandomElement @ 0x0086F510, ForcePrimaryExpression
// @ 0x007293A0). Constants: 0.8f @ 0x01254DA8, 0.45f @ 0x0125D1E0, 0.55f @ 0x0126663C, 1/0.45 @ 0x01266640.
// Layout: as the landed SetExpression 0x00729410; CThing UniqueID (unsigned __int64) +0x18, +0x60 object, def +0x70.
// Shape: retail calls SetExpression out of line at four sites and expands it inline at the last (VC7.1's inline-once
// band): the lane does not reproduce that choice for a plain inline member, so the last site goes through a __forceinline
// copy of the landed body (SetExpressionInline). The id is a named local read before the frame call; the camera through an
// inline accessor; the dot product accumulates Z, Y, X in statements; the morality weights are named float locals.
class C3DVector { public: float X, Y, Z; C3DVector() { } };
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CExpressionState {
    long          AnimIndex;                        // +0x00
    float         MaxWeight;                        // +0x04
    float         Weight;                           // +0x08
    float         OldWeight;                        // +0x0c
    bool          Primary;                          // +0x10
    unsigned char Stage;                            // +0x11
};

struct CExpressionStateList {                       // STLport list<CExpressionState>: one sentinel pointer
    struct node { node* next; node* prev; CExpressionState value; };
    node* head;
    bool empty() const { return head->next == head; }
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

struct CExpressionMap {
    long FindRandomElement(long expressionType, unsigned long* seed);   // 0x0086F510
};

struct CCreatureExpressionDef : CDefPointeeBase {
    unsigned char   _pad08[0x38 - 0x08];
    CExpressionMap* Expressions;                    // +0x38
};

template <class T>
class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (DecRefToZero(Object->m_refCount))
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CCreatureExpressionDef>* out);   // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureExpressionDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingFacing {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15)
    VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) VF(30) VF(31)
    VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47)
    VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) VF(60) VF(61) VF(62) VF(63)
    VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
#undef VF
    virtual C3DVector GetFacing();                  // slot 70 (+0x118)
};
class CThing { public:
    unsigned char  _pad00[0x18];
    unsigned __int64 UniqueID;                      // +0x18
    unsigned long  m_presentBits[9];                // +0x20
    CInterfaceMap  m_interfaces;                    // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CThingFacing*  Facing;                          // +0x60
    unsigned char  _pad64[0x70 - 0x64];
    CDefClassBase* PDef_Object;                     // +0x70
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
class CCamera { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const C3DVector& GetDirection();        // slot 4 (+0x10)
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CCamera* Camera;                                // +0x18
    CCamera* GetCamera() { return Camera; }
    unsigned long* GetPRandomSeed();                // 0x0049D810
};
class CTCHeroStats { public:
    float GetMoralityPercentage() const;            // 0x0057A690
};
inline float Dot(const C3DVector& a, const C3DVector& b)
{
    float d = a.Z * b.Z;
    d += a.Y * b.Y;
    d += a.X * b.X;
    return d;
}
long __fastcall GFGetCurrentFrame();                // 0x0049D870

class CTCCreatureExpression { public:
    virtual void v00();
    CThing*              ParentThing;               // +0x04 (CTCBase)
    unsigned char        _pad08[4];
    CExpressionStateList ExpressionStates;          // +0x0c
    bool                 Blinking;                  // +0x10
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void ForcePrimaryExpression(long animIndex, float maxWeight);   // 0x007293A0
    void SetExpression(long expressionType, float maxWeight);       // 0x00729410
    void SetExpressionInline(long expressionType, float maxWeight); // its inline copy (one site)
    void FrameUpdate();
};

__forceinline void CTCCreatureExpression::SetExpressionInline(long expressionType, float maxWeight)
{
    if (expressionType == 0 && !ExpressionStates.empty())
    {
        CExpressionStateList::node* front = ExpressionStates.head->next;
        if (front->value.AnimIndex == 0)
            front->value.MaxWeight = 1.0f;
        else
            front->value.Primary = false;
    }
    CDefPointer<CCreatureExpressionDef> def;
    ParentThing->PDef_Object->GetSubDef(&def);
    CExpressionMap* expressions = def->Expressions;
    long anim = expressions->FindRandomElement(expressionType, GetWorld()->GetPRandomSeed());
    if (anim)
        ForcePrimaryExpression(anim, maxWeight);
}

void CTCCreatureExpression::FrameUpdate()
{
    unsigned __int64 id = ParentThing->UniqueID;
    if (((id + GFGetCurrentFrame()) & 0xf) != 0)
        return;
    if (ParentThing->Facing) {
        C3DVector view = GetWorld()->GetCamera()->GetDirection();
        if (Dot(view, ParentThing->Facing->GetFacing()) > 0.8f) {
            SetExpression(7, 1.0f);
            return;
        }
    }
    if (ParentThing->HasTC(4)) {
        CTCHeroStats* stats = ParentThing->GetTC<CTCHeroStats>(4);
        if (stats->GetMoralityPercentage() < 0.45f) {
            float weight = 1.0f - stats->GetMoralityPercentage() * 2.2222223f;
            SetExpression(8, weight);
            return;
        }
        if (stats->GetMoralityPercentage() > 0.55f) {
            float weight = (stats->GetMoralityPercentage() - 0.55f) * 2.2222223f;
            SetExpression(9, weight);
            return;
        }
        SetExpression(1, 1.0f);
        return;
    }
    SetExpressionInline(1, 1.0f);
}