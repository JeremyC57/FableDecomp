// CQ_BountyHuntScript::WatchForAllGruntsKilled() -- script thread: collect the BH_GangGruntSpawn things, wait until all of them are dead, then set AllGruntsKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17340, length 294 bytes, plain `ret`; started as a thread at 0x00D14D61.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, AreAllThingsInVectorDead
// (__fastcall, 0x00CBED00), CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptThing's virtual destructor
// (slot 0, flag 0) per element and free 0x00BFEA14 (the vector's storage); through the game script interface (+0x40,
// retail vtable 0x01260F0C): GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&) (slot 75, +0x12c,
// 0x008A8570), NewScriptFrame (slot 7, +0x1c, 0x006E7410). Literal "BH_GangGruntSpawn" 0x012CA46C.
// Layout: Ego_r.pdb CQ_BountyHuntScript - 0x14: AllGruntsKilled +0x61.
// Shape: a local STLport vector (three immediate zero stores) whose __forceinline destructor (by-value _Destroy loop of
// virtual destructor calls, then free) is expanded at all four returns with the free tail shared; the wait loop
// is `for (;;) { if (term) return; if (AllDead) break; yield; }`, followed by two terminating re-tests (a plain
// `inline` destructor is emitted out of line and called once).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern "C" void free(void* p);                         // 0x00BFEA14 (MSVCR71)

class CCPPointerInfo;

class CBaseClass { public:                                    // polymorphic root of the script handles
    CBaseClass();                                      // 0x0099A2D0
    virtual ~CBaseClass();                             // 0x0099A2E0
};

class CScriptThing : public CBaseClass { public:                     // 12 bytes, vtable 0x01238C8C (slot 0: virtual dtor)
    CScriptThing* PImp_Data;                           // +0x04
    CCPPointerInfo* PImp_Info;                         // +0x08
};

template <class T> __forceinline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct vector {                     // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
bool __fastcall AreAllThingsInVectorDead(vector<CScriptThing>* things);   // 0x00CBED00

class CGameScriptInterface { public:
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

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                // 0x00CB7940
};

struct CQ_BountyHuntScript : CScriptBase {
    CGameScriptInterface* m_pInterface;              // +0x40
    void*                 m_pOwner;                  // +0x44
    unsigned char         _pad48[0x19];
    bool                  AllGruntsKilled;           // +0x61
    void WatchForAllGruntsKilled();
};

void CQ_BountyHuntScript::WatchForAllGruntsKilled()
{
    vector<CScriptThing> grunts;
    m_pInterface->GetAllThingsWithScriptName("BH_GangGruntSpawn", grunts);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (AreAllThingsInVectorDead(&grunts))
            break;
        m_pInterface->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    AllGruntsKilled = true;
}