// CTCInventoryMap::PanMap(C2DVector delta) -- pan the zooming map by delta and, once it has moved, select the list entry of the region whose name graphic lies nearest the map centre (within 128 units)
// Grade: DIFFER(873v877) -- whole body matches except the region-scan loop's allocation: retail keeps the iterator cached in edi (lea ecx,[edi+14h]) with the best node and best distance in frame words 0x14/0x0C, the lane puts the best node in edi and reloads the iterator (9 grades: copy ctor, operator=, declaration order, while loop, node local, nested if)
#include <math.h>
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};
struct CRegion {
    C2DVector GetNameGraphicOffset() const;             // 0x006BBF40
};
struct SRegionNode {                                    // STLport _Rb_tree_node<pair<const long, CRegion> >
    long _M_color; SRegionNode* _M_parent; SRegionNode* _M_left; SRegionNode* _M_right;
    long first;                                         // +0x10
    CRegion second;                                     // +0x14
};
struct CRegionIterator {
    SRegionNode* _M_node;
    CRegionIterator() {}
    CRegionIterator(SRegionNode* node) : _M_node(node) {}
    CRegionIterator(const CRegionIterator& o) : _M_node(o._M_node) {}
    CRegionIterator& operator++();                      // 0x00613630
    bool operator!=(const CRegionIterator& other) const { return _M_node != other._M_node; }
    SRegionNode* operator->() const { return _M_node; }
};
struct CRegionMap {                                     // STLport map<long, CRegion>
    SRegionNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    CRegionIterator begin() { return CRegionIterator(_M_header->_M_left); }
    CRegionIterator end() { return CRegionIterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
};
struct CStateFlags {
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void SetFlag(long flag);                    // slot 3 (+0x0C)
    virtual void ClearFlag(long flag);                  // slot 4 (+0x10)
};
struct CListItemBase {
    virtual void b00();
};
struct CListItem : CListItemBase, CStateFlags {
};
struct CListItemVector {
    CListItem** _M_start;
    CListItem** _M_finish;
    CListItem** _M_end_of_storage;
    CListItem*& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CZoomingComponent {
    virtual void z000(); virtual void z001(); virtual void z002(); virtual void z003(); virtual void z004(); virtual void z005();
    virtual void z006(); virtual void z007(); virtual void z008(); virtual void z009(); virtual void z010(); virtual void z011();
    virtual C2DVector GetZoom();                               // slot 12 (+0x30)
    virtual void z013(); virtual void z014(); virtual void z015();
    virtual C2DVector GetMousePos();                           // slot 16 (+0x40)
    virtual void z017(); virtual void z018(); virtual void z019(); virtual void z020(); virtual void z021(); virtual void z022();
    virtual void z023(); virtual void z024(); virtual void z025(); virtual void z026(); virtual void z027(); virtual void z028();
    virtual void z029(); virtual void z030(); virtual void z031(); virtual void z032(); virtual void z033(); virtual void z034();
    virtual void z035(); virtual void z036(); virtual void z037(); virtual void z038(); virtual void z039(); virtual void z040();
    virtual void z041(); virtual void z042(); virtual void z043(); virtual void z044(); virtual void z045(); virtual void z046();
    virtual void z047(); virtual void z048(); virtual void z049(); virtual void z050(); virtual void z051(); virtual void z052();
    virtual void z053(); virtual void z054(); virtual void z055(); virtual void z056(); virtual void z057(); virtual void z058();
    virtual void z059(); virtual void z060(); virtual void z061(); virtual void z062(); virtual void z063(); virtual void z064();
    virtual void z065(); virtual void z066(); virtual void z067(); virtual void z068(); virtual void z069(); virtual void z070();
    virtual void z071(); virtual void z072(); virtual void z073(); virtual void z074(); virtual void z075(); virtual void z076();
    virtual void z077(); virtual void z078(); virtual void z079(); virtual void z080(); virtual void z081(); virtual void z082();
    virtual void z083(); virtual void z084(); virtual void z085(); virtual void z086(); virtual void z087(); virtual void z088();
    virtual void z089(); virtual void z090(); virtual void z091(); virtual void z092(); virtual void z093(); virtual void z094();
    virtual void z095(); virtual void z096(); virtual void z097(); virtual void z098(); virtual void z099(); virtual void z100();
    virtual void z101(); virtual void z102(); virtual void z103(); virtual void z104(); virtual void z105(); virtual void z106();
    virtual void z107(); virtual void z108(); virtual void z109(); virtual void z110(); virtual void z111(); virtual void z112();
    virtual void z113(); virtual void z114(); virtual void z115(); virtual void z116(); virtual void z117(); virtual void z118();
    virtual void z119(); virtual void z120(); virtual void z121(); virtual void z122(); virtual void z123(); virtual void z124();
    virtual void z125(); virtual void z126(); virtual void z127(); virtual void z128(); virtual void z129(); virtual void z130();
    virtual void z131(); virtual void z132(); virtual void z133(); virtual void z134(); virtual void z135(); virtual void z136();
    virtual void z137(); virtual void z138(); virtual void z139(); virtual void z140(); virtual void z141();
    virtual void Pan(C2DVector delta);                         // slot 142 (+0x238)
    virtual void z143(); virtual void z144(); virtual void z145();
    virtual C2DVector GetCentre();                             // slot 146 (+0x248)
};
struct CList {
    virtual void l000(); virtual void l001(); virtual void l002(); virtual void l003(); virtual void l004(); virtual void l005();
    virtual void l006(); virtual void l007(); virtual void l008(); virtual void l009(); virtual void l010(); virtual void l011();
    virtual void l012(); virtual void l013(); virtual void l014(); virtual void l015(); virtual void l016(); virtual void l017();
    virtual void l018(); virtual void l019(); virtual void l020(); virtual void l021(); virtual void l022(); virtual void l023();
    virtual void l024(); virtual void l025(); virtual void l026(); virtual void l027(); virtual void l028(); virtual void l029();
    virtual void l030(); virtual void l031(); virtual void l032(); virtual void l033(); virtual void l034(); virtual void l035();
    virtual void l036(); virtual void l037(); virtual void l038(); virtual void l039(); virtual void l040(); virtual void l041();
    virtual void l042(); virtual void l043(); virtual void l044(); virtual void l045(); virtual void l046(); virtual void l047();
    virtual void l048(); virtual void l049(); virtual void l050(); virtual void l051(); virtual void l052(); virtual void l053();
    virtual void l054(); virtual void l055(); virtual void l056(); virtual void l057(); virtual void l058(); virtual void l059();
    virtual void l060(); virtual void l061(); virtual void l062(); virtual void l063(); virtual void l064(); virtual void l065();
    virtual void l066(); virtual void l067(); virtual void l068(); virtual void l069(); virtual void l070(); virtual void l071();
    virtual void l072(); virtual void l073(); virtual void l074(); virtual void l075(); virtual void l076(); virtual void l077();
    virtual void l078(); virtual void l079(); virtual void l080(); virtual void l081(); virtual void l082(); virtual void l083();
    virtual void l084(); virtual void l085();
    virtual long GetState();                                   // slot 86 (+0x158)
    virtual void l087(); virtual void l088(); virtual void l089(); virtual void l090(); virtual void l091(); virtual void l092();
    virtual void l093(); virtual void l094(); virtual void l095(); virtual void l096(); virtual void l097(); virtual void l098();
    virtual void l099(); virtual void l100(); virtual void l101(); virtual void l102(); virtual void l103(); virtual void l104();
    virtual void l105(); virtual void l106(); virtual void l107(); virtual void l108(); virtual void l109(); virtual void l110();
    virtual void l111(); virtual void l112(); virtual void l113(); virtual void l114(); virtual void l115(); virtual void l116();
    virtual void l117(); virtual void l118(); virtual void l119(); virtual void l120(); virtual void l121(); virtual void l122();
    virtual void l123(); virtual void l124(); virtual void l125(); virtual void l126(); virtual void l127(); virtual void l128();
    virtual void l129(); virtual void l130(); virtual void l131(); virtual void l132(); virtual void l133(); virtual void l134();
    virtual void l135(); virtual void l136(); virtual void l137(); virtual void l138(); virtual void l139(); virtual void l140();
    virtual void l141();
    virtual long GetSelectedValue();                           // slot 142 (+0x238)
    virtual void l143(); virtual void l144(); virtual void l145(); virtual void l146(); virtual void l147(); virtual void l148();
    virtual void l149(); virtual void l150(); virtual void l151(); virtual void l152(); virtual void l153(); virtual void l154();
    virtual void l155(); virtual void l156(); virtual void l157(); virtual void l158(); virtual void l159(); virtual void l160();
    virtual void l161(); virtual void l162();
    virtual CListItemVector* GetItems();                       // slot 163 (+0x28C)
    virtual void l164(); virtual void l165(); virtual void l166(); virtual void l167(); virtual void l168(); virtual void l169();
    virtual void l170(); virtual void l171(); virtual void l172();
    virtual void SelectItem(CListItem* item);                  // slot 173 (+0x2B4)
};
struct CTCInventoryMap {
    unsigned char _pad00[0x188];
    CZoomingComponent* pmap_real;                       // +0x188
    unsigned char _pad18c[0x214 - 0x18c];
    CList*         MapList;                             // +0x214
    CRegionMap     MapRegions;                          // +0x218
    void PanMap(C2DVector delta);
};

void CTCInventoryMap::PanMap(C2DVector delta)
{
    if (MapList && pmap_real && MapList->GetState() == 3) {
        pmap_real->Pan(delta);
        if (sqrt(delta.X * delta.X + delta.Y * delta.Y) < 0.01)
            return;
        C2DVector zoom = pmap_real->GetZoom();
        C2DVector centre = pmap_real->GetCentre();
        C2DVector pos = (pmap_real->GetMousePos() - centre) * -1.0f;
        pos.X = pos.X / pmap_real->GetZoom().X;
        pos.Y = pos.Y / pmap_real->GetZoom().Y;
        if (MapRegions.size() == 0)
            return;
        bool first = true;
        float best = 0.0f;
        CRegionIterator bestIt = 0;
        for (CRegionIterator it = MapRegions.begin(); it != MapRegions.end(); ++it) {
            C2DVector offset = it->second.GetNameGraphicOffset();
            C2DVector d = offset - pos;
            float dist = (float)sqrt(d.X * d.X + d.Y * d.Y);
            if ((dist < best || first) && dist < 128.0f) {
                best = dist;
                first = false;
                bestIt = it;
            }
        }
        if (first == true)
            return;
        long key = bestIt->first;
        if (key == MapList->GetSelectedValue())
            return;
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(2);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(3);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(4);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(5);
        MapList->SelectItem((*MapList->GetItems())[key]);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(2);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(3);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(4);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(5);
    }
}
