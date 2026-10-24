// std::map<void*, CBulletTimeManager::CBulletTimeInfo>::operator[](void* const& key) -- STLport map index: lower_bound, insert a default entry when the key is absent, return the mapped info
// Leaf: ??A
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C6E00, 123 bytes.
// Label correction: the catalog name SplineDataList::FindOrInsert is a placeholder.  The only caller
// is CBulletTimeManager::SetBulletTimeSlowdownMultiplier @ 0x004C6170, which indexes its
// BulletTimeInfoMap (Ego_r.pdb: CBulletTimeManager +0, map<void*, CBulletTimeManager::CBulletTimeInfo>)
// with the requesting object's address and stores the two floats of the result (CBulletTimeInfo:
// Multiplier +0, SoundPitch +4).
// Callee: _Rb_tree::insert_unique(iterator position, const value_type&) @ 0x004C6BC0 (catalog
// "SplineDataList::FindAndInsert"; STLport: it tests `position == leftmost` ([header+8]) and the node
// count first).
// Retail map (STLport, 12 bytes): header node pointer +0, node count +4, comparator +8; node: colour
// +0, parent +4, left +8, right +0xc, key +0x10, CBulletTimeInfo +0x14.
// Shape: STLport map::operator[] -- the inline lower_bound walk (unsigned pointer compare, `jb`), then
// `if (i == end() || key < i->first) i = insert_unique(i, value_type(key, T()))`; the iterator is
// passed by value through its user copy constructor (`push ecx ; mov eax,esp ; mov [eax],edx`) and the
// result comes back through the hidden pointer into a NAMED local (`inserted`, homed in the dead `key`
// argument slot and re-read from the frame), then assigned to `i`: VC7.1 duplicates the return tail,
// keeping retail's block order (assigning the call result to `i` directly reads it through eax; an
// early `return` from either branch reorders the blocks).  CBulletTimeInfo's
// default constructor leaves both floats uninitialised, so the temporary's words are filled with
// whatever eax holds (the key: the `mov [esp+0c] / [esp+10h] / [esp+14h]` chain).
namespace CBulletTimeManagerTypes {
struct CBulletTimeInfo {
    float Multiplier;                           // +0x0
    float SoundPitch;                           // +0x4
    CBulletTimeInfo() { }
};
}
using CBulletTimeManagerTypes::CBulletTimeInfo;

struct SBulletTimeNode {                        // _Rb_tree_node<pair<void* const, CBulletTimeInfo> >
    long Color;                                 // +0x00
    SBulletTimeNode* Parent;                    // +0x04
    SBulletTimeNode* Left;                      // +0x08
    SBulletTimeNode* Right;                     // +0x0c
    void* Key;                                  // +0x10
    CBulletTimeInfo Value;                      // +0x14
};

struct SBulletTimeIterator {
    SBulletTimeNode* _M_node;
    SBulletTimeIterator(SBulletTimeNode* node) : _M_node(node) { }
    SBulletTimeIterator(const SBulletTimeIterator& other) : _M_node(other._M_node) { }
};

struct SBulletTimeValue {                       // pair<void* const, CBulletTimeInfo>
    void* first;
    CBulletTimeInfo second;
    SBulletTimeValue(void* const& key, const CBulletTimeInfo& info) : first(key), second(info) { }
};

struct CBulletTimeInfoMap {                     // map<void*, CBulletTimeInfo>
    SBulletTimeNode* _M_header;                 // +0x0
    unsigned long _M_node_count;                // +0x4
    unsigned long _M_key_compare;               // +0x8
    SBulletTimeIterator insert_unique(SBulletTimeIterator position, const SBulletTimeValue& value);   // 0x004C6BC0
    SBulletTimeIterator lower_bound(void* const& key)
    {
        SBulletTimeNode* y = _M_header;
        SBulletTimeNode* x = _M_header->Parent;
        while (x != 0) {
            if (!(x->Key < key)) {
                y = x;
                x = x->Left;
            } else
                x = x->Right;
        }
        return SBulletTimeIterator(y);
    }
    CBulletTimeInfo& operator[](void* const& key);
};

CBulletTimeInfo& CBulletTimeInfoMap::operator[](void* const& key)
{
    SBulletTimeIterator i = lower_bound(key);
    if (i._M_node == _M_header || key < i._M_node->Key) {
        SBulletTimeIterator inserted = insert_unique(i, SBulletTimeValue(key, CBulletTimeInfo()));
        i = inserted;
    }
    return i._M_node->Value;
}