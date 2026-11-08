// CAIStateGroup_SeekScriptTarget::IsRunnable() -- runnable when the creature's interface-8 component names a live
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound) re-padded by-value maps +8.
// script target (virtual slot 29) and the component's own target pointer (+0x40) is set; remember it in PTarget.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093BEF0, length 167 bytes.
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined HasTC + entry-returning FindEntry, type 8), the component's
// virtual slot 29 (+0x74), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on the component's +0x40 pointer, the
// converting CIntelligentPointer temporary (vtable 0x01238C6C): ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice),
// GetPItem @ 0x00A01B50, dtor @ 0x00A01C10.
// Shape: the component is kept in a local (edi) because it is used twice; no CanSee test here.
// The engine header flattens the base, so the class is modelled locally; the pointer member is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    FABLE_FORCEINLINE const CInterfaceMap::value_type* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        const CInterfaceMap::value_type* sel;
        if (it == map->end()) sel = map->end();
        else if (it->first <= interfaceType) sel = it;
        else sel = map->end();
        return sel;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    FABLE_FORCEINLINE CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CTCScriptTargetSource {                  // the interface-8 component
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetScriptTarget();          // slot 29 (+0x74)
    unsigned char _pad[0x3c];                   // +0x04..+0x3f
    CBaseIntelligentPointer PTarget;        // +0x40
};

struct CAIStateGroup_SeekScriptTarget : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;           // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_SeekScriptTarget::IsRunnable()
{
    if (PBrain->PCreature->HasTC(8)) {
        CTCScriptTargetSource* source = (CTCScriptTargetSource*)PBrain->PCreature->FindEntry(8)->second;
        CThing* target = source->GetScriptTarget();
        if (target != 0 && !(target->m_flags91 & 1) && source->PTarget.GetPItem() != 0) {
            PTarget = target;
            return true;
        }
    }
    return false;
}