// NScript::CPersonalScript_EmptyScript::CheckForHeroActions() -- debug watcher: each script frame, while the hero is alive, fetch the things the hero killed and show "You killed <n> things, of types: <t1>, <t2>." as a screen message
// Leaf: CheckForHeroActions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF5200, 592 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 70 (+0x118) GetHero (by reference),
//   112 (+0x1c0) AddScreenMessage(const CCharString&, ETextGroupSelectionMethod), 7 (+0x1c) NewScriptFrame (interface at +0x40);
//   CScriptThing slots 55 (+0xdc) MsgGetThingsKilled and 75 (+0x12c) IsAlive through the counted implementation pointer;
//   GFIntToCharString 0x0099F830, GFUIntToCharString 0x0099F8B0 ("%u"), operator+(const char*, const CCharString&) 0x0099F690,
//   operator+(const CCharString&, const char*) 0x0099F600 (all __fastcall, hidden result in ecx), CCharString::operator+=
//   0x0099F0A0 / 0x0099F100, ~CCharString 0x0099EAE0, free 0x00BFEA14 (the vector buffer), operator delete 0x00BFE9BC,
//   ~CBaseClass 0x0099A2E0, out-of-line ~CScriptThing 0x004AA840.
// Layout: CScriptBase is 0x40 bytes with the interface pointer at +0x40 (landed 0x00EF8BC0 ChangeAlpha model); CScriptThing
//   {vptr 0x01238C8C, PImp.Data, PImp.Info} (engine/CScriptThing.h is the flat data view; the virtual interface is modelled
//   locally). Strings: 0x012F65C8 "You killed ", 0x012F65D4 " things, of types: ", 0x01238794 ".", 0x012397CC ", ".
// Shape: GOTCHAS b18 recipe -- __forceinline ~CScriptThing with an in-body `#pragma inline_depth(0)` region around the one
//   exit retail destroys the hero out of line (0x004AA840). That exit frees the vector INLINE first, so it leaves the
//   vector's block by `goto` (the vector dtor expands at the goto) and the depth-0 `return` sits after the block (hero
//   only). The hero is scoped inside the loop body so its dtor runs before NewScriptFrame; MsgGetThingsKilled's inline
//   body is `if (Data) return ...; return false;` (`cmp al,bl` on the zero register; the `&&` form gives `test al,al`).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
void operator delete(void* p);                         // 0x00BFE9BC
struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator+=(const CCharString& other); // 0x0099F0A0
    CCharString& operator+=(const char* text);         // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
CCharString __fastcall GFIntToCharString(long value);                           // 0x0099F830
CCharString __fastcall GFUIntToCharString(unsigned long value);                 // 0x0099F8B0
namespace std {
template <class T> struct vector {                     // STLport vector<unsigned long> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
};
}
struct CCPPointerInfo {                                // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
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
struct CBaseClass {
    CBaseClass() { }
    virtual ~CBaseClass();                             // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                     // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }
    __forceinline virtual ~CScriptThing() { }          // out of line 0x004AA840
    virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const;
    virtual void v005() const; virtual void v006() const; virtual void v007() const; virtual void v008() const;
    virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const;
    virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const;
    virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const;
    virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const;
    virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const;
    virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const;
    virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const;
    virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const;
    virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const;
    virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const;
    virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const;
    virtual void v053() const; virtual void v054() const;
    virtual bool MsgGetThingsKilled(std::vector<unsigned long>& killed) const   // slot 55 (+0xdc)
    {
        if (PImp.Data)
            return PImp.Data->MsgGetThingsKilled(killed);
        return false;
    }
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const;
    virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const;
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const;
    virtual void v068() const; virtual void v069() const; virtual void v070() const; virtual void v071() const;
    virtual void v072() const; virtual void v073() const; virtual void v074() const;
    virtual bool IsAlive() const                       // slot 75 (+0x12c)
    {
        return PImp.Data && PImp.Data->IsAlive();
    }
};
typedef long ETextGroupSelectionMethod;
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006();
    virtual void NewScriptFrame() const;               // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();                   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void AddScreenMessage(const CCharString& text, ETextGroupSelectionMethod method) const;   // slot 112 (+0x1c0)
};
struct CScriptBase {
    unsigned char         _pad00[0x40];
    CGameScriptInterface* GSI;                         // +0x40
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};
namespace NScript {
struct CPersonalScript_EmptyScript : CScriptBase {
    void CheckForHeroActions();
};

void CPersonalScript_EmptyScript::CheckForHeroActions()
{
    if (IsActiveThreadTerminating())
        return;
    do {
        {
            CScriptThing hero = GSI->GetHero();
            if (hero.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                {
                    std::vector<unsigned long> killed;
                    if (hero.MsgGetThingsKilled(killed)) {
                        if (IsActiveThreadTerminating())
                            goto terminating;
                        CCharString text = "You killed " + GFIntToCharString(killed.size()) + " things, of types: ";
                        for (unsigned int i = 0; i < killed.size(); ++i) {
                            text += GFUIntToCharString(killed[i]);
                            if (i == killed.size() - 1)
                                text += ".";
                            else
                                text += ", ";
                        }
                        GSI->AddScreenMessage(text, 0);
                    }
                }
            }
            if (false) {
terminating:
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
        }
        GSI->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}
}
