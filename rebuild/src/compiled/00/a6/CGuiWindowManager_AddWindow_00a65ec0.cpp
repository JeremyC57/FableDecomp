// CGuiWindowManager::AddWindow(CGuiWindow* window) -- append the window to the tail of the manager's doubly linked window list (+0xc1c tail, +0xc20 head; window links at +8 prev / +0xc next)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65EC0, length 64 bytes.
// Callees: none. Shape: the tail re-read from `this` after every store through the window (no alias analysis), the empty test taken before the tail store and resolved after it.
// The twin @ 0x00A65F00 links at the head instead. No PDB layout for the retail CGuiWindowManager at +0xc1c; member names are descriptive.
class CGuiWindow { public:
    unsigned char _pad[8];
    CGuiWindow* PPrevWindow;                       // +0x08
    CGuiWindow* PNextWindow;                       // +0x0c
};

class CGuiWindowManager { public:
    unsigned char _pad[0xc1c];
    CGuiWindow* PLastWindow;                       // +0xc1c (the tail of the window list)
    CGuiWindow* PFirstWindow;                      // +0xc20 (the head)
    void AddWindow(CGuiWindow* window);
};

void CGuiWindowManager::AddWindow(CGuiWindow* window)
{
    window->PPrevWindow = PLastWindow;
    if (PLastWindow)
        PLastWindow->PNextWindow = window;
    window->PNextWindow = 0;
    CGuiWindow* last = PLastWindow;
    PLastWindow = window;
    if (!last)
        PFirstWindow = window;
}