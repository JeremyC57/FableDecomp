// CGuiWindowManager::AddWindowToFront(CGuiWindow* window) -- insert the window at the head of the manager's doubly linked window list (+0xc20 head, +0xc1c tail; window links at +8 prev / +0xc next)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65F00, length 64 bytes.
// Callees: none. Shape: the head re-read from `this` after every store through the window (no alias analysis), the empty test taken before the head store and resolved after it.
// The twin @ 0x00A65EC0 (the chunk's "AddWindow") links at the tail. No PDB layout for the retail CGuiWindowManager at +0xc1c; member names are descriptive.
struct CGuiWindow {
    unsigned char _pad[8];
    CGuiWindow* PPrevWindow;                       // +0x08
    CGuiWindow* PNextWindow;                       // +0x0c
};

struct CGuiWindowManager {
    unsigned char _pad[0xc1c];
    CGuiWindow* PLastWindow;                       // +0xc1c (the tail of the window list)
    CGuiWindow* PFirstWindow;                      // +0xc20 (the head)
    void AddWindowToFront(CGuiWindow* window);
};

void CGuiWindowManager::AddWindowToFront(CGuiWindow* window)
{
    window->PNextWindow = PFirstWindow;
    if (PFirstWindow)
        PFirstWindow->PPrevWindow = window;
    window->PPrevWindow = 0;
    CGuiWindow* first = PFirstWindow;
    PFirstWindow = window;
    if (!first)
        PLastWindow = window;
}