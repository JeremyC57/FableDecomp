// NParticleEngine::CPSCSingleSprite::AddToBoundingSphere(C3DBoundingSphere& sphere, float time, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- grow the bounding sphere by every trail point (system-transformed when the system stays with its emitter, radius half the trail width) and by the sprite at its position parameter (system-transformed, radius from the animated frame's aspect-scaled size)
// Grade: DIFFER(962v977) 208/338 -- first full reconstruction (was bodiless): structure matches (in-place TransformPoint, inline CVectorMap::Find over a same-TU LowerBound, landed FloorToLong inline asm, GetNormalisedTime clamp, fmod animation time, lerped size held in a local); residue = register colouring (retail this in edi, trail iterator esi, `result` cached in bl with two return epilogues; VC7.1 here puts this in esi and keeps result in memory) and one frame word (retail packs size.Y over FloorToLong's `next`)
// Leaf: ?AddToBoundingSphere@CPSCSingleSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Retail-asm: 0x00B07892 `fld dword ptr [ebp-10h]` + 0x00B07898 `fistp dword ptr [ebp+14h]` -- the landed FloorToLong inline-assembly helper (fsub 0.5 / fistp / fild / fadd 1 / compare); VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B075D0, 977 bytes, `ret 10h`.
// Callees: CParticleSystem::GetSystemTransform 0x00AF2250, C3DBoundingSphere::AddToBounds 0x00ADDD80, CVectorMap::LowerBound 0x00AF2C50,
//   NParticleEngine::GetTickRate 0x00AEA9F0 (Ego_r.pdb ?GetTickRate@NParticleEngine@@YIJXZ; retail `mov eax,[0x01399504]`),
//   NParticleEngine::GetGraphicBank 0x00AEAAA0 (Ego_r.pdb ?GetGraphicBank@NParticleEngine@@YIABVCGraphicDataBank@@XZ; reads the
//   global SetGraphicBank 0x00AEAA80 writes), CGraphicDataBank::GetNoFramesInGraphic (virtual slot 20), _CIfmod 0x00BFEB28,
//   CParticleSystem::ScaleParticleDimensions(C2DVector&) const 0x00AF8F00. The param callback's slot-1 name is unproven.
// Flags: /O2 /Oy /W3
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
};
class C2DVector { public:
    float X, Y;
};
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Position;
    __forceinline void TransformPoint(C3DVector& v) const      // FableWin.pdb ?TransformPoint@CMatrix3x4@@QBEXAAVC3DVector@@@Z (inline)
    {
        float x = v.X, y = v.Y, z = v.Z;
        v.X = Right.X * x + Up.X * y + Forward.X * z + Position.X;
        v.Y = Right.Y * x + Up.Y * y + Forward.Y * z + Position.Y;
        v.Z = Right.Z * x + Up.Z * y + Forward.Z * z + Position.Z;
    }
};
class C3DBoundingSphere { public:
    void AddToBounds(const C3DVector& point, float radius);          // 0x00ADDD80
};

struct CGraphicFrameTexture { unsigned char m_pad00[6]; unsigned short Width; unsigned short Height; };   // unproven names
struct CGraphicFrame { unsigned char m_pad00[0xc]; CGraphicFrameTexture* Texture; };                     // 16 bytes
struct CGraphicEntry { unsigned char m_pad00[0x28]; CGraphicFrame* Frames; };                            // 0x2c bytes

namespace std {

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};

template <class _Tp> class allocator { };

template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy {   // the end-of-storage pointer
    _Value _M_data;
};

template <class _Tp, class _Alloc> class vector {         // retail STLport vector (12 bytes)
public:
    typedef const _Tp* const_iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::const_iterator const_iterator;
    CArray<value_type> Data;                                // +0x00
    TCompare           Compare;                             // +0x0C (empty comparator)
    bool               Dirty;                               // +0x0D

    const_iterator end() const { return Data.end(); }
    const_iterator LowerBound(const K& key) const;
    const_iterator Find(const K& key) const
    {
        const_iterator it = LowerBound(key);
        const_iterator last = end();
        if (it == last || Compare(key, *it))
            return last;
        return it;
    }
};

