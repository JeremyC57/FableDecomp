// CThingManager::SaveToScript(CCharString& script, long mapNumber, vector<CCharString>* names) -- write the version line, collect the serialisable things on the map and save them
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00522600, length 277 bytes (+ int3 padding), `ret 0Ch`.
// Callees: GFIntToCharString @ 0x0099F830, operator+(const char*, const CCharString&) @ 0x0099F690,
// operator+(const CCharString&, const char*) @ 0x0099F600 (all three LTCG custom convention = __fastcall, hidden result in
// ecx), CCharString::operator+= @ 0x0099F0A0, ~CCharString @ 0x0099EAE0, CThing::IsForceNoSerialise @ 0x004C7BD0, CThing
// virtual slot 22 (+0x58, "should serialise" predicate), the vector<CThing*> reallocating insert @ 0x00436AC0 (catalog
// "Vector_InsertN"), the thing-list writer @ 0x00521EB0 (catalog "Save_WriteEntityHierarchy"), free @ 0x00BFEA14.
// Strings: 0x01245BA4 "Version ", 0x012353B8 ";\r\n".
// Layout (retail CThingManager): Things (STLport list<CThing*> sentinel) +0x18, SerialiseVersionNumber +0xd8 (PDB +0x104);
// CThing CurrentMapNumber (short) +0x8e.
// Shape: the version line is one `+=` of a three-operand concatenation; the retail push_back is inlined (the thing is a
// named address-taken local, the empty tag is homed in the dead `script` slot) and the grow template gets a same-TU
// `auto_inline(off)` body that only reads its const reference and keeps the vector's address to itself; the writer
// 0x00521EB0 gets a same-TU `auto_inline(off)` stand-in that only reads the vector (retail's LTCG knew both, so `last`
// stays in esi through the loop and `first` in ebx across the writer call); the list end is re-read every pass.
#include <new>

class CCharString { public:
    void* m_pData;
    ~CCharString();                                            // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);         // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);                          // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);  // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);  // 0x0099F600

extern "C" void __cdecl free(void* block);                     // 0x00BFEA14

struct SScalarTag { SScalarTag() { } };

template <class T> struct CRetailVector {                      // retail std::vector: first, last, end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
    ~CRetailVector() { if (first) free(first); }
    void InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(const T& value)
    {
        if (last != end) {
            new (last) T(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void* GrowVectorStorage_00436ac0(unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the vector growth template (InsertGrow, instantiated in retail's TU): reads the value through its const
// reference into the new storage and never touches the empty tag (the landed push_back idiom).
template <class T>
__declspec(noinline) void CRetailVector<T>::InsertGrow(T* where, const T& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    T* p = (T*)GrowVectorStorage_00436ac0(count);
    if (p) {
        *p = value;
        first = p;
        last = p + 1;
        end = p + count;
    }
}
#pragma auto_inline(on)

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual bool IsToBeSerialised();                           // slot 22 (+0x58)
    unsigned char _pad04[0x8e - 0x04];
    short         CurrentMapNumber;                            // +0x8e
    bool IsForceNoSerialise();                                 // 0x004C7BD0
};

struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
    CThing*         _M_data;
};

struct CStringVector;

class CThingManager {
public:
    void*           __vftable;                                 // +0x00
    unsigned char   _pad04[0x18 - 0x04];
    SThingListNode* Things;                                    // +0x18 list<CThing*> sentinel
    unsigned char   _pad1c[0xd8 - 0x1c];
    long            SerialiseVersionNumber;                    // +0xd8
    void SaveThingList(CCharString& script, long mode, CRetailVector<CThing*>& things, CStringVector* names);   // 0x00521EB0
    void SaveToScript(CCharString& script, long mapNumber, CStringVector* names);
};

extern const char g_strVersionSpace[];                         // 0x01245BA4 "Version "
extern const char g_strSemicolonCRLF[];                        // 0x012353B8 ";\r\n"

void WriteThingToScript(CCharString& script, CThing* thing, long mode, CStringVector* names);
#pragma auto_inline(off)
// Same-TU stand-in for the thing-list writer 0x00521EB0 (retail's LTCG saw that it only reads the vector, so the
// caller keeps `first` in ebx across the call).
__declspec(noinline) void CThingManager::SaveThingList(CCharString& script, long mode, CRetailVector<CThing*>& things, CStringVector* names)
{
    for (CThing** p = things.first; p != things.last; ++p)
        WriteThingToScript(script, *p, mode, names);
}
#pragma auto_inline(on)

void CThingManager::SaveToScript(CCharString& script, long mapNumber, CStringVector* names)
{
    script += g_strVersionSpace + GFIntToCharString(SerialiseVersionNumber) + g_strSemicolonCRLF;
    CRetailVector<CThing*> things;
    for (SThingListNode* node = Things->_M_next; node != Things; node = node->_M_next) {
        CThing* thing = node->_M_data;
        if (thing->CurrentMapNumber == mapNumber && !thing->IsForceNoSerialise() && thing->IsToBeSerialised())
            things.push_back(thing);
    }
    SaveThingList(script, 2, things, names);
}