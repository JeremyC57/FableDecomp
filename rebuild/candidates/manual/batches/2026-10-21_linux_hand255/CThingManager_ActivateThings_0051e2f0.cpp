// CThingManager::ActivateThings(vector<CThing*>& things) -- bring freshly created things through their three initialisation stages (stage 0: virtual slot 8, stage 1: OnCreate slot 9, stage 2: Activate slot 10) with a progress line before each thing; the hero is skipped while the game component's flag at +0x101 is set, and the stage-2 hero is activated last (when alive)
// Leaf: ?ActivateThings@CThingManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E2F0, length 395 bytes (+5 int3), `ret 4`.
//   Name and argument from the catalog / engine_api (`CThingManager::ActivateThings(vector<CThing*>*)`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, DisplayProgress(const CCharString&, float, bool, bool)
//   @ 0x009E9F40 (`__fastcall`), ~CCharString @ 0x0099EAE0; the things' virtual slots 8/9/10 (+0x20/+0x24/+0x28).
//   Strings "Activate Things" @ 0x01244E24, "Activate Things: OnCreate" @ 0x01244E08, "Activate Things: Initial
//   Activate" @ 0x01244DE4, "Activate Things End" @ 0x01244DD0.
// Shape: each progress line is an inline helper with a named CCharString local (retail builds the string before the
//   float/bool pushes and re-leas its address for the call -- the 0x0051E2F0 "named local in an inline" idiom).
// Layout (retail): CThing ThingType byte +0x10, Dead bit 0 of +0x91, InitialisationStage bits 0..2 of +0x94 (Ego_r.pdb
//   +0xa1/+0xa4, CThing 0x10 lower); CThingManager +0x24 the main game component (Ego_r.pdb Component +0x28 -- the
//   base part is 4 smaller in retail; unproven), whose bool at +0x101 holds the hero back.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
__forceinline void ShowProgress(const char* message)
{
    CCharString text(message);
    DisplayProgress(text, -1.0f, false, false);
}

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void Initialise();                          // slot 8
    virtual void OnCreate();                            // slot 9
    virtual void Activate();                            // slot 10
    unsigned char  _pad04[0x10 - 4];
    unsigned char  ThingType;                           // +0x10
    unsigned char  _pad11[0x91 - 0x11];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  _bits91 : 7;
    unsigned char  _pad92[2];
    unsigned char  InitialisationStage : 3;             // +0x94 bits 0..2
    unsigned char  _bits94 : 5;
};

template<class T> struct vector {
    T* _Myfirst;
    T* _Mylast;
    T* _Myend;
    T* begin() { return _Myfirst; }
    T* end() { return _Mylast; }
};

struct CMainGameComponent {
    unsigned char _pad00[0x101];
    bool          HoldHero;                             // +0x101
};

struct CThingManager {
    unsigned char       _pad00[0x24];
    CMainGameComponent* Component;                      // +0x24
    void ActivateThings(vector<CThing*>& things);
};

void CThingManager::ActivateThings(vector<CThing*>& things)
{
    CThing** it;
    for (it = things.begin(); it != things.end(); ++it) {
        ShowProgress("Activate Things");
        CThing* thing = *it;
        if (thing->InitialisationStage == 0 && (thing->ThingType != 2 || !Component->HoldHero))
            thing->Initialise();
    }
    for (it = things.begin(); it != things.end(); ++it) {
        ShowProgress("Activate Things: OnCreate");
        CThing* thing = *it;
        if (thing->InitialisationStage == 1 && (thing->ThingType != 2 || !Component->HoldHero))
            thing->OnCreate();
    }
    CThing* hero = 0;
    for (it = things.begin(); it != things.end(); ++it) {
        ShowProgress("Activate Things: Initial Activate");
        CThing* thing = *it;
        if (thing->InitialisationStage == 2) {
            if (thing->ThingType == 2)
                hero = thing;
            else
                thing->Activate();
        }
    }
    if (hero && !hero->Dead && !Component->HoldHero)
        hero->Activate();
    ShowProgress("Activate Things End");
}
