struct CFrameData;

struct XResource {
};

struct CFrameData {
    XResource* res0;   // +0
    XResource* res1;   // +4
    int refCount;      // +8
    void ReleaseTexture();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00A77710 ?RemoveConnectionFromConnectionList@CNavRegion@@AAEXPBVCNavRegionConnection@@@Z (was ?Release@XResource@@QAEXPAUCFrameData@@@Z)
class CNavRegionConnection;
struct CFrameData;
class CNavRegion { public: private: void RemoveConnectionFromConnectionList(CNavRegionConnection const*); public: friend struct ::CFrameData; };
void CFrameData::ReleaseTexture()
{
    if (--this->refCount == 0) {
        reinterpret_cast<CNavRegion*>(this->res0)->RemoveConnectionFromConnectionList(reinterpret_cast<CNavRegionConnection const*>(this));
        reinterpret_cast<CNavRegion*>(this->res1)->RemoveConnectionFromConnectionList(reinterpret_cast<CNavRegionConnection const*>(this));
    }
}