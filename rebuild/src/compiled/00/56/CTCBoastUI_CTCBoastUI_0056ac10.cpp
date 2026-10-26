// CTCBoastUI::CTCBoastUI(CThing*) -- boast-UI thing component ctor: empty boast-group map and component vector, flags (allowed to open), registered for updates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056AC10, 112 bytes (padding included).
// Leaf: CTCBoastUI
// Label correction: the catalog row says CMesh::CMesh, but the vtable store 0x0124DCFC is CTCBoastUI
// (rtti-vtables.tsv) and the members match the PDB CTCBoastUI (BoastGroups, Active, Components,
// Quitting, AllowedToOpen, ActivationRequested, ActivateLater; retail STLport containers are 12 bytes,
// so the PDB/engine-header offsets past +0x18 sit 4 and 8 bytes high).
// Shape: CTCBase(parent) (0x00686800, 0xc bytes), vtable; the STLport map's _Rb_tree ctor inlined:
// the header proxy zeroed, then the 0x34-byte header node malloc'd (0x00BFEA0E, `add esp,4` deferred),
// node count 0, header red with null parent and leftmost/rightmost = header (re-read through the
// member); Active = false, the vector's three words zero, Quitting/AllowedToOpen(1)/
// ActivationRequested/ActivateLater; finally the CTCBase member at 0x00686ED0 (tests +9, the
// "in global update list" flag) registers the component.
class CThing;
struct CMenuEntry;
struct CComponent;
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CTCBase { public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    CTCBase(CThing* parent);                        // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

struct CBoast { unsigned char _body[0x20]; };       // CQuestManager::CBoast

struct _Rb_tree_node_base {
    char                 _M_color;                  // +0
    _Rb_tree_node_base*  _M_parent;                 // +4
    _Rb_tree_node_base*  _M_left;                   // +8
    _Rb_tree_node_base*  _M_right;                  // +0xc
};

struct SBoastNode : _Rb_tree_node_base {            // 0x34 bytes
    CMenuEntry* first;
    CBoast      second;
};

struct CBoastMap {                                  // STLport map<CMenuEntry*, CBoast>
    SBoastNode*   _M_header;                        // +0
    unsigned long _M_node_count;                    // +4
    unsigned char _M_key_compare;                   // +8 less<> (empty)
    CBoastMap()
        : _M_header(0)
    {
        _M_header = (SBoastNode*)malloc(sizeof(SBoastNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;                    // red
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CComponentVector {                           // STLport vector<CComponent*>
    CComponent** _M_start;
    CComponent** _M_finish;
    CComponent** _M_end_of_storage;
    CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

class CTCBoastUI : public CTCBase { public:                       // vtable 0x0124DCFC
    CBoastMap        BoastGroups;                   // +0x0c
    bool             Active;                        // +0x18
    CComponentVector Components;                    // +0x1c
    bool             Quitting;                      // +0x28
    bool             AllowedToOpen;                 // +0x29
    bool             ActivationRequested;           // +0x2a
    bool             ActivateLater;                 // +0x2b
    virtual void v00();
    CTCBoastUI(CThing* parent);
};

CTCBoastUI::CTCBoastUI(CThing* parent)
    : CTCBase(parent)
    , Active(false)
    , Quitting(false)
    , AllowedToOpen(true)
    , ActivationRequested(false)
    , ActivateLater(false)
{
    AddToGlobalTCs();
}