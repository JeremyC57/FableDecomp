// CThingManager::WorldUpdateThings() -- per-frame update of every thing on the per-type update lists, then the death pass
// Leaf: ?WorldUpdateThings@CThingManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051FCC0, 189 bytes, tail jump to UpdateDeaths.
// Identity: Ego_r.pdb and FableWin.pdb both carry `?WorldUpdateThings@CThingManager@@QAEXXZ`; the retail caller is the CWorld
//   frame update (0x004A5D1C, `this` = CWorld::PThingManager). Its callees are the PDB-named CThing::FrameUpdate, the virtual
//   CThing::IsFrameUpdated, CThingManager::RemoveFromUpdateList and CThingManager::UpdateDeaths (tail jump).
// Callees: CThing::FrameUpdate 0x004C9D20, CThingManager::RemoveFromUpdateList 0x0051ECC0 (PDB `AAVCThing@@`: by reference;
//   the bound spelling `PAVCThing@@` is wrong), CThingManager::UpdateDeaths 0x0051F070; virtual CThing::IsFrameUpdated (slot 44,
//   +0xB0; Ego_r.pdb `?IsFrameUpdated@CThing@@UBE_NXZ`).
// Layout: CThingManager : CClassFactory (RTTI; retail CClassFactory 0x1C, engine/CClassFactory.h): ClassTypeInfo
//   vector<CClassTypeInfo> +0x04 (retail CClassTypeInfo 0x14: TypeList +0, NoAllocated +4, Used +8, Local +9, AllocFunc +0xC,
//   Name +0x10), FirstRenderOfFrame +0x56, UpdateLists vector<ThingLList> +0x8C (Ego_r.pdb offsets less the Dinkumware
//   container sizes). CThing: UpdateListNext +0x88, the flag byte +0x91 (Dead bit 0, Activated bit 3, InLimbo bit 5; Ego_r.pdb
//   +0x98 / +0xA1, less 0x10). No engine header for CThingManager or CThing.
// Data: the three gate flags 0x01375755 / 0x0137575A / 0x01375759 and the counter 0x013B8A84 have no PDB names and no other
//   references; their names are descriptive.
// Shape: the per-type gates as an if / else-if pair (VC7.1 merges the two flag tests); the update list walked by reading the next
//   link before the thing can be unlinked; GetUpdateListFromType inline (Ego_r.pdb `?GetUpdateListFromType@CThingManager@@QAEAAUThingLList@@J@Z`).
#define CClassFactory CClassFactoryLayout
#include "engine/CClassFactory.h"
#undef CClassFactory

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    _Tp* begin() { return this->_M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
};
}

class CClass;
class CThing;
class CClassTypeInfo { public:                              // retail 0x14
    CClass*       TypeList;                                 // +0x00 list<CClass*> (STLport: the header node pointer)
    unsigned long NoAllocated;                              // +0x04
    bool          Used;                                     // +0x08
    bool          Local;                                    // +0x09
    CClass* (__cdecl* AllocFunc)();                         // +0x0C
    void*         Name;                                     // +0x10 CCharString
};
struct ThingLList {
    CThing* Head;                                           // +0x0
    CThing* Tail;                                           // +0x4
};

