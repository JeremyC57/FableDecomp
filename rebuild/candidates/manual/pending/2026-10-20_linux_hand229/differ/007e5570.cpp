// CTCBetting::GetPlayersMoney() -- the hero's money (less the stake already placed) as "<money text><amount>" from the betting def, or an empty string when betting input is off or the def is missing
// Grade: DIFFER(292v262) -- frame packing: retail's 0x0c frame puts the def pointer in the hero lookup's key word
//   (def zeroed through edi, money in esi), ours gives the def its own word (immediate zero store, money in edi) and
//   the register choice of every later use follows
// Leaf: GetPlayersMoney
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5570, 260 bytes (+12 int3 padding), `ret 4` (hidden CWideString result).
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960, CPlayer::GetHeroThing 0x00487DC0, CVectorMap<
//   ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined lookup of CTCHeroStats, type 4), CDefClassBase::
//   GetSubDef<CBettingDef> 0x007E5820 (manifest template argument propagated), IntToWideString(long) 0x00405F00
//   (__fastcall, hidden result in ecx), CTCBetting::GetText(unsigned long) 0x007E4D70, operator+(const CWideString&,
//   const CWideString&) 0x0099BE70 (__fastcall), ~CWideString 0x0099B510, CWideString(const wchar_t*) 0x0099B6B0
//   (L"" 0x0122D70C); the def's virtual Destroy (slot 1) on release.
// Globals: g_pMainGameComponent 0x013B86A0 (+0x1c PlayerManager), g_bBettingInputEnabled 0x013BAE1D (landed name),
//   the stake 0x013BAE20 (long) and its placed flag 0x013BAE24 (bool) (descriptive names).
// Layout: CTCBase ParentThing +4 (base def +0x70); CTCHeroStats::Money retail +0x3C; the betting def's money text id
//   +0x48 (descriptive); a def's reference count +4.
// Shape: the hero's money comes from an inline helper (`T* p = 0; if (HasTC) p = GetTC;`: the preloaded `xor esi,esi`;
//   its lookup key's frame word is free again for the def pointer declared after it); the def pointer is a zeroed local released
//   by its inline destructor (`if (Object) Object->Release()`, `--RefCount == 0` then the virtual Destroy; the null test
//   folds away on the path that read the def); the result is `GetText(id) + IntToWideString(money)` (right operand built
//   first, its pointer pre-pushed for the __fastcall operator+).
// Tried: the lookup inline in the body, a static inline / `__forceinline` GetHeroMoney() helper, a named result in the
//   success block: the key and the def never share a word (same family as 0x006C0160).
#include <stddef.h>
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CTCHeroStats {                           // interface 4
    unsigned char _pad00[0x3c];
    long Money;                                 // +0x3c
};
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern bool g_bBettingInputEnabled;             // 0x013BAE1D
extern long g_bettingStake;                     // 0x013BAE20
extern bool g_bBettingStakePlaced;              // 0x013BAE24

struct CWideString {
    void* PStringData;
    CWideString(const wchar_t* text);           // 0x0099B6B0
    ~CWideString();                             // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall IntToWideString(long value);                                   // 0x00405F00

struct CBettingDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x48 - 0x08];
    unsigned long MoneyTextID;                  // +0x48
    __forceinline void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CBettingDef>* out);   // 0x007E5820
};
struct CThingWithDef {
    unsigned char  _pad00[0x70];
    CDefClassBase* PBaseDef;                    // +0x70
};

class CTCBetting {
public:
    void*          __vftable;                   // +0x00 (CTCBase)
    CThingWithDef* ParentThing;                 // +0x04
    CWideString GetText(unsigned long textId);  // 0x007E4D70
    static __forceinline long GetHeroMoney()
    {
        CTCHeroStats* stats = 0;
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        if (hero->HasTC(4))
            stats = hero->GetTC<CTCHeroStats>(4);
        return stats->Money;
    }
    CWideString GetPlayersMoney();
};

CWideString CTCBetting::GetPlayersMoney()
{
    long money = GetHeroMoney();
    CDefPointer<CBettingDef> def;
    if (g_bBettingStakePlaced)
        money -= g_bettingStake;
    if (g_bBettingInputEnabled) {
        if (ParentThing->PBaseDef->GetSubDef(&def))
            return GetText(def->MoneyTextID) + IntToWideString(money);
    }
    return CWideString(L"");
}
