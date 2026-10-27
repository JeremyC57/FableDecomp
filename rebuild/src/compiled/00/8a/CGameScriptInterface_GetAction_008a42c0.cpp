// CGameScriptInterface::GetAction(CScriptThing thing) -- the thing's CTCAIScratchpad::Action, or an empty string when the script thing has no thing
// Leaf: ?GetAction@CGameScriptInterface@@UAE?AVCCharString@@VCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008a42c0, 201 bytes, thiscall, `ret 10h` (the hidden result pointer and the 12-byte CScriptThing).
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 797; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?GetAction@CGameScriptInterface@@UAE?AVCCharString@@VCScriptThing@@@Z; it copies Ego_r.pdb CTCAIScratchpad::Action (+0x48) through interface 0xAA (CTCAIScratchpad::GetInterfaceType() is 170, landed).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the empty literal 0x0122D70E), CVectorMap<ETCInterfaceType,
//   CTCBase*, ...>::LowerBound 0x0040F020 (inlined GetInterface), CCharString::CCharString(const CCharString&) 0x0099EC30,
//   operator delete 0x00BFE9BC and CBaseClass::~CBaseClass 0x0099A2E0 (the parameter's inline ~CScriptThing: vtable
//   0x01238C8C, the CCountedPointer<CScriptThing> release, the base destructor).
// Body: CScriptThing::GetThing() is an inline virtual (`PImp.Data == 0 ? 0 : PImp.Data->GetThing()`) called on the by-value object,
//   so it devirtualises and inlines twice; GetInterface's result is ignored (a miss reads the uninitialised scratchpad local).
// Header debt: engine/CScriptThing.h, CThing and engine/CTCAIScratchpad.h are flat layouts; modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                     // +0x00 (CArray: STLport vector)
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                          // +0x00
    CCharString(const char* text, long length);            // 0x0099EBF0
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
};
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBaseClass {
public:
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
class CScriptThing : public CBaseClass {                   // retail 12 bytes: vptr, PImp
public:
    virtual ~CScriptThing() { }
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const { return PImp.Data == 0 ? 0 : PImp.Data->GetThing(); }   // slot 11 (+0x2C)
    CCountedPointer<CScriptThing> PImp;                    // +0x04
};
class CTCAIScratchpad {
public:
    unsigned char _pad00[0x48];
    CCharString   Action;                                   // +0x48
};
class CGameScriptInterface {
public:
    virtual CCharString GetAction(CScriptThing thing);
};
CCharString CGameScriptInterface::GetAction(CScriptThing thing)
{
    if (thing.GetThing() == 0)
        return CCharString("", -1);
    CTCAIScratchpad* scratchpad;
    thing.GetThing()->GetInterface(&scratchpad, (ETCInterfaceType)0xAA);
    return scratchpad->Action;
}