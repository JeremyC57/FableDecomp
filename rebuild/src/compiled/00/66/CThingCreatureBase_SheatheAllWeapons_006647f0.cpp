// CThingCreatureBase::SheatheAllWeapons() -- for an AI creature carrying a weapon: collect the carrying component's weapon things and kill each live one that is no longer held
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006647F0, length 215 bytes, `ret`.
// Callees: CThingCreatureBase::IsPlayer (vtable slot 77, +0x134), IsThingCarryingWeapon(const CThing*) @ 0x006A1540
//   (`__fastcall`), CInterfaceMap::LowerBound @ 0x0040F020 (TC 0x46 = CTCCarrying), the carrying component's weapon
//   collector @ 0x006A22D0 (vector<CThing*>*, bool; manifest "CTCCarrying_RemoveAllWeapons", name unproven),
//   CTCCarrying::IsCarryingThing(CThing*) @ 0x006A1060, CThing::Kill(bool) @ 0x004C9B80, free @ 0x00BFEA14.
// Layout: retail CThing interface bitset +0x20 (TC 0x46 = word +0x28 bit 6), interface map +0x44, Dead +0x91 bit 0.
// Shape: HasTC is the raw bitset test in the `if`; GetTC is the inline lower-bound lookup (`type < it->type` picks
//   the end) with the key in the first frame word and the component in a register; the vector<CThing*> is a local
//   STLport vector (three zero words, `free` of the storage at the end), walked with its end re-read every pass;
//   IS_THING_ALIVE is inline.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

extern "C" void __cdecl free(void* block);      // 0x00BFEA14

class CThing;
struct CThingVector {                           // STLport vector<CThing*> (12 bytes)
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) free(_M_start); }
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
};

struct CTCCarrying {
    void GetWeapons_006a22d0(CThingVector* things, bool flag);   // 0x006A22D0
    bool IsCarryingThing(CThing* thing);                        // 0x006A1060
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5) FILL10(6)
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76();
    virtual bool IsPlayer() const;              // slot 77 (+0x134)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    void Kill(bool immediately);                // 0x004C9B80
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

bool __fastcall IsThingCarryingWeapon(const CThing* thing);    // 0x006A1540
inline bool IS_THING_ALIVE(CThing* thing) { return thing && !thing->Dead; }

class CThingCreatureBase : public CThing {
public:
    void SheatheAllWeapons();
};

void CThingCreatureBase::SheatheAllWeapons()
{
    if (IsPlayer())
        return;
    if (!IsThingCarryingWeapon(this))
        return;
    if (!HasTC(0x46))
        return;
    CTCCarrying* carrying = GetTC<CTCCarrying>(0x46);
    CThingVector weapons;
    carrying->GetWeapons_006a22d0(&weapons, false);
    for (CThing** it = weapons.begin(); it != weapons.end(); ++it) {
        CThing* weapon = *it;
        if (IS_THING_ALIVE(weapon) && !carrying->IsCarryingThing(weapon))
            weapon->Kill(true);
    }
}