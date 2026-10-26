// CWaypointSpeedCheck::Main() -- entity script: track how close the hero gets to this waypoint; with data string "0" the best distance is reset to 1000 every frame; each new best (squared) distance is stored with the waypoint's data number
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDC6C0, 259 bytes (272 with padding), plain `ret`. Entity-script class (no RTTI;
//   name from the manifest label).
// Callees: CScriptThing slot 3 GetDataString() const (CCharString by hidden pointer, twice), ~CCharString 0x0099EAE0,
//   Compare 0x00411570 (the inlined CBasicString<char>::operator==(const char*) 0x004115A0; same-TU stand-in body below,
//   precedent 0x0075D2B0), the script base's IsActiveThreadTerminating 0x00F35B30 (x4), GetHero (slot 70, +0x118) and
//   NewScriptFrame (slot 7) through Game (+4), __fastcall GetSquaredDistanceBetweenThings(const CScriptThing&, const
//   CScriptThing&) 0x00CBE512, __fastcall GFCharStringToInt 0x0099E7F0. Literals "0" 0x01238860, "" 0x0122D70E,
//   1000.0f (0x447A0000, kept in esi).
// Layout: entity script {vptr, Game +4, CScriptThing Me +8, data +0x14}; the data's best squared distance +0x54 and
//   best waypoint number +0x58 (descriptive names).
// Shape: the "0" test is the inline CCharString::operator==(const char*) (null block: strcmp intrinsic `repe cmpsb`
//   with `sete`; otherwise the block compare `? false : true` -> neg/sbb/inc) into a named bool that lives in a frame
//   byte and is re-read every iteration; the loop is a rotated `while (!IsActiveThreadTerminating())`; the distance
//   is spilled (`fst`) into the dead string slot for the later store.
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)       // 0x00411570 (the landed body, stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const { return Compare(PData, text) ? false : true; }   // 0x004115A0 (inlined here)
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
class CScriptThing { public:                                   // vtable 0x01238C8C, 12 bytes
    virtual ~CScriptThing();
    virtual void v01() const; virtual void v02() const;
    virtual CCharString GetDataString() const;          // slot 3 (+0x0c)
    void* PImp_Data;                                    // +0x04
    void* PImp_Info;                                    // +0x08
};
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE512
class CGameScriptInterface { public:                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) CGameScriptInterface::NewScriptFrame @ 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) CGameScriptInterface::GetHero @ 0x00891CA0
};
struct CWaypointSpeedCheckData {                        // the script's shared data (this+0x14)
    unsigned char _pad[0x54];
    float         BestDistanceSquared;                  // +0x54
    long          BestWaypointNumber;                   // +0x58
};
class CWaypointSpeedCheck {
public:
    void*                    __vftable;                 // +0x00
    CGameScriptInterface*    Game;                      // +0x04
    CScriptThing             Me;                        // +0x08
    CWaypointSpeedCheckData* Data;                      // +0x14
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};
void CWaypointSpeedCheck::Main()
{
    bool resetEveryFrame = Me.GetDataString() == "0";
    while (!IsActiveThreadTerminating()) {
        if (resetEveryFrame) {
            if (IsActiveThreadTerminating())
                return;
            Data->BestDistanceSquared = 1000.0f;
        }
        float distance = GetSquaredDistanceBetweenThings(Me, Game->GetHero());
        if (distance < Data->BestDistanceSquared) {
            if (IsActiveThreadTerminating())
                return;
            Data->BestDistanceSquared = distance;
            Data->BestWaypointNumber = GFCharStringToInt(Me.GetDataString());
        }
        Game->NewScriptFrame();
    }
}