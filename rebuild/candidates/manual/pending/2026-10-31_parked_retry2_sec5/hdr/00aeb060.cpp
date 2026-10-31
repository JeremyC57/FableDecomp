// NParticleEngine::CParticleEmitterList::Insert(const CCountedPointer<CParticleEmitter>& emitter) -- add an emitter to the active list by priority: an empty list just takes it; otherwise dead emitters met on the way are erased and the new one goes in front of the first lower-priority emitter; at the end it is appended only while the list is under its maximum; returns whether it was added
// Leaf: Insert
// Grade: DIFFER(301v301) -- local CCountedPointer replaced by engine/CCountedPointer.h (bytes unchanged); residue: in
//   the inline size() loop ours copies begin into eax before the first compare, retail compares esi first; an STLport
//   distance-shaped size() (separate first/last) changed nothing
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB060, 301 bytes (+ int3 padding), `ret 4`.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (list nodes), operator delete 0x00BFE9BC (count block), the list's
//   out-of-line push_back 0x00AEC780, push_front 0x00AEC740 and insert(iterator, const T&) 0x00AEC6F0 (iterator by value,
//   result by hidden pointer).
// Layout: Ego_r.pdb CParticleEmitterList: Active (STLport list, retail 4 bytes: the sentinel pointer) +0,
//   MaxActiveEmitters +4; CParticleEmitter (retail) Priority:3 / Dead (bit 5) in the ushort bit-field word at +0x40.
// Shape: STLport list members inline (size() via the SGI distance loop, the first push_back expanded inline, erase);
//   the insert position is the element before the lower-priority one.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }

namespace NParticleEngine {
struct CParticleEmitter {
    unsigned char  _pad00[0x40];
    unsigned short Priority : 3;                        // +0x40 bits 0-2
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;                            // bit 5
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    bool IsDead() const { return Dead; }
};
}
#include "engine/CCountedPointer.h"
typedef CCountedPointer<NParticleEngine::CParticleEmitter> CEmitterPointer;

struct SEmitterNode {                                   // _List_node<CEmitterPointer>
    SEmitterNode*   _M_next;
    SEmitterNode*   _M_prev;
    CEmitterPointer _M_data;
};
struct CEmitterIterator {
    SEmitterNode* _M_node;
    CEmitterIterator(SEmitterNode* node) : _M_node(node) { }
    CEmitterIterator(const CEmitterIterator& other) : _M_node(other._M_node) { }
};
struct CEmitterList {                                   // STLport list<CEmitterPointer> (retail: the node pointer)
    SEmitterNode* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SEmitterNode* p = _M_node->_M_next; p != _M_node; ) {
            p = p->_M_next;
            ++n;
        }
        return n;
    }
    void push_back(const CEmitterPointer& x)            // 0x00AEC780 out of line
    {
        SEmitterNode* position = _M_node;
        SEmitterNode* node = (SEmitterNode*)malloc(sizeof(SEmitterNode));
        new (&node->_M_data) CEmitterPointer(x);
        SEmitterNode* prev = position->_M_prev;
        node->_M_next = position;
        node->_M_prev = prev;
        prev->_M_next = node;
        position->_M_prev = node;
    }
    void push_front(const CEmitterPointer& x);          // 0x00AEC740
    CEmitterIterator insert(CEmitterIterator position, const CEmitterPointer& x);   // 0x00AEC6F0
    SEmitterNode* erase(SEmitterNode* position)
    {
        SEmitterNode* prev = position->_M_prev;
        SEmitterNode* next = position->_M_next;
        prev->_M_next = next;
        next->_M_prev = prev;
        position->_M_data.~CEmitterPointer();
        free(position);
        return next;
    }
};

namespace NParticleEngine {
struct CParticleEmitterList {
    CEmitterList Active;                                // +0x00
    long         MaxActiveEmitters;                     // +0x04
    bool Insert(const CEmitterPointer& emitter);
};

bool CParticleEmitterList::Insert(const CEmitterPointer& emitter)
{
    if (Active.size() == 0) {
        Active.push_back(emitter);
        return true;
    }
    for (SEmitterNode* it = Active._M_node->_M_next; it != Active._M_node; ) {
        if (it->_M_data->IsDead())
            it = Active.erase(it);
        else {
            if (it->_M_data->Priority < emitter->Priority) {
                if (it == Active._M_node->_M_next) {
                    Active.push_front(emitter);
                    return true;
                }
                Active.insert(CEmitterIterator(it->_M_prev), emitter);
                return true;
            }
            it = it->_M_next;
        }
    }
    if ((long)Active.size() < MaxActiveEmitters) {
        Active.push_back(emitter);
        return true;
    }
    return false;
}
}
