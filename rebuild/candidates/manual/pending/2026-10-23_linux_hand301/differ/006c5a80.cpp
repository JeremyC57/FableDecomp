// Grade: DIFFER(745v737) -- member init, map/list/pointer construction and the reaction-array ctor loop match in shape; the deed loop differs: retail strength-reduces the deed index into a pointer over the name table (i != 0 becomes a pointer compare), fetches the effects def through an extra by-value temporary (two inc/dec pairs), and caches the reaction/attitude vector ends (link-time codegen); ours keeps the index and reloads the ends
// CTCHeroOpinionDeedLog::CTCHeroOpinionDeedLog(CThing* thing) -- builds the empty deed map and witness list, resolves the deed-effects def index of every deed type and caches each deed's per-attitude reaction animation and dialogue tag
#include <stdlib.h>
class CCharString {
public:
    void* Data;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
class CDefStringTable {
public:
    CCharString GetString(long index) const;            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object && --Object->RefCount == 0)
                Object->Destroy();
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
template<class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
struct COpinionDeedReactionDef {
    unsigned char _pad00[0x3c];
    vector<long> Attitudes;                             // +0x3C
    long Animation;                                     // +0x48 (CDefString index)
    unsigned long DialogueTag;                          // +0x4C
};
class COpinionDeedEffectsDef : public CDefClassBase {
public:
    unsigned char _pad08[0x54 - 0x08];
    vector<COpinionDeedReactionDef> Reaction;           // +0x54
    unsigned char _pad60[0x7c - 0x60];
    unsigned char Flags;                                // +0x7C
};
class CDefinitionManager {
public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
    bool GetDef(long index, CDefPointer<const COpinionDeedEffectsDef>* out) const;   // 0x006C76B0
    __forceinline CDefPointer<const COpinionDeedEffectsDef> GetDeedEffectsDef(long index) const
    {
        CDefPointer<const COpinionDeedEffectsDef> def;
        GetDef(index, &def);
        return def;
    }
};
struct CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObject* PItem;
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    __forceinline CIntelligentPointer() {}
    virtual ~CIntelligentPointer() {}
};
class CThing;
struct CDeedHUD {
    unsigned char _pad00[0x5a4];
    long DeedSlots[92];                                 // +0x5A4
};
struct CDisplayEngine {
    unsigned char _pad00[0x74];
    CDeedHUD* HUD;                                      // +0x74
};
class CTCBase {
public:
    CTCBase(CThing* thing);                             // 0x00686800
    virtual void v00();
    CThing* Thing;
    unsigned char _pad08[4];
    const CDefinitionManager* GetDefinitions() const;   // 0x006869D0
    CDisplayEngine* GetDisplayEngine() const;           // 0x00686A70
};
struct _Rb_tree_node_base {
    char _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct CDeedMap {
    _Rb_tree_node_base* _M_header;
    unsigned long _M_node_count;
    char _M_key_compare;
    __forceinline CDeedMap() : _M_header(0)
    {
        _M_header = (_Rb_tree_node_base*)malloc(0x18);
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct CWitnessList {
    _List_node_base* _M_node;
    __forceinline CWitnessList() : _M_node(0)
    {
        _List_node_base* node = (_List_node_base*)malloc(0x24);
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct COpinionDeedReaction {
    CCharString Animations[18];
    unsigned long DialogueTags[18];
};
extern const char* const g_OpinionDeedNames[92];       // 0x0137FBC0
class CTCHeroOpinionDeedLog : public CTCBase {
public:
    CDeedMap OpinionDeedMap;                            // +0x0C
    CWitnessList OpinionDeedsSearchingForWitnesses;     // +0x18
    long NextHandle;                                    // +0x1C
    CIntelligentPointer<CThing> VillageForCrimes;       // +0x20
    long WeaponOutDeedHandle;                           // +0x28
    bool WeaponOutCrimeEnabled;                         // +0x2C
    bool GuardsIgnoreCrimes;                            // +0x2D
    unsigned short DeedEffectsDefIndices[92];           // +0x2E
    COpinionDeedReaction DeedReactions[92];             // +0xE8
    CTCHeroOpinionDeedLog(CThing* thing);
    virtual void v00();
};

CTCHeroOpinionDeedLog::CTCHeroOpinionDeedLog(CThing* thing)
    : CTCBase(thing), NextHandle(0)
{
    long* slot = GetDisplayEngine()->HUD->DeedSlots;
    for (long i = 0; i < 92; ++i) {
        NProgressDisplay::DisplayProgress("CTCHeroOpinionDeedLog 1", -1.0f, false, false);
        DeedEffectsDefIndices[i] = (unsigned short)GetDefinitions()->GetDefGlobalIndexFromName(g_OpinionDeedNames[i]);
        if (i != 0) {
            CDefPointer<const COpinionDeedEffectsDef> def;
            def = GetDefinitions()->GetDeedEffectsDef(DeedEffectsDefIndices[i]);
            if (def->Flags & 4)
                slot[i] = -1;
            COpinionDeedReaction& reaction = DeedReactions[i];
            for (long attitude = 0; attitude < 18; ++attitude) {
                reaction.Animations[attitude] = "";
                reaction.DialogueTags[attitude] = 0;
                bool found = false;
                for (const COpinionDeedReactionDef* r = def->Reaction._M_start; r != def->Reaction._M_finish; ++r) {
                    for (const long* a = r->Attitudes._M_start; a != r->Attitudes._M_finish; ++a) {
                        NProgressDisplay::DisplayProgress("CTCHeroOpinionDeedLog 3", -1.0f, false, false);
                        if (*a == attitude && !found) {
                            reaction.Animations[attitude] = g_DefStringTable.GetString(r->Animation);
                            reaction.DialogueTags[attitude] = r->DialogueTag;
                            found = true;
                        }
                    }
                }
            }
        }
    }
    WeaponOutDeedHandle = -1;
    WeaponOutCrimeEnabled = true;
    GuardsIgnoreCrimes = false;
}
