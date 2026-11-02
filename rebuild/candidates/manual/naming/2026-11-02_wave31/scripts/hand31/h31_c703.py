# h31_c703.py: naming wave 31, family c703 (coordinator note on chunk 703, priorities 2-4):
#  (3) 0x004BF9A0 ?ClearAllPrimitives@CTCGraphicAppearance@NGraphicAppearance@@IAEXXZ is protected (IAE in Ego_r.pdb and FableWin.pdb):
#      the three binders declare it under protected:, and the own row (was the free __fastcall CTCGraphicAppearance_ClearAllPrimitives
#      over a MemberAt38::Clear stand-in) is re-authored as the member on the landed 0x004BF8F0 (OnKill) model, as is its sibling
#      0x004BF960 ?ClearAllPrimitivesDueToBeingOutOfRange@CTCGraphicAppearance@NGraphicAppearance@@QAEXXZ (was the free
#      __fastcall CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange over SubObj_Clear).
#  (4) 0x004C5690 is std::_List_base<CPrimitive, std::allocator<CPrimitive> >::clear (class keys V; the spelling 0x004BF8F0 binds):
#      the invented ?Clear@MemberAt38, ?SubObj_Clear and ?clear@CPrimitiveListObject bindings retire (rows 004bf9a0, 004bf960, 004c0000).
#  (2) 0x004C8BA0 ?WorldUpdateBulletTime@CThing@@UAEXXZ / 0x0082FD90 ?WorldUpdateBulletTime@CThingAICreature@@UAEXXZ are virtual
#      (UAE in both PDBs; CThing vtable 0x01239ABC slot 20): declared virtual in both rows; 0x0082FD90's bare Leaf is fixed, and
#      0x004C8BA0's component call uses engine/CTCBase.h's retail branch (slot 10 = BulletTimeFrameUpdate) in place of v00..v09.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note

STD = '''namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp& reference;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x0: the sentinel node
    void clear();                                                 // _list.c, out of line
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return this->_M_node._M_data; }
    void clear() { _Base::clear(); }
};
}
'''

def own(a, title, retail, body_decl, body, extra_layout, test_note):
    he.put(a, 'src', '''// NGraphicAppearance::CTCGraphicAppearance::%s -- %s
// Leaf: %s
// Retail: Fable.exe (TLC, VC7.1) @ 0x%s.
// Identity: Ego_r.pdb / FableWin.pdb %s (block OnKill 0x004BF8F0 / RemovePrimitive 0x004BF920 /
//   ClearAllPrimitivesDueToBeingOutOfRange 0x004BF960 / ClearAllPrimitives 0x004BF9A0, Ego_r donors 0x004C4780..0x004C4890).
// Callees: the components' slot 1 NGraphicAppearance::CAGraphicAppearanceComponent::OnAppearanceClear(), the STLport 4.6
//   std::_List_base<NGraphicAppearance::CPrimitive, std::allocator<...> >::clear 0x004C5690 (list::clear() is the inline
//   `_Base::clear()`), CWorld::GetFrame 0x0049D870 (?GetFrame@CWorld@@SIJXZ, both PDBs: static __fastcall).
// Layout (retail; Ego_r.pdb with Dinkumware containers in brackets): Primitives list<CPrimitive> +0x38 [+0x40], Components
//   list<CAGraphicAppearanceComponent*> +0x3C [+0x4C] (4-byte STLport lists), FrameUpdatedWasRenderedFlag +0x54%s.
// Model: the landed 0x004BF8F0 (OnKill) model (engine/CTCBase.h is flattened; STLport list as in 0x004BF8F0).
class CThing;

''' % (title, retail[1], retail[2], retail[0].upper(), retail[2], extra_layout) + STD + '''
class CTCBase {                                             // engine/CTCBase.h is flattened (vptr as a data member)
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CWorld {
public:
    static long __fastcall GetFrame();                      // 0x0049D870
};

namespace NGraphicAppearance {
class CPrimitive;                                           // Ego_r.pdb, 16 bytes (node value of Primitives)
class CAGraphicAppearanceComponent {                        // vtable 0x0125A170 (slots 0 / 1 pure)
public:
    virtual void OnAppearanceDraw();                        // slot 0 (real signature returns bool; unused here)
    virtual void OnAppearanceClear() = 0;                   // slot 1 (+0x04)
};

class CTCGraphicAppearance : public CTCBase {               // vtable 0x0123986C
public:
    unsigned char _pad0c[0x38 - 0x0c];                      // MeshEffects, TintColours, PTCAnimation, CachedStaticFocalPosOffset, Scale
    std::list<CPrimitive>                     Primitives;   // +0x38
    std::list<CAGraphicAppearanceComponent*>  Components;   // +0x3C
    unsigned char _pad40[0x54 - 0x40];                      // LightingChannel, MainGraphic, ActivePersistenceFlags
    long          FrameUpdatedWasRenderedFlag;              // +0x54
    unsigned char UpdatePrimitiveReasons;                   // +0x58
    unsigned char SelfIlluminating : 1;                     // +0x59 bit 0
    unsigned char ForceResendEveryRenderFrame : 1;          //        bit 1
    unsigned char ResendPrimsEveryRenderFrame : 1;          //        bit 2
    unsigned char DisableOcclusionTesting : 1;              //        bit 3
    unsigned char DrawAfterWater : 1;                       //        bit 4
    unsigned char CachedStaticFocalPos : 1;                 //        bit 5
    unsigned char RenderedLastFrame : 1;                    //        bit 6
    unsigned char PrimitivesDrawn : 1;                      //        bit 7
''' + body_decl + '''};

''' + body + '''
}
''')

