// CCombatAction_GenericStrikeResponseKnockdownRecoverBase::FinishAction() -- end of a knock-down recovery: take the creature's mode manager out of mode 0x1f, level its facing (Z = 0, normalised; +X when degenerate) and hand the body an upright orientation along it, then the base FinishAction
// Leaf: ?FinishAction@CCombatAction_GenericStrikeResponseKnockdownRecoverBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00866E80, length 294 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), CVectorMap::LowerBound
//          @ 0x0040F020 (inlined GetInterface for type 0x31), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, the physics
//          body's (+0x60) virtual slots 70 (+0x118, GetFacing: C3DVector by hidden pointer) and 75 (+0x12c, the up/forward
//          orientation by reference), C3DVector::AccurateNormalise @ 0x00A14440, CCombatActionBase::FinishAction @ 0x00859A20.
//          Data: the minimum facing length 0x0129BA3C (1e-4f).
// Model: engine/CCombatAction_GenericStrikeResponseKnockdownRecoverBase.h is the flattened PDB layout (CCombatActionBase
//          0x12c there, 0x114 in retail) and names nothing used here, so `this` is the local CCombatActionBase model of the
//          landed CCreatureAction_ScorpionRecover::FinishAction (0x008C4B00); the GetInterface result is ignored.
// Shape: the GetInterface hit/end choice is ONE ternary selecting the entry before its component load (the if-form
//          loads the component into eax and moves it to ecx), and the body is read through an inline GetBody() accessor
//          (straight into ecx before the hidden-pointer push); the 2D length is `X*X + Y*Y` (Y squared first) against
//          the squared constant; the degenerate arm assigns a C3DVector(1, 0, 0) temporary; the orientation pair is
//          built in one temporary (up (0,0,1) as immediates, forward copied).
struct CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector& AccurateNormalise();                       // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
struct CUpForward {                         // the up/forward pair handed to the physics body
    C3DVector Up;
    C3DVector Forward;
    CUpForward(const C3DVector& forward) : Up(0.0f, 0.0f, 1.0f), Forward(forward) { }
};
struct CPhysicsBody {                       // the thing's physics body at +0x60
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacing();   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void SetOrientation(const CUpForward& orientation);   // slot 75 (+0x12c)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad50[0x10];
    CPhysicsBody* m_pBody;                  // +0x60
    CPhysicsBody* GetBody() const { return m_pBody; }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
struct CTCCreatureModeManager {             // interface type 0x31
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};
extern const float g_fMinFacingLength;      // 0x0129BA3C (1e-4)

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void FinishAction();            // 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    virtual void FinishAction();
};

void CCombatAction_GenericStrikeResponseKnockdownRecoverBase::FinishAction()
{
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x1f);
    C3DVector forward = PCreature.GetPItem()->GetBody()->GetFacing();
    forward.Z = 0.0f;
    if (forward.X * forward.X + forward.Y * forward.Y > g_fMinFacingLength * g_fMinFacingLength)
        forward.AccurateNormalise();
    else
        forward = C3DVector(1.0f, 0.0f, 0.0f);
    PCreature.GetPItem()->GetBody()->SetOrientation(CUpForward(forward));
    CCombatActionBase::FinishAction();
}