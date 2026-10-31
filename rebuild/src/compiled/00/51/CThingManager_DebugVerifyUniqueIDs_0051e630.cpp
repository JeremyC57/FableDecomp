// CThingManager::DebugVerifyUniqueIDs(const std::vector<CThing*>& things) -- debug check: for every thing with a unique ID, find the other managed things that carry the same ID and give each of them a fresh one; the first clash also builds both things' def names (debug text whose use is compiled out)
// Leaf: ?DebugVerifyUniqueIDs@CThingManager@@AAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051E630, 369 bytes, `ret 4`.
// Name: Ego_r.pdb ?DebugVerifyUniqueIDs@CThingManager@@AAEXABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z (private;
//   the catalog name).
// Callees: CThing @ 0x004C7BC0 (`mov ecx,[0x013B8A1C] ; jmp 0x0051E2E0`: forwards to the global thing manager's +0x9A byte getter,
//   the result unused) -- named CThing::CheckUID() const (Ego_r.pdb ?CheckUID@CThing@@IBEXXZ, protected void, from the
//   Ego_r CThing name set; PROPOSED, see NOTES.md); the base def's virtual slot 10 GetInstantiationName (+0x28, CDefString by
//   hidden pointer; engine/CDefClassBase.h slot 10); CDefStringTable::GetString(long) const 0x009D49B0 on GDefStringTable
//   0x013CA828 (Ego_r.pdb `dynamic initializer for 'GDefStringTable''); CCharString(const CCharString&) 0x0099EC30;
//   ~CCharString 0x0099EAE0; CThing::CreateUID 0x004C7FF0.
// Layout (retail): CThing UniqueID +0x18 (Ego_r.pdb +0x18), PBaseDef +0x70 (Ego_r +0x80: retail CThing is 0x10 smaller below it);
//   CThingManager +0x18 the STLport std::list<CThing*> of all things (inside the PDB's unnamed base part: descriptive name);
//   CDefString {TablePos} (Ego_r.pdb, 4 bytes).
// Shape: each name is CThing::GetDefName() (Ego_r.pdb, returns a CDefString: `PBaseDef ? PBaseDef->GetInstantiationName() :
//   CDefString(-1)`, a class-typed conditional) and then CDefString::GetString() (Ego_r.pdb ?GetString@CDefString@@QBE?AVCCharString@@XZ,
//   inline over GDefStringTable.GetString(TablePos) with a named result, so the caller's local is copy-constructed from the
//   returned temporary). The match lever: CDefString has a user copy constructor -- without it VC7.1 loads TablePos into
//   eax for the push (retail ecx) and rotates the scratch registers of the second name and the loop tail.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CDefStringTable { public:
    CCharString GetString(long tablePos) const;         // 0x009D49B0
};
extern CDefStringTable GDefStringTable;                 // 0x013CA828
class CDefString { public:
    long TablePos;                                      // +0x00
    CDefString(long tablePos) : TablePos(tablePos) {}
    CDefString(const CDefString& other) : TablePos(other.TablePos) {}
    CCharString GetString() const
    {
        CCharString result = GDefStringTable.GetString(TablePos);
        return result;
    }
};
// CThingBaseDef: local model (only the CDefClassBase slot 10 this row calls).
class CThingBaseDef { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetInstantiationName() const;    // slot 10 (+0x28)
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};
class CThingManager;
class CThing { public:
    unsigned char      _pad00[0x18];
    unsigned __int64   UniqueID;                        // +0x18
    unsigned char      _pad20[0x70 - 0x20];
    CDefPointer<const CThingBaseDef> PBaseDef;          // +0x70
    CDefString GetDefName() const { return PBaseDef ? PBaseDef->GetInstantiationName() : CDefString(-1); }
    void CreateUID();                                   // 0x004C7FF0
protected:
    void CheckUID() const;                              // 0x004C7BC0
    friend class CThingManager;
};
namespace std {
template <class T> class allocator;
template <class T, class A> class vector;
// STLport 4.6 vector<CThing*>: the three words and the iterators this row reads.
template <> class vector<CThing*, allocator<CThing*> > { public:
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThing* const* begin() const { return _M_start; }
    CThing* const* end() const { return _M_finish; }
};
template <class T, class A> class list;
// STLport 4.6 list<CThing*> (4 bytes: the header node pointer) and its node types.
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;
};
template <> class list<CThing*, allocator<CThing*> > { public:
    _List_node<CThing*>* _M_node;
};
}
class CThingManager { public:
    unsigned char _pad00[0x18];                         // the PDB's unnamed base part (Ego_r CThingManager +0x00..+0x27)
    std::list<CThing*, std::allocator<CThing*> > AllThings;   // +0x18 (descriptive name: no PDB member at this offset)
private:
    void DebugVerifyUniqueIDs(const std::vector<CThing*, std::allocator<CThing*> >& things);
};

void CThingManager::DebugVerifyUniqueIDs(const std::vector<CThing*, std::allocator<CThing*> >& things)
{
    bool reported = false;
    for (CThing* const* it = things.begin(); it != things.end(); ++it) {
        CThing* thing = *it;
        thing->CheckUID();
        if (thing->UniqueID) {
            for (std::_List_node_base* node = AllThings._M_node->_M_next; node != AllThings._M_node; node = node->_M_next) {
                CThing* other = static_cast<std::_List_node<CThing*>*>(node)->_M_data;
                if (thing != other && thing->UniqueID == other->UniqueID && thing->UniqueID) {
                    if (!reported && (thing->PBaseDef || other->PBaseDef)) {
                        CCharString thingName = thing->GetDefName().GetString();
                        CCharString otherName = other->GetDefName().GetString();
                        reported = true;
                    }
                    other->CreateUID();
                }
            }
        }
    }
}