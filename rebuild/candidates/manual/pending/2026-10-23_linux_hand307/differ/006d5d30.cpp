// Grade: DIFFER(883v897) -- frame and register allocation: retail homes the nav component in ebp (spilled to the dead key slot), newOutput in bl and a 0x34 frame where the move target, the turn vector and the route output share slots; ours keeps nav in ebx, newOutput in al and a 0x40 frame, and the combat-type test materialises as and/test instead of retail's shr/test with branch threading straight to the new-output block; the flow (combat action request, counted brain-output reset, move-order facing test, route step with the 0x0129BA3C epsilon, turning, counted action) is modelled
// CThingAICreature::UpdateAI(bool newOutput, const CBrainOutput& output, C3DVector& moveTo) -- apply the brain's output:
// a registered combatant first asks the combat manager for its next action; a new output is stored (counted copy),
// and a move order steers the navigation component when the creature already faces the target; otherwise the stored
// output keeps the creature walking its route; true when moveTo was set
// Leaf: ?UpdateAI@CThingAICreature
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D5D30, `ret 0xc`.
// Callees: CVectorMap::LowerBound 0x0040F020, CThingCreatureBase::IsRegisteredCombatant 0x00662410 / GetCombatManager
//   0x00661ED0 / IsPerformingAnyAction 0x006622A0 / SetCurrentAction(CCombatAction*) 0x006644F0 / SetCurrentAction
//   (counted) 0x00664670, CCombatManager::GetNextActionForCreature 0x006EB600 / SetCreatureAsAttemptedToPerformedAction
//   0x006EA540, operator new 0x00BFEA1A, CBrainOutput copy 0x006D6110, CCountedPointer<CBrainOutput>::Reset 0x006D6260,
//   CThing::GetPosition 0x004C73D0, CThingCreatureBase::GetPosition 0x006A4D00, IsFacingCorrectDirectionForMovementToPos
//   0x006D5AF0 (__fastcall), CTCCreatureNavigation::SetNavigationTarget 0x006CB960 / GetNextNavPosOnRoute 0x006CBAC0,
//   UpdateAITurning 0x006D5B50.
// Layout: retail CThingAICreature PCurrentBrainOutput +0x1a0, PBrain +0x1a8 (PDB 0x1d0/0x1d8: the retail object is
//   0x30 smaller there, so the shared header does not fit); interface bitset +0x20, map +0x44.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
struct CNavigationInput {
    C3DVector Destination;
    float     Proximity;
    long      MovementType;
    float     Speed;
};
struct CCountedPointerData { long Count; };
struct CCombatAction;
struct CBrainOutput {
    unsigned long    Types;                     // +0x00 bit 0 move, 3 action, 5 combat
    bool HasType(long type) const { return ((Types >> type) & 1) != 0; }
    long             LookFlags;                 // +0x04
    CNavigationInput NavigationInput;           // +0x08
    void*            PAction[2];                // +0x20 CCountedPointer<CCreatureActionBase>
    C3DVector        MoveToPos;                 // +0x28
    C3DVector        LookAtPos;                 // +0x34
    void*            LookAtThing[2];            // +0x40
    CBrainOutput(const CBrainOutput& other);    // 0x006D6110
};
struct CBrainOutputPointer {
    CBrainOutput*        Obj;
    CCountedPointerData* Data;
    void Reset(CBrainOutput* p);                // 0x006D6260
};
struct CAIBrain {
    unsigned char _pad[0x8e];
    bool          Suspended;                    // +0x8e
};
struct CCreatureNavOutput {
    long      Status;
    C3DVector Offset;
};
struct CTCCreatureNavigation {
    unsigned char _pad[0x56];
    bool          Navigating;                   // +0x56
    void SetNavigationTarget(const CNavigationInput& target);   // 0x006CB960
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5); // 0x006CBAC0
};
struct CCombatManager {
    CCombatAction* GetNextActionForCreature(class CThingAICreature* creature);           // 0x006EB600
    void SetCreatureAsAttemptedToPerformedAction(class CThingAICreature* creature, bool performed); // 0x006EA540
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThingAICreature;
bool __fastcall IsFacingCorrectDirectionForMovementToPos(CThingAICreature* creature, const CBrainOutput& output,
                                                         CTCBase* tc, C3DVector& pos);   // 0x006D5AF0
extern bool g_AIEnabled;                        // 0x01375756
class CThingAICreature {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x1a0 - 0x4c];
    CBrainOutputPointer PCurrentBrainOutput;    // +0x1a0
    CAIBrain*     PBrain;                       // +0x1a8
    void*         PBrainData;
    template<class T> __forceinline bool GetInterface(T** out, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0 (CThing)
    const C3DVector& GetCreaturePosition() const; // 0x006A4D00 (CThingCreatureBase::GetPosition)
    bool IsRegisteredCombatant();               // 0x00662410
    CCombatManager* GetCombatManager();         // 0x00661ED0
    bool IsPerformingAnyAction();               // 0x006622A0
    bool SetCurrentAction(CCombatAction* action);   // 0x006644F0
    void SetCurrentAction(const void* action);  // 0x00664670
    void UpdateAITurning(const C3DVector& movement);  // 0x006D5B50
    bool UpdateAI(bool newOutput, const CBrainOutput& output, C3DVector& moveTo);
};

bool CThingAICreature::UpdateAI(bool newOutput, const CBrainOutput& output, C3DVector& moveTo)
{
    bool result = false;
    if (!g_AIEnabled)
        return result;
    CTCCreatureNavigation* nav;
    GetInterface(&nav, 7);
    CTCBase* movement;
    GetInterface(&movement, 0x43);
    C3DVector turn(0.0f, 0.0f, 0.0f);
    if (IsRegisteredCombatant()) {
        bool combat;
        if (newOutput)
            combat = output.HasType(5);
        else
            combat = PCurrentBrainOutput.Obj && PCurrentBrainOutput.Obj->HasType(5);
        if (combat) {
            CCombatAction* action = GetCombatManager()->GetNextActionForCreature(this);
            if (IsRegisteredCombatant()) {
                bool performed = false;
                if (action && !IsPerformingAnyAction() && SetCurrentAction(action))
                    performed = true;
                GetCombatManager()->SetCreatureAsAttemptedToPerformedAction(this, performed);
            }
        }
    }
    if (newOutput) {
        PCurrentBrainOutput.Reset(new CBrainOutput(output));
        nav->Navigating = false;
        if (output.HasType(0)) {
            C3DVector target = output.MoveToPos;
            turn = target - GetPosition();
            if (IsFacingCorrectDirectionForMovementToPos(this, output, movement, target)) {
                moveTo = target;
                result = true;
                nav->Navigating = true;
                nav->SetNavigationTarget(output.NavigationInput);
            }
        }
        UpdateAITurning(turn);
        if (output.HasType(3))
            SetCurrentAction(&output.PAction);
        return result;
    }
    if (PCurrentBrainOutput.Obj && !PBrain->Suspended) {
        if (PCurrentBrainOutput.Obj->HasType(0) && nav->Navigating) {
            CCreatureNavOutput route = nav->GetNextNavPosOnRoute(PCurrentBrainOutput.Obj->NavigationInput, false, true, true, false, false);
            if (route.Status == 2) {
                extern const float g_MinRouteStep;   // 0x0129BA3C
                if (route.Offset.Z * route.Offset.Z + route.Offset.Y * route.Offset.Y + route.Offset.X * route.Offset.X
                    > g_MinRouteStep * g_MinRouteStep) {
                    GetPosition();
                    C3DVector facing;
                    if (IsFacingCorrectDirectionForMovementToPos(this, *PCurrentBrainOutput.Obj, movement, facing)) {
                        result = true;
                        moveTo = route.Offset + GetCreaturePosition();
                    }
                    turn = route.Offset;
                    UpdateAITurning(turn);
                    return result;
                }
            } else {
                nav->Navigating = false;
            }
        }
        UpdateAITurning(turn);
    }
    return result;
}
