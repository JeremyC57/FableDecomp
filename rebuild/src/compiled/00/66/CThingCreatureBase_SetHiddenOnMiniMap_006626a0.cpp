// CThingCreatureBase::SetHiddenOnMiniMap(bool hidden) -- store the "hidden on the mini map" bit and, when it changed, tell the mini-map manager to refresh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006626A0, length 59 bytes (+ int3 padding).
// Callees: the global manager at 0x013B8790, its one-argument refresh method @ 0x004378C0 (tail-jumped with 0 in the
// reused argument slot; the manifest label "_Cons_val<...>" is propagated).
// Layout (retail, local model): the flag byte at +0x15d, bit 7 = HiddenOnMiniMap (PDB: a bool:1 at +0x185 in
// engine/CThingCreatureBase.h, whose layout is wider than retail ahead of it).
// Shape: a one-bit bool bitfield compared with the bool argument as bytes (`shr cl,7 ; cmp cl,dl`), stored with
// `and al,7Fh ; shl dl,7 ; or al,dl`, then a void call as the last statement turns into the tail jump.
class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool full);       // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CThingCreatureBase { public:
    unsigned char _pad00[0x15d];
    bool          m_flags15d : 7;               // +0x15d bits 0..6
    bool          m_hiddenOnMiniMap : 1;        // +0x15d bit 7
    void SetHiddenOnMiniMap(bool hidden);
};

void CThingCreatureBase::SetHiddenOnMiniMap(bool hidden)
{
    if (m_hiddenOnMiniMap != hidden) {
        m_hiddenOnMiniMap = hidden;
        g_pPlayerGui->UpdateMiniMapMarkers(false);
    }
}