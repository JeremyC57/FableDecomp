# h28_d5.py: naming wave 28, family d5 (DECISIONS_W27_RESOLVED D5) -- CActionBase is the PDB's nested CTCScriptedControl::CActionBase
#  (Ego_r / FableWin: 218 manglings ..CActionBase@CTCScriptedControl@@, none in NTCScriptedControl; the derived actions
#  CActionStopScripting / CActionMoveTo are NTCScriptedControl members), and the action list is the STLport
#  list<CCountedPointer<CTCScriptedControl::CActionBase> > whose out-of-line clear is the c695 spelling of 0x00719280
#  ?clear@?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAEXXZ (was ?clear@CActionList@@QAEXXZ).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
F = 'd5'
LIST = '''namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp, class _Alloc> class _List_base {
protected:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // the 4-byte sentinel pointer
public:
    void clear();                                  // 0x00719280 (_list.c, out of line)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
%s    _Tp& front() { return ((_List_node<_Tp>*)this->_M_node._M_data->_M_next)->_M_data; }
    void clear() { _Base::clear(); }
};
}
'''
SIZE = '''    unsigned int size() const
    {
        unsigned int n = 0;
        for (_List_node_base* p = this->_M_node._M_data->_M_next; p != this->_M_node._M_data; p = p->_M_next)
            ++n;
        return n;
    }
'''
NOTE = ('CActionBase is the nested CTCScriptedControl::CActionBase (Ego_r.pdb / FableWin.pdb;\n'
        '//   DECISIONS_W27 D5) and PActionList the STLport list<CCountedPointer<CTCScriptedControl::CActionBase> >, whose clear 0x00719280\n'
        '//   is ?clear@?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAEXXZ (c695). Was %s\n'
        '//   with the CActionList / SActionNode stand-ins (?clear@CActionList@@QAEXXZ).')

# ---- 0x00713660 NTCScriptedControl::CActionStopScripting::Update
a = '00713660'
he.edit(a, 'src', '''class CThing;
class CCPPointerInfo;

namespace NTCScriptedControl {
class CActionBase;
}

struct SActionNode {
    SActionNode*                     _M_next;      // +0x00
    SActionNode*                     _M_prev;      // +0x04
    NTCScriptedControl::CActionBase* Data;         // +0x08
    CCPPointerInfo*                  Info;         // +0x0c
};
struct CActionList {                               // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SActionNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    void clear();                                  // 0x00719280
};
class CTCScriptedControl { public:
    virtual void v00();''', '''class CThing;
#include "engine/CCountedPointer.h"
''' + LIST % SIZE + '''class CTCScriptedControl { public:
    class CActionBase;                             // nested (Ego_r.pdb / FableWin.pdb CTCScriptedControl::CActionBase)
    virtual void v00();''')
he.edit(a, 'src', '''    CActionList  PActionList;                      // +0x14
    bool         Locked;                           // +0x18
    NTCScriptedControl::CActionBase* GetCurrentAction();''', '''    std::list<CCountedPointer<CActionBase> > PActionList;   // +0x14
    bool         Locked;                           // +0x18
    CActionBase* GetCurrentAction();''')
he.edit(a, 'src', '''namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();''', '''class CTCScriptedControl::CActionBase {           // vtable 0x01265504
public:
    virtual ~CActionBase();''')
he.edit(a, 'src', '''    bool                Terminated;                // +0x09
};

class CActionStopScripting : public CActionBase {''', '''    bool                Terminated;                // +0x09
};

namespace NTCScriptedControl {
class CActionStopScripting : public CTCScriptedControl::CActionBase {''')
he.edit(a, 'src', '''inline NTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList._M_node->_M_next->Data;
}''', '''inline CTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList.front().Data;
}''')
he.edit(a, 'src', '''    NTCScriptedControl::CActionBase* action = GetCurrentAction();''', '''    CActionBase* action = GetCurrentAction();''')
note(a, F, NOTE % 'NTCScriptedControl::CActionBase (an invented namespace)')

