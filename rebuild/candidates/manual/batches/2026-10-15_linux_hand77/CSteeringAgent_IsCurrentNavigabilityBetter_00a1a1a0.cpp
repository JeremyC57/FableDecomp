// CSteeringAgent::IsCurrentNavigabilityBetter() const -- 1 when the current best left or right clearance exceeds the corresponding clearance at the next waypoint, else 0
// Leaf: IsCurrentNavigabilityBetter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A1A1A0, 35 bytes (+13 int3 padding). No callees. Caller 0x00A1F4A8.
// Layout: Ego_r.pdb CSteeringAgent with retail's 4-byte lists (the PDB's three 12-byte lists at +0xC/+0x18/+0x24 are
// one word each here, so everything after them sits 0x18 lower): BestDistLeft +0x40 (PDB +0x58), BestDistRight +0x44,
// BestNextDistLeft +0x48, BestNextDistRight +0x4C.
// Shape: `fcomp ; test ah,41h ; je` is the ordered `>`; `mov eax,1` / `xor eax,eax` is an int-sized result (a bool
// return would use `mov al,1` / `xor al,al`), written as the `||` of the two compares.
struct C2DVector { float X, Y; };
struct CNavigatorExternalInterface;
struct CRetailList { void* m_pHead; };            // retail list: one word

struct CSteeringAgent {
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
    long IsCurrentNavigabilityBetter() const;
};

long CSteeringAgent::IsCurrentNavigabilityBetter() const
{
    return BestDistLeft > BestNextDistLeft || BestDistRight > BestNextDistRight;
}
