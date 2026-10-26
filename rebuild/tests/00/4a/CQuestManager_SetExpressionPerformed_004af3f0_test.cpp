// Behaviour test for CQuestManager::SetExpressionPerformed(const CCharString&) @ 0x004AF3F0
#include <cstdio>
#include <string.h>

class CThing;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CCharString { public: const char* m_text; };

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) { return strcmp(name.m_text, "EXPRESSION_HEROIC_STANCE") == 0 ? 321 : -1; }
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager s; return s; }
};

class CMessageEventManager { public:
    long m_type; unsigned long m_delay; float m_px; long m_data; int m_calls;
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data)
    {
        (void)from; (void)to; (void)value1; (void)value2;
        m_type = type; m_delay = delay; m_px = position.X; m_data = data; ++m_calls;
        return 0;
    }
};

class CWorld { public: CMessageEventManager* PMessageEventManager; };

class CQuestManager { public:
    CWorld* World;
    void SetExpressionPerformed(const CCharString& expression)
    {
        long expressionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(expression);
        World->PMessageEventManager->AddMessage(0x2a, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, expressionIndex);
    }
};

int main()
{
    CMessageEventManager m; m.m_calls = 0;
    CWorld w; w.PMessageEventManager = &m;
    CQuestManager q; q.World = &w;
    CCharString e = { "EXPRESSION_HEROIC_STANCE" };
    q.SetExpressionPerformed(e);
    if (m.m_calls != 1 || m.m_type != 0x2a || m.m_delay != 0x32 || m.m_data != 321 || m.m_px != 0.0f) { std::printf("004af3f0_TEST FAIL\n"); return 1; }
    std::printf("004af3f0_TEST PASS\n");
    return 0;
}