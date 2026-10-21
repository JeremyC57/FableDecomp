// Grade: DIFFER(484v491) -- logic and STLport map lifetime complete (12-byte tree incl. the empty key_compare avoids the and esp,-8 frame); retail loads the name set's end() before ToLower/find and compares after the temporary's dtor (lane: setne bl inside the full expression; named end/found locals, probe b, grew to 504), and retail reloads Sets_first each outer pass with a spilled this/offset (frame 0x20) where the lane keeps the base in edi (0x1c).
// CParticleMorphs::BuildMorphRemap(const set<CCharString>& available) -- for every morph set, resolve each entry's link
// (copy a resolved target's index, or accept the link itself when the entry's lower-cased name is available), collect
// the resolved entries into an index remap, and when the remap is non-empty apply it to every primitive of the set
// Leaf: BuildMorphRemap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ACA0C0, 0x1EB bytes (padded to 496), `ret 4`.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport node allocator), CCharString::ToLower 0x0099ED10, ~CCharString
//   0x0099EAE0, the name set's find 0x004B9370, map<ulong,ulong>::operator[] 0x00553E60, the tree's recursive _M_erase
//   0x0042FE07 (ICF-shared), COptimisedPrimitive::RemapGroups(map*) 0x00AC99F0.
// Layout: local model. The morph-set vector holds 24-byte sets (12-byte CArray of 8-byte counted primitive pointers at
//   +0, then a 12-byte CArray of 16-byte entries at +0xC); an entry is {CCharString name, long link (-1 none), bool
//   resolved, ulong index}. The remap is an STLport map (header node malloc'd by the constructor).
#include <stdlib.h>
struct CCharString {
    void* PStringData;
    CCharString ToLower() const;                    // 0x0099ED10
    ~CCharString();                                 // 0x0099EAE0
};
struct SNameNode;
struct CCharStringSet {
    SNameNode* _M_header;
    SNameNode* find(const CCharString& name) const; // 0x004B9370
};
struct SRemapNode {
    unsigned char _M_color;
    SRemapNode*   _M_parent;
    SRemapNode*   _M_left;
    SRemapNode*   _M_right;
    unsigned long first;
    unsigned long second;
};
struct CRemap {
    SRemapNode*   _M_header;
    unsigned long _M_node_count;
    struct less { } _M_key_compare;
    CRemap()
    {
        _M_header = 0;
        _M_header = (SRemapNode*)malloc(sizeof(SRemapNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    ~CRemap()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
    void _M_erase(SRemapNode* x);                   // 0x0042FE07
    void clear()
    {
        if (_M_node_count != 0) {
            SRemapNode* x = _M_header->_M_parent;
            while (x != 0) {
                _M_erase(x->_M_right);
                SRemapNode* y = x->_M_left;
                free(x);
                x = y;
            }
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    bool empty() const { return _M_node_count == 0; }
    unsigned long& operator[](const unsigned long& key);   // 0x00553E60
};
struct COptimisedPrimitive {
    void RemapGroups(CRemap* remap);                // 0x00AC99F0
};
struct CPrimitivePointer { COptimisedPrimitive* Data; void* Info; };
struct CMorphEntry {
    CCharString   Name;                             // +0x00
    long          Link;                             // +0x04
    bool          Resolved;                         // +0x08
    unsigned long Index;                            // +0x0C
};
struct CMorphSet {
    CPrimitivePointer* Primitives_first;            // +0x00
    CPrimitivePointer* Primitives_last;             // +0x04
    CPrimitivePointer* Primitives_end;              // +0x08
    CMorphEntry*       Entries_first;               // +0x0C
    CMorphEntry*       Entries_last;                // +0x10
    CMorphEntry*       Entries_end;                 // +0x14
};
class CParticleMorphs {
public:
    void*      _pad00;                              // +0x00
    CMorphSet* Sets_first;                          // +0x04
    CMorphSet* Sets_last;                           // +0x08
    CMorphSet* Sets_end;                            // +0x0C
    void BuildMorphRemap(const CCharStringSet& available);
};
void CParticleMorphs::BuildMorphRemap(const CCharStringSet& available)
{
    for (unsigned long i = 0; i < (unsigned long)(Sets_last - Sets_first); ++i) {
        CMorphSet& set = Sets_first[i];
        CRemap remap;
        for (unsigned long j = 0; j < (unsigned long)(set.Entries_last - set.Entries_first); ++j) {
            CMorphEntry& entry = set.Entries_first[j];
            if (entry.Link == -1)
                continue;
            CMorphEntry& target = set.Entries_first[entry.Link];
            if (target.Resolved) {
                entry.Index = target.Index;
                entry.Resolved = true;
            } else if (available.find(entry.Name.ToLower()) != available._M_header) {
                entry.Index = entry.Link;
                entry.Resolved = true;
            }
            if (entry.Resolved)
                remap[j] = entry.Index;
        }
        if (!remap.empty()) {
            for (unsigned long k = 0; k < (unsigned long)(set.Primitives_last - set.Primitives_first); ++k)
                set.Primitives_first[k].Data->RemapGroups(&remap);
        }
    }
}
