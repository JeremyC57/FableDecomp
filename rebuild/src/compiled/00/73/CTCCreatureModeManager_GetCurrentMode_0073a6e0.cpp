// CTCCreatureModeManager::GetCurrentMode() const -- the mode at the front of the mode list
// Leaf: ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ
// Retail: Fable.exe @ 0x0073A6E0, 9 bytes: `mov eax,[ecx+0Ch] ; mov ecx,[eax] ; mov eax,[ecx+8] ; ret`.
// Identity (proven): Ego_r.pdb names ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ and lays PModes
// (std::list<CCountedPointer<CModeBase> >) at +0x0C; the ~30 callers reach this object as interface 0x31 through the thing's
// interface map and then call the mode's virtuals (landed 0x00830F20, 0x006E3180, 0x007B5CA0 call it "GetCurrentMode").
// The manifest label "CPixelShader::CountReferences" is propagated (CPixelShader's is a private virtual with a vtable slot).
// Shape: STLport 4.6 list (one node pointer, 4 bytes); front() = the first node's value, a CCountedPointer whose Data
// pointer is its first word; the reference result is that pointer.
namespace std {
template<class _Tp> class allocator { public: };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp, class _Alloc = allocator<_Tp> > class list { public:      // STLport 4.6 (4 bytes)
    _List_node<_Tp>* _M_node;
    _Tp& front() const { return ((_List_node<_Tp>*)_M_node->_M_next)->_M_data; }
};
}
template<class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T& operator*() const { return *Data; }
};
class CThing;
class CTCBase { public:                         // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCCreatureModeManager : public CTCBase { public:
    class CModeBase;
    std::list<CCountedPointer<CModeBase> > PModes;      // +0x0C
    CModeBase& GetCurrentMode() const;
};
class CTCCreatureModeManager::CModeBase { public:
    virtual void v00();
};

CTCCreatureModeManager::CModeBase& CTCCreatureModeManager::GetCurrentMode() const
{
    return *PModes.front();
}