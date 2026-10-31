// Grade: DIFFER(942v911) -- first pass: the eight float/bool transfers, save loop (def name per tattoo, "Tattoo"+index key built before the empty default via a two-argument Transfer overload) and load loop match in shape; retail parks the loaded CDefPointer in the dead context-parameter slot and caches its Object in esi across the GetDef branch (push_back copy + dtor), ours gives it its own frame slot (+4 frame) and reloads it, so every frame offset shifts
// Header-moved (retry wave 2 sec2c): shared def model (engine/CDefClassBase.h + CDefPointer verify text, NULL_DEF ctor PENDING); DIFFER(959v911)/204 -- worse than the parked local model (942/144): the header Release/frame shape adds 8 bytes of frame
// CTCHeroMorph::OnSerialise(CPersistContext*, long) -- persists the morph floats and the Teenager flag, then the tattoo list as "NumberOfTattoos" plus "Tattoo<i>" def names (loading resolves each name back to a def, into a scratch vector when the display engine says to discard them)
class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    operator const char*() const;                   // 0x0099E4C0
};
inline CCharString EmptyString()
{
    CCharString empty;
    return empty;
}
CCharString __fastcall GFIntToCharString(long value);                              // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);       // 0x0099F690
class CDefString {
public:
    long Index;
};
class CDefStringTable {
public:
    CCharString GetString(long index);              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_DEFPTR_INLINE __forceinline
enum ENullDef { NULL_DEF };                                  // enumerator name unproven (in neither PDB name list)
template <class T> class CDefPointer {
public:
    CDefPointer(ENullDef) : Object(0) { }                               // PROPOSED header addition (FableWin ??0..QAE@W4ENullDef@@@Z)
    FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer(const CDefPointer<U>& other) : Object(other.InternalGetObject())
    {
        if (Object) Object->AddRef();
    }
    FABLE_DEFPTR_INLINE ~CDefPointer() { Release(); }
    FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer& other) { Assign(other.Object); return *this; }
    template <class U> FABLE_DEFPTR_INLINE CDefPointer& operator=(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); return *this; }
    template <class U> FABLE_DEFPTR_INLINE void SetDynamicCast(const CDefPointer<U>& other) { Assign(other.InternalGetObject()); }
    CDefPointeeBase* InternalGetObject() const { return Object; }
    T* Get() const { return static_cast<T*>(Object); }
    T* operator->() const { return Get(); }
    T& operator*() const { return *Get(); }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Reset(T*);                                                      // unproven body (FableWin ?Reset@..QAEXPBV..@Z)
    FABLE_DEFPTR_INLINE void Release()
    {
        if (Object) {
            CDefPointeeBase* object = Object;
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointeeBase* Object;                                             // +0x00
private:
    FABLE_DEFPTR_INLINE void Assign(CDefPointeeBase* object)                                 // the inline body of operator= / SetDynamicCast
    {
        if (Object != object) {
            Release();
            Object = object;
            if (Object) Object->AddRef();
        }
    }
};

class CThingBaseDef : public CDefClassBase {};
inline void* operator new(unsigned int, void* where) throw() { return where; }
extern "C" void free(void*);                        // 0x00BFEA14
struct __false_type {};
typedef CDefPointer<const CThingBaseDef> CTattooPtr;
// Inline by-value name lookup over a named local (no NRVO in VC7.1).
inline CCharString GetDefNameString(const CDefClassBase* def)
{
    CCharString name = g_DefStringTable.GetString(def->GetInstantiationName().Index);
    return name;
}
void __fastcall DestroyRange(CTattooPtr* first, CTattooPtr* last, const __false_type&);   // 0x005B26E9
struct CTattooVector {
    CTattooPtr* _M_start;
    CTattooPtr* _M_finish;
    CTattooPtr* _M_end_of_storage;
    __forceinline CTattooVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CTattooVector()
    {
        DestroyRange(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    long size() const { return _M_finish - _M_start; }
    void reserve(unsigned int n);                   // 0x00721240
    void _M_insert_overflow(CTattooPtr* pos, const CTattooPtr& x, const __false_type&, unsigned int fill, bool atEnd);   // 0x005B3275
    __forceinline void push_back(const CTattooPtr& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CTattooPtr(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CDefinitionManager {
public:
    bool GetDef(const CCharString& name, CTattooPtr* out) const;    // 0x00528760
};
class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long Mode;                                      // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, float& value, const float& defaultValue);                        // 0x00410620
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);            // 0x004109A0
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);                          // 0x004045C0
    // Two-argument string overload: the empty default is built inside, after the caller's name expression.
    __forceinline void Transfer(const char* name, CCharString& value) { Transfer(name, value, EmptyString()); }
};
struct CDisplayEngine {
    unsigned char _pad[0x101];
    bool DiscardTattoos;                            // +0x101
};
class CTCBase {
public:
    virtual void v00();
    void* Thing;                                    // +0x04
    unsigned char _pad08[4];
    const CDefinitionManager* GetDefinitions() const;   // 0x006869D0
    CDisplayEngine* GetDisplayEngine() const;       // 0x00686A80
};
class CTCHeroMorph : public CTCBase {
public:
    unsigned char _pad0c[0x1c - 0x0c];
    CTattooVector Tattoos;                          // +0x1C
    unsigned char _pad28[0x40 - 0x28];
    float Strength;                                 // +0x40
    float Berserk;                                  // +0x44
    float Will;                                     // +0x48
    float Skill;                                    // +0x4C
    float Age;                                      // +0x50
    float Morality;                                 // +0x54
    float Fatness;                                  // +0x58
    float Tan;                                      // +0x5C
    bool Teenager;                                  // +0x60
    void OnSerialise(CPersistContext* context, long version);
};

void CTCHeroMorph::OnSerialise(CPersistContext* context, long)
{
    context->Transfer("Strength", Strength, 0.0f);
    context->Transfer("Will", Will, 0.0f);
    context->Transfer("Skill", Skill, 0.0f);
    context->Transfer("Age", Age, 0.0f);
    context->Transfer("Morality", Morality, 0.0f);
    context->Transfer("Fatness", Fatness, 0.0f);
    context->Transfer("Tan", Tan, 0.0f);
    context->Transfer("Teenager", Teenager, false);
    if (context->IsSaving()) {
        long count = Tattoos.size();
        context->Transfer("NumberOfTattoos", count, 0);
        for (long i = 0; i < count; ++i) {
            CCharString name = GetDefNameString(Tattoos._M_start[i].Get());
            context->Transfer("Tattoo" + GFIntToCharString(i), name);
        }
    } else {
        CTattooVector discarded;
        CTattooVector* target = GetDisplayEngine()->DiscardTattoos ? &discarded : &Tattoos;
        long count;
        context->Transfer("NumberOfTattoos", count, 0);
        target->reserve(count);
        for (long i = 0; i < count; ++i) {
            CCharString name;
            context->Transfer("Tattoo" + GFIntToCharString(i), name);
            CTattooPtr def(NULL_DEF);
            if (GetDefinitions()->GetDef(name, &def))
                target->push_back(def);
        }
    }
}
