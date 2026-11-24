// The fixture substitutes neighbouring engine services. The Update body below is
// synchronized with 00944300.cpp and runs with the VC7.1 debug compiler options.
#include <stdio.h>
#include <assert.h>

class CVillageTaskCarryCrate { };

template<class T> class CIntelligentPointer {
public:
    CIntelligentPointer(T* task = 0) : Task(task) {}
    T* Get() const { return Task; }
    T& operator*() const { assert(Task); return *Task; }
private:
    T* Task;
};

class CThing;
class CTCVillage {
public:
    CTCVillage() : Accept(true), CompleteCount(0), AcceptCount(0), BestCount(0),
                   ExpectedCreature(0), ExpectedTask(0), NextTask(0) {}
    bool SetCrateTaskAsAcceptedByThing(CVillageTaskCarryCrate& task, const CThing& thing) {
        assert(&task == ExpectedTask);
        assert(&thing == ExpectedCreature);
        ++AcceptCount;
        return Accept;
    }
    void SetCrateTaskAsCompleted(CIntelligentPointer<CVillageTaskCarryCrate> task) {
        assert(task.Get() == ExpectedTask);
        ++CompleteCount;
    }
    CVillageTaskCarryCrate* GetBestCrateTaskForThing(const CThing& thing) {
        assert(&thing == ExpectedCreature);
        ++BestCount;
        CVillageTaskCarryCrate* next = NextTask;
        NextTask = 0;
        ExpectedTask = next;
        return next;
    }
    bool Accept;
    int CompleteCount;
    int AcceptCount;
    int BestCount;
    const CThing* ExpectedCreature;
    CVillageTaskCarryCrate* ExpectedTask;
    CVillageTaskCarryCrate* NextTask;
};

class CTCVillageMember {
public:
    CTCVillageMember(CTCVillage* village) : Village(village), NullVisits(0), Visits(0) {}
    CTCVillage* GetPVillage() const { return Visits++ < NullVisits ? 0 : Village; }
    CTCVillage* Village;
    int NullVisits;
    mutable int Visits;
};

class CThing {
public:
    explicit CThing(CTCVillageMember* member) : Member(member) {}
    void PeekTC(const CTCVillageMember** out) const { *out = Member; }
private:
    CTCVillageMember* Member;
};

class CAIStateGroup_WorkInWarehouse {
public:
    explicit CAIStateGroup_WorkInWarehouse(CTCVillageMember* member)
      : Creature(member), Stop(false), MoveResult(true), ClearOnMove(false),
        StopCount(0), MoveCount(0), TerminateCount(0) {}
    void Update();
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;
    CThing Creature;
    bool Stop;
    bool MoveResult;
    bool ClearOnMove;
    int StopCount;
    int MoveCount;
    int TerminateCount;
private:
    const CThing& GetCreature() const { return Creature; }
    bool IsStopping() { ++StopCount; return Stop; }
    bool MoveCrates() {
        ++MoveCount;
        if (ClearOnMove)
            PTask = 0;
        return MoveResult;
    }
    void Terminate() { ++TerminateCount; }
};

void CAIStateGroup_WorkInWarehouse::Update()
{
    while (PTask.Get() && !IsStopping()) {
        const CTCVillageMember* ptc_village_mem;
        GetCreature().PeekTC(&ptc_village_mem);
        CTCVillage* village = ptc_village_mem->GetPVillage();
        if (village) {
            if (village->SetCrateTaskAsAcceptedByThing(*PTask, GetCreature())) {
                if (MoveCrates()) {
                    if (PTask.Get())
                        village->SetCrateTaskAsCompleted(PTask);
                }
            }
            PTask = village->GetBestCrateTaskForThing(GetCreature());
        }
    }
    Terminate();
}

int main()
{
    CVillageTaskCarryCrate first;
    CVillageTaskCarryCrate second;
    for (int mode = 0; mode != 8; ++mode) {
        CTCVillage village;
        CTCVillageMember member(&village);
        CAIStateGroup_WorkInWarehouse group(&member);
        group.PTask = mode != 0 ? &first : 0;
        group.Stop = mode == 1;
        group.MoveResult = mode != 3;
        group.ClearOnMove = mode == 4;
        village.Accept = mode != 2;
        village.ExpectedCreature = &group.Creature;
        village.ExpectedTask = &first;
        village.NextTask = mode == 6 ? &second : 0;
        member.NullVisits = mode == 7 ? 1 : 0;

        group.Update();

        const int work = mode < 2 ? 0 : mode == 6 ? 2 : 1;
        assert(group.TerminateCount == 1);
        assert(group.StopCount == work + (mode == 1 || mode == 7 ? 1 : 0));
        assert(member.Visits == work + (mode == 7 ? 1 : 0));
        assert(village.AcceptCount == work);
        assert(village.BestCount == work);
        assert(group.MoveCount == (mode <= 2 ? 0 : work));
        assert(village.CompleteCount == (mode <= 4 ? 0 : work));
        if (mode == 1)
            assert(group.PTask.Get() == &first);
        else
            assert(group.PTask.Get() == 0);
    }
    puts("00944300_TEST PASS");
    return 0;
}