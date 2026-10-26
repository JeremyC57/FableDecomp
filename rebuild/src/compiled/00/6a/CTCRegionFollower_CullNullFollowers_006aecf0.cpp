// CTCRegionFollower::CullNullFollowers() -- walk the Followers list and erase every entry whose intelligent pointer no longer refers to a thing
// Leaf: CullNullFollowers
// Retail: Fable.exe 0x006AECF0, 137 bytes (plain `ret`; VC7.1 aligns the frame itself, `and esp,-8`, for the 8-byte
// CIntelligentPointer local, as in the landed CTCShopKeeper::EmptyCustomerList 0x00798E10).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50 (x2), SetPItem @ 0x00A01B90,
// ~CBaseIntelligentPointer @ 0x00A01C10, the element's virtual slot 0 (scalar deleting dtor, flag 0 = the explicit
// `~T()` of STLport's _Destroy), free @ 0x00BFEA14. Local vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Layout = Ego_r.pdb CTCRegionFollower (0x18): Followers (STLport list<CIntelligentPointer<CThing>>) +0x0C, whose
// first word is the sentinel node pointer; node = { next +0, prev +4, value +8 }.
// Shape: a copy of each element (inline copy ctor: base ctor, vtable store, SetPItem(other.GetPItem())) is tested;
// STLport's inline erase unlinks the node, destroys the value through the vtable and frees the node.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {    // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
extern "C" void __cdecl free(void* block);   // MSVCR71 import thunk 0x00BFEA14

struct CFollowerNode {
    CFollowerNode*      _M_next;                    // +0
    CFollowerNode*      _M_prev;                    // +4
    CIntelligentPointer<CThing> _M_data;                    // +8
};
struct CFollowerList {                              // STLport list<CIntelligentPointer<CThing>>
    CFollowerNode* _M_node;                         // sentinel
    CFollowerNode* begin() const { return _M_node->_M_next; }
    CFollowerNode* end() const { return _M_node; }
    CFollowerNode* erase(CFollowerNode* pos)
    {
        CFollowerNode* next = pos->_M_next;
        CFollowerNode* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        (&pos->_M_data)->~CIntelligentPointer();
        free(pos);
        return next;
    }
};
class CTCRegionFollower { public:
    virtual void v00();
    void*         ParentThing;                      // +0x04 (CTCBase)
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    CFollowerList Followers;                        // +0x0c
    void CullNullFollowers();
};

void CTCRegionFollower::CullNullFollowers()
{
    CFollowerNode* it = Followers.begin();
    while (it != Followers.end()) {
        CIntelligentPointer<CThing> follower(it->_M_data);
        if (follower.GetPItem() == 0)
            it = Followers.erase(it);
        else
            it = it->_M_next;
    }
}