// CTCCreatureOpinionOfHero::GetHeadPendingDeed() -- walk the pending-deed queue: drop deeds the hero's log no longer has; a
// Header-moved (retry wave 2 sec2c): shared def model (engine/CDefClassBase.h + CDefPointer verify text with the PENDING ENullDef ctor); DIFFER(876v902)/103, residue unchanged by the move
// Grade: DIFFER(876v902) -- structure right; deed/log esi<->edi swap, ebx/esi pushes shrink-wrapped (retail pushes all 4 up front), CDefPointer release copy form (dec edi; mov edx,edi)
// deed that is still recent, already seen, or ignorable while the creature is busy moves to the known deeds; a crime this
// creature's village has not handled is moved too (when a guard is on the level); the first deed left is returned
// Leaf: GetHeadPendingDeed
//
// Retail 0x00683770, 902 bytes, plain `ret`.
// Callees: CTCBase::GetPlayerManager @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature
// @ 0x00487DC0, CVectorMap::LowerBound const @ 0x004365B0 (0x51 deed log, 0x31 mode manager, 0x23 villager), CTCBase::GetWorld
// @ 0x00686A70, GetWorldFrame @ 0x0049D870 (static), CTCHeroOpinionDeedLog::GetDeedFromHandle @ 0x006C58F0 / the effects-def
// getter @ 0x006C5630 (CDefPointer by hidden pointer), operator new @ 0x00BFEA0E, free @ 0x00BFEA14, CTCCreatureModeManager::
// GetCurrentMode @ 0x0073A6E0 (mode slot 1: type), GFGetPVillageInterface @ 0x007EE540 (__fastcall), IsGuardOnLevel
// @ 0x006836D0, CTCVillage::IsCrimeHandled @ 0x0065914B, list<long>::push_front @ 0x00655ECE / pop_front @ 0x00655A2E.
// Global: the frames-per-second long @ 0x01375550.
// Layout: as the landed InformOfDeed / DecriminaliseAllKnownCrimes: LastOpinionReactionFrame +0x0c, PendingDeeds +0x54,
// KnownDeeds list<long>[92] +0x1c8; COpinionDeed type +0, handle +4, thing +0x18, flags +0x20/+0x21, IsCrime +0x23;
// effects def duration +0x48, flags +0x7d.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;              // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterfaceConst(T** ppOut, long interfaceType) const
    {
        if (HasTC(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CPlayer { CThing* GetHeroCreature(); };               // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };         // 0x00449970
struct CWorld { static long __fastcall GetFrame(); };   // 0x0049D870
extern long g_framesPerSecond;                               // 0x01375550
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class COpinionDeedEffectsDef : public CDefClassBase {
public:
    unsigned char _pad25[0x48 - 0x25];
    float         Duration;                                  // +0x48 (seconds)
    unsigned char _pad4c[0x7d - 0x4c];
    unsigned char Flags;                                     // +0x7d bit 1: ignored while busy
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct COpinionDeed {
    long          Type;                                      // +0x00
    long          Handle;                                    // +0x04
    unsigned char _pad08[0x18 - 0x08];
    CBaseIntelligentPointer PThing;                          // +0x18
    bool          Seen;                                      // +0x20
    bool          Reacted;                                   // +0x21
    bool          Decriminalised;                            // +0x22
    bool          IsCrime;                                   // +0x23
};
struct CTCHeroOpinionDeedLog {
    COpinionDeed* GetDeedFromHandle(long handle);            // 0x006C58F0
    CDefPointer<const COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};
struct CMode {
    virtual void v00();
    virtual long GetType();                                  // slot 1
};
struct CTCCreatureModeManager {
    CMode* GetCurrentMode();                                 // 0x0073A6E0
    long GetCurrentModeType() { return GetCurrentMode()->GetType(); }
};
struct CTCVillager;
struct CTCVillage { bool IsCrimeHandled(long handle); };    // 0x0065914B
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);   // 0x007EE540
extern "C" void __cdecl free(void*);
inline void* operator new(unsigned int, void* where) { return where; }
struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    long       _M_data;
};
struct CLongList {
    SListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    long& front() { return _M_node->_M_next->_M_data; }
    void insert(SListNode* position, const long& value)
    {
        SListNode* node = (SListNode*)operator new(sizeof(SListNode));
        new (&node->_M_data) long(value);
        SListNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
    void push_front(const long& value);                      // 0x00655ECE
    void pop_front();                                        // 0x00655A2E
};
inline void CLongList::push_front(const long& value) { insert(_M_node->_M_next, value); }
inline void CLongList::pop_front()
{
    SListNode* node = _M_node->_M_next;
    SListNode* next = node->_M_next;
    SListNode* prev = node->_M_prev;
    prev->_M_next = next;
    next->_M_prev = prev;
    free(node);
}
struct CTCCreatureOpinionOfHero {
    void*         __vftable;                                 // +0x00
    CThing*       ParentThing;                               // +0x04
    unsigned char _pad08[0x0c - 0x08];
    long          LastOpinionReactionFrame;                  // +0x0c
    unsigned char _pad10[0x54 - 0x10];
    CLongList     PendingDeeds;                              // +0x54
    long          LastFrameAppliedDeedEffect[92];            // +0x58
    CLongList     KnownDeeds[92];                            // +0x1c8
    CPlayerManager* GetPlayerManager();                      // 0x00686D40
    CWorld* GetWorld();                                      // 0x00686A70
    bool IsGuardOnLevel();                                   // 0x006836D0
    COpinionDeed* GetHeadPendingDeed();
};

COpinionDeed* CTCCreatureOpinionOfHero::GetHeadPendingDeed()
{
    COpinionDeed* deed = 0;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero) {
    CTCHeroOpinionDeedLog* log;
    hero->GetInterfaceConst(&log, 0x51);
    long age = GetWorld()->GetFrame() - LastOpinionReactionFrame;
    do {
        if (PendingDeeds.empty())
            break;
        deed = log->GetDeedFromHandle(PendingDeeds.front());
        if (!deed) {
            PendingDeeds.pop_front();
            continue;
        }
        bool recent = (float)g_framesPerSecond * log->GetDeedEffectsDef(deed->Type)->Duration > (float)age;
        CTCCreatureModeManager* modeManager;
        bool known;
        if (recent || (deed->Seen && !deed->Reacted))
            known = true;
        else
            known = (ParentThing != deed->PThing.GetPItem() && !deed->IsCrime
                && !(log->GetDeedEffectsDef(deed->Type)->Flags & 2)
                && ParentThing->GetInterfaceConst(&modeManager, 0x31)
                && (modeManager->GetCurrentModeType() == 6 || modeManager->GetCurrentModeType() == 0x29
                    || modeManager->GetCurrentModeType() == 0x2d || modeManager->GetCurrentModeType() == 0xb
                    || modeManager->GetCurrentModeType() == 0x2b || modeManager->GetCurrentModeType() == 0x2a));
        if (known) {
            KnownDeeds[deed->Type].push_front(deed->Handle);
            PendingDeeds.pop_front();
            deed = 0;
            continue;
        }
        CTCVillager* villager;
        if (deed->IsCrime && (deed->Type == 7 || deed->Type == 8) && ParentThing->GetInterfaceConst(&villager, 0x23)
            && GFGetPVillageInterface(villager) && IsGuardOnLevel() && !ParentThing->HasTC(0x90)
            && !GFGetPVillageInterface(villager)->IsCrimeHandled(deed->Handle)) {
            KnownDeeds[deed->Type].push_front(deed->Handle);
            PendingDeeds.pop_front();
            deed = 0;
        }
    } while (!deed);
    }
    return deed;
}
