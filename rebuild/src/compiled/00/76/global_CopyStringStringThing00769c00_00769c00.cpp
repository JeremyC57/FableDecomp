// _global::Copy_StringStringThing_00769c00(SStringStringThing*, SStringStringThing*, SStringStringThing*, const random_access_iterator_tag&, int*) -- SGI __copy over 0x18-byte (string, string, two words, intelligent pointer) records
// Leaf: Copy_StringStringThing_00769c00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769C00, length 108 bytes (+ int3 padding).
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the advanced result is returned.
// Element: 0x18 bytes = two CCharStrings (+0, +4: CCharString::operator= @ 0x0099EFB0), two words (+8, +0xC) and a
// CIntelligentPointer at +0x10 (SetPItem(other.GetPItem()), CBaseIntelligentPointer @ 0x00A01B50 / 0x00A01B90). The
// manifest's `CTCSmokeGenerator::CGenerationPos` label is propagated (that record is 0x14 bytes, see 0x00722C90).
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

class CBaseObject;

struct CBaseIntelligentPointer {             // 8 bytes: vtable + item
    virtual void v00();
    void* PData;                             // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing;

struct SStringStringThing {                  // 0x18 bytes
    CCharString                 First;       // +0x00
    CCharString                 Second;      // +0x04
    long                        Value0;      // +0x08
    long                        Value1;      // +0x0C
    CIntelligentPointer<CThing> PThing;      // +0x10
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SStringStringThing* __fastcall Copy_StringStringThing_00769c00(SStringStringThing* first, SStringStringThing* last, SStringStringThing* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}