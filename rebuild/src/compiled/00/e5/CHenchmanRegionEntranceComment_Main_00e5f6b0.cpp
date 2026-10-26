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
class CBaseClass { public:                               // vtable 0x01231710
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
};
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
class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const; // slot 75 (+0x12c)
};
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