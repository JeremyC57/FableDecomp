// _global::Copy_Record2C_00781720(SRecord2C*, SRecord2C*, SRecord2C*, const random_access_iterator_tag&, int*) -- SGI __copy over 0x2C-byte records (a name, plain fields and two intelligent pointers)
// Leaf: Copy_Record2C_00781720
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00781720, length 137 bytes (+ int3 padding).
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the advanced result is returned.
// Element: 0x2C bytes = CCharString +0 (CCharString::operator= @ 0x0099EFB0), three words +4..+0xC, a byte +0x10, two
// words +0x14/+0x18, then two CIntelligentPointers +0x1C/+0x24 (SetPItem(other.GetPItem()), 0x00A01B50 / 0x00A01B90).
// The manifest's `_Copy_backward<pair<CCharString,C3DHelperPoint>*>` label is propagated (this is a forward copy of a
// 0x2C-byte record); the record stays descriptive.
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

struct SRecord2C {                           // 0x2C bytes
    CCharString                 Name;        // +0x00
    long                        Value0;      // +0x04
    long                        Value1;      // +0x08
    long                        Value2;      // +0x0C
    bool                        Flag;        // +0x10
    long                        Value3;      // +0x14
    long                        Value4;      // +0x18
    CIntelligentPointer<CThing> PThing0;     // +0x1C
    CIntelligentPointer<CThing> PThing1;     // +0x24
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SRecord2C* __fastcall Copy_Record2C_00781720(SRecord2C* first, SRecord2C* last, SRecord2C* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}