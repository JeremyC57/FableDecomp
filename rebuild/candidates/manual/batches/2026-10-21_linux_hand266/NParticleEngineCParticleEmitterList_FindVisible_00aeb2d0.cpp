// NParticleEngine::CParticleEmitterList::FindVisible(list<CCountedPointer<CParticleEmitter> >& visible, long maxCount,
// const CEnginePrimitivePrepareInfo& info) -- walk every emitter: drop dead ones, initialise new ones, and insert the
// visible ones into the caller's sorted list (trimming it back to maxCount from the far end, whose decals and particles
// are cleared); an invisible emitter that dies offscreen is killed and erased, otherwise its decals and particles clear
// Leaf: FindVisible
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB2D0, 0x1C5 bytes (padded to 464), `ret 0Ch`.
// Callees: CParticleEmitter::IsVisible(float, CEnginePrimitivePrepareInfo*) 0x00AED2F0, CParticleEmitterList's sorted
//   insert 0x00AEB190, CParticleEmitter::ClearAllDecals(CIEngine*) 0x00AED140 and its sibling 0x00AED1E0 (both
//   labelled ClearAllDecals), CParticleEmitter::Kill 0x00AEE8C0, CParticleEmitter::Initialise(CIEngine*) 0x00AEE240,
//   the out-of-line list erase 0x00AEC7C0 (ICF-shared with list<CHoverAnim>::erase), operator delete 0x00BFE9BC,
//   free 0x00BFEA14 (the list node allocator).
// Layout: STLport list nodes {next, prev, data}; the emitter flag word at +0x40 (bit 5 Dead, bit 7 ContinuousEmitter,
//   bit 8 Initialised) and flag byte at +0x44 (bit 4 AlwaysUpdate, bit 5 DieIfOffscreen) are the PDB bitfields 0xC
//   lower (12-byte vectors in retail). The prepare info keeps two distances at +0x4/+0x24 and the engine at +0x34.
// Shape: the two distances are read into locals up front; the visibility test is an if/else of two IsVisible calls that
//   the compiler tail-merges into one call (a ternary argument spills a temp); the iterator is a class with a user copy
//   constructor (passed by value through `push ecx; mov eax,esp`, returned through a hidden slot), and the out-of-line
//   erase result is named before assignment (assigning directly reads through eax instead of the slot); each clear pair
//   shares one `CIEngine* engine` local.
#include <stdlib.h>
void __cdecl operator delete(void* block);
struct CIEngine;
struct CCountedItem { };
struct CCPPointerInfo {
    long m_count;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[4];
    float         AlwaysUpdateDistance;             // +0x04
    unsigned char _pad08[0x1c];
    float         Distance;                         // +0x24
    unsigned char _pad28[0xc];
    CIEngine*     Engine;                           // +0x34
};
namespace NParticleEngine {
#pragma pack(push, 1)
struct CParticleEmitter {
    unsigned char  _pad00[0x40];
    unsigned short Priority : 3;                    // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short _bits9 : 7;
    unsigned char  _pad42[2];
    unsigned char  PreWaterEffect : 1;              // +0x44
    unsigned char  WaterEffect : 1;
    unsigned char  ZBufferWriteable : 1;
    unsigned char  Visible : 1;
    unsigned char  AlwaysUpdate : 1;
    unsigned char  DieIfOffscreen : 1;
    bool Initialise(CIEngine* engine);                                      // 0x00AEE240
    void Kill();                                                            // 0x00AEE8C0
    bool IsVisible(float distance, const CEnginePrimitivePrepareInfo* info); // 0x00AED2F0
    void ClearAllDecals(CIEngine* engine);                                  // 0x00AED140
    void ClearAllParticles(CIEngine* engine);                               // 0x00AED1E0
};
#pragma pack(pop)
struct CEmitterPointer {
    CParticleEmitter* Data;
    CCPPointerInfo*   Info;
    ~CEmitterPointer()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct SEmitterNode {
    SEmitterNode*   _M_next;
    SEmitterNode*   _M_prev;
    CEmitterPointer _M_data;
};
struct CEmitterIterator {
    SEmitterNode* _M_node;
    CEmitterIterator() {}
    CEmitterIterator(SEmitterNode* node) : _M_node(node) {}
    CEmitterIterator(const CEmitterIterator& o) : _M_node(o._M_node) {}
    CEmitterIterator& operator=(const CEmitterIterator& o) { _M_node = o._M_node; return *this; }
    CEmitterPointer& operator*() const { return _M_node->_M_data; }
    CEmitterIterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const CEmitterIterator& o) const { return _M_node != o._M_node; }
};
struct CEmitterList {
    SEmitterNode* _M_node;
    CEmitterIterator begin() const { return CEmitterIterator(_M_node->_M_next); }
    CEmitterIterator end() const { return CEmitterIterator(_M_node); }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SEmitterNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    CEmitterIterator erase(CEmitterIterator position)
    {
        SEmitterNode* next = position._M_node->_M_next;
        SEmitterNode* prev = position._M_node->_M_prev;
        SEmitterNode* n = position._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~CEmitterPointer();
        free(n);
        return CEmitterIterator(next);
    }
    CEmitterPointer& back() const { return _M_node->_M_prev->_M_data; }
    void pop_back() { erase(CEmitterIterator(_M_node->_M_prev)); }
    CEmitterIterator EraseOutOfLine(CEmitterIterator position);             // 0x00AEC7C0
};
class CParticleEmitterList {
public:
    CEmitterList Emitters;                          // +0x00
    bool InsertSorted(CEmitterList& visible, const CEmitterPointer& emitter, long maxCount);   // 0x00AEB190
    void FindVisible(CEmitterList& visible, long maxCount, const CEnginePrimitivePrepareInfo& info);
};
void CParticleEmitterList::FindVisible(CEmitterList& visible, long maxCount, const CEnginePrimitivePrepareInfo& info)
{
    float alwaysUpdateDistance = info.AlwaysUpdateDistance;
    float distance = info.Distance;
    CEmitterIterator it = Emitters.begin();
    while (it != Emitters.end()) {
        CEmitterPointer& entry = *it;
        CParticleEmitter* emitter = entry.Data;
        if ((bool)emitter->Dead) {
            it = Emitters.erase(it);
        } else if (emitter->Initialised) {
            bool isVisible;
            if (emitter->AlwaysUpdate)
                isVisible = emitter->IsVisible(alwaysUpdateDistance, &info);
            else
                isVisible = emitter->IsVisible(distance, &info);
            if (isVisible && InsertSorted(visible, entry, maxCount)) {
                while (visible.size() > (unsigned long)maxCount) {
                    CParticleEmitter* last = visible.back().Data;
                    CIEngine* engine = info.Engine;
                    last->ClearAllDecals(engine);
                    last->ClearAllParticles(engine);
                    visible.pop_back();
                }
                ++it;
            } else {
                CParticleEmitter* hidden = entry.Data;
                if (!(bool)hidden->ContinuousEmitter && hidden->DieIfOffscreen) {
                    hidden->Kill();
                    CEmitterIterator next = Emitters.EraseOutOfLine(it);
                    it = next;
                } else {
                    CIEngine* engine = info.Engine;
                    hidden->ClearAllDecals(engine);
                    hidden->ClearAllParticles(engine);
                    ++it;
                }
            }
        } else {
            emitter->Initialise(info.Engine);
            ++it;
        }
    }
}
}
