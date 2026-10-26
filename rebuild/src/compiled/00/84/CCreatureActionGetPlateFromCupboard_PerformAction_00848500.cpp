// CCreatureAction_GetPlateFromCupboard::PerformAction() -- create the plate thing at the cupboard, put it in the creature's carry slot and add creature mode 5
// Leaf: PerformAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00848500, 275 bytes, virtual PerformAction.
// Callees: CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E), CBaseIntelligentPointer::GetPItem 0x00A01B50
//   (PCreature +0x08), CThing::GetPosition 0x004C73D0, CreateObject(long, const C3DVector*, long, float*, unsigned long,
//   const CCharString&) 0x00703210 (__fastcall; landed CAIStateGroupBase::CreateItem form), ~CCharString 0x0099EAE0,
//   CDefClassBase::GetSubDef<CCarryableDef> 0x005CC10E, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface: type
//   0x46 = bit 6 of the word at +0x28, CTCCarrying; type 0x31 = bit 17 of the word at +0x24, CTCCreatureModeManager),
//   CTCCarrying::PutThingInCarrySlot(CThingObject*, long, bool) 0x006A1F80, CTCCreatureModeManager::AddMode 0x0073AE10.
// Layout: retail CCreatureActionBase is 0xa8 bytes; this class's cupboard thing pointer at +0xa8 and plate def index at
//   +0xac; CThing +0x70 the def, +0x90 the player byte (signed); CCarryableDef +0x28 the carry slot.
// Shape: the empty name is an implicit conversion built before any other argument; the player id is an inline
//   long-returning accessor used straight as the CreateObject argument (a named local / byte accessor rotate the
//   movsx and the two frame-address pushes); both component lookups are unchecked (retail reads the uninitialised
//   out variable's slot on the miss path) and the inline GetInterface stores its entry pick as ONE ternary so the
//   carrying component lands straight in ecx; the carryable def is a CDefPointer local released at the end
//   (DecRefToZero, null test folded after the dereference).
class CThing;
class CThingObject;
class C3DVector { public: float x, y, z; };
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CCarryableDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                                   // +0x28
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out); // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-ins (retail's LTCG knowledge): GetSubDef<CCarryableDef> 0x005CC10E (the landed GetSubDef body,
// 0x00485815's shape) and CVectorMap::LowerBound 0x0040F020 -- VC7.1 then knows neither keeps the address of the def
// pointer / lookup key, so the def stays in ebp and the frame words are shared like retail's.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (its retail body: STLport lower_bound over the 8-byte
// {type, component} entries): compiled above the caller so VC7.1 knows the key's address does not escape, which lets
// the key share its frame word with the AddTC name string and the by-value target copy.
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x90 - 0x74];
    signed char m_player;                             // +0x90
    long GetPlayer() const { return m_player; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public: };
CThingObject* __fastcall CreateObject(long definition, const C3DVector* position, long playerID,
                                      float* orientation, unsigned long flags, const CCharString& name);   // 0x00703210
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
class CTCCarrying { public:
    void PutThingInCarrySlot(CThingObject* thing, long slot, bool immediately);   // 0x006A1F80
};
class CTCCreatureModeManager { public:
    bool AddMode(unsigned long mode);                 // 0x0073AE10
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
struct CCreatureAction_GetPlateFromCupboard : CCreatureActionBase {
    CThing* m_pCupboard;                              // +0xa8
    long m_plateDefIndex;                             // +0xac
    void PerformAction();
};

void CCreatureAction_GetPlateFromCupboard::PerformAction()
{
    CThingObject* plate;
    {
        CCharString name("");
        plate = CreateObject(m_plateDefIndex, &m_pCupboard->GetPosition(), PCreature.GetPItem()->GetPlayer(), 0, 0, name);
    }
    CDefPointer<CCarryableDef> carryDef;
    plate->m_pDef->GetSubDef(&carryDef);
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    long slot = carryDef->CarrySlot;
    carrying->PutThingInCarrySlot(plate, slot, true);
    CTCCreatureModeManager* modes;
    PCreature.GetPItem()->GetInterface(&modes, 0x31);
    modes->AddMode(5);
}