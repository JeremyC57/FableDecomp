// CV_BordelloScript::Init() -- reset the bordello quest state: all hero/bordello flags, the client count and slots, the village handle (to an empty script thing) and the beer state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E399D0, length 195 bytes (208 with padding); vtable slot at 0x012E38BC.
// Callees: CBase::CBase @ 0x0099A2D0 / CBase::~CBase @ 0x0099A2E0 (the temporary's base), the count block's destroy
// hook (`call [block+4]`, object [block+8] in ecx), operator delete @ 0x00BFE9BC.
// Layout (retail = Ego_r.pdb CV_BordelloScript - 0x14): HeroPartying +0x48 .. HeroFoundDeedsLocation +0x50,
// ClientsAlive +0x54, ClientInUse[3] +0x58, BordelloVillage (CScriptThing, vtable 0x01238C8C) +0x5c,
// WearingWhoreWig +0x68, HadSex +0x69, BeersDrunk +0x6c, BeerSetToDPad +0x70.
// Shape: the stores follow the source order (HadSex first, WearingWhoreWig between ClientsAlive and the slots);
// `BordelloVillage = CScriptThing();` is the landed CV_FisticuffsClubScript (0x00E82010) temporary + inline counted
// operator= + inline destructor model.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
namespace N_0099a2d0 { struct CBase {
    virtual ~CBase();                              // 0x0099A2E0
    CBase();                                       // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct SScriptThingInfo {                          // the counted block behind CScriptThing::PImp_Info
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};

struct CScriptThing : CBase {                      // vtable 0x01238C8C, 12 bytes
    void* PImp_Data;                               // +0x04
    SScriptThingInfo* PImp_Info;                   // +0x08
    CScriptThing() : PImp_Data(0), PImp_Info(0) { }
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    CScriptThing& operator=(const CScriptThing& other)
    {
        SScriptThingInfo* info = other.PImp_Info;
        void* data = other.PImp_Data;
        if (PImp_Info != info) {
            if (PImp_Info) {
                --PImp_Info->m_references;
                if (PImp_Info->m_references == 0) {
                    PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                    delete PImp_Info;
                }
            }
            PImp_Data = data;
            PImp_Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

namespace NScript {
class CV_BordelloScript {
public:
    unsigned char _base[0x48];                     // CScriptBase + interface/data pair
    bool HeroPartying;                             // +0x48
    bool HeroTricking;                             // +0x49
    bool MagicianSleeping;                         // +0x4a
    bool CutscenePlaying;                          // +0x4b
    bool PlayerOwned;                              // +0x4c
    bool BecomeNunnery;                            // +0x4d
    bool PlayerHasLeftRegion;                      // +0x4e
    bool HeroFoundDeeds;                           // +0x4f
    bool HeroFoundDeedsLocation;                   // +0x50
    long ClientsAlive;                             // +0x54
    bool ClientInUse[3];                           // +0x58
    CScriptThing BordelloVillage;                  // +0x5c
    bool WearingWhoreWig;                          // +0x68
    bool HadSex;                                   // +0x69
    long BeersDrunk;                               // +0x6c
    bool BeerSetToDPad;                            // +0x70
    void Init();
};
} // namespace NScript
using NScript::CV_BordelloScript;

void NScript::CV_BordelloScript::Init()
{
    HadSex = false;
    HeroPartying = false;
    HeroTricking = false;
    MagicianSleeping = false;
    CutscenePlaying = false;
    PlayerOwned = false;
    BecomeNunnery = false;
    PlayerHasLeftRegion = false;
    HeroFoundDeeds = false;
    HeroFoundDeedsLocation = false;
    ClientsAlive = 0;
    WearingWhoreWig = false;
    ClientInUse[0] = false;
    ClientInUse[1] = false;
    ClientInUse[2] = false;
    BordelloVillage = CScriptThing();
    BeersDrunk = 0;
    BeerSetToDPad = false;
}