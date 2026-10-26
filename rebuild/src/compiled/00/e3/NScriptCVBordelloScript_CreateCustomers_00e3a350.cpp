// CV_BordelloScript::CreateCustomers() -- while the Bordello region is loaded, every 15 timer ticks spawn a new BordelloClient (female villagers once the bordello became a nunnery, male otherwise) at M_ClientSpawn while fewer than three clients are alive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E3A350, length 860 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; ~CBaseClass 0x0099A2E0 and operator delete 0x00BFE9BC (the inline ~CScriptThing); CScriptThing slot 6
//   GetPos (+0x18); GameCalls slots 7 NewScriptFrame, 12 IsRegionLoaded, 72 GetThingWithScriptName (by value), 91
//   CreateCreature (by value); the CTimer calls 87..90 through the global interface 0x0143E8F8. Strings "Bordello",
//   "BordelloClient", "M_ClientSpawn", "CREATURE_BS_VILLAGER_FEMALE", "CREATURE_BS_VILLAGER_MALE".
// Layout: Ego_r.pdb CV_BordelloScript minus 0x14 (retail CScriptBase): BecomeNunnery +0x4d, ClientsAlive +0x54.
// Shape: generated script code; CTimer has inline ctor and dtor; the names are implicit conversions (all three built before
//   the GetThingWithScriptName call); both temporaries' ~CScriptThing expand inline.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
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
struct C3DVector { float X, Y, Z; };
struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    virtual void t001() const; virtual void t002() const; virtual void t003() const; virtual void t004() const; virtual void t005() const;
    virtual const C3DVector& GetPos() const;                // slot 6 (+0x18) 0x004AA980
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() { }                       // out of line 0x004AA840
};
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
 virtual bool IsRegionLoaded(CCharString const &) const; //slot12 0x0088e300
 virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
 virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
 virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
 virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
 virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
 virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
 virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
 virtual void v069(); virtual void v070(); virtual void v071();
 virtual CScriptThing GetThingWithScriptName(CCharString const &) const; //slot72 0x008a7d60
 virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
 virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
 virtual long RegisterTimer(void); //slot87 0x006e79e0
 virtual void DeregisterTimer(long); //slot88 0x006e7690
 virtual void SetTimer(long, long); //slot89 0x006e76e0
 virtual long GetTimer(long) const; //slot90 0x006e7710
 virtual CScriptThing CreateCreature(CCharString const &, C3DVector const &, CCharString const &, bool) const; //slot91 0x008a9100
};
extern GameCalls* g_pGameScriptInterface;                   // 0x0143E8F8
struct CTimer {                                             // a script timer: the interface's timer index
    long Index;
    CTimer() { Index = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
namespace NScript {
class CV_BordelloScript : public CScriptBase {
public:
    GameCalls*    Game;                                     // +0x40
    void*         Owner;                                    // +0x44
    unsigned char _pad48[0x4d - 0x48];
    bool          BecomeNunnery;                            // +0x4d
    unsigned char _pad4e[0x54 - 0x4e];
    long          ClientsAlive;                             // +0x54
    void CreateCustomers();
};
} // namespace NScript
using NScript::CV_BordelloScript;

void NScript::CV_BordelloScript::CreateCustomers()
{
    CTimer timer;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (Game->IsRegionLoaded("Bordello")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (timer.GetTime() == 0 && ClientsAlive < 3) {
                if (IsActiveThreadTerminating())
                    return;
                timer.SetTime(15);
                if (BecomeNunnery) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreateCreature("CREATURE_BS_VILLAGER_FEMALE", Game->GetThingWithScriptName("M_ClientSpawn").GetPos(), "BordelloClient", false);
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreateCreature("CREATURE_BS_VILLAGER_MALE", Game->GetThingWithScriptName("M_ClientSpawn").GetPos(), "BordelloClient", false);
                }
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}