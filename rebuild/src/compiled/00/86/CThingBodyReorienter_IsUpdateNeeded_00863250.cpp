// CThingBodyReorienter::IsUpdateNeeded() const -- an update is needed until the body reorientation is done
// Leaf: ?IsUpdateNeeded@CThingBodyReorienter@@QBE_NXZ
// Retail: Fable.exe @ 0x00863250, 10 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): FableWin.pdb ?IsUpdateNeeded@CThingBodyReorienter@@QBE_NXZ (the catalog label agrees); its two callers
// (0x006622DD, 0x00836272) test it on the creature's body-reorienter pointer before calling 0x00863E10, the landed
// CThingBodyReorienter::UpdateBodyReorienation. FableWin.pdb's first member is `bool BodyReorientationDone` (+0).
// Callees: none.
// Layout: no engine header for CThingBodyReorienter: local model from FableWin.pdb (members up to +0x14 sit at their PDB
// offsets; the STLport vector PossibleIntersectingThings is 0xC bytes in retail, so Body is +0x24 and the size 0x28).
// Shape: `mov dl,[ecx] ; xor eax,eax ; test dl,dl ; sete al` is `!bool`.
class CThing;

class CThingBodyReorienter {
public:
    bool IsUpdateNeeded() const;

    bool BodyReorientationDone;                             // +0x00
    bool ReorientingBody;                                   // +0x01
    long ReorientationCount;                                // +0x04
    bool StillCheckingForNewBodyReorientationAngle;         // +0x08
    bool HaveFinalReorientationAngle;                       // +0x09
    float FinalReorientationAngleXY;                        // +0x0C
    long RotationDirection;                                 // +0x10 ERotationDirection
    bool HasPossibleIntersectingThings;                     // +0x14
    unsigned char PossibleIntersectingThings[0xC];          // +0x18 std::vector<CIntelligentPointer<CThing> >
    CThing* Body;                                           // +0x24
};

bool CThingBodyReorienter::IsUpdateNeeded() const
{
    return !BodyReorientationDone;
}