// NUISystem::CFrontEndList::ScrollUp() -- move the front-end list selection up one row (wrapping), shifting every row's colour / position to its neighbour's and refading by distance from the selection
// Leaf: ?ScrollUp@CFrontEndList@NUISystem@@
// Retail: Fable.exe @ 0x0054C4C0, 834 bytes, `ret`, /O2. With at most one row, or at the top of a list that does not
// wrap (a scrolling list: own slot 171, +0x2AC), the front-end manager (CManager::GetInstance 0x0041E5F2) plays the misc-things
// def's "denied" sound (+0x1A4; manager slots 42 GetMiscThingsDef by value, 40 PlaySound) and returns; otherwise it
// plays the scroll sound (+0x194), drops the old selection (row slot 48 SetState(4)), moves SelectedChild up (wrapping
// to the last row) and highlights it (SetState(3)). Each row then takes its upper neighbour's colour (row slots 56
// GetColour / 38 SetColour, over the list's scroll time, own slot 54, 0 when it reports -1) and, for a scrolling list, moves
// by (0, 30) (slot 32); the first row takes the last row's colour. Finally, for a scrolling list, every row is refaded:
// alpha = 255 * max(0, 1 - |SelectedChild - i| * PDefAlphaOffset / 255) (`_ftol` 0x00BFEA70).
// Shape: two guard blocks each playing the denied sound and returning (the landed ScrollDown 0x0054C810 shape; VC7.1
// merges them into retail's single denied tail ahead of the main path, with retail's eax/edx picks; one `!CanScrollUp()`
// guard gives the same block order but swaps those registers, 834v834).
// The selection step goes through a local (`selected`): load, dec, store, store again after the wrap, index from eax.
// The def pointer temporary is released through the ref-count base (`dec` forwarded to the test, virtual slot 1).
// Layout: Ego_r.pdb CList / CFrontEndList names at the retail offsets (SelectedChild +0x15C, ScrollingChildren +0x164,
// PDefAlphaOffset +0x1F8; the PDB's 16-byte vectors put them 0x30..0x44 higher); engine/CFrontEndList.h is a flattened
// 0x240-byte blob, so a local model.
#include <stdlib.h>
#pragma intrinsic(abs)
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CSoundHandle { long m_id; };
struct CUIMiscThingsDef : CDefBase {
    unsigned char _pad08[0x18c];
    CSoundHandle  ListScrollSound;              // +0x194
    unsigned char _pad198[0xc];
    CSoundHandle  ListDeniedSound;              // +0x1a4
};
struct CMiscThingsDefPointer {
    CUIMiscThingsDef* m_p;
    ~CMiscThingsDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CUIMiscThingsDef* operator->() const { return m_p; }
};
namespace NUISystem {
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CRGBColour {
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& c) : Colour(c.Colour) { }
};
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031();
    virtual void MoveBy(const C2DVector& offset, float time, bool immediate);          // slot 32 (+0x80) (name descriptive)
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void SetColour(const CRGBColour& colour, float time, bool immediate);       // slot 38 (+0x98)
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047();
    virtual void SetState(long state);                                                // slot 48 (+0xc0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual C2DVector GetPosition();                                                  // slot 55 (+0xdc)
    virtual CRGBColour GetColour();                                                   // slot 56 (+0xe0)
};
class CManager {
public:
    static CManager* __fastcall GetInstance();                                // 0x0041E5F2
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void PlaySound(const CSoundHandle& sound, bool interrupt);                // slot 40 (+0xa0)
    virtual void v041();
    virtual CMiscThingsDefPointer GetMiscThingsDef();                                 // slot 42 (+0xa8)
};
struct CComponentVector {                       // vector<CComponent*> (12 bytes)
    CComponent** _M_start;
    CComponent** _M_finish;
    CComponent** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CComponent* operator[](unsigned long n) const { return _M_start[n]; }
};
class CFrontEndList {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual float GetScrollTime();                                                    // slot 54 (+0xd8) (name descriptive)
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156();
    virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170();
    virtual bool IsScrollingList();                                                        // slot 171 (+0x2ac) (name descriptive)
    void ScrollUp();
    unsigned char    _pad004[0x158];
    long             SelectedChild;             // +0x15c
    float            LastInputTime;             // +0x160
    CComponentVector ScrollingChildren;         // +0x164
    unsigned char    _pad170[0x88];
    unsigned char    PDefAlphaOffset;           // +0x1f8
};
void CFrontEndList::ScrollUp()
{
    if (ScrollingChildren.size() <= 1)
    {
        CManager::GetInstance()->PlaySound(CManager::GetInstance()->GetMiscThingsDef()->ListDeniedSound, true);
        return;
    }
    if (SelectedChild == 0 && IsScrollingList())
    {
        CManager::GetInstance()->PlaySound(CManager::GetInstance()->GetMiscThingsDef()->ListDeniedSound, true);
        return;
    }
    CManager::GetInstance()->PlaySound(CManager::GetInstance()->GetMiscThingsDef()->ListScrollSound, true);
    ScrollingChildren[SelectedChild]->SetState(4);
    long selected = SelectedChild - 1;
    SelectedChild = selected;
    if (selected < 0)
        selected = ScrollingChildren.size() - 1;
    SelectedChild = selected;
    ScrollingChildren[selected]->SetState(3);
    float time = GetScrollTime() != -1.0f ? GetScrollTime() : 0.0f;
    CRGBColour lastColour = ScrollingChildren[ScrollingChildren.size() - 1]->GetColour();
    C2DVector lastPosition = ScrollingChildren[ScrollingChildren.size() - 1]->GetPosition();
    C2DVector offset(0.0f, 30.0f);
    for (unsigned long i = ScrollingChildren.size() - 1; i != 0; --i) {
        ScrollingChildren[i]->SetColour(ScrollingChildren[i - 1]->GetColour(), time, false);
        if (IsScrollingList())
            ScrollingChildren[i]->MoveBy(offset, time, false);
    }
    ScrollingChildren[0]->SetColour(lastColour, time, false);
    if (IsScrollingList())
        ScrollingChildren[0]->MoveBy(offset, time, false);
    if (IsScrollingList()) {
        CRGBColour colour(255, 255, 255, 255);
        for (unsigned long i = 0; i < ScrollingChildren.size(); ++i) {
            float alpha = 1.0f - (float)abs(SelectedChild - (long)i) * ((float)PDefAlphaOffset * (1.0f / 255.0f));
            colour.A = (unsigned char)(long)(alpha < 0.0f ? 0.0f : alpha * 255.0f);
            ScrollingChildren[i]->SetColour(colour, time, false);
        }
    }
}
}