#pragma auto_inline(off)
// Same-TU body of the binary search (retail instantiates LowerBound in this TU; out-of-line copy @ 0x00AF2C50 for the descriptor map): it
// only reads memory, which lets VC7.1 keep the caller's key in ebx across the call as retail does.
template <class K, class V, class TCompare>
__declspec(noinline) typename CVectorMap<K, V, TCompare>::const_iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key) const
{
    const_iterator first = Data.begin();
    int len = Data.end() - first;
    while (len > 0) {
        int half = len / 2;
        const_iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}
#pragma auto_inline(on)

class CGraphicDataBank { public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual unsigned long GetNoFramesInGraphic(unsigned long index) const;   // slot 20
    unsigned char m_pad04[0x1dc];
    CArray<CGraphicEntry> Graphics;             // +0x1e0
    const CGraphicEntry* PeekGraphic(unsigned long index) const { return index < Graphics.size() ? &Graphics._M_start[index] : 0; }
};

extern const float g_epsilon_0129ba3c;          // 0x0129BA3C (1e-4f)

static __forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

namespace NParticleEngine {

long __fastcall GetTickRate();                                 // 0x00AEA9F0
const CGraphicDataBank& __fastcall GetGraphicBank();          // 0x00AEAAA0

class CParticleSystem { public:
    unsigned char m_pad00[0x50];
    bool          Initialised : 1;                   // +0x50 bit 0
    bool          Enabled : 1;
    bool          Dead : 1;
    bool          NormalisedTimeEnabled : 1;         // bit 3
    bool          UseAllAttractors : 1;
    bool          Visible : 1;
    bool          StayWithEmitter : 1;               // bit 6
    bool          ScaleParticles : 1;                // bit 7
    float         GlobalAlpha;                       // +0x54
    float         FadeAlpha;                         // +0x58
    float         NormalisedTime;                    // +0x5c
    C3DVector     Scale;                             // +0x60
    void GetSystemTransform(CMatrix3x4& transform) const;                  // 0x00AF2250
    void ScaleParticleDimensions(C2DVector& size) const;                   // 0x00AF8F00
    float GetNormalisedTime() const { return NormalisedTimeEnabled ? NormalisedTime : 0.0f; }   // 0x00AF8FE0
};

template <class A, class B> class CParticleUpdateParameters;
template <class A, class B> class CParticleUpdateParamCallback { public:
    virtual ~CParticleUpdateParamCallback();
    virtual bool GetValue(C3DVector& value, B data, float time, long index, const CParticleUpdateParameters<A, B>& params, long count) const;  // slot 1, unproven name
};
template <class A, class B> class CParticleUpdateParamDesc { public:
    virtual ~CParticleUpdateParamDesc();
    CParticleUpdateParamCallback<A, B>* Callback;   // +0x04
    B                                   Data;       // +0x08
};
template <class A, class B> class CParticleUpdateParameters { public:
    template <class K> struct CParticleUpdateCCompareVector {
        bool operator()(const K& key, const std::pair<K, C3DVector>& entry) const { return key < entry.first; }
    };
    template <class K> struct CParticleUpdateCCompareParamDesc {
        bool operator()(const K& key, const std::pair<K, CParticleUpdateParamDesc<A, B> >& entry) const { return key < entry.first; }
    };
    typedef CVectorMap<A, C3DVector, CParticleUpdateCCompareVector<A> >                         CVectorParamMap;
    typedef CVectorMap<A, CParticleUpdateParamDesc<A, B>, CParticleUpdateCCompareParamDesc<A> > CUpdateParamMap;
    virtual ~CParticleUpdateParameters();
    CVectorParamMap VectorParams;                           // +0x04
    CUpdateParamMap UpdateParams;                           // +0x14
};

struct CSpriteTrailNode { CSpriteTrailNode* _M_next; CSpriteTrailNode* _M_prev; C3DVector Position; float Width; };

class CPSCBase { public:
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;
    bool UpdateStarted : 1;
    bool Initialised : 1;
};

class CPSCSingleSprite : public CPSCBase { public:
    unsigned char m_pad0c[0x28];
    long          SpriteBankIndex;              // +0x34
    unsigned long StartColour, MidColour, EndColour;
    float         AnimationTimeSecs;            // +0x44
    float         StartRenderSize;              // +0x48
    float         EndRenderSize;                // +0x4c
    long          SpriteAlignment;              // +0x50
    long          NoCrossedSprites;             // +0x54
    float         InitialAngle;                 // +0x58
    long          FadeInEndInteger;             // +0x5c
    long          FadeOutBeginInteger;          // +0x60
    float         AlphaFadeMinimum;             // +0x64
    float         SizeFadeMinimum;              // +0x68
    unsigned long PositionParam;                // +0x6c
    long          BlendMode, BlendOp, TrailBlendMode, TrailBlendOp;   // +0x70
    long          TrailBankIndex;               // +0x80
    float         TrailWidth;                   // +0x84
    long          MaxTrailLength;               // +0x88
    long          StayWithEmitterIntFactor;     // +0x8c
    bool          UseStartColour, UseMidColour, UseEndColour;   // +0x90
    bool          SelfIlluminating : 1;         // +0x93
    bool          AlphaFadeEnable : 1;
    bool          RotateAroundCentre : 1;
    bool          FaceMe2D : 1;
    bool          FaceMe3D : 1;
    bool          CrossedSprites : 1;           // bit 5
    bool          SizeFadeEnable : 1;
    bool          ForceAnimationTime : 1;       // bit 7
    bool          StayWithEmitter : 1;          // +0x94
    bool          UsePosition, UseSplinePoints, EnteringTimelineEnd;
    unsigned long LastUpdateFrame;              // +0x98
    long          Lifetimer;                    // +0x9c
    C3DVector     Offset;                       // +0xa0
    unsigned char TrailUpdatePrimitive[0xc];    // +0xac CEnginePrimitiveSpriteTrail
    CSpriteTrailNode* Trail;                    // +0xb8 (its Trail list node)

    long GetTrailSize() const
    {
        long n = 0;
        for (CSpriteTrailNode* p = Trail->_M_next; p != Trail; p = p->_M_next)
            ++n;
        return n;
    }
    virtual bool AddToBoundingSphere(C3DBoundingSphere& sphere, float time, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};

bool CPSCSingleSprite::AddToBoundingSphere(C3DBoundingSphere& sphere, float time, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    bool result = false;
    if (TrailBankIndex && GetTrailSize() > 0) {
        float radius = TrailWidth * 0.5f;
        for (CSpriteTrailNode* p = Trail->_M_next; p != Trail; p = p->_M_next) {
            C3DVector pos = p->Position;
            if (system.StayWithEmitter) {
                CMatrix3x4 transform;
                system.GetSystemTransform(transform);
                transform.TransformPoint(pos);
            }
            sphere.AddToBounds(pos, radius);
        }
        result = true;
    }
    if (SpriteBankIndex && PositionParam) {
        CParticleUpdateParameters<unsigned long, unsigned long>::CUpdateParamMap::const_iterator it = params.UpdateParams.Find(PositionParam);
        if (it == params.UpdateParams.end())
            return result;
        C3DVector value;
        if (!it->second.Callback->GetValue(value, it->second.Data, time, 0, params, 1))
            return result;
        C3DVector pos = value;
        CMatrix3x4 transform;
        system.GetSystemTransform(transform);
        transform.TransformPoint(pos);
        float tickRate = (float)GetTickRate();
        time = (float)Lifetimer + time;
        float t = system.GetNormalisedTime();
        if (t < 0.0f)
            t = 0.0f;
        else if (t > 1.0f)
            t = 1.0f;
        float animTime;
        if (!ForceAnimationTime) {
            animTime = t;
            if (t < 0.0f)
                animTime = 0.0f;
            else {
                float limit = 1.0f - g_epsilon_0129ba3c;
                if (t > limit)
                    animTime = limit;
            }
        }
        else {
            float rate = 1.0f / (tickRate * AnimationTimeSecs);
            animTime = (float)fmod((double)(time * rate), 1.0);
        }
        const CGraphicDataBank& bank = GetGraphicBank();
        unsigned long frames = bank.GetNoFramesInGraphic(SpriteBankIndex);
        long frame = FloorToLong((float)(long)frames * animTime);
        if (frame < 0)
            frame = 0;
        else if (frame > (long)frames - 1)
            frame = frames - 1;
        const CGraphicEntry* entry = (unsigned long)SpriteBankIndex < bank.Graphics.size() ? &bank.Graphics._M_start[SpriteBankIndex] : 0;
        C2DVector size;
        float start = StartRenderSize;
        size.X = start + (EndRenderSize - start) * animTime;
        const CGraphicFrameTexture* texture = entry->Frames[frame].Texture;
        size.Y = (float)texture->Height / texture->Width * size.X;
        system.ScaleParticleDimensions(size);
        float factor = 0.5f;
        if (CrossedSprites && SpriteAlignment)
            factor = 1.0f;
        sphere.AddToBounds(pos, sqrtf(size.X * size.X + size.Y * size.Y) * factor);
        return true;
    }
    return result;
}

} // namespace NParticleEngine
