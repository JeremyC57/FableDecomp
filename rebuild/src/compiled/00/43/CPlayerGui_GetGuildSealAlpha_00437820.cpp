// CPlayerGui::GetGuildSealAlpha() const -- returns the guild seal drawer's NPlayerGui::CDrawGuildSeal::GetAlpha
// Retail: Fable.exe @ 0x00437820, 12 bytes: `mov ecx,[ecx+1D0h] ; call 0x00643F9E ; ret` (catalog label "_Cons_val<...>" is wrong).
// Evidence: called three times from NPlayerGui::CDrawQuickAccessIcons::Draw (0x00652FA4) on its CPlayerGui
// (+0x04) with the float result multiplied into icon alphas; callee 0x00643F9E is CDrawGuildSeal::GetAlpha.
// Layout: PDrawGuildSeal (CCountedPointer<NPlayerGui::CDrawGuildSeal>) Data at retail +0x1d0 (engine/CPlayerGui.h
// has the Ego_r.pdb +0x1d8; retail CPlayerGui is 8 below the PDB there, as in landed 0x0043A4D0), so the class
// is modelled locally.
// Shape: the float result goes through a named local (`call ; ret`, not a tail jump).
namespace NPlayerGui {
class CDrawGuildSeal { public:
    float GetAlpha() const;                              // 0x00643F9E
};
}
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
class CPlayerGui { public:
    unsigned char _pad00[0x1d0];
    CCountedPointer<NPlayerGui::CDrawGuildSeal> PDrawGuildSeal;   // +0x1d0
    float GetGuildSealAlpha() const;
};

float CPlayerGui::GetGuildSealAlpha() const
{
    float alpha = PDrawGuildSeal->GetAlpha();
    return alpha;
}