class CThing { public:
    virtual void CUnprovenSlot0();  virtual void CUnprovenSlot1();  virtual void CUnprovenSlot2();  virtual void CUnprovenSlot3();
    virtual void CUnprovenSlot4();  virtual void CUnprovenSlot5();  virtual void CUnprovenSlot6();  virtual void CUnprovenSlot7();
    virtual void CUnprovenSlot8();  virtual void CUnprovenSlot9();  virtual void CUnprovenSlot10(); virtual void CUnprovenSlot11();
    virtual void CUnprovenSlot12(); virtual void CUnprovenSlot13(); virtual void CUnprovenSlot14(); virtual void CUnprovenSlot15();
    virtual void CUnprovenSlot16(); virtual void CUnprovenSlot17(); virtual void CUnprovenSlot18(); virtual void CUnprovenSlot19();
    virtual void CUnprovenSlot20(); virtual void CUnprovenSlot21(); virtual void CUnprovenSlot22(); virtual void CUnprovenSlot23();
    virtual void CUnprovenSlot24(); virtual void CUnprovenSlot25(); virtual void CUnprovenSlot26(); virtual void CUnprovenSlot27();
    virtual void CUnprovenSlot28(); virtual void CUnprovenSlot29(); virtual void CUnprovenSlot30(); virtual void CUnprovenSlot31();
    virtual void CUnprovenSlot32(); virtual void CUnprovenSlot33(); virtual void CUnprovenSlot34(); virtual void CUnprovenSlot35();
    virtual void CUnprovenSlot36(); virtual void CUnprovenSlot37(); virtual void CUnprovenSlot38(); virtual void CUnprovenSlot39();
    virtual void CUnprovenSlot40(); virtual void CUnprovenSlot41(); virtual void CUnprovenSlot42(); virtual void CUnprovenSlot43();
    virtual bool IsFrameUpdated() const;                    // slot 44 (+0xB0)
    void FrameUpdate();                                     // 0x004C9D20
    char          _pad_04[0x84];                            // +0x04
    CThing*       UpdateListNext;                           // +0x88
    char          _pad_8c[0x5];                             // +0x8C
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;          //       bit 1
    unsigned char IncludeInThingScans : 1;                  //       bit 2
    unsigned char Activated : 1;                            //       bit 3
    unsigned char GloballyLoaded : 1;                       //       bit 4
    unsigned char InLimbo : 1;                              //       bit 5
    unsigned char KillLocked : 1;                           //       bit 6
    unsigned char CreatedBySerialisation : 1;               //       bit 7
};

class CThingManager : public CClassFactoryLayout { public:
    char       _pad_1c[0x3A];                               // +0x1C Component .. DeadThingList, the pause-mode bools
    bool       FirstRenderOfFrame;                          // +0x56
    char       _pad_57[0x35];                               // +0x57 UIDToThingMap .. RenderList
    std::vector<ThingLList> UpdateLists;                    // +0x8C
    std::vector<CClassTypeInfo>& GetClassTypeInfo() { return *(std::vector<CClassTypeInfo>*)ClassTypeInfo; }   // +0x04
    ThingLList& GetUpdateListFromType(long type) { return UpdateLists[type]; }
    void RemoveFromUpdateList(CThing& thing);              // 0x0051ECC0
    void UpdateDeaths();                                    // 0x0051F070
    void WorldUpdateThings();
};

extern bool g_UpdateThingsEnabled_01375755;                  // 0x01375755
extern bool g_UpdateThingType1Enabled_0137575a;              // 0x0137575A
extern bool g_UpdateThingType5Enabled_01375759;              // 0x01375759
extern long g_NoThingsUpdated_013b8a84;                      // 0x013B8A84

void CThingManager::WorldUpdateThings()
{
    g_NoThingsUpdated_013b8a84 = 0;
    FirstRenderOfFrame = true;
    if (g_UpdateThingsEnabled_01375755) {
        for (long type = 1; type < 16; ++type) {
            if (type == 1) {
                if (!g_UpdateThingType1Enabled_0137575a)
                    continue;
            } else if (type == 5) {
                if (!g_UpdateThingType5Enabled_01375759)
                    continue;
            }
            if (!GetClassTypeInfo()[type].Used)
                continue;
            CThing* thing = GetUpdateListFromType(type).Head;
            while (thing) {
                ++g_NoThingsUpdated_013b8a84;
                CThing* current = thing;
                thing = thing->UpdateListNext;
                if (!current->Dead && current->Activated && !current->InLimbo) {
                    current->FrameUpdate();
                    if (!current->IsFrameUpdated())
                        RemoveFromUpdateList(*current);
                }
            }
        }
    }
    UpdateDeaths();
}