// CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(long defIndex) -- wear a modifier def, first removing what
// already covers its body areas; a type-0 modifier posts the cover-change messages (0x39/0x3A/0x3B) for its head, torso and
// leg areas; stamps the frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706950, 444 bytes (+ int3 padding), `ret 4`.
// Leaf: AddAppearanceModifierWithReplace
// Callees: as the landed twin AddAppearanceModifier (0x00706880) -- GetDefinitions 0x006869D0, GetDef<CThingObjectDef> 0x0042B040,
// GetSubDef<CAppearanceModifierDef> 0x0057FF92, _M_insert_overflow 0x00708E40, GetWorld() const 0x00686A80, GetWorldFrame
// 0x0049D870 -- plus RemoveAppearanceModifiersCoveringBodyArea 0x007066A0, GetWorld() 0x00686A70 and
// CMessageEventManager::AddMessage 0x00687540; g_FramesPerSecond 0x01375550.
// Shape: the twin's body (same-TU stand-ins, by-hand releases); each message is its own block with a named zero position and a
// named `CWorld* world = GetWorld();` so the zero stores and the GetWorld call precede the argument pushes and the manager is
// read `mov ecx,[eax+60h]` just before the call (an inline `GetWorld()->MessageEventManager->AddMessage(...)` pushes first).
// Layout: local model like the twin -- engine/CTCHeroAttachableAppearanceModifiers.h carries the 16-byte PDB containers;
// retail CAppearanceModifierDef body-area bits +0x40, type +0x50.
#include <new>

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

template<class T> class CDefOutPointer {           // a plain out slot (the def is released by hand)
public:
    T* Object;
    CDefOutPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefOutPointer<const T>* out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x40 - 0x08];
    unsigned long BodyAreasCovered;                 // +0x40 (bit 4, bit 7, bit 8 -> messages 0x39, 0x3A, 0x3B)
    unsigned char _pad44[0x50 - 0x44];
    long Type;                                      // +0x50 EHeroAttachableAppearanceModifierType
};
class CThingObjectDef : public CDefClassBase { };

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefOutPointer<const T>* out) const;   // 0x0042B040 for CThingObjectDef
};
class CDefinitionHolder : public CDefinitionManager { };

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CThing_Retail;
struct CMessageEvent_Retail;
struct CMessageEventManager {
    CMessageEvent_Retail* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing_Retail* from, CThing_Retail* to, float value1, float value2, long data);   // 0x00687540
};
struct CWorld {
    unsigned char _pad00[0x60];
    CMessageEventManager* MessageEventManager;      // +0x60
    static long GetWorldFrame();                    // 0x0049D870
};
extern long g_FramesPerSecond;                      // 0x01375550

struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    bool Active;                                    // +0x04
    CModifierEntry(long defIndex, bool active) : DefIndex(defIndex), Active(active) {}
};

struct __true_type { __true_type() {} };           // empty tag with a user constructor (no byte store)

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x00708E40
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

class CThing;
class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    unsigned long FrameLastChanged;                 // +0x10
    unsigned char Changed : 1;                      // +0x14 bit 0
    unsigned char RecreatePrimitives : 1;
    unsigned char AppearanceModifiersLoaded : 1;
    unsigned char _pad15[0x30 - 0x15];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0 (CTCBase)
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void RemoveAppearanceModifiersCoveringBodyArea(unsigned long bodyAreas, long type);   // 0x007066A0
    void AddAppearanceModifierWithReplace(long defIndex);
};

void* AllocateStorage_00708e40(unsigned long bytes);
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend)
{
    unsigned long oldSize = (unsigned long)(_M_finish - _M_start);
    unsigned long len = oldSize + (oldSize > fill_len ? oldSize : fill_len);
    T* newStart = (T*)AllocateStorage_00708e40(len * sizeof(T));
    T* out = newStart;
    for (T* it = _M_start; it != position; ++it, ++out)
        new (out) T(*it);
    for (unsigned long i = 0; i < fill_len; ++i, ++out)
        new (out) T(x);
    _M_start = newStart;
    _M_finish = out;
    _M_end_of_storage = newStart + len;
}
#pragma auto_inline(on)
void* FindDef_0042b040(long index);
void* FindSubDef_0057ff92(const void* def);
#pragma auto_inline(off)
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefOutPointer<const T>* out) const
{
    T* def = (T*)FindDef_0042b040(index);
    if (!def)
        return false;
    ++def->RefCount;
    out->Object = def;
    return true;
}
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefOutPointer<const T>* out) const
{
    T* def = (T*)FindSubDef_0057ff92(this);
    if (!def)
        return false;
    ++def->RefCount;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
void CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(long defIndex)
{
    CDefOutPointer<const CThingObjectDef> thingOut;
    CDefOutPointer<const CAppearanceModifierDef> appearanceOut;
    if (GetDefinitions()->GetDef(defIndex, &thingOut) && thingOut.Object->GetSubDef(&appearanceOut)) {
        const CAppearanceModifierDef* appearance = appearanceOut.Object;
        RemoveAppearanceModifiersCoveringBodyArea(appearance->BodyAreasCovered, appearance->Type);
        AppearanceModifiers._M_start[appearance->Type].push_back(CModifierEntry(defIndex, true));
        Changed = true;
        if (appearance->Type == 0) {
            if (appearance->BodyAreasCovered & 0x10) {
                C3DVector position(0.0f, 0.0f, 0.0f);
                CWorld* world = GetWorld();
                world->MessageEventManager->AddMessage(0x39, g_FramesPerSecond, position, 0, 0, 0, 0, defIndex);
            }
            if (appearance->BodyAreasCovered & 0x80) {
                C3DVector position(0.0f, 0.0f, 0.0f);
                CWorld* world = GetWorld();
                world->MessageEventManager->AddMessage(0x3a, g_FramesPerSecond, position, 0, 0, 0, 0, defIndex);
            }
            if (appearance->BodyAreasCovered & 0x100) {
                C3DVector position(0.0f, 0.0f, 0.0f);
                CWorld* world = GetWorld();
                world->MessageEventManager->AddMessage(0x3b, g_FramesPerSecond, position, 0, 0, 0, 0, defIndex);
            }
        }
    }
    FrameLastChanged = ((const CTCHeroAttachableAppearanceModifiers*)this)->GetWorld()->GetWorldFrame();
    if (appearanceOut.Object && --appearanceOut.Object->RefCount == 0)
        appearanceOut.Object->Destroy();
    if (thingOut.Object && --thingOut.Object->RefCount == 0)
        thingOut.Object->Destroy();
}
