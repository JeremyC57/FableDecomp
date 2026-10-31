// CSBAgents::Process(const CSteeringAgent* agent) -- steering behaviour: sum an inverse-square repulsion away from every other live, relevant agent closer than the agent's cut-off (in radii) into the behaviour's output
// Leaf: ?Process@CSBAgents@@UAE_NPBVCSteeringAgent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A873E0, 404 bytes, `ret 4`, 8-byte aligned frame (`and esp,-8`).
// PDB (Ego_r/FableWin name sets): ?Process@CSBAgents@@UAE_NPBVCSteeringAgent@@@Z (virtual, the agent by POINTER, not the
//   reference the earlier attempts used); CSBAgents : CSB (vptr, CKinematic Output +0x04) + Agents +0x0c.
// Callees: CSB::ShouldIgnoreAgent(const CSteeringAgent*, const CSteeringAgent*, const CSteeringAgent*) 0x00A87260,
//   CSB::GetDistanceBetweenAgents(const CSteeringAgent*, const CSteeringAgent*, bool) 0x00A872F0 (landed, same spelling),
//   C2DVector::Normalise() 0x00A14540 (bound spelling); virtual slots of CNavigatorExternalInterface (vtable
//   0x0125EC9C of CGroundNavigatorExternalInterface, rtti-vtables + Ego_r override names): 6 PeekNavigatorRadius,
//   8 PeekNavigatorPosition2D, 14 GetPThingToNotAvoid, 20 IsPlayer, 21 IsDead (0x006CDB20).
// Shape levers (each one proven by a grade):
//   - the PDB's CSteeringAgent inline accessors (IsDead, GetTarget, GetPosition, IsPlayer, IsWandering, GetDistCutoff,
//     GetRadius(bool)) give retail's `mov eax,[esi] ; mov ecx,[eax] ; mov eax,[ecx]` vcall registers and the pushed
//     ShouldIgnoreAgent arguments in ecx/edx (the direct NavigatorInterface chains used edx/eax);
//   - the early-out builds its OWN zero kinematic (`Output = CKinematic(C2DVector(0, 0))`) and `force` is declared after
//     it: VC7.1 merges the two paths' identical zero stores ahead of the `je` and frames `away` below `force` as retail does
//     (force declared first and copied in the early-out framed it the other way round).
// Layout: CSteeringAgent is Ego_r.pdb's with STLport one-word lists (DistCutoff +0x34, Wandering +0x5c: retail reads).
//   engine/CSB.h / engine/CSBAgents.h flatten the vptr and type Output as bytes, so CSB / CSBAgents are local models
//   (header debt in NOTES.md).
// g_SBAgentsDisabled_013d2d54 is descriptive: an unnamed .bss bool read only by this function (no write site, no PDB data name set).
class C3DVector;
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    const C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
    const C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
    C2DVector& Normalise();                                   // 0x00A14540
};
class CKinematic { public:
    C2DVector Vector;
    CKinematic() { }
    CKinematic(const C2DVector& v) : Vector(v) { }
};
class CSteeringAgent;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    typedef T* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
};
}
typedef std::vector<const CSteeringAgent*> CSteeringAgentList;
class CNavigatorExternalInterface { public:
    virtual bool IsMovementBlockedAt(const C3DVector&, const C3DVector&, bool&, bool&) const = 0;   // slot 0
    virtual float GetTerrainDesirabilityAt(const C3DVector&) const = 0;                            // slot 1
    virtual float GetTerrainHeightAt(const C3DVector&) const = 0;                                  // slot 2
    virtual float GetThemeHeightAt(const C3DVector&) const = 0;                                    // slot 3
    virtual float FindPosZAt(const C3DVector&, bool) const = 0;                                    // slot 4
    virtual float FindPosZAtIncludingThemes(const C3DVector&, bool) const = 0;                     // slot 5
    virtual float PeekNavigatorRadius(bool physical) const = 0;                                    // slot 6 (+0x18)
    virtual C2DVector PeekNavigatorOrientation() const = 0;                                        // slot 7
    virtual C2DVector PeekNavigatorPosition2D() const = 0;                                         // slot 8 (+0x20)
    virtual C3DVector PeekNavigatorPosition3D() const = 0;                                         // slot 9
    virtual float PeekNavigatorMaxSpeed(const C3DVector&) const = 0;                               // slot 10
    virtual const CSteeringAgentList& GetNavigatorObstacles() const = 0;                           // slot 11
    virtual CSteeringAgentList& GetNavigatorObstacles() = 0;                                       // slot 12
    virtual CSteeringAgentList* GetPNavigatorObstacles() = 0;                                      // slot 13
    virtual const CSteeringAgent* GetPThingToNotAvoid() const = 0;                                 // slot 14 (+0x38)
    virtual float GetProximityForDirectApproach() const = 0;                                       // slot 15
    virtual long GetNavigationLevelAt(const C3DVector&) const = 0;                                 // slot 16
    virtual long GetConstantFPS() const = 0;                                                       // slot 17
    virtual float GetMinPathCost() const = 0;                                                      // slot 18
    virtual float GetMaxPathCost() const = 0;                                                      // slot 19
    virtual bool IsPlayer() const = 0;                                                             // slot 20 (+0x50)
    virtual bool IsDead() const = 0;                                                               // slot 21 (+0x54)
};
struct CRetailList { void* _M_node; };             // STLport list: one word
class CSteeringAgent { public:
    CNavigatorExternalInterface* NavigatorInterface;          // +0x00
    bool                         DummyObstacle;               // +0x04
    float                        RepulsionMultiplier;         // +0x08
    CRetailList                  SteeringBehaviours;          // +0x0C
    CRetailList                  SteeringBehaviourInputs;     // +0x10
    CRetailList                  SteeringBehaviourConstraints; // +0x14
    float                        CurrentMaxSpeed;             // +0x18
    float                        CurrentSpeed;                // +0x1C
    C2DVector                    Goal;                        // +0x20
    C2DVector                    NextGoal;                    // +0x28
    int                          TimeToReacquireGoal;         // +0x30
    float                        DistCutoff;                  // +0x34
    C2DVector                    Steer;                       // +0x38
    float                        BestDistLeft;                // +0x40
    float                        BestDistRight;               // +0x44
    float                        BestNextDistLeft;            // +0x48
    float                        BestNextDistRight;           // +0x4C
    float                        ProximityForArrival;         // +0x50
    int                          TimeSinceBestApproach;       // +0x54
    float                        BestApproach;                // +0x58
    bool                         Wandering;                   // +0x5C
    float GetRadius(bool physical) const { return NavigatorInterface->PeekNavigatorRadius(physical); }
    C2DVector GetPosition() const { return NavigatorInterface->PeekNavigatorPosition2D(); }
    const CSteeringAgent* GetTarget() const { return NavigatorInterface->GetPThingToNotAvoid(); }
    bool IsDead() const { return NavigatorInterface->IsDead(); }
    bool IsPlayer() const { return NavigatorInterface->IsPlayer(); }
    bool IsWandering() const { return Wandering; }
    float GetDistCutoff() const { return DistCutoff; }
};
extern bool g_SBAgentsDisabled_013d2d54;          // 0x013D2D54 (descriptive: a never-written .bss debug switch, read only here)

