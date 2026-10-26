// COpinionDeedFilter_IsCrimeToRunAndFindAGuard::operator()(const COpinionDeed& deed) const -- deed filter: a crime deed (types 2..9) that is a crime not yet reported to the village, or deed type 53 carrying extra data (!= -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681750, length 55 bytes (`ret 4`, bool in al).
// Layout from the PDB (COpinionDeed, 40 bytes): DeedType +0x00, DeedHandle +0x04, Position +0x08, FrameHappened +0x14,
// PTarget +0x18, WasSearchingForWitnesses +0x20, IsStillHappening +0x21, IsCrimeReportedToVillage +0x22, IsCrime +0x23,
// ExtraData +0x24. The filter itself is an empty 1-byte functor. Shape: a `switch` on the deed type -- the eight
// consecutive crime cases sharing one arm lower to the `cmp 2 ; jl ; cmp 9 ; jle` range test, case 53 is tested after
// it and placed first; `return x != -1 ? true : false;` gives the bare `setne al` (a plain `return x != -1;` goes
// through `xor ecx,ecx ; setne cl ; mov al,cl`); the shared `xor al,al` tail is the fall-out `return false`.
// Leaf: ??RCOpinionDeedFilter_IsCrimeToRunAndFindAGuard
enum EOpinionDeedType { OPINION_DEED_TYPE_NONE = 0 };
class C3DVector { public: float X, Y, Z; };
struct CIntelligentPointerThing { void* m_pData; void* m_pBlock; };
class COpinionDeed { public:
    EOpinionDeedType         DeedType;                    // +0x00
    long                     DeedHandle;                  // +0x04
    C3DVector                Position;                    // +0x08
    long                     FrameHappened;               // +0x14
    CIntelligentPointerThing PTarget;                     // +0x18
    bool                     WasSearchingForWitnesses;    // +0x20
    bool                     IsStillHappening;            // +0x21
    bool                     IsCrimeReportedToVillage;    // +0x22
    bool                     IsCrime;                     // +0x23
    long                     ExtraData;                   // +0x24
};
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public:
    bool operator()(const COpinionDeed& deed) const;
};
bool COpinionDeedFilter_IsCrimeToRunAndFindAGuard::operator()(const COpinionDeed& deed) const
{
    switch (deed.DeedType) {
    case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
        if (deed.IsCrime && !deed.IsCrimeReportedToVillage)
            return true;
        break;
    case 53:
        return deed.ExtraData != -1 ? true : false;
    }
    return false;
}