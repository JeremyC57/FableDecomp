// CQ_BountyHuntScript::WatchForAllLeaderChargeGruntsKilled() -- collect every BH_GangGruntSpawn thing, wait until they are all dead, then (unless the thread is terminating) record AllLeaderChargeGruntsKilled
// Levers: ~vector<CScriptThing> __forceinline over an STLport-style by-value Destroy(first, last) helper; `if (Term) return; flag = true; }` with depth 0 covering the end (closing-brace exit out of line, the in-loop return inline)
// Leaf: WatchForAllLeaderChargeGruntsKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1CAC0, length 214 bytes.
// Identity: the catalog label CV_AmbushScamScript::WatchForAllAmbushersDead is propagated from the twin 0x00D1C9E0
// (same body, flag +0x5f). Both are spawned (function pointer stored at +0x34 of a spawned-function object, vtable
// 0x012C9EDC) from the bounty-hunt quest code at 0x00D1BEB9 / 0x00D1C665, and the flags fit Ego_r.pdb
// CQ_BountyHuntScript - 0x14: AllLeaderGruntsKilled +0x5f (the twin), AllLeaderChargeGruntsKilled +0x60 (this one).
// The method name is descriptive.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
// AreAllThingsInVectorDead @ 0x00CBED00 (`__fastcall`), CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed),
// ~vector<CScriptThing> @ 0x008AC970 (the out-of-line copy), free @ 0x00BFEA14; through the interface (+0x40)
// GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&) (slot 75, +0x12c) and NewScriptFrame (slot 7).
// Shape: the vector local is zero-constructed before the implicit name conversion; its in-class destructor (virtual
// element destructors with 0, then free) is inlined on the terminating return.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();                                // slot 0
    void* PImp_Data;
    void* PImp_Info;
};
inline void Destroy(CScriptThing* first, CScriptThing* last)
{
    for (; first != last; ++first)
        first->~CScriptThing();
}
struct CScriptThingVector {                                 // STLport vector<CScriptThing>, 12 bytes in retail
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~CScriptThingVector()
    {
        Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
bool __fastcall AreAllThingsInVectorDead(const CScriptThingVector& things);   // 0x00CBED00
class CGameScriptInterface { public:                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c) CGameScriptInterface::GetAllThingsWithScriptName @ 0x008A8570
};

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CQ_BountyHuntScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x17];                             // +0x48 QuestFailed .. AllDeputy2GruntsKilled
    bool AllLeaderGruntsKilled;                             // +0x5f
    bool AllLeaderChargeGruntsKilled;                       // +0x60
    void WatchForAllLeaderChargeGruntsKilled();
};
} // namespace NScript
using NScript::CQ_BountyHuntScript;

void NScript::CQ_BountyHuntScript::WatchForAllLeaderChargeGruntsKilled()
{
    CScriptThingVector grunts;
    GSI->GetAllThingsWithScriptName("BH_GangGruntSpawn", grunts);
    while (!AreAllThingsInVectorDead(grunts)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    AllLeaderChargeGruntsKilled = true;
}
#pragma inline_depth()