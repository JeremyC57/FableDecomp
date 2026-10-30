// CQ_RansomVictimScript::MakeNPCComment(name, npc, comment) -- if the NPC is alive and the comment timer has run out, start a conversation between the NPC and the hero with the NPC's pre- or post-rescue line for this comment; for the father's-guard comment also let the other NPC (the victim when the guard is speaking, else the guard) answer with its response line when it is alive, different from the speaker and the line exists; restart the comment timer and report that a comment was made
// Leaf: ?MakeNPCComment@CQ_RansomVictimScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE3F90, 1263 bytes (int3 padded to 1264); `ret 0Ch`.
// Callees: ~CScriptThing 0x004AA840, CScriptThing copy 0x004ABE90 (and inline), CScriptThing::IsAlive 0x004AB130 (slot 75),
//   CScriptThing::GetDataString 0x004AA900, IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   operator+ 0x0099F690 (const char*, CCharString) / 0x0099F600 (CCharString, const char*) / 0x0099F570 (CCharString,
//   CCharString) (all __fastcall); impl slot 78 (+0x138) equality; g_pGameScriptInterface 0x0143E8F8 timer slots 89, 90;
//   pGlobalScriptDef 0x0143E90C (+0x50 comment interval); Game slots 70, 72, 358, 364, 365, 366.
// Layout (retail = Ego_r.pdb - 0x14): RescuedVictim +0x4b, NPCCommentTimer +0x68.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define THING_INLINE_COPY
#define VEC_DTOR
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class C3DVector { public: float X, Y, Z; };
struct CScriptThingImpl {
    virtual void i000();
    virtual void i001();
    virtual void i002();
    virtual void i003();
    virtual void i004();
    virtual void i005();
    virtual const C3DVector& GetPos() const;            // imp slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069();
    virtual void SetToKillOnLevelUnload(bool kill);     // imp slot 70 (+0x118)
    virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074(); virtual void i075(); virtual void i076(); virtual void i077();
    virtual bool IsEqualTo(const CScriptThingImpl* other) const;   // imp slot 78 (+0x138)
};
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual CCharString GetDataString() const;         // slot 3 (+0x0c) 0x004AA900
    virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const             // slot 6 (+0x18) 0x004AA980
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void SetToKillOnLevelUnload(bool kill)      // slot 70 (+0x118) 0x004AB0C0
    {
        if (PImp.Data)
            PImp.Data->SetToKillOnLevelUnload(kill);
    }
    virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04 Data, +0x08 Info
#ifdef THING_INLINE_COPY
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
#else
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
#endif
    CScriptThing() {}
    __forceinline bool operator==(const CScriptThing& other) const { return PImp.Data->IsEqualTo(other.PImp.Data); }
    __forceinline bool operator!=(const CScriptThing& other) const { return !PImp.Data->IsEqualTo(other.PImp.Data); }
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    ~CScriptThing() {}
};
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
#ifndef VEC_DTOR
#define VEC_DTOR __forceinline
#endif
    VEC_DTOR ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:
    long Index;
    __forceinline ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // 0x00CD4470
    void Set(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long Get() const { return g_pGameScriptInterface->GetTimer(Index); }
};
class CScriptDef { public:
    unsigned char _pad[0x50];
    long NPCCommentInterval;                            // +0x50
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
enum ENPCComment { NPC_COMMENT_VICTIM, NPC_COMMENT_GUARD };
namespace NScript {
class CQ_RansomVictimScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    bool QuestStartScreened;                            // +0x48
    bool MissionSucceeded;                              // +0x49
    bool MissionFailed;                                 // +0x4a
    bool RescuedVictim;                                 // +0x4b
    unsigned char _pad4c[0x68 - 0x4c];
    CTimer NPCCommentTimer;                             // +0x68
    bool MakeNPCComment(const CCharString& name, const CScriptThing& npc, ENPCComment comment);
};
} // namespace NScript
using NScript::CQ_RansomVictimScript;

bool NScript::CQ_RansomVictimScript::MakeNPCComment(const CCharString& name, const CScriptThing& npc, ENPCComment comment)
{
    CScriptThing speaker = npc;
    const CScriptThing& thing = speaker;
    if (!thing.IsAlive()) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    if (NPCCommentTimer.Get() > 0) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    long conversation = Game->AddNewConversation(speaker, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    if (RescuedVictim) {
        if (IsActiveThreadTerminating())
            return false;
        Game->AddLineToConversation(conversation, "TEXT_QST_B06_POST_RESC_" + speaker.GetDataString() + "_" + name, false, speaker, Game->GetHero());
    } else {
        if (IsActiveThreadTerminating())
            return false;
        Game->AddLineToConversation(conversation, "TEXT_QST_B06_PRE_RESC_" + speaker.GetDataString() + "_" + name, false, speaker, Game->GetHero());
    }
    if (comment == NPC_COMMENT_GUARD) {
        if (IsActiveThreadTerminating())
            return false;
        CScriptThing guard = Game->GetThingWithScriptName("VictimsFatherGuard");
        CScriptThing other = (guard.IsAlive() && speaker == guard) ? Game->GetThingWithScriptName("Victim") : Game->GetThingWithScriptName("VictimsFatherGuard");
        if (other.IsAlive() && other != speaker && Game->TextEntryExists("TEXT_QST_B06_PRE_RESC_" + other.GetDataString() + "_" + name + "_RESPONSE")) {
            if (IsActiveThreadTerminating())
                return false;
            Game->AddPersonToConversation(conversation, other);
            Game->AddLineToConversation(conversation, "TEXT_QST_B06_PRE_RESC_" + other.GetDataString() + "_" + name + "_RESPONSE", false, other, Game->GetHero());
        }
    }
    NPCCommentTimer.Set(pGlobalScriptDef->NPCCommentInterval);
    return true;
}