# ---- 0x00714900 NTCScriptedControl::CActionMoveTo::Update
a = '00714900'
he.edit(a, 'src', '''class CCPPointerInfo;
namespace NTCScriptedControl { class CActionBase; }
struct SActionNode {
    SActionNode*                     _M_next;      // +0x00
    SActionNode*                     _M_prev;      // +0x04
    NTCScriptedControl::CActionBase* Data;         // +0x08
    CCPPointerInfo*                  Info;         // +0x0c
};
struct CActionList {                               // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    void clear();                                  // 0x00719280
};
class CTCScriptedControl { public:
    void*        __vftable;
    CThing*      ParentThing;                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CActionList  PActionList;                      // +0x14
    NTCScriptedControl::CActionBase* GetCurrentAction();''', '''#include "engine/CCountedPointer.h"
''' + LIST % '' + '''class CTCScriptedControl { public:
    class CActionBase;                             // nested (Ego_r.pdb / FableWin.pdb CTCScriptedControl::CActionBase)
    void*        __vftable;
    CThing*      ParentThing;                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
    std::list<CCountedPointer<CActionBase> > PActionList;   // +0x14
    CActionBase* GetCurrentAction();''')
he.edit(a, 'src', '''namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();''', '''class CTCScriptedControl::CActionBase {           // vtable 0x01265504
public:
    virtual ~CActionBase();''')
he.edit(a, 'src', '''    bool                Terminated;                // +0x09
};

class CActionMoveTo : public CActionBase {''', '''    bool                Terminated;                // +0x09
};

namespace NTCScriptedControl {
class CActionMoveTo : public CTCScriptedControl::CActionBase {''')
he.edit(a, 'src', '''inline NTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList._M_node->_M_next->Data;
}''', '''inline CTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList.front().Data;
}''')
he.edit(a, 'src', '''    NTCScriptedControl::CActionBase* action = GetCurrentAction();''', '''    CActionBase* action = GetCurrentAction();''')
note(a, F, NOTE % 'NTCScriptedControl::CActionBase (an invented namespace)')

# ---- 0x00713850 CTCScriptedControl::InterruptControl (CActionBase was a global struct)
a = '00713850'
he.edit(a, 'src', '''struct CActionBase {                                // CTCScriptedControl::CActionBase, vtable 0x01265504
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Exit();                            // slot 5 (+0x14)
    void* PTCScript;                                // +0x04
    bool  Activated;                                // +0x08
    bool  Terminated;                               // +0x09
};
class CCPPointerInfo;
struct SActionNode {
    SActionNode*    _M_next;                        // +0x00
    SActionNode*    _M_prev;                        // +0x04
    CActionBase*    Data;                           // +0x08
    CCPPointerInfo* Info;                           // +0x0c
};
struct CActionList {                                // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    void clear();                                   // 0x00719280
};
class CTCScriptedControl { public:
    virtual void v00();''', LIST % '' + '''class CTCScriptedControl { public:
    class CActionBase {                             // vtable 0x01265504 (Ego_r.pdb / FableWin.pdb CTCScriptedControl::CActionBase)
    public:
        virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
        virtual void Exit();                        // slot 5 (+0x14)
        void* PTCScript;                            // +0x04
        bool  Activated;                            // +0x08
        bool  Terminated;                           // +0x09
    };
    virtual void v00();''')
he.edit(a, 'src', '''    CActionList         PActionList;                // +0x14
    bool                Locked;                     // +0x18
    CActionBase* GetCurrentAction() { if (PActionList.empty()) return 0; return PActionList._M_node->_M_next->Data; }''',
'''    std::list<CCountedPointer<CActionBase> > PActionList;   // +0x14
    bool                Locked;                     // +0x18
    CActionBase* GetCurrentAction() { if (PActionList.empty()) return 0; return PActionList.front().Data; }''')
note(a, F, NOTE % 'a global struct CActionBase')
