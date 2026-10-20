// CLadyGreysBed::Main() -- Lady Grey's bed entity script: wait (a script frame at a time) until the hero uses the bed, then until the hero has slept in it, then give the hero "OBJECT_SILVER_KEY"
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE0150, 269 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, IsDone 0x00F35B30 (on `this`, propagated label), Me's
//   CScriptThing slot 20 MsgIsUsedBy (+0x50, 0x004AAB10), CGameScriptInterface slots 7 NewScriptFrame, 50 MsgOnHeroSlept
//   (+0xc8), 121 GiveHeroObject (+0x1e4).
// Shape: two rotated `while (!X) { NewScriptFrame(); if (IsDone()) return; }` waits; the string temporary in the first loop
//   condition dies before the branch, so the negated result is materialised (`neg ; sbb ; inc` into bl); the out bool of
//   MsgOnHeroSlept is a byte in the frame's padding; the call on the Me member dispatches through its vtable.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThing {                                   // 12 bytes, vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50) 0x004AAB10
    unsigned char _body[8];
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049();
    virtual bool MsgOnHeroSlept(bool& slept) const;   // slot 50 (+0xc8)
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void GiveHeroObject(const CCharString& object, long number, bool b) const;   // slot 121 (+0x1e4)
};
class CLadyGreysBed {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    bool IsDone();                            // 0x00F35B30
    void Main();
};

void CLadyGreysBed::Main()
{
    while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    bool slept;
    while (!Game->MsgOnHeroSlept(slept)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    if (!IsDone())
        Game->GiveHeroObject("OBJECT_SILVER_KEY", -1, false);
}
