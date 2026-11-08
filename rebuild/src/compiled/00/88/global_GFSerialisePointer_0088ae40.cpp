// GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer<CThing>& pointer, unsigned long level) -- persist a thing pointer as its UID: saving writes the live thing's UID remapped for the save (0 for none or dead); loading reads it back and, when non-zero, points at a fresh THING_DUMMY_FOR_SERIALISATION placeholder carrying the remapped UID
// Leaf: ?GFSerialisePointer@@YIXAAVCPersistContext@@PBDAAV?$CIntelligentPointer@VCThing@@@@K@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088AE40, 431 bytes of code (+ int3 padding), /Gr `__fastcall`
//   (context in ecx, name in edx, pointer and level on the stack), `ret 8`.
// Identity: Ego_r/FableWin PDB `?GFSerialisePointer@@YIXAAVCPersistContext@@PBDAAV?$CIntelligentPointer@VCThing@@@@K@Z`
//   (the catalog label agrees; the landed CTCDNone::Create 0x00835A20 lists it as a caller).
// Callees (PDB spellings): CBaseIntelligentPointer::GetPItem() const 0x00A01B50,
//   CThingManager::RemapUIDForSerialisation(bool, bool, unsigned long, unsigned __int64) const 0x0051E210 (edx:eax result),
//   CPersistContext::Transfer<unsigned __int64>(const char*, unsigned __int64&, const unsigned __int64&) 0x0044BB00,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const 0x009AD410,
//   CTCDNone::Create(long, const C3DVector&, bool) 0x00835A20 (static __fastcall),
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound 0x0040F020 (inside CThing::GetTC<T>),
//   CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10.
// Globals: g_pThingManager 0x013B8A1C (descriptive, as the landed CTCBase::PeekWorld 0x00686A80), the
//   CIntelligentPointer<CThing> vtable 0x01238C6C, "THING_DUMMY_FOR_SERIALISATION" 0x0125E9EC.
// Layout: retail CThing UniqueID +0x18, UsingComponents bitset +0x20 (0xEB = CTCThingUIDDummyForSerialisation: word 7
//   bit 11), Components +0x44, Dead bit 0 of +0x91; CTCThingUIDDummyForSerialisation ThingUID +0x10 (PDB);
//   CPersistContext Mode +0x18 (PDB; 1 or 3 = saving); CThingManager World +0x24 (retail, see 0x00686A80);
//   CWorld DefinitionManager +0x10 (PDB).
// Shape: each arm its own Transfer with a `0` default literal; save: alive test on one GetPItem, the UID copied through
//   the inline GetUniqueID() then remapped in place; load: `uid != 0`, remap, the placeholder created inside a block
//   holding the def-name string with the definition manager reached through the PDB's inline const accessors
//   CThingManager::PeekWorld() and CWorld::PeekDefinitionManager() (the plain member chain rotates eax/edx/ecx one step
//   off retail), the dummy component through GetTC<T>(T**) into a zeroed pointer, and
//   `pointer = CIntelligentPointer<CThing>(thing)` (the temporary's implicit destructor calls the base one directly).
#include "fable_compiler.h"
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
private:
    void* PData;                                            // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return static_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    T* operator->() const { return GetPItem(); }
};

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

#include "engine/C3DVector.h"

class CTCBase;
enum ETCInterfaceType { TCI_THING_UID_DUMMY_FOR_SERIALISATION = 0xEB };   // descriptive enumerator name
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCThingUIDDummyForSerialisation {                    // interface type 0xEB
public:
    unsigned char    _pad00[0x10];
    unsigned __int64 ThingUID;                              // +0x10
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_THING_UID_DUMMY_FOR_SERIALISATION; }
};

class CThing : public CBaseObject {
public:
    unsigned char    _pad04[0x18 - 0x04];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned long    UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap    Components;                            // +0x44
    unsigned char    _pad54[0x3d];
    unsigned char    Dead : 1;                              // +0x91 bit 0
    unsigned char    CreatedByOwnerThingCreation : 1;

    unsigned __int64 GetUniqueID() const { return UniqueID; }   // Ego_r ?GetUniqueID@CThing@@QBE_KXZ
    template <class T> FABLE_FORCEINLINE bool GetTC(T** out)        // Ego_r ??$GetTC@VCTCThingUIDDummyForSerialisation@@@CThing@@QAE_NPAPAV...
    {
        long type = T::StatGetInterfaceType();
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
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
    const CGameDefinitionManager& PeekDefinitionManager() const { return *DefinitionManager; }   // Ego_r ?PeekDefinitionManager@CWorld@@QBEABVCGameDefinitionManager@@XZ
};

class CThingManager {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                                    // +0x24 (retail)
    const CWorld& PeekWorld() const { return *World; }      // Ego_r ?PeekWorld@CThingManager@@QBEABVCWorld@@XZ
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool level_less, unsigned long level, unsigned __int64 uid) const;   // 0x0051E210
};
extern CThingManager* g_pThingManager;                      // 0x013B8A1C

class CPersistContext {
public:
    enum EMode { MODE_SAVE = 1, MODE_SAVE_LEVEL = 3 };      // descriptive enumerator names
    unsigned char _pad00[0x18];
    EMode         Mode;                                     // +0x18
    bool IsSaving() const { return Mode == MODE_SAVE || Mode == MODE_SAVE_LEVEL; }
    template <class T> void Transfer(const char* name, T& value, const T& default_value);   // 0x0044BB00 for T = unsigned __int64
};

void __fastcall GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer<CThing>& pointer, unsigned long level);

void __fastcall GFSerialisePointer(CPersistContext& context, const char* name, CIntelligentPointer<CThing>& pointer, unsigned long level)
{
    if (context.IsSaving()) {
        unsigned __int64 uid = 0;
        CThing* thing = pointer.GetPItem();
        if (thing != 0 && !thing->Dead) {
            uid = pointer->GetUniqueID();
            uid = g_pThingManager->RemapUIDForSerialisation(true, false, level, uid);
        }
        context.Transfer<unsigned __int64>(name, uid, 0);
    }
    else {
        unsigned __int64 uid = 0;
        context.Transfer<unsigned __int64>(name, uid, 0);
        if (uid != 0) {
            uid = g_pThingManager->RemapUIDForSerialisation(false, false, level, uid);
            CThing* thing;
            {
                CCharString def_name("THING_DUMMY_FOR_SERIALISATION");
                thing = CTCDNone::Create(g_pThingManager->PeekWorld().PeekDefinitionManager().GetDefGlobalIndexFromName(def_name),
                                         C3DVector(0.0f, 0.0f, 0.0f), true);
            }
            CTCThingUIDDummyForSerialisation* dummy = 0;
            thing->GetTC(&dummy);
            dummy->ThingUID = uid;
            pointer = CIntelligentPointer<CThing>(thing);
        }
    }
}