// GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer<CThing>& pointer, unsigned long level) -- persist a thing pointer as its UID: saving writes the live thing's UID remapped for the save (0 for none or dead); loading reads it back and, when non-zero, points at a fresh THING_DUMMY_FOR_SERIALISATION placeholder carrying the remapped UID
// Grade: DIFFER(433v431) -- control flow, calls, Transfer/Remap/Create/GetTC/intelligent-pointer shape match; register allocation differs throughout (save arm: retail keeps the thing in eax and loads `level` after the UID pushes, ours hoists `level` into edx and moves the thing to ecx; the Transfer `lea` temporaries and the World->DefinitionManager chain rotate through other registers)
// Leaf: ?GFSerialisePointer@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088AE40, 432 bytes incl. int3 padding (431 of code), /Gr `__fastcall`
//   (context in ecx, name in edx, pointer and level on the stack), `ret 8`.
// Identity: FableWin/Ego_r PDB `?GFSerialisePointer@@YIXAAVCPersistContext@@PBDAAV?$CIntelligentPointer@VCThing@@@@K@Z`
//   (the catalog label agrees; the landed CTCDNone::Create 0x00835A20 lists it as a caller).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CThingManager::RemapUIDForSerialisation(bool, bool,
//   long, unsigned __int64) 0x0051E210 (edx:eax), CPersistContext::Transfer(const char*, unsigned __int64&,
//   const unsigned __int64&) 0x0044BB00, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410, CTCDNone::Create(long,
//   const C3DVector&, bool) 0x00835A20 (static __fastcall), CVectorMap LowerBound 0x0040F020,
//   CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10.
// Globals: g_pThingManager 0x013B8A1C (retail CThingManager World +0x24; CWorld DefinitionManager +0x10, PDB), the
//   CIntelligentPointer<CThing> vtable 0x01238C6C, "THING_DUMMY_FOR_SERIALISATION" 0x0125E9EC.
// Layout: retail CThing UID +0x18, interface bitset +0x20 (0xEB = CTCThingUIDDummyForSerialisation: word 7 bit 11),
//   interface map +0x44, Dead bit 0 of +0x91; CTCThingUIDDummyForSerialisation ThingUID +0x10; CPersistContext mode
//   +0x18 (1 or 3 = saving).
// Shape: as the landed CTCActivationTrigger::OnSerialise -- each arm its own Transfer with a `0` default literal; save:
//   alive test on one GetPItem, the UID copied then remapped in place; load: `uid != 0`, remap, the placeholder created
//   inside a block holding the def-name string, the dummy component through GetTC<T>(T**) into a zeroed pointer, and
//   `pointer = CIntelligentPointer<CThing>(thing)` (the temporary's implicit destructor calls the base one directly).
class CBaseObject {
public:
    virtual ~CBaseObject();
};

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    void* m_pData;                                          // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* Get() const { return static_cast<T*>(GetPItem()); }
    T* operator->() const { return Get(); }
};

class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                          // +0x00
    SEntry* last;                                           // +0x04
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CTCThingUIDDummyForSerialisation {                    // interface type 0xEB
public:
    unsigned char    _pad00[0x10];
    unsigned __int64 ThingUID;                              // +0x10
    static long StatGetInterfaceType() { return 0xEB; }
    void SetThingUID(unsigned __int64 uid) { ThingUID = uid; }
};

class CThing : public CBaseObject {
public:
    unsigned char    _pad04[0x18 - 0x04];
    unsigned __int64 UID;                                   // +0x18
    unsigned long    UsingComponents[9];                    // +0x20 bitset
    CInterfaceMap    Components;                            // +0x44
    unsigned char    _pad4c[0x91 - 0x4c];
    unsigned char    Dead : 1;                              // +0x91 bit 0
    unsigned char    CreatedByOwnerThingCreation : 1;

    template <class T> __forceinline bool GetTC(T** out)
    {
        long type = T::StatGetInterfaceType();
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCDNone {
public:
    static CThing* __fastcall Create(long def_index, const C3DVector& position, bool map_zero);   // 0x00835A20
};

class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;  // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { };

class CWorld {
public:
    unsigned char           _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;              // +0x10
};

class CThingManager {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                                    // +0x24
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool level_less, long level, unsigned __int64 uid);  // 0x0051E210
};
extern CThingManager* g_pThingManager;                      // 0x013B8A1C

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long          Mode;                                     // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& default_value);   // 0x0044BB00
};

void __fastcall GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer<CThing>& pointer, unsigned long level);

void __fastcall GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer<CThing>& pointer, unsigned long level)
{
    if (context.IsSaving()) {
        unsigned __int64 uid = 0;
        CThing* thing = pointer.Get();
        if (thing != 0 && !thing->Dead) {
            uid = pointer->UID;
            uid = g_pThingManager->RemapUIDForSerialisation(true, false, level, uid);
        }
        context.Transfer(name, uid, 0);
    }
    else {
        unsigned __int64 uid = 0;
        context.Transfer(name, uid, 0);
        if (uid != 0) {
            uid = g_pThingManager->RemapUIDForSerialisation(false, false, level, uid);
            CThing* thing;
            {
                CCharString def_name("THING_DUMMY_FOR_SERIALISATION");
                thing = CTCDNone::Create(g_pThingManager->World->DefinitionManager->GetDefGlobalIndexFromName(def_name),
                                         C3DVector(0.0f, 0.0f, 0.0f), true);
            }
            CTCThingUIDDummyForSerialisation* dummy = 0;
            thing->GetTC(&dummy);
            dummy->SetThingUID(uid);
            pointer = CIntelligentPointer<CThing>(thing);
        }
    }
}
