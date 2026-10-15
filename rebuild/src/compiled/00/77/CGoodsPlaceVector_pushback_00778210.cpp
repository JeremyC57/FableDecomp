// CGoodsPlaceVector::push_back(const SGoodsPlace& x) -- STLport vector push_back of a 36-byte plain record: copy it to the finish when there is room, else the overflow path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00778210, length 112 bytes (100 + int3 padding).
// Label correction: the catalog "CScriptModuleTransitionBetweenPoints::OnInitLeaf" is a propagated name;
// the body is an STLport vector<T>::push_back (`this` is the vector: start +0, finish +4, end_of_storage +8)
// called from the goods-place builders CTCGraphicAppearance_InitGoodsPlaces (0x00774AF7) and
// CTCShop::GetFreeSaleItemPlaces (0x00774BFC). The 36-byte element is named after them.
// Callees: _M_insert_overflow(pos, x, __true_type, 1, true) @ 0x00777FE0.
// Shape: placement new under VC's null test with a `__forceinline` memberwise copy ctor (nine dword
// moves through esi; the implicit copy is `rep movsd`, even over named fields), then
// `add [ecx+4],24h`; the empty __true_type temporary (user ctor, no byte store) is homed in the `x`
// argument slot.
struct SGoodsPlace {                        // 36 bytes
    long m_00, m_04, m_08, m_0c, m_10, m_14, m_18, m_1c, m_20;
    __forceinline SGoodsPlace(const SGoodsPlace& o)
        : m_00(o.m_00), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10),
          m_14(o.m_14), m_18(o.m_18), m_1c(o.m_1c), m_20(o.m_20) {}
};
struct __true_type { __true_type() {} };    // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }
struct CGoodsPlaceVector {
    SGoodsPlace* m_start;                   // +0x00
    SGoodsPlace* m_finish;                  // +0x04
    SGoodsPlace* m_end_of_storage;          // +0x08
    void _M_insert_overflow(SGoodsPlace* position, const SGoodsPlace& x, const __true_type&,
                            unsigned long fill_len, bool atend);   // 0x00777FE0
    void push_back(const SGoodsPlace& x);
};
void CGoodsPlaceVector::push_back(const SGoodsPlace& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SGoodsPlace(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __true_type(), 1, true);
}