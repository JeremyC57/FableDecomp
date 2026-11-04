// CSoundEventSet::CSoundEventSet(long eventCount, long groupCount, long poolSize) -- EH-framed constructor of the libpk sound event set: the keyed tree at +0, the per-event tables sized from the event count (the first one twice over), their five index views, the group tables, four more keyed trees, the event-slot table, then the handle pool initialised for the pool size
// Leaf: ??0CSoundEventSet@@QAE@JJJ@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C222D0, 583 bytes, thiscall `ret 0Ch`, /EHsc (EH states 0..0x1b).
// Identity: libpk sound event set (landed caller CSoundEventManager ctor 0x00C11D00 constructs it at +0x2c).
// Callees: PKCTRRedblackTree 0x00C194F0 inside five inline keyed-tree wrappers (vtables 0x012B3EC8, 0x012B3BA8, 0x012B3860,
//   0x012B38A8, 0x012B38F0), the table / view ctors, CHandleBasedMemoryPool_012b4098 0x00C28AE0 + Initialise 0x00C28240.
// Shape: one init-list entry per member, the pool initialised in the body.
struct PKCTRAllocator;
class PKCTRRedblackTree {                          // libpk red-black tree, 0x28 bytes
public:
    unsigned char _body[0x28];
    PKCTRRedblackTree(PKCTRAllocator* allocator);  // 0x00C194F0
    ~PKCTRRedblackTree();                          // 0x00C19250
};
struct CKeyedTreeBase {                            // the keyed-tree interface: handed the derived tree's address
    virtual void v00();
    PKCTRRedblackTree* m_pTree;                    // +0x04
    CKeyedTreeBase(PKCTRRedblackTree* tree) : m_pTree(tree) { }
    ~CKeyedTreeBase() { }
};
struct CVtable_012b3ec8 : CKeyedTreeBase {         // 0x30 bytes
    PKCTRRedblackTree m_tree;                      // +0x08
    CVtable_012b3ec8() : CKeyedTreeBase(&m_tree), m_tree(0) { }
    virtual void v00();
    ~CVtable_012b3ec8();
};
struct CVtable_012b3ba8 : CKeyedTreeBase {
    PKCTRRedblackTree m_tree;
    CVtable_012b3ba8() : CKeyedTreeBase(&m_tree), m_tree(0) { }
    virtual void v00();
    ~CVtable_012b3ba8();                           // 0x00C249E0
};
struct CVtable_012b3860 : CKeyedTreeBase {
    PKCTRRedblackTree m_tree;
    CVtable_012b3860() : CKeyedTreeBase(&m_tree), m_tree(0) { }
    virtual void v00();
    ~CVtable_012b3860();
};
struct CVtable_012b38a8 : CKeyedTreeBase {
    PKCTRRedblackTree m_tree;
    CVtable_012b38a8() : CKeyedTreeBase(&m_tree), m_tree(0) { }
    virtual void v00();
    ~CVtable_012b38a8();
};
struct CVtable_012b38f0 : CKeyedTreeBase {
    PKCTRRedblackTree m_tree;
    CVtable_012b38f0() : CKeyedTreeBase(&m_tree), m_tree(0) { }
    virtual void v00();
    ~CVtable_012b38f0();
};

struct CTable_00c239b0 { unsigned char _body[0x18]; CTable_00c239b0(long count); ~CTable_00c239b0(); };   // 0x00C239B0
struct CTable_00c23c10 { unsigned char _body[0x18]; CTable_00c23c10(long count); ~CTable_00c23c10(); };   // 0x00C23C10
struct CTable_00c23e00 { unsigned char _body[0x18]; CTable_00c23e00(long count); ~CTable_00c23e00(); };   // 0x00C23E00
struct CTable_00c24050 { unsigned char _body[0x18]; CTable_00c24050(long count); ~CTable_00c24050(); };   // 0x00C24050
struct CView_00c252d0 { unsigned char _body[0x30]; CView_00c252d0(CTable_00c23c10* table); ~CView_00c252d0(); };   // 0x00C252D0
struct CView_00c25370 { unsigned char _body[0x30]; CView_00c25370(CTable_00c23c10* table); ~CView_00c25370(); };   // 0x00C25370
struct CView_00c25410 { unsigned char _body[0x30]; CView_00c25410(CTable_00c23c10* table); ~CView_00c25410(); };   // 0x00C25410
struct CView_00c25460 { unsigned char _body[0x30]; CView_00c25460(CTable_00c23c10* table); ~CView_00c25460(); };   // 0x00C25460
struct CView_00c254b0 { unsigned char _body[0x30]; CView_00c254b0(CTable_00c23c10* table); ~CView_00c254b0(); };   // 0x00C254B0

class CHandleBasedMemoryPool_012b4098 {           // the libpk build of the handle pool (vtable 0x012B4098)
public:
    unsigned char _body[0x454];
    CHandleBasedMemoryPool_012b4098();                                  // 0x00C28AE0
    ~CHandleBasedMemoryPool_012b4098();
    bool Initialise(unsigned long size, unsigned long flags);           // 0x00C28240
};

struct CSoundEventSet {
    CVtable_012b3ec8 m_eventTree;                  // +0x000
    CTable_00c239b0  m_events;                     // +0x030
    CTable_00c23c10  m_table48;                    // +0x048 (2 x events)
    CTable_00c23c10  m_table60;                    // +0x060
    CTable_00c23c10  m_table78;                    // +0x078
    CTable_00c23c10  m_table90;                    // +0x090
    CTable_00c23c10  m_tableA8;                    // +0x0a8
    CTable_00c23c10  m_tableC0;                    // +0x0c0
    CView_00c252d0   m_viewD8;                     // +0x0d8 over +0x48
    CView_00c25370   m_view108;                    // +0x108 over +0x60
    CView_00c25410   m_view138;                    // +0x138 over +0x78
    CView_00c25410   m_view168;                    // +0x168 over +0x90
    CView_00c25410   m_view198;                    // +0x198 over +0xa8
    CView_00c25460   m_view1C8;                    // +0x1c8 over +0xc0
    CTable_00c23e00  m_groups;                     // +0x1f8
    CTable_00c23c10  m_table210;                   // +0x210
    CView_00c254b0   m_view228;                    // +0x228 over +0x210
    CVtable_012b3ba8 m_tree258;                    // +0x258
    CVtable_012b3860 m_tree288;                    // +0x288
    CVtable_012b38a8 m_tree2B8;                    // +0x2b8
    CVtable_012b38f0 m_tree2E8;                    // +0x2e8
    CTable_00c24050  m_slots;                      // +0x318
    long             m_330;                        // +0x330
    CHandleBasedMemoryPool_012b4098 m_pool;        // +0x334
    CSoundEventSet(long eventCount, long groupCount, long poolSize);
};

CSoundEventSet::CSoundEventSet(long eventCount, long groupCount, long poolSize)
    : m_events(eventCount),
      m_table48(eventCount * 2),
      m_table60(eventCount),
      m_table78(eventCount),
      m_table90(eventCount),
      m_tableA8(eventCount),
      m_tableC0(eventCount),
      m_viewD8(&m_table48),
      m_view108(&m_table60),
      m_view138(&m_table78),
      m_view168(&m_table90),
      m_view198(&m_tableA8),
      m_view1C8(&m_tableC0),
      m_groups(groupCount),
      m_table210(groupCount),
      m_view228(&m_table210),
      m_slots(eventCount),
      m_330(0)
{
    m_pool.Initialise(poolSize, 0);
}