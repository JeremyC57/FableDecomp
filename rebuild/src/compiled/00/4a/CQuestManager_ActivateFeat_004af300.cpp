// CQuestManager::ActivateFeat(long) -- post message 0x38 (feat activated, delay 50) carrying the feat index to the world's message event manager; always succeeds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF300, length 68 bytes.
// Callees: CMessageEventManager::AddMessage(EMessageEventType, unsigned long, const C3DVector&, CThing*, CThing*,
// float, float, long) @ 0x00687540 (manifest prototype; the result is dropped).
// Identity: FableWin.pdb ?ActivateFeat@CQuestManager@@QAE_NJ@Z (bool result, `ret 4`).
// Layout: the retail CQuestManager holds World at +0x7c (PDB +0xd8; see the retail view in 0x004AF670's header);
// CWorld::PMessageEventManager (a CCountedPointer) has its object at +0x60 (PDB).
// Shape: a 12-byte frame for the zero position: C3DVector(0, 0, 0) is a float temporary whose three immediate
// zero stores are scheduled after the eight pushes; the feat index is pushed first (right-to-left evaluation),
// the manager is fetched last; `mov al,1` returns true.
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};

class CWorld { public:
    unsigned char         _pad_0x00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60 (CCountedPointer<CMessageEventManager>::Data)
};

class CQuestManager { public:
    unsigned char _pad_0x00[0x7c];
    CWorld*       World;                           // +0x7c
    bool ActivateFeat(long featIndex);
};

bool CQuestManager::ActivateFeat(long featIndex)
{
    World->PMessageEventManager->AddMessage(0x38, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, featIndex);
    return true;
}