own('004bf9a0', 'ClearAllPrimitives()',
    ('004bf9a0', 'tell every registered appearance component to clear itself, destroy all primitives, then stamp the next frame',
     '?ClearAllPrimitives@CTCGraphicAppearance@NGraphicAppearance@@IAEXXZ'),
    'protected:\n    void ClearAllPrimitives();\n', '''void CTCGraphicAppearance::ClearAllPrimitives()
{
    for (std::list<CAGraphicAppearanceComponent*>::iterator it = Components.begin(); it != Components.end(); ++it)
        (*it)->OnAppearanceClear();
    Primitives.clear();
    FrameUpdatedWasRenderedFlag = CWorld::GetFrame() + 1;
}''', '', '')
own('004bf960', 'ClearAllPrimitivesDueToBeingOutOfRange()',
    ('004bf960', 'when primitives were drawn: clear every component and all primitives, stamp the next frame, clear PrimitivesDrawn',
     '?ClearAllPrimitivesDueToBeingOutOfRange@CTCGraphicAppearance@NGraphicAppearance@@QAEXXZ'),
    '    void ClearAllPrimitivesDueToBeingOutOfRange();\n', '''void CTCGraphicAppearance::ClearAllPrimitivesDueToBeingOutOfRange()
{
    if (PrimitivesDrawn) {
        for (std::list<CAGraphicAppearanceComponent*>::iterator it = Components.begin(); it != Components.end(); ++it)
            (*it)->OnAppearanceClear();
        Primitives.clear();
        FrameUpdatedWasRenderedFlag = CWorld::GetFrame() + 1;
        PrimitivesDrawn = false;
    }
}''', ', PrimitivesDrawn +0x59 bit 7', '')
note('004bf9a0', 'c703', 'the PDB protected member (was the invented free ?CTCGraphicAppearance_ClearAllPrimitives@@YIXPAUCTCGraphicAppearance@@@Z\n'
     '//   over the invented ?Clear@MemberAt38@@QAEXXZ binding of 0x004C5690)')
note('004bf960', 'c703', 'the PDB member (was the invented free __fastcall CTCGraphicAppearance_ClearAllPrimitivesDueToBeingOutOfRange\n'
     '//   over the invented ?SubObj_Clear@@YIXPAUSubObj@@@Z binding of 0x004C5690)')

# the three out-of-line binders: ClearAllPrimitives is protected (IAE)
for a in ('004bf9e0', '004bfce0', '004c01e0'):
    he.resub(a, 'src', r'\n    void ClearAllPrimitives\(\);( +// 0x004BF9A0)\n',
             r'\nprotected:\n    void ClearAllPrimitives();\1 (protected: ?ClearAllPrimitives@CTCGraphicAppearance@NGraphicAppearance@@IAEXXZ)\npublic:\n', 1)
    note(a, 'c703', 'ClearAllPrimitives 0x004BF9A0 is protected (?ClearAllPrimitives@CTCGraphicAppearance@NGraphicAppearance@@IAEXXZ,\n'
         '//   Ego_r.pdb / FableWin.pdb); was bound as the public QAE spelling')

# 0x004C0000: ClearAllPrimitives inline here (protected); Primitives is the STLport list, so its clear is _List_base<CPrimitive>::clear
a = '004c0000'
he.edit(a, 'src', '''struct CPrimitiveListObject {                           // STLport list<CPrimitive>
    CPrimitiveListNode* m_head;
    void clear();                                       // 0x004C5690
};
''', STD)
he.edit(a, 'src', '    CPrimitiveListObject Primitives;                    // +0x38 list<CPrimitive> (header-node pointer)',
                  '    std::list<CPrimitive> Primitives;                   // +0x38 list<CPrimitive> (STLport: the sentinel-node pointer)')
