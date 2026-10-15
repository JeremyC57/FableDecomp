// NGameText::CGroupEntry::CGroupEntry(const CCountedPointer<CGroup>& group, long id) -- a text-group resource entry: CResource base, an empty id vector, a shared reference to the group and the entry id; registers its memory size with the resource list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C8060, length 128 bytes (padded to 144).
// Callees: CResource::CResource @ 0x009FBEC0, the group's Sizeof @ 0x009D5620 (manifest "CVertexBufferWin32::DoSizeof",
//          propagated: `mov eax,[ecx+24h] ; add eax,48h`), CResource::SetResourceSize @ 0x009FBF20.
// Vtable: NGameText::CGroupEntry 0x0129B478 (rtti-vtables.tsv).
// Shape: the 12-byte vector member's inline constructor zeroes its words from the shared `xor edx,edx`; the counted
// pointer is copy-constructed (inc of the count block); the size is a local `sizeof(*this) + capacity * 4` (computed
// into edi before the pointer test -- one expression evaluates the Sizeof first) then `+ Group.Sizeof() -
// sizeof(Group)` with the counted pointer's Sizeof an inline ternary (`Data ? Data->Sizeof() + 8 : 8`, the 8 kept in
// both arms and the -8 folded into the final `lea`).
// Model: local (CResource from Ego_r.pdb: vptr, ResourceList +8, Prev +0x0c, Next +0x10, ResourceSize +0x14,
// LastUsedFrame +0x18; the derived members from the bytes).
// Leaf: ??0CGroupEntry@NGameText@@
struct CResource {                                 // 0x1c bytes
    virtual void v00();
    unsigned char _pad04[0x18];
    CResource();                                   // 0x009FBEC0
    void SetResourceSize(unsigned long size);      // 0x009FBF20
};
template <class T> struct vector {                 // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned long capacity() const { return _M_end_of_storage - _M_start; }
};
namespace NGameText {
struct CGroup {
    unsigned long Sizeof() const;                  // 0x009D5620
};
}
template <class T> struct CCountedPointer {
    T*    Data;
    long* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++*Info;
    }
    unsigned long Sizeof() const { return Data ? Data->Sizeof() + sizeof(CCountedPointer) : sizeof(CCountedPointer); }
};
namespace NGameText {
struct CGroupEntry : CResource {
    vector<long>             m_ids;                // +0x1c
    CCountedPointer<CGroup>  m_group;              // +0x28
    long                     m_id;                 // +0x30
    virtual void v00();
    CGroupEntry(const CCountedPointer<CGroup>& group, long id);
};

CGroupEntry::CGroupEntry(const CCountedPointer<CGroup>& group, long id)
    : m_group(group), m_id(id)
{
    unsigned long size = sizeof(CGroupEntry) + m_ids.capacity() * sizeof(long);
    SetResourceSize(size + m_group.Sizeof() - sizeof(m_group));
}
}