class CSB { public:
    virtual ~CSB();
    CKinematic Output;                            // +0x04
    bool ShouldIgnoreAgent(const CSteeringAgent* agent, const CSteeringAgent* other, const CSteeringAgent* ignore);   // 0x00A87260
    float GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative);        // 0x00A872F0
    virtual bool Process(const CSteeringAgent* agent) = 0;
};
class CSBAgents : public CSB { public:
    CSteeringAgentList* Agents;                   // +0x0c
    virtual bool Process(const CSteeringAgent* agent);
};

bool CSBAgents::Process(const CSteeringAgent* agent)
{
    if (g_SBAgentsDisabled_013d2d54) {
        Output = CKinematic(C2DVector(0.0f, 0.0f));
        return true;
    }
    C2DVector force;
    force.X = force.Y = 0.0f;
    const CSteeringAgent* target = agent->GetTarget();
    for (CSteeringAgentList::iterator it = Agents->begin(); it != Agents->end(); ++it) {
        if ((*it)->IsDead())
            continue;
        if (ShouldIgnoreAgent(agent, *it, target))
            continue;
        C2DVector away = agent->GetPosition() - (*it)->GetPosition();
        float dist = GetDistanceBetweenAgents(agent, *it, true);
        if (dist < 0.0f && !(*it)->IsPlayer() && !agent->IsWandering())
            continue;
        float cutoff = agent->GetDistCutoff();
        if (dist < cutoff / agent->GetRadius(false)) {
            away.Normalise();
            if (dist < 1.0f)
                dist = 1.0f;
            away *= 1.0f / (dist * dist);
            force += away;
        }
    }
    Output = CKinematic(force);
    return true;
}