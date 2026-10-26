// CGameScriptInterface::WorldUpdate(bool active) -- once per game second (and never during a movie sequence or cutscene) count every running script timer down by one, stopping at zero
// Leaf: ?WorldUpdate@CGameScriptInterface@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E75C0, 104 bytes (112 with int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb name set `?WorldUpdate@CGameScriptInterface@@QAEX_N@Z`; `this` +0x3C is the ScriptTimers
//   map<long, long> (Ego_r.pdb offset 0x3C) whose node values (+0x14) are decremented, and the two virtual tests are the
//   interface's own IsInMovieSequence (slot 395, +0x62C) and IsInCutscene (slot 386, +0x608) as the landed script
//   models name them. Single caller 0x004A5D91. The catalog's "UpdateFrame_ProcessRegions" is descriptive and wrong.
// Callees: the two virtual slots, GFGetCurrentFrame @ 0x0049D870, the STLport map iterator's out-of-line
//   operator++ @ 0x0059299D (`_M_node = _Rb_global::_M_increment(_M_node)`, ICF'd from another TU).
// Globals: g_framesPerSecond @ 0x01375550.
// Layout: CGameScriptInterface (Ego_r.pdb / engine/CGameScriptInterface.h): vtable +0, ScriptTimers +0x3C (STLport
//   map: header node; begin = header->_M_left; node key +0x10, value +0x14). Modelled locally because the generated
//   header is a method-less struct (no virtual interface, no members to define).
// Shape: the frames-per-second global is read into a local before the frame call (held in edi across it, as the
//   landed CTCHeroStats::FrameUpdate 0x0057D5A0); the iterator is address-taken by the out-of-line ++ and lives in
//   the dead `active` argument slot.
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
struct STimerPair {
    long first;                                             // +0x10 in the node (timer id)
    long second;                                            // +0x14 in the node (seconds left)
};
struct STimerNode : _Rb_tree_node_base {
    STimerPair _M_value_field;                              // +0x10
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    STimerPair* operator->() const { return &((STimerNode*)_M_node)->_M_value_field; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
    _Rb_tree_iterator& operator++();                        // 0x0059299D
};
struct CScriptTimerMap {                                    // STLport map<long, long> (12 bytes retail)
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned int        _M_node_count;                      // +0x04
    unsigned int        _M_key_compare;                     // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                                   // 0x0049D870
extern long g_framesPerSecond;                              // 0x01375550

#define V1(n) virtual void v##n();
#define V10(n) V1(n##0) V1(n##1) V1(n##2) V1(n##3) V1(n##4) V1(n##5) V1(n##6) V1(n##7) V1(n##8) V1(n##9)
#define V100(n) V10(n##0) V10(n##1) V10(n##2) V10(n##3) V10(n##4) V10(n##5) V10(n##6) V10(n##7) V10(n##8) V10(n##9)

class CGameScriptInterface {
public:
    V100(0) V100(1) V100(2)                                 // slots 0 .. 299
    V10(30) V10(31) V10(32) V10(33) V10(34) V10(35) V10(36) V10(37)   // slots 300 .. 379
    V1(380) V1(381) V1(382) V1(383) V1(384) V1(385)
    virtual bool IsInCutscene() const;                      // slot 386 (+0x608)
    V1(387) V1(388) V1(389) V1(390) V1(391) V1(392) V1(393) V1(394)
    virtual bool IsInMovieSequence() const;                 // slot 395 (+0x62C)

    unsigned char   _members04[0x3c - 0x04];                // +0x04 World .. HeroSThing
    CScriptTimerMap ScriptTimers;                           // +0x3C

    void WorldUpdate(bool active);
};

void CGameScriptInterface::WorldUpdate(bool active)
{
    if (active && !IsInMovieSequence() && !IsInCutscene()) {
        long fps = g_framesPerSecond;
        if (CWorld::GetFrame() % fps == 0) {
            for (CScriptTimerMap::iterator it = ScriptTimers.begin(); it != ScriptTimers.end(); ++it)
                if (it->second > 0)
                    --it->second;
        }
    }
}