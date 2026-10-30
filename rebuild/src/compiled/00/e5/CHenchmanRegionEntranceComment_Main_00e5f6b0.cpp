// CHenchmanRegionEntranceComment::Main() -- collect every "BodyGuard" thing; walk them and, for the first one within 10 units of this marker, ask the bodyguard quest to make region-entrance comment 2 (priority 2) with this marker's data string, stopping once a comment was made
// Grade: RELOCATION_MATCH -- the vector's operator[] goes through an inline begin() (`*(begin() + n)`), which gives retail's element-address registers (offset in edx, start in eax) and Me's vtable in edx
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5F6B0, length 335 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("BodyGuard" @ 0x012CB3F0), IsDone
//   @ 0x00F35B30, IsDistanceBetweenThingsUnder @ 0x00CBE2FF (`__fastcall`), CBaseClass ctor @ 0x0099A2D0 (the default
//   CScriptThing temporary: vtable 0x01238C8C, PImp zeroed), ~CScriptThing @ 0x004AA840, CScriptThing slot 3
//   GetDataString (CCharString by value), CV_BodyGuardScript::MakeHenchmanComment(const CCharString&, long,
//   EHENCHMAN_COMMENT, const CScriptThing&) @ 0x00E5EBD0; CGameScriptInterface (vtable 0x01260F0C) slot 75
//   GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>&).
// Layout: Ego_r.pdb CHenchmanRegionEntranceComment (44 bytes): entity-script base {vptr, Game +4, CScriptThing Me +8},
//   ParentClass (CV_BodyGuardScript*) +0x14, PMasterData +0x18, HenchmanVector (STLport vector<CScriptThing>) +0x1C;
//   CScriptThing = engine/CScriptThing.h {vptr, PImp {Data, Info}} over CBaseClass, modelled locally for its virtuals.
// Shape: `size() == 0` exits through a duplicated `if (IsActiveThreadTerminating()) return;` tail; the walk is `for (unsigned i ...)` with
//   the element offset strength-reduced into a frame word; the comment result survives the temporaries in bl.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo;
class CScriptThing : public CBaseClass { public:                 // vtable 0x01238C8C, 12 bytes
    virtual void v001(); virtual void v002();
    virtual CCharString GetDataString() const;     // slot 3 (+0x0c)
    CScriptThing*   PImp_Data;                     // +0x04
    CCPPointerInfo* PImp_Info;                     // +0x08
    CScriptThing() : PImp_Data(0), PImp_Info(0) { }
    virtual ~CScriptThing();                               // 0x004AA840
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T* begin() { return _M_start; }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
enum EHENCHMAN_COMMENT { HENCHMAN_COMMENT_REGION_ENTRANCE = 2 };
struct CV_BodyGuardScript {
    bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT comment, const CScriptThing& about = CScriptThing());   // 0x00E5EBD0
};
struct CQ_SunnyvaleMasterData;

class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CHenchmanRegionEntranceComment : public CEntityScriptBase {
public:
    void*                      __vftable;          // +0x00
    CGameScriptInterface*      Game;               // +0x04
    CScriptThing               Me;                 // +0x08
    CV_BodyGuardScript*        ParentClass;        // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;        // +0x18
    std::vector<CScriptThing>  HenchmanVector;     // +0x1C
    __forceinline void WalkHenchmen()
    {
        for (unsigned int i = 0; i < HenchmanVector.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsUnder(Me, HenchmanVector[i], 10.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                if (ParentClass->MakeHenchmanComment(Me.GetDataString(), 2, HENCHMAN_COMMENT_REGION_ENTRANCE))
                    break;
            }
        }
        IsActiveThreadTerminating();
    }
    void Main();
};

void CHenchmanRegionEntranceComment::Main()
{
    Game->GetAllThingsWithScriptName("BodyGuard", HenchmanVector);
    if (HenchmanVector.size() == 0) {
        IsActiveThreadTerminating();
        return;
    }
    WalkHenchmen();
}