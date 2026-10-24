#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
// Model of CCombatSequenceBase's scalar deleting destructor @ 0x008B3430: the virtual destructor destroys the
// CombatActions vector (each action's counted pointer released, the block freed), then the object is deleted.
struct CCPPointerInfo {
    unsigned int RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
static int g_deleted = 0;
static int g_freed = 0;
static void __fastcall CountDelete(void*) { ++g_deleted; }
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CCombatActionBase;
class CCombatAction {
public:
    CCountedPointer<CCombatActionBase> CreatureAction;
};
template <class _Tp>
class Vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    Vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~Vector()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p)
            p->~_Tp();
        if (_M_start != 0) { ++g_freed; free(_M_start); }
    }
};
struct CThingCreatureBase { int Id; };
class CCombatSequenceBase {
public:
    CCombatSequenceBase() : Running(false), NoAvailableActions(0), CurrentActionIndex(0), Creature(0), Target(0),
                            CreateCalled(false) { }
    virtual ~CCombatSequenceBase() { }
    virtual void PureSlot1() = 0;
    virtual void PureSlot2() = 0;
    bool                 Running;
    int                  NoAvailableActions;
    int                  CurrentActionIndex;
    Vector<CCombatAction> CombatActions;
    CThingCreatureBase*  Creature;
    CThingCreatureBase*  Target;
    bool                 CreateCalled;
};
class CTestSequence : public CCombatSequenceBase {
public:
    void PureSlot1() { }
    void PureSlot2() { }
};
static CCPPointerInfo* NewInfo(unsigned int refs)
{
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = refs;
    info->DeleteFunc = CountDelete;
    info->Data = 0;
    return info;
}
int main()
{
    bool ok = true;
    CCPPointerInfo* shared = NewInfo(2);                // still referenced elsewhere after the sequence dies
    CCombatSequenceBase* seq = new CTestSequence;
    CCombatAction* block = (CCombatAction*)malloc(3 * sizeof(CCombatAction));
    for (int i = 0; i < 3; ++i)
        new (block + i) CCombatAction;
    block[0].CreatureAction.Info = NewInfo(1);
    block[1].CreatureAction.Info = shared;
    seq->CombatActions._M_start = block;
    seq->CombatActions._M_finish = block + 3;
    seq->CombatActions._M_end_of_storage = block + 3;
    delete seq;                                         // through the virtual (deleting) destructor
    if (g_deleted != 1 || shared->RefCount != 1 || g_freed != 1) ok = false;
    CCombatSequenceBase* empty = new CTestSequence;
    delete empty;                                       // empty vector: nothing released or freed
    if (g_deleted != 1 || g_freed != 1) ok = false;
    delete shared;
    if (!ok) { std::printf("008b3430_TEST FAIL\n"); return 1; }
    std::printf("008b3430_TEST PASS\n");
    return 0;
}