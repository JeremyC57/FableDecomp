// CQuestManager::RemoveAllBoasts  @ 004aff80

struct BoastList {
    int data;
    void Clear();   // 0x4b8c00
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};  // 0x49d870

class CQuestManager { public:
    char pad[0x64];
    int  field64;    // +0x64
    BoastList a;     // +0x68
    BoastList b;     // +0x6c
    void RemoveAllBoasts();
};

void CQuestManager::RemoveAllBoasts()
{
    this->a.Clear();
    this->b.Clear();
    this->field64 = CWorld::GetFrame();
}