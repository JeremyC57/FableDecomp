// CQuestManager::GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames) -- clear both lists, then for every active quest card holding a quest-card component append its script quest name and its definition's display name, both to scriptNames (retail's own behaviour: defNames is only cleared)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B2B70, length 288 bytes, `ret 8`.
// Callees: vector<CCharString>::erase(first, last) 0x0043336A (the inline clear, x2), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (TC 0x6c), CCharString(const
// CCharString&) 0x0099EC30 (the placement copies), vector<CCharString>::_M_insert_overflow 0x00433530, CThing::GetDefName
// 0x004C7CC0 (CDefString by hidden pointer), CDefStringTable::GetString 0x009D49B0 on g_DefStringTable 0x013CA828,
// ~CCharString 0x0099EAE0.
// Note: retail appends BOTH strings to the first vector (esi) -- the second list is only cleared; the source keeps
// that behaviour.
// Layout (retail): ActiveQuestCards (STLport list<CIntelligentPointer<CThing> >, head pointer, node data +8) +0x5c;
// CThing TC mask +0x20 (0x6c = word 3 bit 12), Components +0x44; Ego_r.pdb CTCQuestCard ScriptQuestName +0x28.
// Shape: the uninitialised `CTCQuestCard* tc; if (HasTC) tc = GetTC;` of the landed 0x004B0C80 family (VC keeps the
// first argument's word as the stale value); STLport push_back with a placement copy under VC's null test and a
// `__false_type` tag with a user ctor; the key, the tag and the def-name temporary all reuse the dead argument
// slots; `this` is parked in the entry frame word for the head re-read.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
struct __false_type { __false_type() {} };             // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct vector {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                       // vector<CCharString>: 0x0043336A
    void clear() { erase(begin(), end()); }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x00433530
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (_M_insert_overflow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    T* p = (T*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        new (p) T(x);
    
}
#pragma auto_inline(on)

class CDefString { public:                                    // a definition-string handle: one table index
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    const CTCEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase* Find(long type) const
    {
        const CTCEntry* it = LowerBound((ETCInterfaceType)type);
        if (it == _M_finish || type < it->first)
            it = _M_finish;
        return it->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) const { return Components.Find(type); }
    CDefString GetDefName() const;                     // 0x004C7CC0
};
class CTCQuestCard { public:                                  // TC type 0x6c (Ego_r.pdb)
    unsigned char _pad00[0x28];
    CCharString   ScriptQuestName;                     // +0x28
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};

class CQuestManager { public:
    unsigned char _pad00[0x5c];
    _List_node<CIntelligentPointer<CThing> >* ActiveQuestCards;   // +0x5c (list head)
    void GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames);
};

void CQuestManager::GetAllActiveQuestInfo(vector<CCharString>& scriptNames, vector<CCharString>& defNames)
{
    scriptNames.clear();
    defNames.clear();
    for (_List_node<CIntelligentPointer<CThing> >* n = ActiveQuestCards->_M_next; n != ActiveQuestCards; n = n->_M_next) {
        CThing* card = n->_M_data.GetPItem();
        CTCQuestCard* tc;
        if (card->HasTC(0x6c))
            tc = (CTCQuestCard*)card->GetTC(0x6c);
        if (tc) {
            scriptNames.push_back(tc->ScriptQuestName);
            {
                CCharString name = g_DefStringTable.GetString(card->GetDefName());
                scriptNames.push_back(name);
            }
        }
    }
}