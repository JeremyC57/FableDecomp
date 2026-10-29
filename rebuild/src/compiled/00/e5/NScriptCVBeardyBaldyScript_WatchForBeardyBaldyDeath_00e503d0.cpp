// CV_BeardyBaldyScript::WatchForBeardyBaldyDeath() -- look up BB_BeardyBaldyMan, wait until he has been killed (by anyone), then (unless the thread is terminating) record BeardyBaldyKilled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E503D0, length 232 bytes (240 with padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, CBaseClass::~CBaseClass
// @ 0x0099A2E0, CScriptThing::~CScriptThing @ 0x004AA840 (the out-of-line copy), operator delete @ 0x00BFE9BC,
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed); through the interface (+0x40)
// GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer) and NewScriptFrame (slot 7, +0x1c); the
// script thing's implementation slot 18 (+0x48, MsgIsKilledBy(const CCharString&)) behind the inlined
// CScriptThing::MsgIsKilledBy (0x004AAAF0: null PImp -> false). Literal "" @ 0x0122D70E.
// Layout (retail = Ego_r.pdb CV_BeardyBaldyScript - 0x14): BeardyBaldyKilled +0x68.
// Shape: both strings are IMPLICIT CCharString conversions (built before the interface/PImp are read); the handle has
// THREE destructor sites (the terminating return inside the wait loop, a second `if (IsActiveThreadTerminating()) return;`
// after it, and the end): VC7.1 expands the inline destructor at the first site only and calls the out-of-line
// ~CScriptThing (0x004AA840) at the other two, which tail-merge (the two-site `if (!...)` form expands both).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CBaseClass {
public:
    virtual ~CBaseClass();                                       // 0x0099A2E0
    CBaseClass();                                                // 0x0099A2D0
};

void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
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
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer);  // slot 18 (+0x48)
};
class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);
    ~CScriptThing() {}                                      // out of line 0x004AA840
    bool MsgIsKilledBy(const CCharString& killer)           // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
};
class CGameScriptInterface { public:                       // CGameScriptInterface, retail vtable @ 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name);   // slot 72 (+0x120) CGameScriptInterface::GetThingWithScriptName @ 0x008A7D60
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
class CV_BeardyBaldyScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x20];                             // +0x48 quest flags, phase, required hair strings
    bool BeardyBaldyKilled;                                 // +0x68
    void WatchForBeardyBaldyDeath();
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;

void NScript::CV_BeardyBaldyScript::WatchForBeardyBaldyDeath()
{
    CScriptThing beardy = GSI->GetThingWithScriptName("BB_BeardyBaldyMan");
    while (!beardy.MsgIsKilledBy("")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    BeardyBaldyKilled = true;
}