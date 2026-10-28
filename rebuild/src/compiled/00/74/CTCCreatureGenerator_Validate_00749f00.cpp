// CTCCreatureGenerator::Validate(CCharString& error) const -- false (with the reason in error) when no creature families are set, or when a family generates hidden without hidden start positions, uses the unsupported generator-animation type, or generates in the open without open positions
// Leaf: Validate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749F00, 512 bytes (`ret 4`, 0x18-byte frame, no EH frame).
//         Ego_r.pdb: `?Validate@CTCCreatureGenerator@@UBE_NAAVCCharString@@@Z` (vtable slot 17).
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0, operator=(const CCharString&) @ 0x0099EFB0, operator+=(const char*)
//          @ 0x0099F100, ~CCharString @ 0x0099EAE0, operator+(const char*, const CCharString&) @ 0x0099F690 (__fastcall, hidden
//          result in ecx); CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<CCreatureGenerationFamilyDef>
//          @ 0x0074D220 (catalog "GetDef<CThingPhysicalSwitchDef>", one body of the shared template); the def's virtual slot 10
//          GetDefName (+0x28, CDefString by value) and slot 1 Destroy; CDefStringTable::GetString(long) @ 0x009D49B0 on
//          g_DefStringTable 0x013CA828; the STLport random-access __find<long> @ 0x0074C9B0 (__fastcall, value and tag by reference).
// Layout: retail CTCCreatureGenerator: CreatureFamilies (CArray<CDefIndex>) begin +0x0c / end +0x10, OpenPositions begin +0x60 /
//         end +0x64, HiddenPositions begin +0x6c / end +0x70 (12-byte retail vectors; Ego_r.pdb +0x70 / +0x80 with 16-byte ones).
//         CCreatureGenerationFamilyDef GenerationTypes begin +0x50 / end +0x54 (PDB +0x58).
// Strings: 0x01268BD8, 0x01268BC0, 0x01268B98, 0x01268B50, 0x01268B00.
// Shape: the family name is a named CCharString local in its own block (both temporaries destroyed right after the
//        assignment, the name pushed by `lea`); the def is a CDefPointer local filled by a same-TU GetDef stand-in and released
//        inline at each exit without a null test; the __find instantiation is compiled in this TU with its real read-only body,
//        so the type range is read once (end into edi before begin into ebp: `last` declared first); value 2 lives in its own
//        slot set once before the loop, values 3/0/1 reuse the def's frame word, and the empty iterator tag is homed in the
//        (still live) error argument slot; the family index is passed through CDefIndex's conversion to long.
class CCharString { public:
    void* PStringData;
    ~CCharString();                                                  // 0x0099EAE0
    const CCharString& operator=(const char* text);                        // 0x0099EFE0
    const CCharString& operator=(const CCharString& other);                // 0x0099EFB0
    CCharString& operator+=(const char* text);                       // 0x0099F100
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                             // 0x013CA828
class CDefString { public:                                                  // a def name: its offset into the def string table
    long m_index;
    CDefString(const CDefString& other) : m_index(other.m_index) { }
};
enum ECreatureGeneratorGenerateType {
    GENERATE_TYPE_0 = 0,                                             // needs open positions
    GENERATE_TYPE_1 = 1,                                             // needs open positions
    GENERATE_TYPE_2 = 2,                                             // needs hidden start positions
    GENERATE_GENERATOR_ANIMATION = 3                                 // not supported
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
// The out-of-line STLport __find instantiation (retail 0x0074C9B0) is compiled in this TU: its body (the landed
// global_Find0074c9b0 four-way unrolled scan) only reads memory, so VC7.1 keeps the range in registers across the calls.
#pragma auto_inline(off)
template <class T> __declspec(noinline) const T* __fastcall __find(const T* first, const T* last, const T& value,
                                                                   const random_access_iterator_tag&)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}
#pragma auto_inline(on)
template <class T> inline const T* find(const T* first, const T* last, const T& value)
{
    return __find(first, last, value, random_access_iterator_tag());
}

template <class T> struct CVector {                                  // STLport vector (12 bytes retail)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
class C3DVector { public: float X, Y, Z; };
class CDefIndex { public: long Index; operator long() const { return Index; } };

class CCreatureGenerationFamilyDef { public:
    virtual void v00();
    virtual void Destroy();                                          // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName() const;                           // slot 10 (+0x28)
    long RefCount;                                                   // +0x04
    unsigned char _pad08[0x48];
    CVector<long> GenerationTypes;                                   // +0x50
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
void* FindDef_CDefinitionManager_GetDef_0();                         // stand-in callee of the stand-in body below
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CCreatureGenerationFamilyDef>* out) const;   // 0x0074D220
};
class CGameDefinitionManager : public CDefinitionManager
{
};
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CCreatureGenerationFamilyDef>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CTCBase
{
public:
    virtual void v00();
    virtual bool Validate(CCharString& error) const;
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCCreatureGenerator : public CTCBase {
public:
    virtual bool Validate(CCharString& error) const;

public:
    void*                   ParentThing;                             // +0x04 (CTCBase)
    long                    m_tcBase08;                              // +0x08
    CVector<CDefIndex>      CreatureFamilies;                        // +0x0c
    unsigned char           _pad18[0x48];
    CVector<C3DVector>      OpenPositions;                           // +0x60
    CVector<C3DVector>      HiddenPositions;                         // +0x6c
};

bool CTCCreatureGenerator::Validate(CCharString& error) const
{
    if (CreatureFamilies.empty()) {
        error = "no creature families specified - what am I supposed to generate?";
        return false;
    }
    for (const CDefIndex* it = CreatureFamilies.begin(); it != CreatureFamilies.end(); ++it) {
        CDefPointer<CCreatureGenerationFamilyDef> def;
        GetDefinitionManager().GetDef(*it, &def);
        {
            CCharString name = g_DefStringTable.GetString(def->GetDefName().m_index);
            error = "creature family " + name;
        }
        const long* last = def->GenerationTypes.end();
        const long* first = def->GenerationTypes.begin();
        if (find(first, last, (long)GENERATE_TYPE_2) != last && HiddenPositions.empty()) {
            error += " needs to have hidden start positions";
            return false;
        }
        if (find(first, last, (long)GENERATE_GENERATOR_ANIMATION) != last) {
            error += " has generate type GENERATE_GENERATOR_ANIMATION and it's not supported";
            return false;
        }
        if (find(first, last, (long)GENERATE_TYPE_0) != last && OpenPositions.empty()) {
            error += " needs to create creatures in the open and I can't find any navigable positions";
            return false;
        }
        if (find(first, last, (long)GENERATE_TYPE_1) != last && OpenPositions.empty()) {
            error += " needs to create creatures in the open and I can't find any navigable positions";
            return false;
        }
    }
    return true;
}