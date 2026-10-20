// NScript::CQ_AmbushTradersScript::WatchForBanditsDeath() -- watch thread: each frame count the bandits that are no longer alive; once every bandit is down (and the thread is not terminating) set AllBanditsDead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0B960, length 239 bytes, plain `ret` / tail `jmp`.
// Identity: catalog CQ_AmbushTradersScript::WatchForBanditsDeath; it reads the class's AllBandits vector and sets its
// AllBanditsDead flag (Ego_r.pdb layout).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing slot 75 IsAlive (+0x12C) on each element;
// CGameScriptInterface slot 7 NewScriptFrame (+0x1C) through GSI (+0x40).
// Layout: retail quest base 0x48 bytes; AllBanditsDead +0x52 (PDB +0x66), AllBandits +0xA8 (PDB +0xBC; retail STLport
// vectors are 12 bytes, _M_start +0xA8, _M_finish +0xAC); CScriptThing is 12 bytes (vptr + counted PImp).
// Shape: `while (!terminating)` rotated; size() is the signed pointer difference divided by 12 (`imul 2AAAAAABh ;
// sar edx,1`) re-read at every test because the virtual IsAlive call may change the vector; the element is
// `*(begin() + i)` with the byte offset strength-reduced into ebx; the counter compare is `je` (equality) and the
// loop index compare `jb` (unsigned size_type); the all-dead exit re-tests termination and ends in the discarded
// IsActiveThreadTerminating() tail `jmp`.
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp_Data;                                    // +0x04 (CCountedPointer<CScriptThingImp>)
    void* PImp_Info;                                    // +0x08
};

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
};

namespace std {
template <class T> struct vector {                      // retail STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
}

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char _flags048[0x0a];                      // +0x48 .. +0x52 (QuestFailed .. ConvoyTrigger2)
    bool AllBanditsDead;                                // +0x52
    unsigned char _pad053[0x55];                        // +0x53 .. +0xa8 (flags, counters, speech timers)
    std::vector<CScriptThing> AllBandits;               // +0xa8
    void WatchForBanditsDeath();
};

void CQ_AmbushTradersScript::WatchForBanditsDeath()
{
    while (!IsActiveThreadTerminating()) {
        long deadCount = 0;
        for (unsigned long i = 0; i < AllBandits.size(); ++i) {
            if (!AllBandits[i].IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                ++deadCount;
            }
        }
        if (deadCount == AllBandits.size()) {
            if (IsActiveThreadTerminating())
                return;
            AllBanditsDead = true;
            IsActiveThreadTerminating();
            return;
        }
        GSI->NewScriptFrame();
    }
}
}