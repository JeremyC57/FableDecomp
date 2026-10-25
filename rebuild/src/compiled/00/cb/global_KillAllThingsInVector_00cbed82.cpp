// KillAllThingsInVector(CGameScriptInterfaceBase& game, const vector<CScriptThing>& things, bool fade) -- remove every living script thing in the vector through the game script interface
#pragma optimize("s", on)
// Leaf: ?KillAllThingsInVector@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBED82, 56 bytes, `__fastcall` global (interface in ecx, vector in
//   edx, the bool on the stack: `ret 4`) (PDB ?KillAllThingsInVector@@YIXAAVCGameScriptInterfaceBase@@ABV?$vector@
//   VCScriptThing@@...@@_N@Z). No frame although the bool sits on the stack: this body is built without the module's
//   `optimize("y",off)` (with it VC7.1 builds an ebp frame for the stack argument, 59 v 56); `("s",on)` alone.
// Callees: CScriptThing (vtable 0x01238C8C) slot 75 IsAlive (+0x12c); CGameScriptInterface slot 108 RemoveThing
//   (+0x1b0, 0x008910D0, ?RemoveThing@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z).
// Shape: begin read once into esi, end re-read every pass; the interface is kept in ebx, the bool argument re-read
//   from its stack slot for every call (`push [esp+14h]`), the second RemoveThing flag is the constant true.
struct CScriptThing {                       // 12 bytes, vtable 0x01238C8C (vptr, then the counted PImp pointer pair)
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011();
    virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017();
    virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029();
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035();
    virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041();
    virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053();
    virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059();
    virtual void t060();
    virtual bool IsUnconscious() const;     // slot 61 (+0xf4, 0x004AAFA0)
    virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067();
    virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073();
    virtual void t074();
    virtual bool IsAlive() const;           // slot 75 (+0x12c, 0x004AB130)
    void* m_pImpData;                       // +0x04 CCountedPointer<CThing> PImp
    void* m_pImpInfo;                       // +0x08
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
}
class CGameScriptInterfaceBase {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool fade, bool fadeFlag) const;   // slot 108 (+0x1b0)
};

void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const std::vector<CScriptThing>& things, bool fade)
{
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (it->IsAlive())
            game.RemoveThing(*it, fade, true);
}