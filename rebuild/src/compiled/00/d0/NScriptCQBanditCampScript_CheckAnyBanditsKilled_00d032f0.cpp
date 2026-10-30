// CQ_BanditCampScript::CheckAnyBanditsKilled() -- wait for the camp path to load, then each frame count the hero's bandit kills; the first kill fails the "kill no bandits" boast and reaching the def's quota completes "kill many bandits"
// Leaf: ?CheckAnyBanditsKilled@CQ_BanditCampScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D032F0, length 645 bytes (padded to 656).
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// _ftol 0x00BFEA70; _STLP_alloc_proxy<unsigned long*> ctor 0x004502EB; vector<unsigned long>::erase(first, last) 0x0046496F;
// ~vector<unsigned long> 0x0045151C; CScriptThing::MsgGetThingsKilled (CScriptThing vtable 0x01238C8C slot 55, +0xdc);
// CGameScriptInterface slots 7 NewScriptFrame, 12 IsRegionLoaded, 70 GetHero, 327 AddQuestInfoCounter, 335 UpdateQuestInfoCounter.
// Layout: script = Ego_r.pdb - 0x14 (BoastBegun +0x4a); Data = CQ_SunnyvaleMasterData (no shift: BanditCampKillNoBandits
// +0x115, BanditCampKillManyBandits +0x11a); pGlobalScriptDef (0x0143E90C) BAC_BoastBanditKill is PDB 0xf38, retail +0xe88.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
template <class T> struct allocator {                  // STLport allocator<T>: empty
    allocator() { }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p);    // 0x004502EB
};
template <class T> struct _Vector_base {
    T*                    _M_start;
    T*                    _M_finish;
    _STLP_alloc_proxy<T>  _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
struct ThingIDVector : _Vector_base<unsigned long> {   // std::vector<unsigned long>, 12 bytes
    ThingIDVector() : _Vector_base<unsigned long>(allocator<unsigned long>()) { }
    ~ThingIDVector();                                  // 0x0045151C
    unsigned long* begin() { return _M_start; }
    unsigned long* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned long& operator[](unsigned int i) { return *(_M_start + i); }
    unsigned long* erase(unsigned long* first, unsigned long* last);   // 0x0046496F
    void clear() { erase(begin(), end()); }
};
class CScriptThing { public:                                  // vtable 0x01238C8C
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09();
    virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23(); virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47(); virtual void s48(); virtual void s49();
    virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54();
    virtual bool MsgGetThingsKilled(ThingIDVector& killed) const;   // slot 55 (+0xdc) 0x004AAEE0
    void* PImp_Data;
    void* PImp_Info;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0xe88];
    float BAC_BoastBanditKill;                         // +0xe88
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x115];
    bool BanditCampKillNoBandits;                      // +0x115
    unsigned char _pad116[4];
    bool BanditCampKillManyBandits;                    // +0x11a
};
namespace NScript {
class CQ_BanditCampScript {
public:
    unsigned char           _body[0x40];               // CQuestScriptBase / CScriptBase
    CGameScriptInterface*   Game;                      // +0x40
    CQ_SunnyvaleMasterData* Data;                      // +0x44
    bool AttackedOuterGateGuards;                      // +0x48
    bool Gate1Open;                                    // +0x49
    bool BoastBegun;                                   // +0x4a
    bool IsActiveThreadTerminating();                  // 0x00CB7940
    void CheckAnyBanditsKilled();
};
} // namespace NScript
using NScript::CQ_BanditCampScript;

void NScript::CQ_BanditCampScript::CheckAnyBanditsKilled()
{
    long killedCount = 0;
    if (!BoastBegun) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
    }
    BoastBegun = true;
    long counter = -1;
    if (Data->BanditCampKillManyBandits) {
        if (IsActiveThreadTerminating()) return;
        counter = Game->AddQuestInfoCounter("HUD_QUEST_ICON_BANDIT", (long)pGlobalScriptDef->BAC_BoastBanditKill, 1.0f);
    }
    if (IsActiveThreadTerminating()) return;
    do {
        {
        ThingIDVector killed;
        if (Game->GetHero().MsgGetThingsKilled(killed)) {
            if (IsActiveThreadTerminating()) return;
            for (unsigned int i = 0; i < killed.size(); ++i) {
                if (killed[i] & 4) {
                    if (IsActiveThreadTerminating()) return;
                    ++killedCount;
                }
            }
            killed.clear();
        }
        if (Data->BanditCampKillNoBandits && killedCount != 0) {
            if (IsActiveThreadTerminating()) return;
            Data->BanditCampKillNoBandits = false;
        }
        if (!Data->BanditCampKillManyBandits) {
            float killedF = (float)killedCount;
            if (killedF >= pGlobalScriptDef->BAC_BoastBanditKill) {
                if (IsActiveThreadTerminating()) return;
                Data->BanditCampKillManyBandits = true;
                Game->UpdateQuestInfoCounter(counter, (long)(pGlobalScriptDef->BAC_BoastBanditKill - killedF), -1);
            }
        }
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}