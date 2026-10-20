// CTCPhotojournalCapture::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the capture screen is open: tick and draw its UI components with the time since the last render, switch an engine flag off on the second frame, and from then on render the inventory backdrop
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D5D0, 227 bytes (`ret 4`). CTCPhotojournalCapture vtable slot 11 (catalog "vfunc_11";
//   GOTCHAS chunk 169: slot 11 = DrawPreparePrimitivesForRendering; PDB ?DrawPreparePrimitivesForRendering@CTCPhotojournalCapture@@UAEXABVCEnginePrimitivePrepareInfo@@@Z).
// Callees: GFGetTime() @ 0x009E1BC0 (`__fastcall`, double), the components' virtual slots 1 (+0x04, given the elapsed time) and
//   2 (+0x08, given info.Engine and four zeros) -- names from use --, the 3D engine's virtual slot 76 (+0x130, given false) through
//   g_pMainGameComponent @ 0x013B86A0 ->PDisplayEngine (+0x28) ->Engine3D (+0x2c), CVectorMap<...>::LowerBound @ 0x0040F020
//   (interface 0x11), NInventory::CTCInventoryBase::RenderUIBackdropQuad(float) @ 0x005BC6D6 (the only (float) void member in the
//   PDB class; manifest "CSpellContainerList::AddChild" is propagated). 1.0f is an immediate push.
// Layout: Ego_r.pdb CTCPhotojournalCapture (PDB = retail): FrameCounter +0x14, ScreenOpen +0x19, LastRenderTime +0x20 (double),
//   Components +0x2c (12-byte retail vector); CEnginePrimitivePrepareInfo Engine +0x34; CDisplayEngine Engine3D +0x2c.
// Shape: the elapsed float and the component loop sit in their own block (the float's frame word is reused for the
//   interface key); `++FrameCounter == 2` is the load/inc/compare/store, the `>= 2` test re-reads the member.
double __fastcall GFGetTime();                          // 0x009E1BC0
struct CTCBase;
struct CIEngine {
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
    virtual void v035();
    virtual void v036();
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
    virtual void SetEngineFlag76(bool on);              // slot 76 (+0x130)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                               // +0x34
};
namespace NUISystem {
    struct CComponent {
        virtual ~CComponent();                          // slot 0
        virtual void Update(float elapsed);             // slot 1 (+0x04)
        virtual void Draw(CIEngine* engine, long a, long b, long c, long d);   // slot 2 (+0x08)
    };
}
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CDisplayEngine {
    unsigned char _pad00[0x2c];
    CIEngine*     Engine3D;                             // +0x2c
};
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                     // +0x28 scoped_ptr
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CTCInventory {                                   // interface 0x11 (NInventory::CTCInventoryBase)
    void RenderUIBackdropQuad(float alpha);             // 0x005BC6D6
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CWideString { void* PStringData; };
struct CTCPhotojournalCapture {
    void*         __vftable;                            // +0x00 (CTCBase)
    CThing*       ParentThing;                          // +0x04
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CWideString   Description;                          // +0x0c
    bool          DiscardScreenShot;                    // +0x10
    long          FrameCounter;                         // +0x14
    bool          FirstRenderFrame;                     // +0x18
    bool          ScreenOpen;                           // +0x19
    double        LastRenderTime;                       // +0x20
    CWideString   FileName;                             // +0x28
    vector<NUISystem::CComponent*> Components;          // +0x2c
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCPhotojournalCapture::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (ScreenOpen) {
        {
            float elapsed = (float)(GFGetTime() - LastRenderTime);
            LastRenderTime = GFGetTime();
            for (unsigned long i = 0; i < Components.size(); ++i) {
                Components[i]->Update(elapsed);
                Components[i]->Draw(info.Engine, 0, 0, 0, 0);
            }
        }
        if (++FrameCounter == 2)
            g_pMainGameComponent->PDisplayEngine->Engine3D->SetEngineFlag76(false);
        if (FrameCounter >= 2) {
            CTCInventory* inventory;
            if (ParentThing->GetInterface(&inventory, 0x11))
                inventory->RenderUIBackdropQuad(1.0f);
        }
    }
}
