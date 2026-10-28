// CThingManager::SaveMainPlayerCreatureState(CCharString& script) -- collect the hero plus every persisted inventory item the hero owns on the hero's map and write them as a thing list
// Leaf: SaveMainPlayerCreatureState
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, vector<CThing*> grow @ 0x00436AC0,
// CThing::IsGameStatePersisted @ 0x004C8280, CThing::IsLevelLess @ 0x004C7930, CThing::SetCurrentMapNumber @ 0x004C7580,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (non-const copy), CTCInventoryItem::GetInventoryOwner @ 0x005D8E90, thing-list writer @ 0x00521EB0,
// ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// Shape: both vectors stay in the frame (the grow template's same-TU body keeps the vector's address; the writer's
// same-TU stand-in fills the string vector's first/last and reads its end, so the vector's words are not reloaded around
// the ~CCharString loop); push_back keeps `last` in a local; the hero is a CThingCreature* upcast into a CThing* temporary
// (the separate [esp+0x10] slot), the loop thing is pushed through an rvalue cast (its own [esp+0x1c] slot).
// Layout (retail CThingManager): Things list sentinel +0x18, PlayerManager +0x30, Saving flag +0x9a;
// CThing UsingComponents +0x20, Components +0x44, CurrentMapNumber +0x8e, flags byte +0x91.
#include <new>

class CCharString { public:
    void* m_pData;
    ~CCharString();                                            // 0x0099EAE0
};
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

template <class T> struct CRetailVector {
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
    ~CRetailVector()
    {
        for (T* p = first; p != last; ++p)
            p->~T();
        if (first) free(first);
    }
};
template <class T> __forceinline void Construct(T* p, const T& value) { new (p) T(value); }
template <class T> struct CRetailPtrVector {
    T* first;
    T* last;
    T* end;
    CRetailPtrVector() : first(0), last(0), end(0) { }
    ~CRetailPtrVector() { if (first) free(first); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(const T& value)
    {
        T* finish = last;
        if (finish != end) {
            Construct(finish, value);
            last = finish + 1;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

void* GrowVectorStorage_00436ac0(void* vec, unsigned long count);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void CRetailPtrVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_00436ac0(this, count);
    if (p)
        *p = value;
}
#pragma auto_inline(on)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x8e - 0x4c];
    short CurrentMapNumber;                             // +0x8e
    unsigned char _pad90;
    unsigned char Flags91;                              // +0x91
    bool IsGameStatePersisted();                        // 0x004C8280
    bool IsLevelLess();                                 // 0x004C7930
    void SetCurrentMapNumber(long map);                 // 0x004C7580
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CTCInventoryItem { public:
    CThing* GetInventoryOwner();                        // 0x005D8E90
};
class CThingCreature : public CThing { };
class CThingPlayerCreature : public CThingCreature { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };    // 0x00449970

struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing*         _M_data;
};

class CThingManager {
public:
    void*           __vftable;                          // +0x00
    unsigned char   _pad04[0x18 - 0x04];
    SThingListNode* Things;                             // +0x18
    unsigned char   _pad1c[0x30 - 0x1c];
    CPlayerManager* PlayerManager;                      // +0x30
    unsigned char   _pad34[0x9a - 0x34];
    bool            Saving;                             // +0x9a
    void SaveThingList(CCharString& script, long mode, CRetailPtrVector<CThing*>& things, CRetailVector<CCharString>* names);   // 0x00521EB0
    void SaveMainPlayerCreatureState(CCharString& script);
};


CCharString* WriteThings(CCharString& script, long mode, CThing** first, CThing** last, CCharString* capacity, unsigned long* count);
#pragma auto_inline(off)
__declspec(noinline) void CThingManager::SaveThingList(CCharString& script, long mode, CRetailPtrVector<CThing*>& things, CRetailVector<CCharString>* names)
{
    unsigned long count;
    CCharString* written = WriteThings(script, mode, things.first, things.last, names->end, &count);
    names->first = written;
    names->last = written + count;
}
#pragma auto_inline(on)

void CThingManager::SaveMainPlayerCreatureState(CCharString& script)
{
    Saving = true;
    CThingCreature* hero = PlayerManager->GetMainPlayer()->GetPControlledCreature();
    long map = hero->CurrentMapNumber;
    CRetailPtrVector<CThing*> things;
    things.push_back(hero);
    for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (thing->IsGameStatePersisted()) {
            if (thing->IsLevelLess() || (thing->Flags91 & 0x20))
                thing->SetCurrentMapNumber(map);
            if (thing->CurrentMapNumber == map && thing->HasTC(0x1a)
                && ((CTCInventoryItem*)thing->GetTC(0x1a))->GetInventoryOwner() == hero)
                things.push_back((CThing*)thing);
        }
    }
    CRetailVector<CCharString> names;
    Saving = true;
    SaveThingList(script, 2, things, &names);
    Saving = false;
}