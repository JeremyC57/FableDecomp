# h24_misc.py: naming wave 24, family misc -- single-row respells of chunk 691 queue targets:
#  0x004BB600 ~CGameScriptThingManager: its base stand-in CScriptThingListBase_004bb600::Clear is the Ego_r member list<CScriptThingInfo>
#    ScriptThings whose inline destructor calls _List_base<CScriptThingInfo, allocator<...> >::clear 0x004BA810 (labels_chunk691).
#  0x007066A0 CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea: the static frame getter is
#    ?GetFrame@CWorld@@SIJXZ 0x0049D870 (ESTABLISHED; was on a CWorld_Calls stand-in).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
A = '004bb600'
he.edit(A, 'src', '''// declaration order), then the inlined base destructor; `this` in edi, the member in esi.
''', '''// declaration order), then the inlined base destructor; `this` in edi, the member in esi.
// Naming (naming wave 24): the "base" at +0 is Ego_r.pdb's first member, list<CScriptThingInfo> ScriptThings, whose inline destructor
//   clears it through ?clear@?$_List_base@VCScriptThingInfo@CGameScriptThingManager@@V?$allocator@...@std@@@std@@QAEXXZ 0x004BA810
//   (labels_chunk691; the landed own row) and frees the sentinel; the second member is NullThing. Was a CScriptThingListBase_004bb600
//   stand-in base with a Clear(); the members are destroyed in reverse order as before (NullThing, then ScriptThings).
''')
he.edit(A, 'src', '''struct CScriptThingListBase_004bb600 {             // the base at +0: the thing buffer, cleared then freed
    void* m_pBuffer;                               // +0x00
    void Clear();                                  // 0x004BA810
    ~CScriptThingListBase_004bb600()
    {
        Clear();
        if (m_pBuffer)
            free(m_pBuffer);
    }
};

class CGameScriptThingManager : public CScriptThingListBase_004bb600 { public:
    CScriptThing m_nullThing;                      // +0x04
    ~CGameScriptThingManager();
};''', '''namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _List_base {      // STLport list base: the sentinel node pointer
public:
    void* _M_node;                                 // +0x00
    void clear();                                  // 0x004BA810
    ~_List_base()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };
}

class CGameScriptThingManager { public:
    class CScriptThingInfo;
    std::list<CScriptThingInfo> ScriptThings;      // +0x00
    CScriptThing NullThing;                        // +0x04
    ~CGameScriptThingManager();
};''')
A = '007066a0'
he.edit(A, 'src', '''struct CWorld_Calls {
    static long __fastcall GetFrame();                  // 0x0049D870
};''', '''class CWorld { public:
    static long __fastcall GetFrame();                  // 0x0049D870 (Ego_r ?GetFrame@CWorld@@SIJXZ; naming wave 24, was CWorld_Calls)
};''')
# 0x00686A80 is CTCBase::PeekWorld, not GetWorld: Ego_r.pdb puts ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ at 0x006B1E60 and
# ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 16 bytes later at 0x006B1E70 (FableWin.pdb: 0x02190D40 / 0x02190D60), exactly as retail
# GetWorld 0x00686A70 (bound, chunk 654) is followed by 0x00686A80; 0x004BC750 already binds PeekWorld there. Protected const.
he.edit(A, 'src', '''    CWorld_Calls& GetWorld() const;                     // 0x00686A80
protected:
''', '''protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80 (Ego_r.pdb ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ; naming wave 24, was GetWorld)
''')
he.edit(A, 'src', '    FrameLastChanged = GetWorld().GetFrame();', '    FrameLastChanged = PeekWorld().GetFrame();')
he.edit(A, 'src', 'CTCBase::GetWorld @ 0x00686A80', 'CTCBase::PeekWorld @ 0x00686A80')
