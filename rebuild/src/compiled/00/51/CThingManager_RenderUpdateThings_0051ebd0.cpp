// CThingManager::RenderUpdateThings(const CRenderUpdateContext& context) -- for update lists 1..15 whose list record enables rendering, runs the two render-update virtuals on every living, render-updated thing of the list
// Leaf: RenderUpdateThings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051EBD0, length 124 bytes (`ret 4`, + int3 padding).
// Callees: CThing vtable 0x01239ABC slot 21 (+0x54, 0x004C7460 -- manifest "CThingFilter_IsUnconscious::operator()" is a propagated
// byte-twin label) and slot 26 (+0x68, 0x004CD1D0), both taking the context argument.
// Layout (retail, local model): CThingManager +4 points at an array of 20-byte per-list records whose enable byte is at +8;
// UpdateLists (vector<ThingLList> begin) at retail +0x8c; CThing update next link +0x88, flag byte +0x91 (bit 0 dead, bit 3
// render-updated).
// Shape: `for (long i = 1; i < 16; ++i)` strength-reduced into two cursors (ebx = i*20, ebp = i*8, `cmp ebx,140h`); `this` is
// spilled to the frame across the inner walk; the next link is read before the flag tests.
class CRenderUpdateContext;
class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void RenderUpdate(const CRenderUpdateContext& context);       // slot 21 (+0x54)
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual void RenderUpdatePost(const CRenderUpdateContext& context);   // slot 26 (+0x68)
    unsigned char _pad04[0x88 - 0x04];
    CThing*       m_pNextUpdate;                // +0x88
    unsigned char _pad8c[0x91 - 0x8c];
    unsigned char m_dead : 1;                   // +0x91 bit 0
    unsigned char m_91_1 : 2;
    unsigned char m_renderUpdated : 1;          // +0x91 bit 3
    unsigned char m_91_4 : 4;
};
struct ThingLList {                             // 8 bytes: head, tail
    CThing* m_pHead;
    CThing* m_pTail;
};
struct SUpdateListRecord {                      // 20 bytes
    unsigned char _pad00[8];
    bool          m_bRender;                    // +0x08
    unsigned char _pad09[0x14 - 0x09];
};
struct CThingManager {
    void*              __vftable;
    SUpdateListRecord* m_pUpdateListRecords;    // +0x04
    unsigned char      _pad08[0x8c - 0x08];
    ThingLList*        UpdateLists;             // +0x8c
    void RenderUpdateThings(const CRenderUpdateContext& context);
};

void CThingManager::RenderUpdateThings(const CRenderUpdateContext& context)
{
    for (long i = 1; i < 16; ++i) {
        if (m_pUpdateListRecords[i].m_bRender) {
            CThing* next = UpdateLists[i].m_pHead;
            while (next) {
                CThing* thing = next;
                next = thing->m_pNextUpdate;
                if (!thing->m_dead && thing->m_renderUpdated) {
                    thing->RenderUpdate(context);
                    thing->RenderUpdatePost(context);
                }
            }
        }
    }
}