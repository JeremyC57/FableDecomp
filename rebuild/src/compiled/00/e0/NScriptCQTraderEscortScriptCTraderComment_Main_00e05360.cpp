// NScript::CQ_TraderEscortScript::CTraderComment::Main() -- trader-escort comment marker: with an is-alive filter on Me, each frame look for a live DarkwoodTrader within the script def's comment distance; the first one that makes Me's comment removes the marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E05360, length 585 bytes, plain `ret`.
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 / ~CScriptThing 0x004AA840 (the CIsThingAlive filter, vtable
//   0x012C32F0, inline destructor) and SetPredicate 0x00F35B10; IsActiveThreadTerminating 0x00F35B30; CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0; IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall);
//   CQ_TraderEscortScript::MakeTraderComment 0x00E01900; ~vector<CScriptThing> 0x008AC970 (out of line at two returns;
//   inline elsewhere: virtual element destructor slot 0, free 0x00BFEA14); CScriptThing slots 3 GetDataString (+0x0c, by
//   value), 75 IsAlive (+0x12c); GameCalls slots 7 NewScriptFrame, 75 GetAllThingsWithScriptName, 108 RemoveThing. The
//   script def 0x0143E90C (+0xE10 TE_TraderCommentDistance). String "DarkwoodTrader".
// Layout: Ego_r.pdb CTraderComment: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (as the landed
//   CDarkwoodTrader::OnPredicateFail 0x00E01760); CScriptDef TE_TraderCommentDistance PDB +0xEC0 = retail +0xE10.
// Shape: generated script code; the trader list is a function-scope vector whose destructor VC7.1 expands at three exits
//   and calls out of line at the two under `#pragma inline_depth(0)`.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThing {                                   // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                            // slot 0, out of line 0x004AA840
    virtual void t001() const; virtual void t002() const;
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c) 0x004AA900
    virtual void t004() const; virtual void t005() const; virtual void t006() const; virtual void t007() const; virtual void t008() const; virtual void t009() const; virtual void t010() const; virtual void t011() const;
    virtual void t012() const; virtual void t013() const; virtual void t014() const; virtual void t015() const; virtual void t016() const; virtual void t017() const; virtual void t018() const; virtual void t019() const;
    virtual void t020() const; virtual void t021() const; virtual void t022() const; virtual void t023() const; virtual void t024() const; virtual void t025() const; virtual void t026() const; virtual void t027() const;
    virtual void t028() const; virtual void t029() const; virtual void t030() const; virtual void t031() const; virtual void t032() const; virtual void t033() const; virtual void t034() const; virtual void t035() const;
    virtual void t036() const; virtual void t037() const; virtual void t038() const; virtual void t039() const; virtual void t040() const; virtual void t041() const; virtual void t042() const; virtual void t043() const;
    virtual void t044() const; virtual void t045() const; virtual void t046() const; virtual void t047() const; virtual void t048() const; virtual void t049() const; virtual void t050() const; virtual void t051() const;
    virtual void t052() const; virtual void t053() const; virtual void t054() const; virtual void t055() const; virtual void t056() const; virtual void t057() const; virtual void t058() const; virtual void t059() const;
    virtual void t060() const; virtual void t061() const; virtual void t062() const; virtual void t063() const; virtual void t064() const; virtual void t065() const; virtual void t066() const; virtual void t067() const;
    virtual void t068() const; virtual void t069() const; virtual void t070() const; virtual void t071() const; virtual void t072() const; virtual void t073() const; virtual void t074() const;
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    void* PImpData;
    void* PImpInfo;
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern "C" void free(void* p);                          // 0x00BFEA14 (MSVCR71)
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {                      // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()                                           // out-of-line COMDAT copy 0x008AC970
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* begin() { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return *(begin() + i); }
};
struct GameCalls {                                      // CGameScriptInterface, retail vtable 0x01260F0C
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const;
    virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const;
    virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const;
    virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const;
    virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const;
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const;
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const; virtual void v071() const;
    virtual void v072() const; virtual void v073() const; virtual void v074() const;
    virtual long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
    virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const;
    virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const; virtual void v090() const; virtual void v091() const;
    virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const;
    virtual void v100() const; virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const;
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
};
struct CScriptDef {
    unsigned char _pad[0xe10];
    long          TE_TraderCommentDistance;             // +0xe10 (PDB +0xec0)
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
namespace NScript {
struct CPredicateBase {
    virtual bool IsTrue() = 0;
};
struct CIsThingAlive : CPredicateBase {                 // vtable 0x012C32F0
    CScriptThing Thing;                                 // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
    ~CIsThingAlive() { }
    virtual bool IsTrue();
};
struct CQ_TraderEscortScript {
    enum ETraderComment { TRADER_COMMENT_0, TRADER_COMMENT_1 };
    bool MakeTraderComment(const CCharString& text, const CScriptThing& trader, ETraderComment comment);   // 0x00E01900
    struct CTraderComment;
};
struct CQ_TraderEscortScript::CTraderComment {
    void*                  VTable;
    GameCalls*             Game;                        // +0x04
    CScriptThing           Me;                          // +0x08
    CQ_TraderEscortScript* ParentClass;                 // +0x14
    void*                  PMasterData;                 // +0x18
    void SetPredicate(const CPredicateBase& predicate); // 0x00F35B10
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CQ_TraderEscortScript::CTraderComment::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    vector<CScriptThing> traders;
    Game->GetAllThingsWithScriptName("DarkwoodTrader", traders);
    long distance = g_pScriptDef->TE_TraderCommentDistance;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        for (unsigned int i = 0; i < traders.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            if (traders[i].IsAlive() && IsDistanceBetweenThingsUnder(Me, traders[i], (float)distance)) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                if (ParentClass->MakeTraderComment(Me.GetDataString(), traders[i], TRADER_COMMENT_1)) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
                    Game->RemoveThing(Me, false, true);
                    return;
#pragma inline_depth()
                }
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}
}