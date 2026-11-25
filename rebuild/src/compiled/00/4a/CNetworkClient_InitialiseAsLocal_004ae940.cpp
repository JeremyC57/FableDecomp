class CMainGameComponent;

class CNetworkClient
{
public:
    unsigned char unused00[0x2660];
    bool initialised;                 // +0x2660
    bool host;                        // +0x2661
    bool updateEnabled;               // +0x2662
    unsigned char unused2663;
    void* localPlayer;                // +0x2664
    void* hostPlayer;                 // +0x2668
    unsigned long localFrame;         // +0x266C
    unsigned char unused2670[8];
    CMainGameComponent* gameComponent; // +0x2678

    bool InitialiseAsLocal(CMainGameComponent* component);
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x0099A350 ?Initialise@CInitBaseClass@@IAE_NXZ (was ?InitialiseNetworkBase@CNetworkClient@@QAE_NXZ)
class CNetworkClient;
class CInitBaseClass { public: protected: bool Initialise(); public: friend class ::CNetworkClient; };
bool CNetworkClient::InitialiseAsLocal(CMainGameComponent* component)
{
    if (reinterpret_cast<CInitBaseClass*>(this)->Initialise() == true)
    {
        host = false;
        localPlayer = 0;
        hostPlayer = 0;
        localFrame = 0;
        gameComponent = component;
        updateEnabled = true;
        initialised = true;
        return true;
    }
    return false;
}