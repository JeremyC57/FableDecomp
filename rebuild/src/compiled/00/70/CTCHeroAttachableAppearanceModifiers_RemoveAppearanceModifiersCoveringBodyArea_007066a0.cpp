// CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea(long bodyAreaFlags, EHeroAttachableAppearanceModifierType type) -- collect every worn modifier of the type whose appearance def covers any of the body areas (for type 2, one that only shares the 0x100 area with a 0x100 request is kept), remove them one by one and stamp the change frame
// Leaf: RemoveAppearanceModifiersCoveringBodyArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007066A0, 352 bytes (`ret 8`). Ego_r.pdb
// `?RemoveAppearanceModifiersCoveringBodyArea@CTCHeroAttachableAppearanceModifiers@@QAEXJW4EHeroAttachableAppearanceModifierType@@@Z`.
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the local STLport list<long>), CTCBase::GetDefinitions @ 0x006869D0,
// CDefinitionManager::GetDef<CThingObjectDef> @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef> @ 0x0057FF92,
// RemoveAppearanceModifier(long) @ 0x00706370, CTCBase::GetWorld @ 0x00686A80, the world frame getter @ 0x0049D870 (static).
// Layout: as the landed RemoveAllAppearanceModifiers 0x00706640 (local model; retail 12-byte containers): FrameLastChanged +0x10,
// AppearanceModifiers +0x30 (vector< vector<CModifierEntry{DefIndex, Rendered}> >). CAppearanceModifierDef CoversBodyAreaFlags
// +0x40 (Ego_r.pdb +0x48 less two 4-byte container shrinks).
// Shape: same-TU noinline stand-in bodies for GetDef / GetSubDef (the out pointers do not escape: the sub def stays in ebp and the
// releases are `dec [reg+4]`; with both def pointers declared at the loop top the failed-GetDef path skips the sub def release);
// the type's vector is a named reference; push_back reads the old tail once.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CThing;
struct CWorld_Calls {
    static long __fastcall GetFrame();                  // 0x0049D870
};
struct CModifierEntry {
    long DefIndex;
    bool Rendered;
};
struct CModifierVector {
    CModifierEntry* _Myfirst;
    CModifierEntry* _Mylast;
    CModifierEntry* _Myend;
    CModifierEntry* begin() { return _Myfirst; }
    CModifierEntry* end() { return _Mylast; }
};
struct CModifierVectorVector {
    CModifierVector* _Myfirst;
    CModifierVector* _Mylast;
    CModifierVector* _Myend;
    CModifierVector& operator[](long i) { return _Myfirst[i]; }
};

struct SLongNode {
    SLongNode* _M_next;
    SLongNode* _M_prev;
    long       _M_data;
};
struct CLongList {                                      // STLport list<long>
    SLongNode* _M_node;
    CLongList()
    {
        SLongNode* node = (SLongNode*)malloc(sizeof(SLongNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CLongList()
    {
        clear();
        free(_M_node);
    }
    void clear()
    {
        SLongNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SLongNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    void push_back(const long& x)
    {
        SLongNode* pos = _M_node;
        SLongNode* node = (SLongNode*)malloc(sizeof(SLongNode));
        new (&node->_M_data) long(x);
        SLongNode* prev = pos->_M_prev;
        node->_M_next = pos;
        node->_M_prev = prev;
        prev->_M_next = node;
        pos->_M_prev = node;
    }
};

template<class T> class CDefPointer;
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object)
            Object->Release();
    }
    T* operator->() const { return Object; }
};
class CThingObjectDef : public CDefClassBase { };
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x40 - 0x08];
    long CoversBodyAreaFlags;                           // +0x40
};
class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 for CThingObjectDef
};
void* FindDef_007066a0(long index);
void* FindSubDef_007066a0(const void* def);
#pragma auto_inline(off)
// Same-TU stand-in bodies for the two def lookups (instantiated elsewhere in retail).
template<class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    out->Object = (const T*)FindDef_007066a0(index);
    return out->Object != 0;
}
template<class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    out->Object = (const T*)FindSubDef_007066a0(this);
    return out->Object != 0;
}
#pragma auto_inline(on)

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CWorld_Calls& GetWorld() const;                     // 0x00686A80
    const CDefinitionManager* GetDefinitions();         // 0x006869D0
};
typedef long EHeroAttachableAppearanceModifierType;

class CTCHeroAttachableAppearanceModifiers : public CTCBase { public:
    void* __vftable_0c;                                 // +0x0c secondary vptr
    unsigned long FrameLastChanged;                     // +0x10
    bool Changed : 1;                                   // +0x14 bit 0
    bool RecreatePrimitives : 1;                        // +0x14 bit 1
    bool AppearanceModifiersLoaded : 1;                 // +0x14 bit 2
    unsigned char PrimHandles[0xc];                     // +0x18
    unsigned char HiddenMaterialNames[0xc];             // +0x24
    CModifierVectorVector AppearanceModifiers;          // +0x30
    unsigned char Graphics[0xc];                        // +0x3c
    void RemoveAppearanceModifier(long defIndex);       // 0x00706370
    void RemoveAppearanceModifiersCoveringBodyArea(long bodyAreaFlags, EHeroAttachableAppearanceModifierType type);
};

void CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea(long bodyAreaFlags,
                                                                                     EHeroAttachableAppearanceModifierType type)
{
    CModifierVector& modifiers = AppearanceModifiers[type];
    CLongList toRemove;
    for (CModifierEntry* it = modifiers.begin(); it != modifiers.end(); ++it) {
        CDefPointer<const CThingObjectDef> thingDef;
        CDefPointer<const CAppearanceModifierDef> modifierDef;
        if (GetDefinitions()->GetDef(it->DefIndex, &thingDef)) {
            if (thingDef->GetSubDef(&modifierDef)) {
                long covers = modifierDef->CoversBodyAreaFlags;
                if (covers & bodyAreaFlags) {
                    bool remove = true;
                    if (type == 2 && (bodyAreaFlags & 0x100) && (covers & 0x100))
                        remove = (covers & bodyAreaFlags & ~0x100) != 0;
                    if (remove)
                        toRemove.push_back(it->DefIndex);
                }
            }
        }
    }
    for (SLongNode* node = toRemove._M_node->_M_next; node != toRemove._M_node; node = node->_M_next)
        RemoveAppearanceModifier(node->_M_data);
    FrameLastChanged = GetWorld().GetFrame();
}