// CTCWound::AddWound(EBodyLocation location) -- pick a random wound morph for the body location from the creature def; an identical wound already on the thing just has its duration reset, otherwise a fresh wound goes on the front of the list and the list is trimmed to 15 from the back
// Leaf: AddWound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00757B30, length 352 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?AddWound@CTCWound@@QAEXW4EBodyLocation@@@Z`).
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const CCreatureDef>*) 0x00485815 (manifest
//   "GetSubDef<CAbilityDef>": a byte-identical folded instantiation); CTCBase::PeekWorld() const 0x00686A80; CWorld random
//   seed accessor 0x0049D810 (as the landed AddPossibleWoundGivenStrikePoint 0x00757F90); CWoundMorphs::
//   GetPRandomWoundForLocation(EBodyLocation, unsigned long&, long&) const 0x00757870 (Ego_r.pdb name; manifest
//   "CVectorMap_FindRandomEntry"); CWorld::GetWorldFrame() 0x0049D870 (static); malloc 0x00BFEA0E / free 0x00BFEA14
//   (STLport list node, 0x4C bytes); the def's virtual slot 1 on the last release.
// Layout: CTCWound (Ego_r.pdb = retail): ParentThing +0x04, LastFrameChanged +0x0c, Wounds +0x10 (4-byte STLport list);
//   CTCWound::CWound (0x44): Desc +0x00 (CWoundMorphs::CEntry, 40 bytes), Duration +0x28, four decal entry IDs
//   +0x2c..+0x38, MorphIndex +0x3c, Dead +0x40, HadUpdate +0x41; CCreatureDef PWoundMorphs +0x28 (CCountedPointer, data
//   first); CThing def +0x70 (as landed).
// Shape: the def is a CDefPointer local released through DecRefToZero, filled by a same-TU GetSubDef stand-in (with an
//   external one the zero store is not sunk and the release keeps its null test); the morph table is read into a named
//   local before the -1 morph index is declared (otherwise the -1 store lands before the argument push); CEntry::operator== is the intrinsic memcmp (`repe cmpsd`, 10 dwords); the new wound is a
//   CWound temporary pushed on the front (STLport insert(begin(), x): retail homes begin() in the dead argument slot);
//   the trim is `while (size() > 15) pop_back()` with STLport's walking size().
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

enum EBodyLocation { BODY_LOCATION_NONE = 0 };

class CWoundMorphs { public:
    class CEntry { public:
        EBodyLocation Location;
        long          ReplacingTextureIndex;
        long          ReplacingBumpMapIndex;
        long          WoundTextureIndex;
        long          WoundBumpMapIndex;
        long          ScarTextureIndex;
        long          ScarBumpMapIndex;
        long          FadeToScarDuration;
        long          TotalDuration;
        float         PermanentScarStrength;
        bool operator==(const CEntry& other) const { return memcmp(this, &other, sizeof(CEntry)) == 0; }
    };
    const CEntry* GetPRandomWoundForLocation(EBodyLocation location, unsigned long& seed, long& index) const;   // 0x00757870
};
template <class T> class CCountedPointer { public:
    T*    Data;                                              // +0x00
    void* Count;                                             // +0x04
    T* operator->() const { return Data; }
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CCreatureDef { public:
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CCountedPointer<CWoundMorphs> PWoundMorphs;              // +0x28
};
template <class T> class CDefPointer { public:
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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
class CDefinitionManager { public:
    CDefPointer<CCreatureDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);  // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                          // +0x1c
    bool GetSubDef(CDefPointer<CCreatureDef>* out);         // 0x00485815
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef 0x00485815 (the landed PickUpGoods 0x00848730 model): VC7.1 then knows the def
// pointer's address does not escape, so the null test before the release folds and the zero store sinks like retail's.
bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCreatureDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCreatureDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* m_pDef;                                   // +0x70
};
class CWorld {
public:
    unsigned long* GetRandomSeed() const;                    // 0x0049D810
    static long GetWorldFrame();                             // 0x0049D870
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CListIterator {                    // STLport _List_iterator
    SListNode<T>* _M_node;
    CListIterator(SListNode<T>* n) : _M_node(n) { }
    T* operator->() const { return &_M_node->_M_data; }
    CListIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CListIterator& other) const { return _M_node != other._M_node; }
};
template <class T> struct CRetailList {                      // STLport list (4 bytes: the head node)
    SListNode<T>* _M_node;
    CListIterator<T> begin() { return CListIterator<T>(_M_node->_M_next); }
    CListIterator<T> end() { return CListIterator<T>(_M_node); }
    unsigned int size() const
    {
        unsigned int n = 0;
        for (SListNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    CListIterator<T> insert(CListIterator<T> position, const T& x)
    {
        SListNode<T>* tmp = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        T* data = &tmp->_M_data;
        if (data != 0)
            *data = x;
        SListNode<T>* n = position._M_node;
        SListNode<T>* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return CListIterator<T>(tmp);
    }
    void push_front(const T& x) { insert(begin(), x); }
    void pop_back()
    {
        SListNode<T>* n = _M_node->_M_prev;
        SListNode<T>* next = n->_M_next;
        SListNode<T>* prev = n->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(n);
    }
};

class CTCBase
{
public:
    virtual void v00();
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCWound : public CTCBase { public:
    class CWound { public:
        CWoundMorphs::CEntry Desc;                           // +0x00
        unsigned long        Duration;                       // +0x28
        long                 WoundTextureDecalEntryID;       // +0x2c
        long                 WoundBumpDecalEntryID;          // +0x30
        long                 ScarTextureDecalEntryID;        // +0x34
        long                 ScarBumpDecalEntryID;           // +0x38
        long                 MorphIndex;                     // +0x3c
        bool                 Dead;                           // +0x40
        bool                 HadUpdate;                      // +0x41
        CWound(const CWoundMorphs::CEntry& desc, long morphIndex)
            : Desc(desc), Duration(0), WoundTextureDecalEntryID(0), WoundBumpDecalEntryID(0), ScarTextureDecalEntryID(0),
              ScarBumpDecalEntryID(0), MorphIndex(morphIndex), Dead(false), HadUpdate(false) { }
    };
    CThing*               ParentThing;                       // +0x04
    unsigned char         _pad08[4];
    unsigned long         LastFrameChanged;                  // +0x0c
    CRetailList<CWound>   Wounds;                            // +0x10
    void AddWound(EBodyLocation location);
};

void CTCWound::AddWound(EBodyLocation location)
{
    CDefPointer<CCreatureDef> def;
    ParentThing->m_pDef->GetSubDef(&def);
    const CWoundMorphs* morphs = def->PWoundMorphs.Data;
    long morphIndex = -1;
    const CWoundMorphs::CEntry* entry = morphs->GetPRandomWoundForLocation(location, *PeekWorld().GetRandomSeed(), morphIndex);
    if (entry != 0) {
        LastFrameChanged = CWorld::GetWorldFrame();
        for (CListIterator<CWound> it = Wounds.begin(); it != Wounds.end(); ++it) {
            if (*entry == it->Desc) {
                it->Duration = 0;
                return;
            }
        }
        Wounds.push_front(CWound(*entry, morphIndex));
        while (Wounds.size() > 15)
            Wounds.pop_back();
    }
}