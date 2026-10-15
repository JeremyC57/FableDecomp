// CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList(std::vector<CThing*>& coverThings) -- sorts the candidate cover things by how well they lie in the direction from the creature to its threat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00979260, length 130 bytes, `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the threat pointer at +0x14), CThing::GetPosition
// @ 0x004C73D0 (three times: creature, threat, creature again; manifest label DoSizeof is propagated),
// C2DVector::Normalise @ 0x00A14540 (thiscall on the comparator's direction member), the SGI std::sort
// instantiation @ 0x00979870 (__fastcall under the algorithm TUs' /Gr: first in ecx, last in edx, the 16-byte comparator
// by value on the stack).
// Shape: the creature is read into a named local before the threat's GetPItem (edi), both positions are fetched
// before the direction `threat - creature` is computed as a full C3DVector local through the inline operator-
// (its dead Z subtraction is dropped but the 12-byte frame slot stays: `sub esp,0Ch`, a C2DVector local gives 8);
// the comparator
// {origin (the creature position's x/y, re-fetched), direction} is constructed in place in the argument area
// (`sub esp,10h ; mov ecx,esp`, four dword moves) and its constructor normalises the direction (`add ecx,8`);
// the sort arguments are `end()` then `begin()` (right to left). The comparator needs a user copy constructor
// to be built in place in the argument area (else it is built in the frame and copied), and its origin/direction
// come from C3DVectors through a C2DVector(const C3DVector&) constructor (dword moves, no x87).
// Model: local (engine/CAIStateGroup_CreatureAdvanceInCover.h is a flattened base blob).
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector* Normalise();                         // 0x00A14540
};

struct CThing {
    const C3DVector* GetPosition() const;           // 0x004C73D0
};

struct CCoverThingCompare {                         // the sort predicate: origin + unit direction
    C2DVector Origin;
    C2DVector Direction;
    CCoverThingCompare(const C3DVector& origin, const C3DVector& direction)
        : Origin(origin), Direction(direction) { Direction.Normalise(); }
    CCoverThingCompare(const CCoverThingCompare& other) : Origin(other.Origin), Direction(other.Direction) { }
};

void __fastcall SortCoverThings(CThing** first, CThing** last, CCoverThingCompare compare);   // 0x00979870 std::sort

struct CThingVector {                               // std::vector<CThing*> {first, last, end of storage}
    CThing** m_pFirst;
    CThing** m_pLast;
    CThing** m_pEnd;
    CThing** begin() { return m_pFirst; }
    CThing** end() { return m_pLast; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CreatureAdvanceInCover : CAIStateGroupBase {
    CBaseIntelligentPointer PThreat;                // +0x14 CIntelligentPointer<CThing>
public:
    virtual void ProcessCoverThingList(CThingVector& coverThings);
};

void CAIStateGroup_CreatureAdvanceInCover::ProcessCoverThingList(CThingVector& coverThings)
{
    CThing* creature = PBrain->Creature;
    CThing* threat = PThreat.GetPItem();
    const C3DVector* creaturePos = creature->GetPosition();
    const C3DVector* threatPos = threat->GetPosition();
    C3DVector direction = *threatPos - *creaturePos;
    SortCoverThings(coverThings.begin(), coverThings.end(), CCoverThingCompare(*PBrain->Creature->GetPosition(), direction));
}