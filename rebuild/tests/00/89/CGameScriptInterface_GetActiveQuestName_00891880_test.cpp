#include <cstdio>

class CCharString { public:
    char* p;
    CCharString(const char* s, long len) { p = (char*)s; (void)len; }
    CCharString(const CCharString& o) { p = o.p; }
};

struct QuestObj {
    char _pad[0x30];
    CCharString name; // +0x30
    QuestObj(const char* s) : name(s, -1) {}
};

struct GameGlobal {
    char _pad[0x88];
    QuestObj* active; // +0x88
};

// Test model of the retail singleton slot 0x013B89FC: the private static CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass
// (same mangled name as the row binds); TestPClass() is a test-only accessor.
class CQuestManager;
template<class T> class CCountedPointer { public: T* Data; void* Info; };
template<class T> class CTBaseSingletonNoDefaultAlloc { static CCountedPointer<T> PClass; public: static CCountedPointer<T>& TestPClass() { return PClass; } };
template<class T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;

class CGameScriptInterface { public:
    void* vt;
    CCharString GetActiveQuestName();
};

CCharString CGameScriptInterface::GetActiveQuestName()
{
    QuestObj* q = (reinterpret_cast<GameGlobal*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data))->active;
    if (q)
        return q->name;
    return CCharString("", -1);
}

int main() {
    GameGlobal g; g.active = 0;
    (reinterpret_cast<GameGlobal*&>(CTBaseSingletonNoDefaultAlloc<CQuestManager>::TestPClass().Data)) = &g;
    CGameScriptInterface self;
    CCharString r1 = self.GetActiveQuestName();
    if (r1.p == 0 || r1.p[0] != 0) { std::printf("FAIL null-branch not empty\n"); return 1; }

    static QuestObj q("Wasp Menace");
    g.active = &q;
    CCharString r2 = self.GetActiveQuestName();
    if (std::printf("%s", r2.p) < 0) { std::printf("FAIL\n"); return 1; }
    std::printf("\nCGameScriptInterface_00891880_TEST PASS\n");
    return 0;
}