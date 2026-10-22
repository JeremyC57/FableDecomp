// Grade: DIFFER(555v551) -- allocator temporary: retail passes one empty local (esp+0x13) to every _Destroy and the finder; ours materialises a byte temporary per call; this sits in ebp instead of edi
// CMap::InitCachedVolumes()
// Leaf: ?InitCachedVolumes@CMap@
void* operator new(unsigned int size);              // 0x00BFEA1A
void operator delete(void* p);                      // 0x00BFE9BC
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
struct CVolume { long a, b, c; };                   // 12-byte sorted element
struct CNullAllocator {};
void __fastcall DestroyRange(CVolume* first, CVolume* last, const CNullAllocator& a);   // 0x00825AE0
struct CVolumeLess {};
void __fastcall IntrosortLoop(CVolume* first, CVolume* last, CVolume* tag, long depth, CVolumeLess comp);   // 0x008274B0
void __fastcall InsertionSort(CVolume* first, CVolume* last, CVolumeLess comp);                            // 0x008270E0
void __fastcall UnguardedInsertionSort(CVolume* first, CVolume* last, CVolume* tag, CVolumeLess comp);     // 0x00825B40
struct CVolumeVector {
    CVolume*      _M_start;
    CVolume*      _M_finish;
    CVolume*      _M_end_of_storage;
    CVolumeLess   Compare;                          // +0x0c
    bool          Sorted;                           // +0x0d
    CVolumeVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Sorted(false) {}
    ~CVolumeVector()
    {
        DestroyRange(_M_start, _M_finish, CNullAllocator());
        if (_M_start)
            free(_M_start);
    }
    __forceinline void Sort()
    {
        CVolume* first = _M_start;
        CVolume* last = _M_finish;
        if (first != last) {
            long n = last - first;
            long lg = 0;
            for (; n != 1; n >>= 1)
                ++lg;
            IntrosortLoop(first, last, 0, lg * 2, Compare);
            CVolumeLess comp = Compare;
            if (last - first > 16) {
                InsertionSort(first, first + 16, comp);
                UnguardedInsertionSort(first + 16, last, 0, comp);
            } else
                InsertionSort(first, last, comp);
        }
    }
};
struct CCachedVolumes : CVolumeVector {
    void Set(const CVolumeVector& volumes);         // 0x00A5ACA0
};
struct CThing;
struct CThingVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
};
struct CPhysicsMeshObject {
    void*         __vftable;
    void*         m_pMesh;
    unsigned char _pad08[0x38];
    ~CPhysicsMeshObject();                          // 0x00A127D0
    void AddVolumes(CVolumeVector& volumes);        // 0x00A12EE0
};
struct CTCPhysicsBase {
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
    virtual void v090(); virtual void v091(); virtual void v092();
    virtual void* GetPhysicsMeshParameters();       // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject();      // 0x006B0850
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};
struct CMapUpdateSink;
struct CRegionPredicate {
    CMapUpdateSink* Sink;
    long            Region;
};
struct CEmpty {};
struct CThingFinder {
    CRegionPredicate* Predicate;
    CEmpty*           Extra;
};
struct CThingManager {
    void FindThings(const CThingFinder& finder, CThingVector& out);   // 0x008266E0
};
struct CMapRegionTable {
    virtual void r00(); virtual void r01(); virtual void r02(); virtual void r03(); virtual void r04(); virtual void r05();
    virtual void r06(); virtual void r07(); virtual void r08(); virtual void r09(); virtual void r10(); virtual void r11();
    virtual void r12(); virtual void r13(); virtual void r14(); virtual void r15(); virtual void r16();
    virtual long GetRegionIndex(const void* key);  // slot 17 (+0x44)
};
struct CMapOwner {
    virtual void o00(); virtual void o01(); virtual void o02(); virtual void o03(); virtual void o04(); virtual void o05();
    virtual void o06(); virtual void o07(); virtual void o08(); virtual void o09(); virtual void o10(); virtual void o11();
    virtual CMapUpdateSink* GetUpdateSink();       // slot 12 (+0x30)
    virtual CMapRegionTable* GetRegionTable();     // slot 13 (+0x34)
    unsigned char  _pad04[0x20 - 0x04];
    CThingManager* PThingManager;                  // +0x20
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
};
struct CMap {
    void*                       __vftable;
    unsigned char               _pad04[0x08 - 0x04];
    scoped_ptr<CCachedVolumes>  CachedVolumes;     // +0x08
    unsigned char               _pad0c[0x20 - 0x0c];
    CMapOwner*                  POwner;            // +0x20
    unsigned char               _pad24[0x1d24 - 0x24];
    unsigned char               RegionKey[4];      // +0x1d24
    void InitCachedVolumes();
};

void CMap::InitCachedVolumes()
{
    CachedVolumes.reset(new CCachedVolumes);
    CMapOwner* owner = POwner;
    owner->GetRegionTable()->GetRegionIndex(RegionKey);
    owner->GetUpdateSink();
    CVolumeVector volumes;
    CThingVector things;
    owner = POwner;
    long region = owner->GetRegionTable()->GetRegionIndex(RegionKey);
    CRegionPredicate predicate;
    predicate.Sink = owner->GetUpdateSink();
    predicate.Region = region;
    CEmpty extra;
    CThingFinder finder;
    finder.Predicate = &predicate;
    finder.Extra = &extra;
    POwner->PThingManager->FindThings(finder, things);
    long count = things._M_finish - things._M_start;
    for (long i = 0; i < count; ++i) {
        CThing* thing = things._M_start[i];
        if (thing->PhysicsTC->GetPhysicsMeshParameters()) {
            CPhysicsMeshObject mesh = thing->PhysicsTC->GetPhysicsMeshObject();
            mesh.AddVolumes(volumes);
        }
    }
    volumes.Sort();
    volumes.Sorted = false;
    CachedVolumes->Set(volumes);
}
