// CAIStateGroupBase::WaitForMessageEvent(long waitMode, const CFilter_Bind3& filter, CMessageEvent** event) -- AI state-group coroutine helper: poll the world's message-event manager for the most recent message passing a Bind3<IsOfType, HasExtraData, OccuredBetween> filter, yielding a frame between polls
// Leaf: WaitForMessageEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091A670, length 120 bytes (ret 0Ch).
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010, CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (landed names),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3> @ 0x0091A5B0; virtual slot 2 (+8) of the brain at +4
// (the coroutine yield).
// Label correction: the manifest's "CCOE_AltarEvil::Main" is propagated over byte-identical bodies; this is a
// CAIStateGroupBase template helper instantiated per filter type (0x00969760 is the twin for the other filter).
// Callers: 0x00919ABD, 0x00947E74, 0x009684DD, 0x0096858D (all pass waitMode 2 or 1, the filter from their frame).
// Shape: returns 1 when the state group is stopping (checked on entry and after every yield), 2 when a message was
// found (the result is stored through the out pointer before the null test), and 4 once a single poll failed unless
// waitMode is 1 (wait forever: `cmp ebp,1 ; mov eax,4 ; je loop` -- a result local set to 4 at the bottom of the
// do/while body, the 2 and 1 cases `break` out with their own value; returning the constants directly puts the
// `mov eax,4` after the branch). The world's message manager (+0x60) is read into a
// local before the filter is pushed (the chained call pushes the argument first).
struct CMessageEvent;
struct CFilter_Bind3;                                  // the bound message filter, passed by address
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CFilter_Bind3& filter);   // 0x0091A5B0
};
struct CWorld {
    unsigned char         _pad[0x60];
    CMessageEventManager* MessageEventManager; // +0x60
};
struct CBrain {
    virtual void v00();
    virtual void v01();
    virtual void Yield();                     // slot 2 (+8)
};

class CAIStateGroupBase {
public:
    void*   __vftable;                        // +0x00
    CBrain* PBrain;                           // +0x04
    bool IsStopping();                        // 0x008FD010
    CWorld* GetPWorld();                      // 0x008FCFA0 (PBrain->+0x1c->+0x24)
    long WaitForMessageEvent(long waitMode, const CFilter_Bind3& filter, CMessageEvent** event);
};

long CAIStateGroupBase::WaitForMessageEvent(long waitMode, const CFilter_Bind3& filter, CMessageEvent** event)
{
    if (IsStopping())
        return 1;
    long result;
    do {
        CMessageEventManager* manager = GetPWorld()->MessageEventManager;
        *event = manager->FindMostRecentMessage(filter);
        if (*event) {
            result = 2;
            break;
        }
        PBrain->Yield();
        if (IsStopping()) {
            result = 1;
            break;
        }
        result = 4;
    } while (waitMode == 1);
    return result;
}