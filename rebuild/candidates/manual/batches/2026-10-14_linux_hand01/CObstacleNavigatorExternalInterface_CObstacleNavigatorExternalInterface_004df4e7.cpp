#pragma optimize("s", on)
// CObstacleNavigatorExternalInterface::CObstacleNavigatorExternalInterface(const CObstacleOwner&) -- own vtable (0x01241D04), Obstacle taken from the owner's +4, empty DummyAgentList vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF4E7, length 32 bytes. No callees.
// PDB layout: {vptr, CThing* Obstacle +4, vector<const CSteeringAgent*> DummyAgentList +8}
// (0x18 bytes). The vector is the VC6-era Dinkumware {first, last, end} triple, zeroed by its
// constructor; the argument is a reference to an object holding the obstacle thing at +4.
struct CThing;
struct CSteeringAgent;

struct CObstacleOwner {
    void* m_vptr;
    CThing* m_pThing;                      // +4
};

struct CRetailVectorAgent {                // VC6-era std::vector<const CSteeringAgent*>
    const CSteeringAgent** m_first;
    const CSteeringAgent** m_last;
    const CSteeringAgent** m_end;
    CRetailVectorAgent() : m_first(0), m_last(0), m_end(0) { }
};

struct CObstacleNavigatorExternalInterface {   // vtable 0x01241D04
    virtual void v00();
    CThing* Obstacle;                      // +4
    CRetailVectorAgent DummyAgentList;     // +8
    CObstacleNavigatorExternalInterface(const CObstacleOwner& owner);
};

CObstacleNavigatorExternalInterface::CObstacleNavigatorExternalInterface(const CObstacleOwner& owner)
    : Obstacle(owner.m_pThing)
{
}
