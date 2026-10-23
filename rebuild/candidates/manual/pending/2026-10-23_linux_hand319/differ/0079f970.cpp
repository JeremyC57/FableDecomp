// Grade: DIFFER(8v1052) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CLightningBeam::UpdatePositions() -- refresh the beam's start and end points: for the source and target intelligent
//   pointers (when alive) take the emitter position, a graphic helper point or the thing position, falling back to the
//   stored points; x87 interpolation of the intermediate points
// Leaf: UpdatePositions
// Stub: x87-heavy; first-pass skeleton only, not reconstructed yet.
struct CThing;
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                          // 0x00A01B50
};
class CLightningBeam {
public:
    unsigned char           _pad00[4];
    CBaseIntelligentPointer Source;                    // +0x04
    void UpdatePositions();
};
void CLightningBeam::UpdatePositions()
{
    Source.GetPItem();
}
