// CTCCreditsUI::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info) -- while the credits are active and not quitting, advance every UI component by the real time since the last render and draw it into the info's target
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E49D0, length 119 bytes (+ int3 padding). Vtable 0x012705AC slot 11 (catalog
// "vfunc_11"; slot 11 of the CTC vtables is DrawPreparePrimitivesForRendering, e.g. CTCBetting 0x007E5680).
// Callees: GFGetTime @ 0x009E1BC0 (double on the x87 stack, twice), NUISystem::CComponent slot 1 (+4, Update(float)) and slot 2
// (+8, the draw call taking the info's +0x34 word and four zeros).
// Layout: Ego_r.pdb CTCCreditsUI with a retail 12-byte vector: Active +0xC, Components +0x10, Quitting +0x1C, CreditPart +0x20,
// LastRenderTime +0x28 (double).
// Shape: the elapsed time is a named float local (kept in ebx); STLport operator[] is `*(begin() + i)` re-read for each call;
// size() recomputed every pass (`sar 2 ; jb`).
double __fastcall GFGetTime();                  // 0x009E1BC0

class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    long m_target;                              // +0x34
};
namespace NUISystem {
class CComponent { public:
    virtual void v00();
    virtual void Update(float deltaTime);                                   // slot 1
    virtual void Draw(long target, long a, long b, long c, long d);        // slot 2
};
}
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
class CCharString { public: void* m_pData; };
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info);   // slot 11
    class CThing* ParentThing;                 // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCCreditsUI : public CTCBase { public:
    bool Active;                                // +0x0c
    CRetailVector<NUISystem::CComponent*> Components;   // +0x10
    bool Quitting;                              // +0x1c
    CCharString CreditPart;                     // +0x20
    double LastRenderTime;                      // +0x28
    virtual void DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info);
};

void CTCCreditsUI::DrawPreparePrimitivesForRendering(CEnginePrimitivePrepareInfo* info)
{
    if (Active && !Quitting) {
        float deltaTime = (float)(GFGetTime() - LastRenderTime);
        LastRenderTime = GFGetTime();
        for (unsigned int i = 0; i < Components.size(); ++i) {
            Components[i]->Update(deltaTime);
            Components[i]->Draw(info->m_target, 0, 0, 0, 0);
        }
    }
}