he.edit(a, 'src', '    void ClearAllPrimitives()                           // 0x004BF9A0 out of line; inlined here',
                  'protected:\n    void ClearAllPrimitives()                           // 0x004BF9A0 out of line (protected, IAE); inlined here')
he.edit(a, 'src', '''        FrameUpdatedWasRenderedFlag = CWorld::GetFrame() + 1;
    }
    void UnregisterComponent(''', '''        FrameUpdatedWasRenderedFlag = CWorld::GetFrame() + 1;
    }
public:
    void UnregisterComponent(''')
note(a, 'c703', 'Primitives is the STLport std::list<CPrimitive>: its clear 0x004C5690 binds\n'
     '//   ?clear@?$_List_base@VCPrimitive@NGraphicAppearance@@V?$allocator@VCPrimitive@NGraphicAppearance@@@std@@@std@@QAEXXZ (the 0x004BF8F0\n'
     '//   spelling; was the invented ?clear@CPrimitiveListObject@@QAEXXZ); the inline ClearAllPrimitives is protected (IAE in both PDBs)')

# (2) WorldUpdateBulletTime is virtual
a = '004c8ba0'
he.edit(a, 'src', '''class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual void UpdateBulletTime();                 // slot 10 (+0x28)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};
''', '''#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                          // slot 10 (+0x28) BulletTimeFrameUpdate, ToBeRemoved +0x08
''')
he.edit(a, 'src', '    void*               __vftable;                   // +0x00\n',
                  '    virtual void WorldUpdateBulletTime();            // slot 20 of 0x01239ABC (?WorldUpdateBulletTime@CThing@@UAEXXZ); vptr +0x00\n')
he.edit(a, 'src', '''    }
    void WorldUpdateBulletTime();
};''', '''    }
};''')
he.edit(a, 'src', '            tc->UpdateBulletTime();', '            tc->BulletTimeFrameUpdate();')
note(a, 'c703', 'WorldUpdateBulletTime is virtual (?WorldUpdateBulletTime@CThing@@UAEXXZ, Ego_r.pdb / FableWin.pdb; was the\n'
     '//   QAE spelling over a `void* __vftable` member); the component call is CTCBase slot 10 BulletTimeFrameUpdate from engine/CTCBase.h\'s\n'
     '//   retail branch (was a local CTCBase with placeholder slots v00..v09)', leaf='?WorldUpdateBulletTime@CThing@@UAEXXZ')
a = '0082fd90'
he.edit(a, 'src', '    void WorldUpdateBulletTime();               // 0x004C8BA0 (slot 20)',
                  '    virtual void WorldUpdateBulletTime();       // 0x004C8BA0 (slot 20; ?WorldUpdateBulletTime@CThing@@UAEXXZ)')
he.edit(a, 'src', '''class CThingAICreature : public CThingCreatureBase { public:  // vtable 0x0127293C
    void WorldUpdateBulletTime();''', '''class CThingAICreature : public CThingCreatureBase { public:  // vtable 0x0127293C
    virtual void WorldUpdateBulletTime();       // slot 20''')
note(a, 'c703', 'both WorldUpdateBulletTime are virtual (UAE in Ego_r.pdb / FableWin.pdb; was QAE and a bare Leaf); the base call\n'
     '//   CThing::WorldUpdateBulletTime() stays a direct call', leaf='?WorldUpdateBulletTime@CThingAICreature@@UAEXXZ')

# (4) the own row of 0x004C5690: allocator is class-keyed (STLport 4.6 `template <class _Tp> class allocator`), so the emitted
#     clear is the V?$allocator spelling the callers bind (was U?$allocator under the bare Leaf "clear")
a = '004c5690'
he.edit(a, 'src', '''template <class T> struct allocator {
    __forceinline void deallocate''', '''template <class T> class allocator {                // STLport 4.6 _alloc.h: class key `class` (V?$allocator)
public:
    __forceinline void deallocate''')
note(a, 'c703', 'std::allocator is class-keyed: the emitted clear is the V?$allocator spelling the callers 0x004BF8F0 / 0x004BF960 /\n'
     '//   0x004BF9A0 / 0x004C0000 bind (was U?$allocator from `struct allocator`, under the bare Leaf "clear")',
     leaf='?clear@?$_List_base@VCPrimitive@NGraphicAppearance@@V?$allocator@VCPrimitive@NGraphicAppearance@@@std@@@std@@QAEXXZ')
