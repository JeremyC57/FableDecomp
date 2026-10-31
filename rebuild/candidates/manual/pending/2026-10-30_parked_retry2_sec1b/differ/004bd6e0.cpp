// Grade: DIFFER(828v845) 139 of 286 lines (was 161): setup.Info read back from the just-published global g_CurrentPrepareInfo (info no longer kept in a callee-saved register) -- residue: retail homes position in its dead parameter slot [esp+50h] (ours esi), pushes ebx/ebp/esi later, and the frame order differs (transform at +1Ch in retail, +10h here)
// CTCGraphicAppearance::SetPrimitive(CEnginePrimitivePrepareInfo* info, C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation) -- prepares every appearance primitive with the thing's decals, lights and trigger set, retrying the ones that failed once, and clears the needs-prepare flag when all succeed
#include <stdlib.h>
class CBase {
public:
    CBase();                                            // 0x0099A2D0
    ~CBase();                                           // 0x0099A2E0
};
class CRefCountedShared {
public:
    virtual void v00();
    virtual void Destroy();
    long RefCount;
};
class CAppearanceTransform : public CBase {
public:
    virtual void v00();
};
class CAppearanceSharedPointer : public CBase {
public:
    CRefCountedShared* Object;
    __forceinline CAppearanceSharedPointer() : Object(0) {}
    __forceinline ~CAppearanceSharedPointer()
    {
        if (Object) {
            --Object->RefCount;
            if (Object->RefCount == 0)
                Object->Destroy();
        }
        Object = 0;
    }
    virtual void v00();
};
struct _Rb_tree_node_base {
    char _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct CTriggerSet {
    _Rb_tree_node_base* _M_header;
    unsigned long _M_node_count;
    char _M_key_compare;
    void _M_erase(_Rb_tree_node_base* node);            // 0x004ABA70
    __forceinline CTriggerSet() : _M_header(0)
    {
        _M_header = (_Rb_tree_node_base*)malloc(0x14);
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
    __forceinline ~CTriggerSet()
    {
        if (_M_node_count) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        free(_M_header);
    }
};
struct CEnginePrimitivePrepareInfo {
    void* v00;
    class C3DPositionInterpolator* Position;            // +0x04
};
class C3DOrientationInterpolator;
struct CPrimitiveVector {
    void* _M_start;
    void* _M_finish;
};
struct CDecalSet {
    unsigned char _pad00[0x0c];
    CPrimitiveVector Decals;                            // +0x0C
};
struct CPrimitiveSetup {
    CEnginePrimitivePrepareInfo* Info;
    C3DPositionInterpolator* Position;
    C3DOrientationInterpolator* Orientation;
    CAppearanceTransform* Transform;
    CPrimitiveVector* Decals;
    CPrimitiveVector* Lights;
    CTriggerSet* Triggers;
    long DetailMode;
    bool HasTriggers;
};
class CTCGraphicAppearance;
class CAppearancePrimitive {
public:
    virtual bool Prepare(CTCGraphicAppearance* owner, CPrimitiveSetup* setup);   // slot 0
    virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool NeedsUpdate();                         // slot 4 (+0x10)
};
struct CPrimitiveNode {
    CPrimitiveNode* _M_next;
    CPrimitiveNode* _M_prev;
    CAppearancePrimitive* _M_data;
};
inline void* operator new(unsigned int, void* where) { return where; }
struct CPrimitiveList {
    CPrimitiveNode* _M_node;
    __forceinline CPrimitiveList()
    {
        CPrimitiveNode* node = (CPrimitiveNode*)malloc(0xc);
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    __forceinline ~CPrimitiveList()
    {
        CPrimitiveNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            CPrimitiveNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        free(_M_node);
    }
    __forceinline void push_back(CAppearancePrimitive* const& value)
    {
        CPrimitiveNode* node = (CPrimitiveNode*)malloc(0xc);
        new (&node->_M_data) CAppearancePrimitive*(value);
        CPrimitiveNode* pos = _M_node;
        node->_M_next = pos;
        node->_M_prev = pos->_M_prev;
        pos->_M_prev->_M_next = node;
        pos->_M_prev = node;
    }
};
class CTCDecals {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c();
    virtual CDecalSet* GetDecals();                     // slot 29 (+0x74)
};
class CTCLights {
public:
    CPrimitiveVector* GetLights();                      // 0x0070A570
};
class CTCActivationReceptor {
public:
    void GetTriggers(CTriggerSet* triggers);            // 0x00706860
};
struct CTCPair { long Type; void* TC; };
struct CTCMap {
    CTCPair* _M_start;
    CTCPair* _M_finish;
    CTCPair* LowerBound(const long& key);               // 0x004365B0
    CTCPair* LowerBoundLights(const long& key);         // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CTCMap TCs;                                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
};
class CTransformSource {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b(); virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c();
    virtual void GetShared(CAppearanceSharedPointer* a, CAppearanceSharedPointer* b);   // slot 29 (+0x74)
};
extern CEnginePrimitivePrepareInfo* g_CurrentPrepareInfo;   // 0x013B8A18
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
};
class CTCGraphicAppearance : public CTCBase {
public:
    unsigned char _pad0c[0x24 - 0x0c];
    CTransformSource* TransformSource;                  // +0x24
    unsigned char _pad28[0x3c - 0x28];
    CPrimitiveNode* Primitives;                         // +0x3C (list header node)
    unsigned char _pad40[0x58 - 0x40];
    unsigned char DetailMode;                           // +0x58
    unsigned char Flags59;                              // +0x59
    void SetPrimitive(CEnginePrimitivePrepareInfo* info, C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation);
};

void CTCGraphicAppearance::SetPrimitive(CEnginePrimitivePrepareInfo* info, C3DPositionInterpolator* position, C3DOrientationInterpolator* orientation)
{
    g_CurrentPrepareInfo = info;
    position = info->Position;
    if (Primitives->_M_next == Primitives)
        return;
    CAppearanceTransform transform;
    CAppearanceSharedPointer sharedA;
    CAppearanceSharedPointer sharedB;
    if (TransformSource)
        TransformSource->GetShared(&sharedA, &sharedB);
    CTriggerSet triggers;
    CPrimitiveSetup setup;
    setup.Info = g_CurrentPrepareInfo;
    setup.Position = position;
    setup.Orientation = orientation;
    setup.Transform = &transform;
    setup.Decals = 0;
    setup.Lights = 0;
    setup.Triggers = &triggers;
    setup.HasTriggers = false;
    setup.DetailMode = (!DetailMode && (Flags59 & 4)) ? 4 : DetailMode;
    CThing* thing = Thing;
    if (thing->HasTC(3)) {
        long key = 3;
        CTCPair* it = thing->TCs.LowerBound(key);
        if (it == thing->TCs._M_finish || it->Type > 3)
            it = thing->TCs._M_finish;
        CDecalSet* decals = ((CTCDecals*)it->TC)->GetDecals();
        if (decals->Decals._M_start != decals->Decals._M_finish)
            setup.Decals = &decals->Decals;
    }
    thing = Thing;
    if (thing->HasTC(0xe9)) {
        long key = 0xe9;
        CTCPair* it = thing->TCs.LowerBoundLights(key);
        if (it == thing->TCs._M_finish || it->Type > 0xe9)
            it = thing->TCs._M_finish;
        CPrimitiveVector* lights = ((CTCLights*)it->TC)->GetLights();
        if (lights->_M_start != lights->_M_finish)
            setup.Lights = lights;
    }
    thing = Thing;
    if (thing->HasTC(0x5e)) {
        long key = 0x5e;
        CTCPair* it = thing->TCs.LowerBound(key);
        if (it == thing->TCs._M_finish || it->Type > 0x5e)
            it = thing->TCs._M_finish;
        ((CTCActivationReceptor*)it->TC)->GetTriggers(&triggers);
        if (triggers._M_node_count)
            setup.HasTriggers = true;
    }
    bool allPrepared = true;
    CPrimitiveList failed;
    for (CPrimitiveNode* node = Primitives->_M_next; node != Primitives; node = node->_M_next) {
        CAppearancePrimitive* primitive = node->_M_data;
        bool needsUpdate = primitive->NeedsUpdate();
        bool flag = (Flags59 & 4) || needsUpdate;
        Flags59 = (unsigned char)((Flags59 & ~4) | (flag << 2));
        if (!primitive->Prepare(this, &setup))
            failed.push_back(primitive);
    }
    for (CPrimitiveNode* node = failed._M_node->_M_next; node != failed._M_node; node = node->_M_next)
        if (!node->_M_data->Prepare(this, &setup))
            allPrepared = false;
    if (allPrepared)
        DetailMode = 0;
}
