// Grade: DIFFER(408v406) -- register/schedule: retail computes index*8 into edi BEFORE the epsilon compare and keeps pos in ebx (ours: pos edi, index*8 ebx computed after the branch), and its kill-path counted-pointer temporary shares [ebp-8] with the offset position (ours [ebp-0x10]).
// NPlayerGui::CDrawQuickAccessIcons::DrawEffectAtPos(const CVector2<long>& pos, long effectType, long index) -- keep the quick-access particle effect
// `index` at the screen position: for a non-zero position, kill the effect if it moved, and (re)create a 2D emitter at pos + the GUI def offset;
// for a zero position, kill any effect and clear its stored position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064546F, length 406 bytes, `ret 0Ch`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CParticleEmitter::Kill 0x00AEE8C0, CIntelligentPointer<T>(T*) 0x00655318,
// operator= 0x0065533C, ~CBaseIntelligentPointer 0x00A01C10, PeekGuiDef 0x00438570, CRightHandedSet(const C3DVector&, const C3DVector&)
// 0x00487050, CParticleEmitterManager::Get 0x00AEAAF0 (static; manifest "GFGetPreMainMemoryUsed"), Create2DEmitter 0x00AEB790. g_fEpsilon 0x0129BA3C.
// Layout: effect pointers vector +0x7c, effect positions vector +0x88 (8-byte elements); CPlayerGuiDef offset +0x6f0 (descriptive).
// Tried: unsigned index, inline GetLengthSquared, implicit `= 0` conversions (moves the create-path temporaries), operand order of !=.
// Leaf: DrawEffectAtPos
#pragma optimize("y", off)
#pragma optimize("s", on)
extern float g_fEpsilon;                          // 0x0129BA3C
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
template <class T> struct CVector2 {
    T X, Y;
    __forceinline bool operator!=(const CVector2& o) const { return X != o.X || Y != o.Y; }
};
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(const C3DVector& forward, const C3DVector& up);   // 0x00487050
};
struct CParticleEmitter {
    void Kill();                                   // 0x00AEE8C0
};
struct CParticleEmitterManager {
    static CParticleEmitterManager* __fastcall Get();   // 0x00AEAAF0
    CParticleEmitter* Create2DEmitter(long type, const C2DVector& pos, long layer, bool flag, const CRightHandedSet& set);   // 0x00AEB790
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    void* m_pData;
    void* GetPItem() const;                        // 0x00A01B50
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item);                  // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
    T* operator->() const { return (T*)GetPItem(); }
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x6f0];
    C2DVector QuickAccessEffectOffset;             // +0x6f0
};
const CPlayerGuiDef* __fastcall PeekGuiDef();      // 0x00438570

namespace NPlayerGui {
struct CDrawQuickAccessIcons {
    unsigned char _pad00[0x7c];
    CIntelligentPointer<CParticleEmitter>* Effects;      // +0x7c vector start
    unsigned char _pad80[0x88 - 0x80];
    CVector2<long>* EffectPositions;                     // +0x88 vector start
    void DrawEffectAtPos(const CVector2<long>& pos, long effectType, long index);
};

void CDrawQuickAccessIcons::DrawEffectAtPos(const CVector2<long>& pos, long effectType, long index)
{
    float x = (float)pos.X;
    float y = (float)pos.Y;
    if (x * x + y * y > g_fEpsilon * g_fEpsilon) {
        if (Effects[index].GetPItem() && pos != EffectPositions[index]) {
            Effects[index]->Kill();
            Effects[index] = CIntelligentPointer<CParticleEmitter>(0);
        }
        if (!Effects[index].GetPItem()) {
            C2DVector p = C2DVector((float)pos.X, (float)pos.Y) + PeekGuiDef()->QuickAccessEffectOffset;
            Effects[index] = CIntelligentPointer<CParticleEmitter>(CParticleEmitterManager::Get()->Create2DEmitter(
                effectType, p, 0x43, true, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
            EffectPositions[index] = pos;
        }
    } else if (Effects[index].GetPItem()) {
        Effects[index]->Kill();
        Effects[index] = CIntelligentPointer<CParticleEmitter>(0);
        EffectPositions[index].X = 0;
        EffectPositions[index].Y = 0;
    }
}
}
