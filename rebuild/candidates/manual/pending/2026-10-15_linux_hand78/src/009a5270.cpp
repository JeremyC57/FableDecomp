// CSystemManager::SetAsActive(bool active) -- record the application's activation: on the inactive -> active transition of an application that has been active before, flag a restore (Restore); then store the new state and mark the application as having been active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A5270, length 60 bytes.
// Callees: none.
// Shape: `active` in al, the `== true` test of EverBeenActive as `mov dl,1 ; cmp bl,dl` (bl needs `push ebx`),
// the two tail stores duplicated into the transition arm ahead of its branch (Ever first, through dl) and into
// the shared exit (ApplicationActive first, then an immediate 1), `ret 4`. The restore test has to be its own
// `if` nested inside the transition test: folding it into one `&&` chain lets VC hoist the ApplicationActive
// store above the second branch (60 v 60 DIFFER), and an explicit transition arm with a local copy changes the
// register colouring (57 v 60).
// Model: PDB CSystemManager names (ApplicationActive +9, EverBeenActive, Restore). The retail layout runs 8
// bytes past the PDB's between +0x60 and EverBeenActive (PDB 0xd4/0xe2 -> retail 0xdc/0xea); no engine header.
struct CSystemManager {
    unsigned char _pad00[9];
    bool          ApplicationActive;               // +0x09
    unsigned char _pad0a[0xdc - 0x0a];
    bool          EverBeenActive;                  // +0xdc
    unsigned char _padDd[0xea - 0xdd];
    bool          Restore;                         // +0xea
    void SetAsActive(bool active);
};

void CSystemManager::SetAsActive(bool active)
{
    if (active && !ApplicationActive) {
        if (EverBeenActive == true)
            Restore = true;
    }
    ApplicationActive = active;
    EverBeenActive = true;
}
