// CClassFactory::GetClassNumberFromName(const CCharString& name) -- index of the used class type whose name equals `name`; otherwise builds "Unable to find class with name <name>" and throws CGenericException
// Leaf: GetClassNumberFromName
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A37060, 208 bytes incl. padding (ret 4).
// Callees: Compare(const char*, const char*) @ 0x00411570 (__fastcall, in the inline CCharString ==),
//          CCharString(const char*, long) @ 0x0099EBF0, operator+(const CCharString&, const char*) @ 0x0099F600,
//          operator+(const CCharString&, const CCharString&) @ 0x0099F570 (both __fastcall, result in ecx),
//          ~CCharString @ 0x0099EAE0, operator const char* @ 0x0099E4C0, _CxxThrowException @ 0x00BFEB84
//          (ThrowInfo 0x013692F8 = CGenericException, a 1-byte empty class). Strings "" 0x0122D70E,
//          "Unable to find class with name " 0x0129CF3C.
// Layout: retail CClassFactory (landed ReregisterAllocatedClass 0x00A37300 model; rebuild/include/engine/CClassFactory.h is
//          the Ego_r layout, 4 bytes larger before MaxClassTypeID): ClassTypeInfo vector {first +4}, MaxClassTypeID +0x10;
//          20-byte CClassTypeInfo: Used +8, Name +0x10 (CCharString).
// Shape: the CCharString == of the landed CMapBrushList::GetScriptedMapBrush (same block, or equal lengths and Compare == 0);
//          the message is traced by reference (release build: only the const char* conversion survives); no EH frame.
#include <stddef.h>
struct CCharStringData { const char* m_pData; unsigned long m_length; };
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
struct CCharString {
    CCharStringData* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = m_pData;
        CCharStringData* b = other.m_pData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570

inline void TraceError(const CCharString& text) { (const char*)text; }

struct CGenericException { };

struct CClassTypeInfo_Retail {                  // 20 bytes
    void*         TypeList;                     // +0x00 (retail list: sentinel pointer)
    unsigned long NoAllocated;                  // +0x04
    bool          Used;                         // +0x08
    bool          Local;                        // +0x09
    void*         AllocFunc;                    // +0x0C
    CCharString   Name;                         // +0x10
};

struct CClassFactory {
    void*                  __vftable;           // +0x00
    CClassTypeInfo_Retail* ClassTypeInfo_first; // +0x04
    CClassTypeInfo_Retail* ClassTypeInfo_last;  // +0x08
    CClassTypeInfo_Retail* ClassTypeInfo_end;   // +0x0C
    unsigned long          MaxClassTypeID;      // +0x10
    unsigned long          NoAllocatedClasses;  // +0x14
    unsigned long GetClassNumberFromName(const CCharString& name);
};

unsigned long CClassFactory::GetClassNumberFromName(const CCharString& name)
{
    unsigned long count = MaxClassTypeID;
    unsigned long i = 0;
    if (i < count) {
        const CCharString* n = &ClassTypeInfo_first->Name;
        do {
            const CClassTypeInfo_Retail* info = (const CClassTypeInfo_Retail*)((const char*)n - offsetof(CClassTypeInfo_Retail, Name));
            if (info->Used && name == *n)
                return i;
            ++i;
            n = &info[1].Name;
        } while (i < count);
    }
    CCharString message = CCharString("") + "Unable to find class with name " + name;
    TraceError(message);
    CGenericException exception;
    throw exception;
}