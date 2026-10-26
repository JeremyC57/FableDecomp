// NScreenEffect::CScreenEffectPriorityStack::SetAreaTrackPos(CRadialBlurEntry* entry, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION track) -- sets posparam.AreaTrackPos (+0x58) of an entry still in RadialBlurKeys
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00881150, length 55 bytes.
// Leaf: ?SetAreaTrackPos@CScreenEffectPriorityStack@NScreenEffect
// Retail bytes: (the find prologue of 0x00881010) ; mov ecx,[esi] ; mov eax,[esp+4] ; cmp eax,ecx ; pop esi ; je E ; mov eax,[eax+10h] ; mov ecx,[esp+0Ch] ; mov [eax+58h],ecx ; E: pop ecx ; ret 8
// Callees: the RadialBlurKeys/DisplacementKeys set's find @ 0x008839E0 (iterator through the hidden result) and erase(key) @ 0x00884710.
// Caller: CEnvironmentSpellEffectInterpolator (its priority stack at +0x90, 0x006B82E0..0x006B83A0).
// The enum argument is a plain 4-byte store.
// Retail model (STLport containers): CScreenEffectPriorityStack (vtable 0x01278058) is
//   +0x00 vptr, +0x04 RadialParamStack[3] (4-byte STLport lists), +0x10 RadialBlurKeys (set<CStackEntry*>:
//   header node, node count, comparator = 12 bytes), +0x1c CurrentRadialBlurParams (48), +0x4c
//   CurrentRadialBlurPosParams (32), +0x6c DisplacementParamStack[3], +0x78 DisplacementKeys, +0x84
//   CurrentDisplacementParam -- the PDB layout with 4-byte lists and 12-byte sets (the PDB build's debug
//   containers are 12/16 bytes). A set node is {color, parent, left, right, value} so the key sits at +0x10.
//   CRadialBlurEntry (vtable 0x01278050, 96 bytes) = CStackEntry {vptr, curdurationcount, durationcount,
//   fadingout} + param (CRadialBlurParams, +0x10) + posparam (CRadialBlurPositionParams, +0x40).
// The entry argument is a derived-entry pointer: binding it to the set's `CStackEntry* const&` key makes a
// conversion temporary, which VC7.1 homes in the argument's own (now dead) slot -- the retail
// `mov [esp+N],eax` that stores the argument back over itself before `lea` passes its address.
class C3DVector { public:
    float X, Y, Z;
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() {}
};
struct CRadialBlurParams {                     // 48 bytes
    CRGBFloatColour ThresholdColour;           // +0x00
    CRGBFloatColour IntensityColour;           // +0x10
    float Length;                              // +0x20
    float Fade;                                // +0x24
    float Range;                               // +0x28
    float Twist;                               // +0x2c
    CRadialBlurParams() {}                     // user default constructor: leaves the record uninitialised
};
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { ENVIRONMENT_RADIAL_BLUR_TRACK_NONE = 0 };
struct CRadialBlurPositionParams {             // 32 bytes
    C3DVector AreaCenter;                      // +0x00
    C3DVector BlurCenter;                      // +0x0c
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;   // +0x18
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;   // +0x1c
};
extern long g_framesPerSecond;                 // 0x01375550 (15)

namespace NScreenEffect {

class CScreenEffectPriorityStack { public:
    struct CStackEntry {                       // vtable 0x01277FF0
        virtual void v00();
        long curdurationcount;                 // +0x04
        long durationcount;                    // +0x08
        bool fadingout;                        // +0x0c
    };
    struct CRadialBlurEntry : CStackEntry {    // vtable 0x01278050
        CRadialBlurParams         param;       // +0x10
        CRadialBlurPositionParams posparam;    // +0x40
    };
    struct CDisplacementMonochromeEffectColourEntry : CStackEntry {   // vtable 0x01277FF8
        CRGBFloatColour paramcol;              // +0x10
    };
    struct SKeyNode {                          // STLport _Rb_tree_node<CStackEntry*>
        long         _M_color;                 // +0x00
        SKeyNode*    _M_parent;                // +0x04
        SKeyNode*    _M_left;                  // +0x08
        SKeyNode*    _M_right;                 // +0x0c
        CStackEntry* _M_value_field;           // +0x10
    };
    struct CKeySet {                           // STLport set<CStackEntry*>
        struct iterator {
            SKeyNode* _M_node;
            iterator() {}
            iterator(SKeyNode* node) : _M_node(node) {}
            CStackEntry* operator*() const { return _M_node->_M_value_field; }
            bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
        };
        SKeyNode*     _M_header;               // +0x00
        unsigned long _M_node_count;           // +0x04
        unsigned long _M_key_compare;          // +0x08 (empty less<>, padded)
        iterator find(CStackEntry* const& key);          // 0x008839E0
        unsigned long erase(CStackEntry* const& key);    // 0x00884710
        iterator end() { return iterator(_M_header); }
    };

    virtual void v00();
    void*                     RadialParamStack[3];          // +0x04
    CKeySet                   RadialBlurKeys;               // +0x10
    CRadialBlurParams         CurrentRadialBlurParams;      // +0x1c
    CRadialBlurPositionParams CurrentRadialBlurPosParams;   // +0x4c
    void*                     DisplacementParamStack[3];    // +0x6c
    CKeySet                   DisplacementKeys;             // +0x78
    CRGBFloatColour           CurrentDisplacementParam;     // +0x84

    CRadialBlurParams GetRadialBlurParam(CRadialBlurEntry* entry);
    void SetAreaCenter(CRadialBlurEntry* entry, const C3DVector& center);
    void SetBlurCenter(CRadialBlurEntry* entry, const C3DVector& center);
    void SetAreaTrackPos(CRadialBlurEntry* entry, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION track);
    void SetBlurTrackPos(CRadialBlurEntry* entry, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION track);
    void DeleteRadialBlurEntry(CRadialBlurEntry* entry, float fadeTime);
    void DeleteDisplacementEntry(CDisplacementMonochromeEffectColourEntry* entry, float fadeTime);
};

void CScreenEffectPriorityStack::SetAreaTrackPos(CRadialBlurEntry* entry, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION track)
{
    CKeySet::iterator it = RadialBlurKeys.find(entry);
    if (it != RadialBlurKeys.end())
        static_cast<CRadialBlurEntry*>(*it)->posparam.AreaTrackPos = track;
}

}   // namespace NScreenEffect