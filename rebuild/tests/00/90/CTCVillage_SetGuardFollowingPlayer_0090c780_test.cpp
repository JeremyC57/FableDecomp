// Behaviour test for 0x0090C780 CTCVillage::SetGuardFollowingPlayer: the guard is wrapped in a converting
// CIntelligentPointer<CThingAICreature> temporary and copied into GuardFollowingPlayer (SetPItem of GetPItem); the
// temporary is destroyed afterwards. Null clears the pointer.
#include <cstdio>
class CBaseObject { public: int Id; };
static int g_tempDtors = 0;
class CBaseIntelligentPointer { public:
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_tempDtors; }
    CBaseObject* GetPItem() const { return PData; }
    void SetPItem(CBaseObject* item) { PData = item; }
private:
    CBaseObject* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThingAICreature : public CBaseObject { };
class CTCVillage { public:
    bool TeacherTeaching;
    CIntelligentPointer<CThingAICreature> GuardFollowingPlayer;
    long NextFrameToFollowPlayer;
    void SetGuardFollowingPlayer(CThingAICreature* guard) { GuardFollowingPlayer = guard; }
};
int main()
{
    CThingAICreature guard;
    CTCVillage village;
    village.NextFrameToFollowPlayer = 7;
    village.SetGuardFollowingPlayer(&guard);
    if (village.GuardFollowingPlayer.GetPItem() != &guard || g_tempDtors != 1 || village.NextFrameToFollowPlayer != 7) { std::printf("0090c780_TEST FAIL\n"); return 1; }
    village.SetGuardFollowingPlayer(0);
    if (village.GuardFollowingPlayer.GetPItem() != 0 || g_tempDtors != 2) { std::printf("0090c780_TEST FAIL\n"); return 1; }
    std::printf("0090c780_TEST PASS\n");
    return 0;
}