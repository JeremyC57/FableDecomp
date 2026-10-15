// CQuestManager::SetExpressionPerformed(const CCharString&) -- look up the expression's global definition index and post message 0x2a (expression performed, delay 50) carrying it to the world's message event manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF3F0, length 83 bytes.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0 (no arguments, returns the manager),
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410,
// CMessageEventManager::AddMessage(...) @ 0x00687540 (eight arguments, the result dropped).
// Identity: FableWin.pdb ?SetExpressionPerformed@CQuestManager@@QAEXABVCCharString@@@Z.
// Layout: World at +0x7c (retail), CWorld::PMessageEventManager object at +0x60.
// Shape: the index is a named local computed first (its CCharString argument is pushed before the Get call); the
// zero position is a float C3DVector temporary stored with immediates after the pushes (written as the call's last
// argument instead, the temporary's stores are hoisted above the index calls).
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CCharString {
    void* PStringData;
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

struct CGameDefinitionManager : CDefinitionManager {
    static CGameDefinitionManager& Get();          // 0x0044C6B0
};

struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};

struct CWorld {
    unsigned char         _pad_0x00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60 (CCountedPointer<CMessageEventManager>::Data)
};

struct CQuestManager {
    unsigned char _pad_0x00[0x7c];
    CWorld*       World;                           // +0x7c
    void SetExpressionPerformed(const CCharString& expression);
};

void CQuestManager::SetExpressionPerformed(const CCharString& expression)
{
    long expressionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(expression);
    World->PMessageEventManager->AddMessage(0x2a, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, expressionIndex);
}
