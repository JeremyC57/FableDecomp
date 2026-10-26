// CTCCreatureNavigation::OnCreate() -- build the creature's possible navigators from its def's navigator type list (A* or flyer, each created by the world's navigator manager over a ground-navigator view of the creature) and start with the first one
// Leaf: OnCreate
//
// Retail 0x006CC6B0, 321 bytes (plain `ret`).
// Callees: CTCBase::GetDefinitionManager() @ 0x006869D0, CDefinitionManager::GetDef<CThingCreatureDef>(long, CDefPointer*) @
// 0x00513160, CTCBase::GetThingManager() @ 0x00686AE0, CGroundNavigatorExternalInterface ctor @ 0x006CD890 / dtor @ 0x006CDD50,
// CNavigatorAStar::Create @ 0x00A1CBF0 and CNavigatorFlyer::Create @ 0x00A18F70 (both `__fastcall`), the inlined push_back's
// growth path `_M_insert_overflow` @ 0x006CD5A0, the def's virtual slot 1 on release.
// Layout: Ego_r.pdb CTCCreatureNavigation PNavigator +0x0c, PossibleNavigators +0x10 (12-byte STLport vector in retail);
// Shape: each case loads the creature and the navigator manager into locals before building the temporary ground-navigator
// interface (the call's right-to-left order would build the interface first); push_back takes its value by value (one shared
// slot for both cases) and its growth path has a same-TU auto_inline(off) stand-in (one tag slot); the type list is bound once
// as `const vector& types = def->NavigatorTypes` -- spelled `def->` in the loop, VC7.1 proves the def non-null and drops the
// release's null test.
// CThingCreatureDef NavigatorTypes at retail +0xc8 (PDB +0xcc: an earlier 16-byte PDB vector is 12 bytes in retail);
// the thing manager's navigator manager +0x34; CThing's definition index is the word at +0x8c.
#include <new>

enum ENavigatorType { NAVIGATOR_TYPE_NONE = 0, NAVIGATOR_TYPE_ASTAR = 1, NAVIGATOR_TYPE_FLYER = 2 };
struct __true_type { __true_type() { } };

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CNavigatorTypeVector {                           // STLport vector<ENavigatorType>
    ENavigatorType* _M_start;
    ENavigatorType* _M_finish;
    ENavigatorType* _M_end_of_storage;
    const ENavigatorType* begin() const { return _M_start; }
    const ENavigatorType* end() const { return _M_finish; }
};
class CThingCreatureDef : public CDefPointeeBase { public:
    unsigned char        _pad08[0xc8 - 0x08];
    CNavigatorTypeVector NavigatorTypes;                // +0xc8
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<const CThingCreatureDef>* out);   // 0x00513160
};
void* FindDef(long index);
#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line GetDef template: it writes the out pointer only inline.
bool CDefinitionManager::GetDef(long index, CDefPointer<const CThingCreatureDef>* out)
{
    void* def = FindDef(index);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                            // +0x8c
};
class CThingCreatureBase;
class CNavigatorManager;
class CThingManager { public:
    unsigned char      _pad00[0x34];
    CNavigatorManager* NavigatorManager;                // +0x34
};
class CNavigatorExternalInterface { public:
    virtual void v00();
    virtual ~CNavigatorExternalInterface();
};
class CGroundNavigatorExternalInterface : public CNavigatorExternalInterface { public:
    CThingCreatureBase* Creature;                       // +0x04
    float               Radius;                         // +0x08
    CGroundNavigatorExternalInterface(CThingCreatureBase* creature);   // 0x006CD890
    ~CGroundNavigatorExternalInterface();               // 0x006CDD50
};
class CNavigatorBase;
class CNavigatorAStar { public:
    static CNavigatorBase* __fastcall Create(CNavigatorManager* manager, CNavigatorExternalInterface* external);   // 0x00A1CBF0
};
class CNavigatorFlyer { public:
    static CNavigatorBase* __fastcall Create(CNavigatorManager* manager, CNavigatorExternalInterface* external);   // 0x00A18F70
};
struct CNavigatorVector {                               // STLport vector<CNavigatorBase*>
    CNavigatorBase** _M_start;
    CNavigatorBase** _M_finish;
    CNavigatorBase** _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    CNavigatorBase*& front() { return *_M_start; }
    void _M_insert_overflow(CNavigatorBase** pos, CNavigatorBase* const& x, const __true_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x006CD5A0
    void push_back(CNavigatorBase* x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CNavigatorBase*(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1UL, true);
    }
};

void* GrowVectorStorage(void* vector, unsigned long count);
#pragma auto_inline(off)
// Same-TU body of the growth template: reads the value only through its const reference and never touches the empty tag.
void CNavigatorVector::_M_insert_overflow(CNavigatorBase** pos, CNavigatorBase* const& x, const __true_type& tag,
                                          unsigned long fillLength, bool atEnd)
{
    CNavigatorBase** p = (CNavigatorBase**)GrowVectorStorage(this, fillLength);
    if (p)
        *p = x;
}
#pragma auto_inline(on)

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CDefinitionManager* GetDefinitionManager();         // 0x006869D0
    CThingManager* GetThingManager();                   // 0x00686AE0
};
class CTCCreatureNavigation : public CTCBase {
public:
    CNavigatorBase*  PNavigator;                        // +0x0c
    CNavigatorVector PossibleNavigators;                // +0x10
    void OnCreate();
};

void CTCCreatureNavigation::OnCreate()
{
    CDefPointer<const CThingCreatureDef> def;
    GetDefinitionManager()->GetDef(ParentThing->DefIndex, &def);
    const CNavigatorTypeVector& types = def->NavigatorTypes;
    for (const ENavigatorType* it = types.begin(); it != types.end(); ++it) {
        switch (*it) {
        case NAVIGATOR_TYPE_ASTAR: {
            CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
            CNavigatorManager* manager = GetThingManager()->NavigatorManager;
            PossibleNavigators.push_back(CNavigatorAStar::Create(manager, &CGroundNavigatorExternalInterface(creature)));
            break;
        }
        case NAVIGATOR_TYPE_FLYER: {
            CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
            CNavigatorManager* manager = GetThingManager()->NavigatorManager;
            PossibleNavigators.push_back(CNavigatorFlyer::Create(manager, &CGroundNavigatorExternalInterface(creature)));
            break;
        }
        }
    }
    if (!PossibleNavigators.empty())
        PNavigator = PossibleNavigators.front();
}