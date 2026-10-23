// Grade: DIFFER(839v839): scratch-register choice only -- retail sizes the name vector in eax/edx (begin kept in edx) and the prefix data in ecx, ours edx/ecx and edx; same eax/ecx/edx rotation at the Create player arg, the GetTC key address, SetPodium's parent, the pair push and the sort's comparator/end loads
// CTCBoastingPodium::OnInitialActivate() -- spawn a boasting position object at every "boast position" dummy of the podium's mesh (numbered by the suffix after the prefix), tell each position its podium and file it by number; anything that is not a boasting position is killed; finally sort the position map
// Leaf: OnInitialActivate
// Retail 0x00814260, 839 bytes, `and esp,-8` frame, `ret`. Callees: CTCGraphicAppearance::GetAllDummyObjectNames 0x004BDB40 /
// GetDummyObject 0x004C08C0, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0 / operator= 0x0099EFB0 /
// GetSubString 0x0099EC70, Compare 0x00411570 (__fastcall), strncmp 0x00BFEAF8, CStringParser ctor 0x00404720 /
// ReadAsInteger 0x009BA540 (inline dtor: comment list 0x004046B0, ~CBase 0x0099A2E0), CTCBase::GetDefinitions 0x006869D0,
// CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CThingObject::Create 0x00703210 (__fastcall), LowerBound
// 0x0040F020, CTCBoastingPosition::SetPodium 0x008147D0, CThing::Kill 0x004C9B80, CBaseIntelligentPointer ctor 0x00A01B10 /
// GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x01238C6C), the position map's push_back
// 0x006A3CD0 and sort 0x006A3F20 (__fastcall), free 0x00BFEA14. Globals: the dummy-name prefix 0x013BAF24 and the
// position object def name 0x013BAF28 (CCharString).
// Layout: CTCBoastingPodium ParentThing +0x04, BoastingPositions CVectorMap +0x0c (vector, comparator +0x18, unsorted
// flag +0x19); CThing graphic appearance +0x64, player +0x90, component bits +0x20 / map +0x44.
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int n);   // 0x00BFEAF8
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14
struct CCharStringData {
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString GetSubString(long start, long length) const;   // 0x0099EC70
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
extern CCharString g_BoastPositionDummyPrefix;          // 0x013BAF24
extern CCharString g_BoastPositionDefName;              // 0x013BAF28
struct CCharStringVector {                              // vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CCharStringVector()
    {
        CCharString* last = _M_finish;
        for (CCharString* p = _M_start; p != last; ++p)
            p->~CCharString();
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    CCharString& operator[](unsigned long i) { return _M_start[i]; }
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CBase { virtual ~CBase(); };                     // dtor 0x0099A2E0
struct CStringParserCommentList { void* head; ~CStringParserCommentList(); };   // 0x004046B0
struct CStringParser : CBase {
    char* String; unsigned long BaseLineOffset; CCharString Filename; CCharString TempCharString;
    CStringParserCommentList Comments; void (*ErrorCallback)(CCharString*); long CursorPos[3];
    CStringParser(const CCharString&, void (*)(CCharString*), const CCharString&, long);   // 0x00404720
    long ReadAsInteger();                               // 0x009BA540
};
struct CTCGraphicAppearance {
    void GetAllDummyObjectNames(CCharStringVector* names);   // 0x004BDB40
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CTCBaseInterface;
struct CInterfaceMap {
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* GraphicAppearance;            // +0x64
    unsigned char _pad68[0x90 - 0x68];
    signed char   Player;                               // +0x90
    void Kill(bool silent);                             // 0x004C9B80
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                           CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};
struct CTCBoastingPosition {
    void SetPodium(CThing* podium);                     // 0x008147D0
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void* GetPItem() const;                             // 0x00A01B50
    void SetPItem(void* item);                          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CKeyPairCompareLess { };
struct SPositionPair {
    long                        first;
    CIntelligentPointer<CThing> second;
    SPositionPair(long key, const CIntelligentPointer<CThing>& value) : first(key), second(value) {}
};
void __fastcall SortPositionPairs(SPositionPair* first, SPositionPair* last, CKeyPairCompareLess less);   // 0x006A3F20
struct CPositionMap {                                   // CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess>
    SPositionPair* _M_start;
    SPositionPair* _M_finish;
    SPositionPair* _M_end_of_storage;
    CKeyPairCompareLess Less;                           // +0x0c
    bool Unsorted;                                      // +0x0d
    void push_back(const SPositionPair& pair);          // 0x006A3CD0
    __forceinline void Add(long key, const CIntelligentPointer<CThing>& value)
    {
        push_back(SPositionPair(key, value));
        Unsorted = true;
    }
    __forceinline void Sort()
    {
        if (Unsorted) {
            SortPositionPairs(_M_start, _M_finish, Less);
            Unsorted = false;
        }
    }
};
struct CTCBoastingPodium {
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    unsigned char _pad08[0x4];
    CPositionMap  BoastingPositions;                    // +0x0c
    CDefinitionManager* GetDefinitions();               // 0x006869D0 (CTCBase)
    void OnInitialActivate();
};

void CTCBoastingPodium::OnInitialActivate()
{
    CCharStringVector names;
    CTCGraphicAppearance* appearance = ParentThing->GraphicAppearance;
    appearance->GetAllDummyObjectNames(&names);
    CCharString lastName("");
    long count = 0;
    for (unsigned long i = 0; i < names.size(); ++i) {
        if (strncmp(names[i].GetPString(), g_BoastPositionDummyPrefix.GetPString(), g_BoastPositionDummyPrefix.Length()) == 0) {
            {
                CCharString& name = names[i];
                if (name == lastName)
                    ++count;
                else {
                    lastName = name;
                    count = 0;
                }
            }
            C3DVector position;
            CRightHandedSet orientation;
            appearance->GetDummyObject(names[i], count, 0, &position, &orientation, 1.0f);
            CCharString suffix = names[i].GetSubString(g_BoastPositionDummyPrefix.Length(), names[i].Length());
            if (suffix.Length() > 0) {
                CStringParser parser(suffix, 0, CCharString(""), 0);
                long index = parser.ReadAsInteger();
                CThing* thing;
                {
                    CCharString noScriptName("");
                    thing = CThingObject::Create(GetDefinitions()->GetDefGlobalIndexFromName(g_BoastPositionDefName),
                                                 &position, ParentThing->Player, &orientation, 0, noScriptName);
                }
                if (thing->HasTC(0x71)) {
                    thing->GetTC<CTCBoastingPosition>(0x71)->SetPodium(ParentThing);
                    BoastingPositions.Add(index, CIntelligentPointer<CThing>(thing));
                } else
                    thing->Kill(false);
            }
        }
    }
    BoastingPositions.Sort();
}
