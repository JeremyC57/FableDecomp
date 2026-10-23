// CTCWound::OnSerialise(CPersistContext&, bool) -- save or load the wound list: the count, then each wound's duration and morph index under numbered names; a loaded wound is rebuilt from the creature def's wound morph table and appended, then the list is trimmed to 15
// Leaf: ?OnSerialise@CTCWound
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757C90, 765 bytes (+ int3 padding), `ret 8`.
// Callees: CPersistContext::Transfer(const char*, long&, const long&) 0x00410BE0 / Transfer(const char*, unsigned long&,
//   const unsigned long&) 0x004106F0; GFIntToCharString 0x0099F830 / operator+(const char*, const CCharString&) 0x0099F690
//   (both `__fastcall`); CCharString::operator const char*() const 0x0099E4C0 / ~CCharString 0x0099EAE0;
//   CDefClassBase::GetSubDef<CCreatureDef> 0x00485815 (same-TU stand-in, as the landed AddWound 0x00757B30);
//   CWoundMorphs::GetPWoundWithIndex(long) const 0x00757930; malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list node).
// Layout: as the landed AddWound 0x00757B30; CPersistContext Mode +0x18.
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int size);
#pragma intrinsic(memcmp)

enum EBodyLocation { BODY_LOCATION_NONE = 0 };

struct CWoundMorphs {
    struct CEntry {
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
    const CEntry* GetPWoundWithIndex(long index) const;     // 0x00757930
};
template <class T> struct CCountedPointer {
    T*    Data;                                              // +0x00
    void* Count;                                             // +0x04
    T* operator->() const { return Data; }
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CCreatureDef {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x28 - 0x08];
    CCountedPointer<CWoundMorphs> PWoundMorphs;              // +0x28
};
template <class T> struct CDefPointer {
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
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    operator const char*() const;                            // 0x0099E4C0
};
CCharString __fastcall GFIntToCharString(long value);       // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                      // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue);                       // 0x00410BE0
    void Transfer(const char* name, unsigned long& value, const unsigned long& defaultValue);     // 0x004106F0
};
struct CDefinitionManager {
    CDefPointer<CCreatureDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase {
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
struct CThing {
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
    void push_back(const T& x) { insert(end(), x); }
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

template <class T>
__forceinline void SaveValue(CPersistContext& context, const char* name, T& value, const T& defaultValue)
{
    context.Transfer(name, value, defaultValue);
}

struct CTCWound {
    struct CWound {
        CWoundMorphs::CEntry Desc;                           // +0x00
        unsigned long        Duration;                       // +0x28
        long                 WoundTextureDecalEntryID;       // +0x2c
        long                 WoundBumpDecalEntryID;          // +0x30
        long                 ScarTextureDecalEntryID;        // +0x34
        long                 ScarBumpDecalEntryID;           // +0x38
        long                 MorphIndex;                     // +0x3c
        bool                 Dead;                           // +0x40
        bool                 HadUpdate;                      // +0x41
        CWound(const CWoundMorphs::CEntry& desc, unsigned long duration, long morphIndex)
            : Desc(desc), Duration(duration), WoundTextureDecalEntryID(0), WoundBumpDecalEntryID(0), ScarTextureDecalEntryID(0),
              ScarBumpDecalEntryID(0), MorphIndex(morphIndex), Dead(false), HadUpdate(false) { }
    };
    virtual void v00();
    CThing*               ParentThing;                       // +0x04
    unsigned char         _pad08[4];
    unsigned long         LastFrameChanged;                  // +0x0c
    CRetailList<CWound>   Wounds;                            // +0x10
    const CWorld& PeekWorld() const;                         // 0x00686A80 (CTCBase)
    void OnSerialise(CPersistContext& context, bool transfer);
};

void CTCWound::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        long count = Wounds.size();
        context.Transfer("NumberOfWounds", count, 0L);
        long i = 0;
        CListIterator<CWound> it = Wounds.begin();
        if (it != Wounds.end()) {
            const unsigned long noDuration = 0;
            const long noMorphIndex = 0;
            do {
                unsigned long duration = it->Duration;
                SaveValue(context, "WoundDuration" + GFIntToCharString(i), duration, noDuration);
                long morphIndex = it->MorphIndex;
                SaveValue(context, "WoundMorphIndex" + GFIntToCharString(i), morphIndex, noMorphIndex);
                ++it;
                ++i;
            } while (it != Wounds.end());
        }
    } else {
        long count = 0;
        context.Transfer("NumberOfWounds", count, 0L);
        long i = 0;
        if (i < count) {
            const unsigned long noDuration = 0;
            const long noMorphIndex = 0;
            do {
                unsigned long duration = 0;
                SaveValue(context, "WoundDuration" + GFIntToCharString(i), duration, noDuration);
                long morphIndex = -1;
                SaveValue(context, "WoundMorphIndex" + GFIntToCharString(i), morphIndex, noMorphIndex);
                CDefPointer<CCreatureDef> def;
                ParentThing->m_pDef->GetSubDef(&def);
                const CWoundMorphs::CEntry* entry = def->PWoundMorphs->GetPWoundWithIndex(morphIndex);
                if (entry)
                    Wounds.push_back(CWound(*entry, duration, morphIndex));
            } while (++i < count);
        }
        while (Wounds.size() > 15)
            Wounds.pop_back();
    }
}