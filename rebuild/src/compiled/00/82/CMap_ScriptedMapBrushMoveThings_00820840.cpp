// CMap::ScriptedMapBrushMoveThings(const CScriptedMapBrush&) -- move every thing the scripted brush lists to its brush position and facing angles
// Leaf: ?ScriptedMapBrushMoveThings@CMap@@MAEXABVCScriptedMapBrush@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: the local C2DCoordI removed too (engine/C3DVector.h includes engine/C2DCoordI.h, Ego_r TPI 0xa59e: public long X/Y, size 8).
// Callees: CScriptedMapBrush::GetThingsToMove(std::list<unsigned __int64>&) const 0x0082E860, CScriptedMapBrush::GetThingsMovement(unsigned __int64) const 0x0082E4E0,
//   CThingManager::GetPThingWithUID(unsigned __int64) 0x0051ED80, CWorldMap::ConvertLocalCoordsToWorldMapCoords(long, const C3DVector&) const 0x004FC4A0,
//   virtual CWorld::GetThingManager() slot 3, CWorld::GetWorldMap() slot 13, CWorldMap::GetMapNumberAt(const C2DCoordI&) const slot 17,
//   CTCPhysicsBase::ResetPosition slot 31, SetFacingAngleXY/YZ/XZ slots 66/67/68; malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list nodes).
// Layout (retail, local models; NOTES.md header debt): CMap World +0x20, WorldPos +0x1D24; CThing PhysicsTC +0x60, Dead bit +0x91;
//   filler virtual slots vNN are unnamed placeholders, only the named slots are proven.
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/C3DVector.h"
class C3DAngles {
public:
    float AngleXY, AngleYZ, AngleXZ;
};

namespace std {
template <class T> class allocator { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : public _List_node_base { T _M_data; };
template <class T> struct _Nonconst_traits { typedef T& reference; };
template <class T, class Traits> struct _List_iterator {
    typedef typename Traits::reference reference;
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* p) : _M_node(p) { }
    reference operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
template <class T, class A = allocator<T> > class list {
public:
    _List_node_base* _M_node;
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    list() : _M_node(0)
    {
        _List_node_base* n = (_List_node_base*)malloc(sizeof(_List_node<T>));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
    ~list() { clear(); free(_M_node); }
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    void clear()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
}

class CScriptedMapBrush {
public:
    class CThingPosAndOrientation {
    public:
        C3DVector Position;
        C3DAngles FacingAngles;
    };
    const CThingPosAndOrientation& GetThingsMovement(unsigned __int64 uid) const;
    void GetThingsToMove(std::list<unsigned __int64>& things) const;
};

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void ResetPosition(const C3DVector& pos);       // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngleXY(float angle);             // slot 66 (+0x108)
    virtual void SetFacingAngleYZ(float angle);             // slot 67 (+0x10C)
    virtual void SetFacingAngleXZ(float angle);             // slot 68 (+0x110)
};
class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   Dead : 1;                   // +0x91 bit 0
};
class CThingManager {
public:
    CThing* GetPThingWithUID(unsigned __int64 uid);
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;   // slot 17 (+0x44)
    C3DVector ConvertLocalCoordsToWorldMapCoords(long map, const C3DVector& pos) const;
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager& GetThingManager();   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap& GetWorldMap();           // slot 13 (+0x34)
};
class CMap {
public:
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    CWorld*       World;                        // +0x20
    unsigned char _pad24[0x1d24 - 0x24];
    C2DCoordI     WorldPos;                     // +0x1D24
protected:
    virtual void ScriptedMapBrushMoveThings(const CScriptedMapBrush& brush);
};

void CMap::ScriptedMapBrushMoveThings(const CScriptedMapBrush& brush)
{
    std::list<unsigned __int64> things;
    brush.GetThingsToMove(things);
    long mapNumber = World->GetWorldMap().GetMapNumberAt(WorldPos);
    for (std::list<unsigned __int64>::iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = World->GetThingManager().GetPThingWithUID(*it);
        if (thing && !thing->Dead) {
            CScriptedMapBrush::CThingPosAndOrientation movement = brush.GetThingsMovement(*it);
            CTCPhysicsBase* physics = thing->PhysicsTC;
            physics->ResetPosition(World->GetWorldMap().ConvertLocalCoordsToWorldMapCoords(mapNumber, movement.Position));
            thing->PhysicsTC->SetFacingAngleXY(movement.FacingAngles.AngleXY);
            thing->PhysicsTC->SetFacingAngleYZ(movement.FacingAngles.AngleYZ);
            thing->PhysicsTC->SetFacingAngleXZ(movement.FacingAngles.AngleXZ);
        }
    }
}