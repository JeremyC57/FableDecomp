// CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface(const CGroundNavigatorExternalInterface& other) -- the copy constructor: copies the creature and radius, then registers the creature navigation's agent vector in CNavigatorExternalInterface::AgentLists unless it is already there
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CE670, length 151 bytes (padded to 160).
// Callees: the component-map LowerBound @ 0x0040F020 (kind 7), malloc @ 0x00BFEA0E (the 12-byte list node).
// Globals: CNavigatorExternalInterface::AgentLists @ 0x013D2834 (STLport list<vector<const CSteeringAgent*>*>, its sentinel
// pointer; Ego_r.pdb `dynamic initializer for 'CNavigatorExternalInterface::AgentLists'`).
// Vtable: CGroundNavigatorExternalInterface 0x0125EC9C (the base CNavigatorExternalInterface store is dead and removed).
// Shape: the vector pointer is the inline GetPNavigatorObstacles (0x006CD7B0, Ego_r.pdb returns
// vector<const CSteeringAgent*>*: &CTCCreatureNavigation::Agents, null without a kind-7 component); an inlined
// `find` over the list then `push_back` (node from malloc, null-checked placement copy of the value, linked before the
// sentinel).
// Model: local polymorphic classes (engine/CGroundNavigatorExternalInterface.h flattens the vptr; Creature +4, Radius +8).
// Leaf: ??0CGroundNavigatorExternalInterface
#include <new>
extern "C" void* malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void free(void* p);                     // 0x00BFEA14
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020 (the key by address)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
class CThingCreatureBase { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    template <class T> __forceinline bool GetInterface(T** out, long kind)   // the out-parameter lookup: the component stays in eax
    {
        if (HasTC(kind)) {
            *out = (T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
class CSteeringAgent;
namespace std { template <class T> class allocator; template <class T, class A> class vector; }
typedef std::vector<const CSteeringAgent*, std::allocator<const CSteeringAgent*> > CSteeringAgentVector;
struct CTCCreatureNavigation_Retail {              // component kind 7
    unsigned char _pad[0x40];
    unsigned char Agents[12];                      // +0x40 vector<const CSteeringAgent*> (STLport, 12 bytes; PDB +0x44)
};
struct _List_node {
    _List_node*          _M_next;
    _List_node*          _M_prev;
    CSteeringAgentVector* _M_data;
};
struct CAgentLists_Retail {                        // STLport list<vector<const CSteeringAgent*>*>: the sentinel node pointer
    _List_node* _M_node;
};
class CNavigatorExternalInterface { public:               // vtable 0x01241D74
    virtual void v00();
    virtual ~CNavigatorExternalInterface() {}
    static CAgentLists_Retail AgentLists;          // 0x013D2834 (the agent vectors of every live ground navigator)
};
class CGroundNavigatorExternalInterface : public CNavigatorExternalInterface { public:   // vtable 0x0125EC9C
    CThingCreatureBase* Creature;                  // +0x04
    float               Radius;                    // +0x08
    __forceinline CSteeringAgentVector* GetPNavigatorObstacles()   // inline twin of 0x006CD7B0
    {
        CTCCreatureNavigation_Retail* navigation;
        if (Creature->GetInterface(&navigation, 7))
            return (CSteeringAgentVector*)navigation->Agents;
        return 0;
    }
    virtual void v00();
    CGroundNavigatorExternalInterface(const CGroundNavigatorExternalInterface& other);
    virtual ~CGroundNavigatorExternalInterface();
};

CGroundNavigatorExternalInterface::CGroundNavigatorExternalInterface(const CGroundNavigatorExternalInterface& other)
    : Creature(other.Creature), Radius(other.Radius)
{
    CSteeringAgentVector* agents = GetPNavigatorObstacles();
    _List_node* sentinel = AgentLists._M_node;
    _List_node* it = sentinel->_M_next;
    while (it != sentinel && it->_M_data != agents)
        it = it->_M_next;
    if (it == sentinel) {
        _List_node* node = (_List_node*)malloc(sizeof(_List_node));
        new (&node->_M_data) CSteeringAgentVector*(agents);
        _List_node* prev = sentinel->_M_prev;
        node->_M_next = sentinel;
        node->_M_prev = prev;
        prev->_M_next = node;
        sentinel->_M_prev = node;
    }
}