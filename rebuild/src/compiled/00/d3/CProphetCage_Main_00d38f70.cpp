// CProphetCage::Main() -- a Fire Heart prophet cage: under an alive filter on Me, wait for this cage's prophet to be released or killed; then wait for the OPENCAGE cutscene flag, burst the cage with the crystal implode (released) or crystal open (killed) effect, fade the cage out, clear the flag and mark the cage activated; then idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38F70, length 955 bytes, plain `ret`.
// Callees: CIsThingAlive filter (vtable 0x012C32F0, CScriptThing copy 0x004ABE90, out-of-line ~CScriptThing 0x004AA840),
// SetPredicate 0x00F35B10, IsActiveThreadTerminating 0x00F35B30, CCharString 0x0099EBF0 / 0x0099EAE0 ("OPENCAGE"
// 0x012CCC28, "FOO" 0x012CD1E0, "CRYSTAL_IMPLODE_01" 0x012CD1CC, "CRYSTAL_OPEN_01" 0x012CD1BC), map<CCharString,bool>
// operator[] 0x008ADF10 (folded with the C2DVector instantiation the manifest names), ~CBaseClass 0x0099A2E0, operator
// delete 0x00BFE9BC, FadeOutAndKillEntity 0x00CBE716 (__fastcall); CScriptThing slot 6 GetPos; CGameScriptInterface
// slots 7 NewScriptFrame, 100 CreateEffect.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, IWasActivated +0x1c, MyID
// +0x20 (Ego_r.pdb). Retail CQ_FireHeartScript (Ego_r.pdb less 0x14 and 4 per STLport container, as the landed
// SetupPuzzleBlocks 0x00D36140): CutsceneFlags +0x4c, ProphetsKilled +0xc8, ProphetsReleased +0xd4 (byte vectors).
// Shape: the CMagicBarrierGraveyard::Main 0x00CE0600 model (__forceinline ~CScriptThing, inline_depth(0) around the
// filter); the discarded CreateEffect result is destroyed inline.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
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
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    __forceinline ~CScriptThing() {}                     // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
};
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool removeOnFade);   // 0x00CBE716
class CGameScriptInterface { public:                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const; virtual void NewScriptFrame() const; /* slot 7 (+0x1c) */ virtual void v008() const; virtual void v009() const;
    virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const;
    virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const;
    virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const;
    virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const;
    virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual void v070() const; virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const;
    virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const;
    virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const;
    virtual CScriptThing CreateEffect(const CCharString& effect, const C3DVector& position, const CCharString& bone, float angle, bool b, bool c) const;   // slot 100 (+0x190) 0x0089F9E0 public: virtual class CScriptThing __fastcall CGameScriptInterface::CreateEffect(class CCharString const &, class C3DVector const &, class CCharString const &, float, bool, bool) const
};

namespace NScript {
class __declspec(novtable) CPredicateBase
{
public:
    virtual void f00();
};
}
#pragma inline_depth(0)
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:                                   // vtable 0x012C32F0
    CScriptThing m_thing;                                // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    virtual ~CIsThingAlive() { }
};
}
using NScript::CIsThingAlive;
#pragma inline_depth()
struct CCutsceneFlagMap {                                // map<CCharString, bool> (12 bytes in retail)
    unsigned char _body[0xc];
    bool& operator[](const CCharString& key);           // 0x008ADF10
};
namespace NScript {
class CQ_FireHeartScript { public:                              // retail-offset view (see Layout)
    unsigned char    _pad00[0x4c];
    CCutsceneFlagMap CutsceneFlags;                      // +0x4c
    unsigned char    _pad58[0xc8 - 0x58];
    bool*            ProphetsKilled;                     // +0xc8 (vector<bool> _M_start)
    unsigned char    _padcc[0xd4 - 0xcc];
    bool*            ProphetsReleased;                   // +0xd4 (vector<bool> _M_start)
};
}
class CEntityScriptBase { public:                               // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CProphetCage : CEntityScriptBase {
    NScript::CQ_FireHeartScript* ParentClass;            // +0x14
    void*                        PMasterData;            // +0x18
    bool                         IWasActivated;          // +0x1c
    long                         MyID;                   // +0x20
    void Main();
};

void CProphetCage::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!IWasActivated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->ProphetsReleased[MyID]) {
            if (IsActiveThreadTerminating())
                return;
            while (ParentClass->CutsceneFlags["OPENCAGE"] != true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CreateEffect("CRYSTAL_IMPLODE_01", Me.GetPos(), "FOO", 0.0f, false, false);
            FadeOutAndKillEntity(Me, 0.2f, true, true);
            ParentClass->CutsceneFlags["OPENCAGE"] = false;
            IWasActivated = true;
        } else if (ParentClass->ProphetsKilled[MyID]) {
            if (IsActiveThreadTerminating())
                return;
            while (ParentClass->CutsceneFlags["OPENCAGE"] != true) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->CreateEffect("CRYSTAL_OPEN_01", Me.GetPos(), "FOO", 0.0f, false, false);
            FadeOutAndKillEntity(Me, 0.2f, true, true);
            ParentClass->CutsceneFlags["OPENCAGE"] = false;
            IWasActivated = true;
        }
    }
    while (!IsActiveThreadTerminating())
        Game->NewScriptFrame();
}