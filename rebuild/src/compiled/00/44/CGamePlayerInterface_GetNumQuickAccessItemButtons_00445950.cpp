// CGamePlayerInterface::GetNumQuickAccessItemButtons() const -- number of entries in the QuickAccessItemButtons button list (8-byte CCountedPointer<CInputTypeBase const> elements)
// Retail: Fable.exe @ 0x00445950, 16 bytes. Catalog label CShaderPreParser::GetConstantCount is wrong (shared by 0x00445940 and
// 0x00445950). Evidence: called by CTCInventory 0x00590E4F, NPlayerGui::CDrawQuickAccessIconsPC 0x006575FE and 0x00581A01/0x00581FE3; Ego_r.pdb CGamePlayerInterface: PControlsDef +0x7a0, ControlsDefIndex +0x7a4, Controls
// (CVectorMap, retail 16 bytes) +0x7a8, then the three STLport (12-byte) vectors AssignableSpecialMoveButtons +0x7b8,
// QuickAccessItemButtons +0x7c4, ContextSensitiveItemButtons +0x7d0 (PDB +0x7bc/+0x7cc/+0x7dc with 16-byte vectors).
// Shape: a signed pointer difference (`sar eax,3`), not the unsigned size().

class CInputTypeBase;
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
};
template <class T> struct CRetailVector {               // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct CGamePlayerInterface_Retail {                     // retail view of the tail (engine header has the PDB offsets)
    unsigned char _pad00[0x7b8];
    CRetailVector<CCountedPointer<const CInputTypeBase> > AssignableSpecialMoveButtons;   // +0x7b8
    CRetailVector<CCountedPointer<const CInputTypeBase> > QuickAccessItemButtons;         // +0x7c4
    CRetailVector<CCountedPointer<const CInputTypeBase> > ContextSensitiveItemButtons;    // +0x7d0
    long GetNumQuickAccessItemButtons() const;
};

long CGamePlayerInterface_Retail::GetNumQuickAccessItemButtons() const
{
    return QuickAccessItemButtons._M_finish - QuickAccessItemButtons._M_start;
}