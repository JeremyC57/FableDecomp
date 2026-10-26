// CV_TempleOfLightScript::WatchForAcolyteExit() -- script thread: once the hero has attacked, collect the TOL_AcolyteGood things, wait until all are gone and set AllAcolytesEscaped
// Leaf: ?WatchForAcolyteExit@CV_TempleOfLightScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EDD060, length 270 bytes, plain `ret`.
// Label: catalog "CQ_BountyHuntScript::WatchForAllGruntsKilled" is propagated; the body is started as the thread
// "WatchForAcolyteExit" (0x012F3E28) by CV_TempleOfLightScript::Main (0x00EDB190, functor store at 0x00EDB3AD).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, AreAllThingsInVectorDead (__fastcall, 0x00CBED00), vector<CScriptThing>::~vector
// (out-of-line COMDAT copy 0x008AC970, propagated label "~vector<CConversation>"), CScriptThing's virtual destructor
// (slot 0) and free 0x00BFEA14 in the inlined copy; through the game script interface (+0x40): NewScriptFrame
// (slot 7, +0x1c), GetAllThingsWithScriptName (slot 75, +0x12c). Literal "TOL_AcolyteGood" 0x012F3E54.
// Layout: Ego_r.pdb CV_TempleOfLightScript - 0x14: AttackedByHero +0x5b, AllAcolytesEscaped +0x5d.
// Shape: the inline-once destructor band: retail expands the local vector's destructor at the wait loop's terminating
// return and calls the out-of-line copy at the normal end (the two closing terminating tests jump to that call). The
// lane calls the out-of-line copy at every site, so the vector's destruction is written per exit: a __forceinline copy
// of the STLport ~vector body at the wait-loop return and the out-of-line ~vector (Destroy) once at the merged end.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

struct CCPPointerInfo;

struct CBaseClass {                                    // polymorphic root of the script handles
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};

struct CScriptThing : CBaseClass {                     // 12 bytes, vtable 0x01238C8C (slot 0: virtual dtor)
    CScriptThing* PImp_Data;                           // +0x04
    CCPPointerInfo* PImp_Info;                         // +0x08
};

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct vector {                     // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    void Destroy();                                    // ~vector, out of line: 0x008AC970
    __forceinline void DestroyInline()                 // the same body, expanded once (VC's inline-once band)
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
bool __fastcall AreAllThingsInVectorDead(vector<CScriptThing>* things);   // 0x00CBED00

struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();                                       // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things); // slot 75 (+0x12c)
};

struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                // 0x00CB7940
};

namespace NScript {
class CV_TempleOfLightScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    long DonationTotal;                              // +0x48
    long DonationThisVisit;                          // +0x4c
    long DonationCounter;                            // +0x50
    bool DonationInvalid;                            // +0x54
    bool DonationActive;                             // +0x55
    bool DisplayQuestInfo;                           // +0x56
    bool WeaponWarningComplete;                      // +0x57
    bool WeaponUnsheathed;                           // +0x58
    bool TempleIntroComplete;                        // +0x59
    bool InsideTemple;                               // +0x5a
    bool AttackedByHero;                             // +0x5b
    bool AcolyteAttackMessageComplete;               // +0x5c
    bool AllAcolytesEscaped;                         // +0x5d
    void WatchForAcolyteExit();
};
} // namespace NScript
using NScript::CV_TempleOfLightScript;

void NScript::CV_TempleOfLightScript::WatchForAcolyteExit()
{
    while (!AttackedByHero) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    vector<CScriptThing> acolytes;
    m_pInterface->GetAllThingsWithScriptName("TOL_AcolyteGood", acolytes);
    for (;;) {
        if (IsActiveThreadTerminating()) {
            acolytes.DestroyInline();
            return;
        }
        if (AreAllThingsInVectorDead(&acolytes))
            break;
        m_pInterface->NewScriptFrame();
    }
    if (!IsActiveThreadTerminating() && !IsActiveThreadTerminating())
        AllAcolytesEscaped = true;
    acolytes.Destroy();
}