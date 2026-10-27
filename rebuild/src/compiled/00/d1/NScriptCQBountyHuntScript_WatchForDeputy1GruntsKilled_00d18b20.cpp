// NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled() -- script thread: collect the BH_GangGruntSpawn things, wait until all are dead (stopping when the thread is terminated) and set AllDeputy1GruntsKilled
// Leaf: ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00d18b20, 214 bytes, thiscall, plain `ret`.
// Identity (proven): its only reference is the functor store at 0x00D18906 (thread "WatchForDeputy1GruntsKilled"), the spawned-function registration of that thread name; Ego_r.pdb ?WatchForDeputy1GruntsKilled@CQ_BountyHuntScript@NScript@@QAEXXZ; the flag it sets is Ego_r.pdb CQ_BountyHuntScript::AllDeputy1GruntsKilled (PDB +0x71, retail +0x5d with the script base 0x14 smaller).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, AreAllThingsInVectorDead (__fastcall) 0x00CBED00, vector<CScriptThing>::~vector 0x008AC970 (the out-of-line
//   copy at the normal end), CScriptThing's virtual destructor (slot 0) and free 0x00BFEA14 in the inlined copy; through the
//   game script interface (+0x40): GetAllThingsWithScriptName (slot 75, +0x12c), NewScriptFrame (slot 7, +0x1c).
//   Literal "BH_GangGruntSpawn" 0x012CA46C.
// Shape (family L140 = the landed WatchForAcolyteExit 0x00EDD060's model): retail expands the local vector's destructor at
//   the wait loop's terminating return and calls the out-of-line copy at the normal end; the lane calls the out-of-line
//   copy at every site, so the destruction is written per exit (a __forceinline copy of the STLport ~vector body at the
//   return, the out-of-line ~vector at the end, written in both arms of the final terminating test so VC7.1 tail-merges
//   the call and hoists its `lea ecx` above the branch), as in the landed twin.
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

namespace NScript {
class CQ_BountyHuntScript : public CScriptBase {
public:
    CGameScriptInterface* m_pInterface;              // +0x40
    unsigned char         _pad44[0x5d - 0x44];
    bool                  AllDeputy1GruntsKilled;              // +0x5d
    void WatchForDeputy1GruntsKilled();
};
} // namespace NScript

void NScript::CQ_BountyHuntScript::WatchForDeputy1GruntsKilled()
{
    vector<CScriptThing> things;
    m_pInterface->GetAllThingsWithScriptName("BH_GangGruntSpawn", things);
    while (!AreAllThingsInVectorDead(&things)) {
        m_pInterface->NewScriptFrame();
        if (IsActiveThreadTerminating()) {
            things.DestroyInline();
            return;
        }
    }
    if (IsActiveThreadTerminating()) {
        things.Destroy();
        return;
    }
    AllDeputy1GruntsKilled = true;
    things.Destroy();
}