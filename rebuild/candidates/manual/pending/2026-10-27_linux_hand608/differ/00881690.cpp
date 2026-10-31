// NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry(float time, const CRadialBlurParams& param, const CRadialBlurPositionParams& posparam, ScreenEffectPriority priority) -- append a timed radial-blur entry to the stack of its priority, register it in the key set and return it as the effect handle
// Grade: DIFFER(229v229) -- scheduling only: retail sets up the param `rep movsd` (`mov ecx,0Ch ; lea edi,[esp+28h]`) before the fadingout byte and vtable stores, ours after. Tried this pass: PDB-exact names (CEnvironmentSpellEffectInterface::CRadialBlurParams), named frames local, body-assigned CStackEntry members, body-assigned param, temporary entry pushed directly, stack reference before the entry.
// Leaf: ?InsertRadialBlurEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEPAXMABUCRadialBlurParams@CEnvironmentSpellEffectInterface@@ABUCRadialBlurPositionParams@4@W4ScreenEffectPriority@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00881690, length 229 bytes (`ret 10h`).
// Callees: __ftol2 @ 0x00BFEA70 (frames = (long)(time * frame rate), the int frame rate @ 0x01375550), malloc @ 0x00BFEA0E
// (the 0x68-byte list node), CRadialBlurEntry::CRadialBlurEntry(const CRadialBlurEntry&) @ 0x00882C90 (landed), the key
// set's insert(const value_type&) @ 0x00883FD0 (pair<iterator,bool> through a hidden pointer, ignored).
// Layout: entry = Ego_r.pdb CRadialBlurEntry (vtable 0x01278050 over CStackEntry 0x01277FF0: curdurationcount +4,
// durationcount +8, fadingout +0xc, CRadialBlurParams param +0x10, CRadialBlurPositionParams posparam +0x40); the
// stack's RadialBlurParamStack[3] lists (one head-node word each in retail) sit at +0x04, the key set at +0x10.
// Shape: the twin of the landed InsertDisplacementEntry 0x00881780 with the list's _Buynode inlined (malloc, then the
// copy constructor under the placement-new null test of &node->_Myval); the 48-byte POD param copies as `rep movsd`
// into the named entry, posparam memberwise (C3DVector has a user copy constructor); the handle stays in esi and the
// set insert takes it from the dead `time` slot (by-value parameter of a __forceinline AddRadialBlurKey).
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);       // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

class C3DVector { public:
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};
class CRGBFloatColour { public: float r, g, b, a; };

extern const long g_FramesPerSecond;                 // 0x01375550

class CEnvironmentSpellEffectInterface {
public:
struct CRadialBlurParams {
    CRGBFloatColour ThresholdColour;                 // +0x00
    CRGBFloatColour IntensityColour;                 // +0x10
    float Length;                                    // +0x20
    float Fade;                                      // +0x24
    float Range;                                     // +0x28
    float Twist;                                     // +0x2c
};
struct CRadialBlurPositionParams {
    C3DVector AreaCenter;                            // +0x00
    C3DVector BlurCenter;                            // +0x0c
    long      AreaTrackPos;                          // +0x18 ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION
    long      BlurTrackPos;                          // +0x1c ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION
};
};
typedef CEnvironmentSpellEffectInterface::CRadialBlurParams CRadialBlurParams;
typedef CEnvironmentSpellEffectInterface::CRadialBlurPositionParams CRadialBlurPositionParams;

namespace NScreenEffect {

enum ScreenEffectPriority { SEP_LOW, SEP_MEDIUM, SEP_HIGH };

class CScreenEffectPriorityStack {
public:
    struct CStackEntry {                             // vtable 0x01277FF0
        CStackEntry(long frames) : curdurationcount(0), durationcount(frames), fadingout(false) {}
        virtual ~CStackEntry() {}
        long curdurationcount;                       // +0x04
        long durationcount;                          // +0x08
        bool fadingout;                              // +0x0c
    };
    struct CRadialBlurEntry : CStackEntry {          // vtable 0x01278050
        CRadialBlurEntry(const CRadialBlurParams& p, const CRadialBlurPositionParams& pos, long frames)
            : CStackEntry(frames), param(p), posparam(pos) {}
        CRadialBlurEntry(const CRadialBlurEntry& other);   // 0x00882C90
        virtual ~CRadialBlurEntry() {}
        CRadialBlurParams         param;             // +0x10
        CRadialBlurPositionParams posparam;          // +0x40
    };

    template <class T> struct list {
        struct _Node { _Node* _Next; _Node* _Prev; T _Myval; };
        _Node* _Myhead;
        void push_back(const T& value)
        {
            _Node* where = _Myhead;
            _Node* node = (_Node*)malloc(sizeof(_Node));
            new (&node->_Myval) T(value);
            _Node* prev = where->_Prev;
            node->_Prev = prev;
            node->_Next = where;
            prev->_Next = node;
            where->_Prev = node;
        }
        T& back() { return _Myhead->_Prev->_Myval; }
    };

    struct CKeySet {                                 // set<CStackEntry*>
        struct iterator { void* _Ptr; };
        struct insert_result { iterator first; bool second; insert_result() {} };
        insert_result insert(CStackEntry* const& key);   // 0x00883FD0
        unsigned char _body[0x0C];
    };

    void* InsertRadialBlurEntry(float time, const CRadialBlurParams& param, const CRadialBlurPositionParams& posparam,
                                ScreenEffectPriority priority);
    __forceinline void AddRadialBlurKey(CStackEntry* key) { RadialBlurKeys.insert(key); }

private:
    void*                  __vftable;                // +0x00
    list<CRadialBlurEntry> RadialBlurParamStack[3];  // +0x04
    CKeySet                RadialBlurKeys;           // +0x10
};

void* CScreenEffectPriorityStack::InsertRadialBlurEntry(float time, const CRadialBlurParams& param,
                                                        const CRadialBlurPositionParams& posparam, ScreenEffectPriority priority)
{
    CRadialBlurEntry entry(param, posparam, (long)(time * g_FramesPerSecond));
    list<CRadialBlurEntry>& stack = RadialBlurParamStack[priority];
    stack.push_back(entry);
    CStackEntry* key = &stack.back();
    AddRadialBlurKey(key);
    return key;
}

} // namespace NScreenEffect
