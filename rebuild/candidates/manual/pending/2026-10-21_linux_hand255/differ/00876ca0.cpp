// Grade: DIFFER(362v368) -- loop/call/float shape matches; retail FPU-enregisters the copied object position's X/Y (fld x, fld y, z int-copied) and the step*offset temp's X/Y, spilling dx instead (frame 0x44 v 0x38); our copy is three int moves and d stays on the FPU stack.
// NCameraModes::CTools::PotentialFieldBufferObjects(const C3DVector& pos, const list<const CThingGameObject*>& objects) const -- the gradient of the camera's object-avoidance potential field at pos: sample the field at pos and at pos plus one unit step along X, Y and Z (each object adds 0.1*extent / (distance - sqrt(extent))), and return the finite differences divided by the step
// Leaf: PotentialFieldBufferObjects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00876CA0, length 366 bytes (`ret 0Ch`, C3DVector by hidden pointer).
//   Ego_r.pdb: ?PotentialFieldBufferObjects@CTools@NCameraModes@@QBE?AVC3DVector@@ABV3@ABV?$list@PBVCThingGameObject@@...@std@@@Z.
// Callees: NCameraModes::CTools::GetMeshExtent(const CThing*) @ 0x00875F00 (float in st0), CThing::GetPosition() const
//   @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated).
// Globals: 0x01385D20 the sample step (float), 0x01385D24 the four sample offsets (C3DVector[4]: centre, X, Y, Z; the
//   PotentialFieldBufferPoints sibling 0x00876E10 walks the same table); 0x0122EE58 = 0.1f, 0x0122DED8 = 1.0f.
// Layout: STLport list<const CThingGameObject*> {node*} with node {next, prev, value}.
#include <math.h>
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float Length() const { return sqrt(X * X + Y * Y + Z * Z); }
};
inline C3DVector operator*(float f, const C3DVector& v) { return C3DVector(f * v.X, f * v.Y, f * v.Z); }

struct CThing {
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CThingGameObject : CThing {};

template <class T> struct CListNode { CListNode* _M_next; CListNode* _M_prev; T _M_data; };
template <class T> struct CRetailList {                 // STLport list: the sentinel node pointer
    CListNode<T>* _M_node;
};

extern float     g_PotentialFieldStep;                  // 0x01385D20
extern C3DVector g_PotentialFieldSamples[4];            // 0x01385D24

namespace NCameraModes {
struct CTools {
    float GetMeshExtent(const CThing* thing) const;     // 0x00875F00
    C3DVector PotentialFieldBufferObjects(const C3DVector& pos, const CRetailList<const CThingGameObject*>& objects) const;
};
}

C3DVector NCameraModes::CTools::PotentialFieldBufferObjects(const C3DVector& pos, const CRetailList<const CThingGameObject*>& objects) const
{
    float field[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (CListNode<const CThingGameObject*>* node = objects._M_node->_M_next; node != objects._M_node; node = node->_M_next) {
        for (int i = 0; i < 4; ++i) {
            C3DVector sample = g_PotentialFieldStep * g_PotentialFieldSamples[i] + pos;
            float extent = GetMeshExtent(node->_M_data);
            C3DVector position = node->_M_data->GetPosition();
            C3DVector d = sample - position;
            field[i] += (extent * 0.1f) / (d.Length() - (float)sqrt(extent));
        }
    }
    float inv = 1.0f / g_PotentialFieldStep;
    return C3DVector((field[1] - field[0]) * inv, (field[2] - field[0]) * inv, (field[3] - field[0]) * inv);
}
