// CDefinitionManager::AddDefClass(const CDefClassInit& init) -- register a def class: show "Add Def Class" progress, build its class info
// (name + factory, empty instance list), add it under the CRC of the name to the class-info vector map and re-sort it; when defs are being
// compiled or not yet loaded, instantiate the "NULLDEF_<class>" default def, load the class's first instance and persist it with a
// clear-mode context.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B0AC0, length 404 bytes, `ret 4`.
// Callees: CCharString 0x0099EBF0 / 0x0099E4B0 / 0x0099EC30 / 0x0099EAE0 / operator= 0x0099EFB0; DisplayProgress 0x009E9F40 (__fastcall);
// CCRC::Calc 0x004014A0 (__fastcall); STLport vector<unsigned long> copy ctor 0x00535470 (manifest "Std_Vector_Construct_Copy"); class-info
// vector push_back 0x009B9170; std::sort over the 24-byte pairs 0x009B9490 (manifest "Heap_MakePriority_24byte"); free 0x00BFEA14;
// operator+(const char*, const CCharString&) 0x0099F690 (__fastcall, hidden result in ecx); InstantiateDef 0x009ADC90; class-info lookup
// 0x009AD2E0 (manifest "CFactionDatabase::FindFactionBySymbolName" is wrong); LoadDef 0x009AD6E0; CPersistContext::InitClearMode
// 0x009BAA70; def slot 18 (+0x48) persist, slot 1 release. Globals: bytes 0x0138E189 / 0x013CA7D8 (descriptive names).
// Layout: local model (retail offsets): class-info CVectorMap at +0x78 (vector, compare byte +0xc, unsorted flag +0xd), InstantiatedDefs at
// +0xa4 (landed 0x009ADA90 model); CDefClassInfo {Name, Create, vector<unsigned long> Instances} = 0x14, pair with the CRC key = 0x18.
// Shape: the progress text is an IMPLICIT const char* -> CCharString conversion (built before the other arguments are pushed; the explicit
// temporary is built after); the map's Add (sets the flag) and Sort (re-reads it) are separate forceinline members so the flag is stored and
// re-tested; CCharString's GetString/GetLength are written null-first (`p == 0 ? x : p->...`); CDefClassInfo assigns Create then Name in
// its ctor body (an init-list Create is stored before the vector zeroes).
// Leaf: AddDefClass
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
struct CCharStringData { char* Data; long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
struct CCRC { static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
template <class T> struct CRetailVector {   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CRetailVector(const CRetailVector& other);   // 0x00535470
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T& operator[](unsigned long i) { return _M_start[i]; }
};
struct CDefClassBase;
typedef CDefClassBase* (*TDefCreateFn)();
struct CDefClassInit {
    CCharString Name;                                  // +0
    TDefCreateFn Create;                               // +4
};
struct CDefClassInfo {
    CCharString Name;                                  // +0
    TDefCreateFn Create;                               // +4
    CRetailVector<unsigned long> Instances;            // +8
    CDefClassInfo(const CDefClassInit& init) { Create = init.Create; Name = init.Name; }
};
struct SDefClassInfoPair {
    unsigned long first;
    CDefClassInfo second;
    SDefClassInfoPair(unsigned long key, const CDefClassInfo& value) : first(key), second(value) {}
};
struct CKeyPairCompareLess {};
void __fastcall SortDefClassInfo(SDefClassInfoPair* first, SDefClassInfoPair* last, CKeyPairCompareLess comp);   // 0x009B9490
struct CDefClassInfoMap {
    SDefClassInfoPair* _M_start;
    SDefClassInfoPair* _M_finish;
    SDefClassInfoPair* _M_end_of_storage;
    CKeyPairCompareLess Comp;                          // +0x0c
    bool Unsorted;                                     // +0x0d
    void push_back(const SDefClassInfoPair& value);    // 0x009B9170
    __forceinline void Add(unsigned long key, const CDefClassInfo& value)
    {
        push_back(SDefClassInfoPair(key, value));
        Unsorted = true;
    }
    __forceinline void Sort()
    {
        if (Unsorted) {
            SortDefClassInfo(_M_start, _M_finish, Comp);
            Unsorted = false;
        }
    }
};
struct CDefClassBase {
    virtual void v00();
    virtual void Release();                            // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void Persist(struct CPersistContext& context);   // slot 18 (+0x48)
    long RefCount;                                     // +4
};
struct CDefPointer {
    CDefClassBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            Object->RefCount = Object->RefCount + 1;
    }
    ~CDefPointer()
    {
        if (--Object->RefCount == 0)
            Object->Release();
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CPersistContext {
    unsigned char _body[0x2c];
    void InitClearMode();                              // 0x009BAA70
};
struct CInstantiatedDefInfo {
    long ClassName;
    long InstantiationName;
    unsigned long ClassIndex;
    CDefPointer PDef;                                  // +0x0c
};
extern bool g_bDefsCompiling;                          // 0x0138E189
extern bool g_bDefsLoaded;                             // 0x013CA7D8
struct CDefinitionManager {
    unsigned char _pad00[0x78];
    CDefClassInfoMap DefClassInfo;                     // +0x78
    unsigned char _pad88[0xa4 - 0x88];
    CInstantiatedDefInfo** InstantiatedDefs;           // +0xa4 CArray start
    bool InstantiateDef(const CCharString& className, const CCharString& instantiationName, bool a);   // 0x009ADC90
    CDefClassInfo* GetClassInfo(const CCharString& className);   // 0x009AD2E0
    void LoadDef(unsigned long globalIndex);           // 0x009AD6E0
    CDefPointer GetPDefFromGlobalIndex(unsigned long globalIndex)
    {
        LoadDef(globalIndex);
        return InstantiatedDefs[globalIndex]->PDef;
    }
    void AddDefClass(const CDefClassInit& init);
};

void CDefinitionManager::AddDefClass(const CDefClassInit& init)
{
    DisplayProgress("Add Def Class", -1.0f, false, false);
    CDefClassInfo info(init);
    DefClassInfo.Add(CCRC::Calc(0, init.Name.GetString(), init.Name.GetLength()), info);
    DefClassInfo.Sort();
    if (g_bDefsCompiling || !g_bDefsLoaded) {
        CCharString nullName = "NULLDEF_" + init.Name;
        InstantiateDef(init.Name, nullName, false);
        CDefPointer def = GetPDefFromGlobalIndex(GetClassInfo(init.Name)->Instances._M_start[0]);
        CPersistContext context;
        context.InitClearMode();
        def->Persist(context);
    }
}