// NScreenEffect::CScreenEffectPriorityStack::InsertDisplacementEntry(float time, const CRGBFloatColour& colour, ScreenEffectPriority priority) -- append a timed displacement colour entry to the stack of its priority, register it in the key set and return it as the effect handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00881780, length 157 bytes (+3 int3 padding).
// PDB: ?InsertDisplacementEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEPAXMABVCRGBFloatColour@@W4ScreenEffectPriority@2@@Z.
// Callees: __ftol2 @ 0x00BFEA70 (frames = (long)(time * frame rate), the int frame rate @ 0x01375550), the list's
// _Buynode(const value_type&) @ 0x00883F10 (copy-constructs a 0x28-byte node: links + the 0x20-byte entry),
// the key set's insert(const value_type&) @ 0x00883FD0 (pair<iterator,bool> through a hidden pointer, ignored).
// Entry (CDisplacementMonochromeEffectColourEntry, vtable 0x01277FF8, over CStackEntry 0x01277FF0): +4 a zero word,
// +8 frame count, +0xC bool, +0x10 CRGBFloatColour. Retail lists are one head-node word each (DisplacementParamStack
// [3] at +0x6C), the key set sits at +0x78.
// Shape: the entry is a named local; the inline push_back captures the head node BEFORE _Buynode (edi survives the
// call) and links before it; the handle (&back()) stays in esi for the return while the set insert takes it by
// reference from the dead `time` argument slot -- the by-value parameter of a __forceinline AddDisplacementKey.
class CRGBFloatColour { public: float R, G, B, A; };

extern const long g_FramesPerSecond;                 // 0x01375550

namespace NScreenEffect {

enum ScreenEffectPriority { SEP_LOW, SEP_MEDIUM, SEP_HIGH };

class CScreenEffectPriorityStack {
public:
    struct CStackEntry {                             // vtable 0x01277FF0
        CStackEntry(long frames) : Unknown04(0), Frames(frames), Finished(false) {}
        virtual ~CStackEntry() {}
        long Unknown04;                              // +0x04
        long Frames;                                 // +0x08
        bool Finished;                               // +0x0C
    };
    struct CDisplacementMonochromeEffectColourEntry : CStackEntry {   // vtable 0x01277FF8
        CDisplacementMonochromeEffectColourEntry(long frames, const CRGBFloatColour& colour)
            : CStackEntry(frames), Colour(colour) {}
        virtual ~CDisplacementMonochromeEffectColourEntry() {}
        CRGBFloatColour Colour;                      // +0x10
    };

    template <class T> struct list {
        struct _Node { _Node* _Next; _Node* _Prev; T _Myval; };
        _Node* _Myhead;
        _Node* _Buynode(const T& value);             // 0x00883F10
        void push_back(const T& value)
        {
            _Node* where = _Myhead;
            _Node* node = _Buynode(value);
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

    void* InsertDisplacementEntry(float time, const CRGBFloatColour& colour, ScreenEffectPriority priority);
    __forceinline void AddDisplacementKey(CStackEntry* key) { DisplacementKeys.insert(key); }

private:
    void*                                          __vftable;                   // +0x00
    unsigned char                                  _radial[0x6C - 0x04];
    list<CDisplacementMonochromeEffectColourEntry> DisplacementParamStack[3];   // +0x6C
    CKeySet                                        DisplacementKeys;            // +0x78
};

void* CScreenEffectPriorityStack::InsertDisplacementEntry(float time, const CRGBFloatColour& colour, ScreenEffectPriority priority)
{
    CDisplacementMonochromeEffectColourEntry entry((long)(time * g_FramesPerSecond), colour);
    list<CDisplacementMonochromeEffectColourEntry>& stack = DisplacementParamStack[priority];
    stack.push_back(entry);
    CStackEntry* key = &stack.back();
    AddDisplacementKey(key);
    return key;
}

} // namespace NScreenEffect