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
// Layout (supersedes any local-model note above): engine/CQuestManager.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CQuestManager CQuestManagerDonor
#include "engine/CQuestManager.h"
#undef CQuestManager
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CCharString { public:
    void* PStringData;
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();          // 0x0044C6B0
};

class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};

class CWorld { public:
    unsigned char         _pad_0x00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60 (CCountedPointer<CMessageEventManager>::Data)
};

class CQuestManager : public CQuestManagerDonor {
 public:
    void SetExpressionPerformed(const CCharString& expression);
};

void CQuestManager::SetExpressionPerformed(const CCharString& expression)
{
    long expressionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(expression);
    World->PMessageEventManager->AddMessage(0x2a, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, expressionIndex);
}