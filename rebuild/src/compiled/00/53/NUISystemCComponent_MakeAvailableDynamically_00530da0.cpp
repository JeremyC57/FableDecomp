// NUISystem::CComponent::MakeAvailableDynamically() -- brighten every UI state's colour (the inline MakeAvailable loop: R, G, B of each CUIState scaled by 4/3), then fade straight to the current state's colour keeping the component's alpha
// Leaf: ?MakeAvailableDynamically@CComponent
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530DA0, 288 bytes, `__fastcall`, plain `ret`; virtual, CComponent vtable 0x0124608C
//   slot 95 (and the 40 derived NUISystem vtables, e.g. CSprite 0x0122F5D4, CYesNo 0x0122F884). Label correction: the
//   manifest's "DynamicListEntry::MakeAvailableWithCallbacks" is propagated; Ego_r.pdb has
//   ?MakeAvailableDynamically@CComponent@NUISystem@@UAEXXZ, and the vtable pairs it with MakeAvailable 0x00530CE0 (slot 94).
// Callees: own slot 82 (+0x148) GetStates() 0x0052F0E0, slot 50 (+0xC8) GetCurrentState() 0x00533B30, slot 14 (+0x38)
//   GetColour() 0x0052F500 (CRGBColour by value through a hidden pointer), slot 38 (+0x98) ChangeColour(const CRGBColour&,
//   float, bool) 0x0052EC60, _ftol 0x00BFEA70; the factor is the float at 0x01246054 (4/3).
// Layout / model: the landed MakeAvailable 0x00530CE0 (STLport map<unsigned long, CUIState>, CUIState::Colour +0x18); the
//   CRGBColour copy constructor goes through Set (byte-wise copies, as the landed CRGBColour models).
// Shape: `CComponent::MakeAvailable()` qualified and __forceinline (VC7.1 does not auto-inline the loop; retail has it inline
//   with its loop-head alignment); the colour copy-constructed
//   from the current state, its alpha replaced from GetColour(), then ChangeColour(colour, 0, false).
namespace NUISystem {

struct CRGBColour {                         // 4 bytes: B, G, R, A
    unsigned char B;                        // +0x00
    unsigned char G;                        // +0x01
    unsigned char R;                        // +0x02
    unsigned char A;                        // +0x03
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
struct C2DVector { float X, Y; };
enum EStateChangeType { STATE_CHANGE_NONE };
class CUIState { public:                           // Ego_r.pdb 36 bytes
    unsigned long    StateChangeFlag;       // +0x00
    EStateChangeType StateChangeType;       // +0x04
    C2DVector        Position;              // +0x08
    C2DVector        Zoom;                  // +0x10
    CRGBColour       Colour;                // +0x18
    float            UpdateTime;            // +0x1C
    bool             LinearChange;          // +0x20
};

struct SStateNode {                         // STLport _Rb_tree_node<pair<const unsigned long, CUIState> >
    bool          _M_color;                 // +0x00
    SStateNode*   _M_parent;                // +0x04
    SStateNode*   _M_left;                  // +0x08
    SStateNode*   _M_right;                 // +0x0C
    unsigned long first;                    // +0x10
    CUIState      second;                   // +0x14
};
struct CStateMapIterator {                  // STLport _Rb_tree_iterator
    SStateNode* _M_node;
    CStateMapIterator(SStateNode* node) : _M_node(node) { }
    bool operator!=(const CStateMapIterator& other) const { return _M_node != other._M_node; }
    SStateNode* operator->() const { return _M_node; }
    CStateMapIterator& operator++()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        } else {
            SStateNode* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
        return *this;
    }
};
struct CStateMap {                          // STLport map<unsigned long, CUIState> (retail 12 bytes)
    SStateNode*   _M_header;                // +0x00
    unsigned long _M_node_count;            // +0x04
    unsigned char _M_key_compare;           // +0x08
    CStateMapIterator begin() { return CStateMapIterator(_M_header->_M_left); }
    CStateMapIterator end() { return CStateMapIterator(_M_header); }
};

class CComponent {
public:
    virtual ~CComponent();                  // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual CRGBColour GetColour() const;   // slot 14 (+0x38), 0x0052F500: the colour at +0x84 by value
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void ChangeColour(const CRGBColour& colour, float time, bool unused);   // slot 38 (+0x98), 0x0052EC60
    virtual void _v39();
    virtual void _v40();
    virtual void _v41();
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void _v46();
    virtual void _v47();
    virtual void _v48();
    virtual void _v49();
    virtual CUIState* GetCurrentState();      // slot 50 (+0xC8), 0x00533B30
    virtual void _v51();
    virtual void _v52();
    virtual void _v53();
    virtual void _v54();
    virtual void _v55();
    virtual void _v56();
    virtual void _v57();
    virtual void _v58();
    virtual void _v59();
    virtual void _v60();
    virtual void _v61();
    virtual void _v62();
    virtual void _v63();
    virtual void _v64();
    virtual void _v65();
    virtual void _v66();
    virtual void _v67();
    virtual void _v68();
    virtual void _v69();
    virtual void _v70();
    virtual void _v71();
    virtual void _v72();
    virtual void _v73();
    virtual void _v74();
    virtual void _v75();
    virtual void _v76();
    virtual void _v77();
    virtual void _v78();
    virtual void _v79();
    virtual void _v80();
    virtual void _v81();
    virtual CStateMap& GetStates();         // slot 82 (+0x148), 0x0052F0E0: the map at +0x24
    virtual void _v83();
    virtual void _v84();
    virtual void _v85();
    virtual void _v86();
    virtual void _v87();
    virtual void _v88();
    virtual void _v89();
    virtual void _v90();
    virtual void _v91();
    virtual void MakeUnavailable();         // slot 92, 0x00530A40
    virtual void MakeUnavailableDynamically();   // slot 93, 0x00530B00
    virtual void MakeAvailable();           // slot 94, 0x00530CE0
    virtual void MakeAvailableDynamically();     // slot 95, 0x00530DA0
};

}

namespace NUISystem {

__forceinline void CComponent::MakeAvailable()
{
    for (CStateMapIterator it = GetStates().begin(); it != GetStates().end(); ++it)
    {
        CRGBColour& colour = it->second.Colour;
        colour.R = (unsigned char)(colour.R * 1.33333337f);
        colour.G = (unsigned char)(colour.G * 1.33333337f);
        colour.B = (unsigned char)(colour.B * 1.33333337f);
    }
}

void CComponent::MakeAvailableDynamically()
{
    CComponent::MakeAvailable();
    CRGBColour colour = GetCurrentState()->Colour;
    colour.A = GetColour().A;
    ChangeColour(colour, 0.0f, false);
}

}