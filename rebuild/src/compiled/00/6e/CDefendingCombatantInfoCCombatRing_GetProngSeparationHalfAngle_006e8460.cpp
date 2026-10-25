// CDefendingCombatantInfo::CCombatRing::GetProngSeparationHalfAngle() const -- half the angular spacing (in turns) between the ring's prongs
// Leaf: ?GetProngSeparationHalfAngle@CCombatRing@CDefendingCombatantInfo@@QBEMXZ
// Retail: Fable.exe @ 0x006E8460, 51 bytes. PDB ?GetProngSeparationHalfAngle@CCombatRing@CDefendingCombatantInfo@@QBEMXZ
// (descriptive check: the only float-returning const CCombatRing member in the PDB name set; the caller 0x006E964C sits
// in the combat-ring code at 0x006E94D0).
// Label correction: the manifest row "CWorldMap::DrawGetNoMaps" is a propagation over a different body; this one
// divides a 28-byte-element vector (CProng) at +0x10, which is CCombatRing::Prongs (Ego_r.pdb: RotationAngle +0x00,
// DeltaRotation +0x04, Index +0x08, RingThickness +0x0C, Prongs +0x10; CProng is 28 bytes {Blocked, Prong,
// ProngBlockageTimer, POccupier}). No engine header exists for the nested class.
// Float pool: 1.0f 0x0122DED8, 0.5f 0x0122F59C.
// Shape: the prong count is the signed /28 pointer difference stored as a long in the `push ecx` frame word for the
// `fild` (an unsigned size_t would add the 2^32 fix-up), then `(1.0f / count) * 0.5f` (fdivr then fmul).
#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };   // retail C3DVector is 1-aligned
#pragma pack(pop)

class CDefendingCombatantInfo {
public:
    class CProng {
    public:
        bool          Blocked;                  // +0x00
        C3DVector     Prong;                    // +0x01
        long          ProngBlockageTimer;       // +0x10
        unsigned char POccupier[8];             // +0x14 CIntelligentPointer<CThingCreatureBase>
    };
    template <class T> class CVector {          // STLport vector<T> (12 bytes in retail)
    public:
        T* _M_start;
        T* _M_finish;
        T* _M_end_of_storage;
        unsigned int size() const { return _M_finish - _M_start; }
    };
    class CCombatRing {
    public:
        float            RotationAngle;         // +0x00
        float            DeltaRotation;         // +0x04
        long             Index;                 // +0x08
        float            RingThickness;         // +0x0C
        CVector<CProng>  Prongs;                // +0x10
        float GetProngSeparationHalfAngle() const;
    };
};

float CDefendingCombatantInfo::CCombatRing::GetProngSeparationHalfAngle() const
{
    long count = Prongs.size();
    return (1.0f / count) * 0.5f;
}