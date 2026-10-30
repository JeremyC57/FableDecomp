// NUISystem::CList::InitialiseOffsets(unsigned long count) -- start the scrolling layout centred on the visible children
// Leaf: ?InitialiseOffsets@CList@NUISystem@@UAEXK@Z
// Retail 0x00536B4F, 277 bytes, `ret 4` (Ego_r.pdb ?InitialiseOffsets@CList@NUISystem@@UAEXK@Z); /Os + optimize("y",off)
// TU like the landed CList rows. Half the child count (count / 2 for an odd count, count / 2 - 1 for an even one;
// unsigned -> float with the 2^32 bias 0x0122DCB4) steps the first scrolling child back from the list position:
// next position = (half * GetPositionOffset().X, -(half * GetPositionOffset().Y)) and next alpha = 1.0f -
// GetAlphaOffset() * half; when the list's UI def is PC style (CUIDef::PCStyle +0x221, read through the by-value
// CDefPointer<const CUIDef> of LoadAndReturnDef(), released inline) the position is reset to (0, 0). The next-child
// position is set twice.
// Virtual slots (CList vtable 0x012462E4), names Ego_r.pdb: 108 LoadAndReturnDef() const (landed 0x00530EC0),
// 147 GetPositionOffset (UAE?AVC2DVector@@XZ, body 0x0053CDCA), 148 GetAlphaOffset (UAEMXZ, 0x0053CDE1),
// 150 SetNextScrollingChildAlpha(float) (0x0053CDF2), 151 SetNextScrollingChildPosition(C2DVector&) (0x0053CDFF).
// PCStyle at retail +0x221 (PDB +0x255): the landed CComponent::IsPCStyle 0x00530740.
// Layout: local model as the landed CList rows (engine/CList.h is the Ego_r flattened layout; retail offsets differ).
// Shape: both arms written out with the conversion `(count >> 1)` / `((count >> 1) - 1)` repeated in each expression
// (VC7.1 hoists the arms' identical leading GetPositionOffset() call above the branch, CSEs the converted half into the
// dead `count` slot and cross-jumps the arms); the alpha is named as `1.0f - GetAlphaOffset() * half` (retail's
// in-place `fsubr [1.0f]`, the vtable reloaded for the Set call); the def flag is read into a bool before the release.
#pragma optimize("s", on)
#pragma optimize("y", off)

class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};

namespace NUISystem {

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                 // slot 1
};
#pragma pack(push, 2)
struct CUIDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x221 - 8];
    bool          PCStyle;                  // +0x221 (PDB +0x255)
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
#pragma pack(pop)
template <class T> struct CDefPointer {
    T* Data;
    __forceinline ~CDefPointer()
    {
        if (Data) {
            const_cast<CUIDef*>(Data)->ReleaseTemporary();
            Data = 0;
        }
    }
    T* operator->() const { return Data; }
};

class CList
{
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
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer<const CUIDef> LoadAndReturnDef() const;    // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146();
    virtual C2DVector GetPositionOffset();                      // slot 147 (+0x24c)
    virtual float GetAlphaOffset();                             // slot 148 (+0x250)
    virtual void v149();
    virtual void SetNextScrollingChildAlpha(float alpha);       // slot 150 (+0x258)
    virtual void SetNextScrollingChildPosition(C2DVector& position);   // slot 151 (+0x25c)
    virtual void InitialiseOffsets(unsigned long count);        // (slot not needed here; placed last)
};

void CList::InitialiseOffsets(unsigned long count)
{
    C2DVector position;
    if (count & 1)
    {
        position.X = GetPositionOffset().X * (count >> 1);
        position.Y = -(GetPositionOffset().Y * (count >> 1));
        float alpha = 1.0f - GetAlphaOffset() * (count >> 1);
        SetNextScrollingChildAlpha(alpha);
    }
    else
    {
        position.X = GetPositionOffset().X * ((count >> 1) - 1);
        position.Y = -(GetPositionOffset().Y * ((count >> 1) - 1));
        float alpha = 1.0f - GetAlphaOffset() * ((count >> 1) - 1);
        SetNextScrollingChildAlpha(alpha);
    }
    bool pcStyle = LoadAndReturnDef()->PCStyle;
    if (pcStyle)
    {
        position.X = 0.0f;
        position.Y = 0.0f;
    }
    SetNextScrollingChildPosition(position);
    SetNextScrollingChildPosition(position);
}

}
