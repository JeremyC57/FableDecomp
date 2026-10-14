// NUISystem::CComponent::ChangeZoomDelta(const C2DVector&, long, long) -- ChangeZoom to the current zoom plus a delta
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052EC20, length 55 bytes (+ int3 padding).
// Callees: ChangeZoom (vslot 35, `call [eax+8Ch]`) with the summed vector built in the 8-byte frame
// and the two pass-through dwords.
// Vtable 0x0122F5D4 slot 36. The temporary C2DVector(Zoom.X + delta.X, Zoom.Y + delta.Y) is constructed
// in place in the frame while the trailing arguments are pushed (right to left), then its address is
// pushed last; `add esp,8` releases the frame after the __fastcall callee cleans its own arguments.
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

struct CComponent_Retail;
struct CCountedPointer_CComponent { CComponent_Retail* m_pObject; void* m_pCount; };   // 8 bytes

// NUISystem::CComponent, retail view of the Ego_r PDB layout (348 bytes there): the States map at +0x28
// is 8 bytes in retail (16 in the PDB) and each vector/list/set is 4 (or 8) bytes smaller, so Zoom sits
// at +0x5c (PDB +0x64), Children at +0xb0 (PDB +0xb8), PParent at +0xc8 (PDB +0xd8) and Layer at +0x12f
// (PDB +0x157). Virtual slots from the NUISystem::CSprite vtable 0x0122F5D4 (rtti-vtables.tsv).
struct CComponent_Retail {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void ChangeZoom(const C2DVector& zoom, long time, long flags);   // slot 35
    virtual void ChangeZoomDelta(const C2DVector& delta, long time, long flags);   // slot 36
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();

    unsigned char _pad_0x04[0x58];                          // +0x04
    C2DVector     Zoom;                                     // +0x5c
    unsigned char _pad_0x64[0x4c];                          // +0x64
    CRetailVector<CCountedPointer_CComponent> Children;     // +0xb0
    unsigned char _pad_0xbc[0x0c];                          // +0xbc ChildrenToDelete
    CComponent_Retail* PParent;                             // +0xc8
    unsigned char _pad_0xcc[0x63];                          // +0xcc
    char          Layer;                                    // +0x12f
};

void CComponent_Retail::ChangeZoomDelta(const C2DVector& delta, long time, long flags)
{
    ChangeZoom(C2DVector(Zoom.X + delta.X, Zoom.Y + delta.Y), time, flags);
}