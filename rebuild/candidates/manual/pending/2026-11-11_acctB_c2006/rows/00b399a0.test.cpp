// Behaviour test for 0x00B399A0 CEngineInternalAttachedPrimitive::UpdateOutputPrimitive(float): a self-contained host model of the
// row's logic. The parent transform (animated-mesh parents only) is pre-multiplied by the attachment transform (flag 2), the child's
// cached inverse (flag 1, computed on demand for static-mesh children; failure disables the output) and the attachment transform
// again (flag 4), rescaled back to engine units (third row x0.01, first two rows x-0.01), set on the output primitive and enabled.
#include <cstdio>
#include <cmath>

struct M34 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void SetMultiply(const M34& m1, const M34& m2) {
        E11 = m1.E11 * m2.E11 + m1.E12 * m2.E21 + m1.E13 * m2.E31;
        E12 = m1.E11 * m2.E12 + m1.E12 * m2.E22 + m1.E13 * m2.E32;
        E13 = m1.E11 * m2.E13 + m1.E12 * m2.E23 + m1.E13 * m2.E33;
        E21 = m1.E21 * m2.E11 + m1.E22 * m2.E21 + m1.E23 * m2.E31;
        E22 = m1.E21 * m2.E12 + m1.E22 * m2.E22 + m1.E23 * m2.E32;
        E23 = m1.E21 * m2.E13 + m1.E22 * m2.E23 + m1.E23 * m2.E33;
        E31 = m1.E31 * m2.E11 + m1.E32 * m2.E21 + m1.E33 * m2.E31;
        E32 = m1.E31 * m2.E12 + m1.E32 * m2.E22 + m1.E33 * m2.E32;
        E33 = m1.E31 * m2.E13 + m1.E32 * m2.E23 + m1.E33 * m2.E33;
        E41 = m1.E41 * m2.E11 + m1.E42 * m2.E21 + m1.E43 * m2.E31 + m2.E41;
        E42 = m1.E41 * m2.E12 + m1.E42 * m2.E22 + m1.E43 * m2.E32 + m2.E42;
        E43 = m1.E41 * m2.E13 + m1.E42 * m2.E23 + m1.E43 * m2.E33 + m2.E43;
    }
    void PreMultiply(const M34& m) { M34 copy = *this; SetMultiply(m, copy); }
};

static M34 Make(float a, float b, float c, float d, float e, float f, float g, float h, float i, float x, float y, float z) {
    M34 m; m.E11 = a; m.E12 = b; m.E13 = c; m.E21 = d; m.E22 = e; m.E23 = f; m.E31 = g; m.E32 = h; m.E33 = i; m.E41 = x; m.E42 = y; m.E43 = z; return m;
}

struct Output { bool enabled; bool set; M34 transform; };

struct Attached {
    int ParentType, ChildType;      // 2 = animated mesh parent, 1 = static mesh child
    unsigned long Flags;
    M34 PrimitiveTransform, ChildInverseTransform, Parent, ChildSource;
    bool ChildInverseTransformValid, ChildInverseComputable, ParentAvailable;
    Output* output;
    int childInverseCalls;
    bool GetParentTransform(M34& m, float) { bool result = false; switch (ParentType) { case 2: if (ParentAvailable) { m = Parent; result = true; } break; } return result; }
    bool GetChildInverseTransform() {
        if (ChildInverseTransformValid) return true;
        ++childInverseCalls;
        bool valid = false;
        switch (ChildType) { case 1: if (ChildInverseComputable) { ChildInverseTransform = ChildSource; valid = true; } break; }
        ChildInverseTransformValid = valid;
        return valid;
    }
    static void Redo(M34& m) { m.E31 *= 0.01f; m.E32 *= 0.01f; m.E33 *= 0.01f; m.E11 *= -0.01f; m.E12 *= -0.01f; m.E13 *= -0.01f; m.E21 *= -0.01f; m.E22 *= -0.01f; m.E23 *= -0.01f; }
    void UpdateOutputPrimitive(float interp) {
        if (!output) return;
        M34 transform;
        if (GetParentTransform(transform, interp)) {
            if (Flags & 2) transform.PreMultiply(PrimitiveTransform);
            if (Flags & 1) {
                if (GetChildInverseTransform()) transform.PreMultiply(ChildInverseTransform);
                else { output->enabled = false; return; }
            }
            if (Flags & 4) transform.PreMultiply(PrimitiveTransform);
            Redo(transform);
            output->transform = transform; output->set = true;
            output->enabled = true;
        } else
            output->enabled = false;
    }
};

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    Output out; Attached a;
    a.ParentType = 2; a.ChildType = 1; a.Flags = 0;
    a.Parent = Make(1, 0, 0, 0, 1, 0, 0, 0, 1, 10, 20, 30);
    a.PrimitiveTransform = Make(0, 1, 0, -1, 0, 0, 0, 0, 1, 1, 2, 3);
    a.ChildSource = Make(2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0);
    a.ChildInverseTransformValid = false; a.ChildInverseComputable = true; a.ParentAvailable = true; a.output = &out; a.childInverseCalls = 0;
    // No flags: the parent transform rescaled.
    out.enabled = false; out.set = false;
    a.UpdateOutputPrimitive(0.5f);
    Check(out.enabled && out.set && Near(out.transform.E11, -0.01f) && Near(out.transform.E33, 0.01f) && Near(out.transform.E41, 10.0f), "plain parent");
    // Flag 2: P * parent (rotation of P, translation P.T * R + T).
    a.Flags = 2; a.UpdateOutputPrimitive(0.5f);
    Check(Near(out.transform.E12, -0.01f) && Near(out.transform.E21, 0.01f) && Near(out.transform.E41, 11.0f) && Near(out.transform.E42, 22.0f) && Near(out.transform.E43, 33.0f), "flag 2");
    // Flag 1: the child inverse is computed once and cached.
    a.Flags = 1; a.UpdateOutputPrimitive(0.5f); a.UpdateOutputPrimitive(0.5f);
    Check(a.childInverseCalls == 1 && Near(out.transform.E11, -0.02f) && Near(out.transform.E22, -0.02f) && Near(out.transform.E33, 0.02f), "flag 1 cached");
    // Flags 2|1|4 in order: P * (C * (P * parent)).
    a.Flags = 7; a.UpdateOutputPrimitive(0.5f);
    M34 expect = a.Parent; expect.PreMultiply(a.PrimitiveTransform); expect.PreMultiply(a.ChildSource); expect.PreMultiply(a.PrimitiveTransform); Attached::Redo(expect);
    Check(Near(out.transform.E11, expect.E11) && Near(out.transform.E12, expect.E12) && Near(out.transform.E41, expect.E41) && Near(out.transform.E43, expect.E43), "flags 7");
    // The child inverse cannot be computed: disabled, transform untouched.
    a.ChildInverseTransformValid = false; a.ChildInverseComputable = false; out.set = false; a.Flags = 1;
    a.UpdateOutputPrimitive(0.5f);
    Check(!out.enabled && !out.set, "child inverse failure");
    // Parent not animated or without transform: disabled.
    a.ChildInverseComputable = true; a.Flags = 0; a.ParentType = 1; out.enabled = true;
    a.UpdateOutputPrimitive(0.5f);
    Check(!out.enabled, "static parent");
    a.ParentType = 2; a.ParentAvailable = false; out.enabled = true;
    a.UpdateOutputPrimitive(0.5f);
    Check(!out.enabled, "no parent transform");
    // No output primitive: nothing happens.
    a.output = 0; a.UpdateOutputPrimitive(0.5f);
    if (g_fail) { std::printf("00b399a0_TEST FAIL\n"); return 1; }
    std::printf("00b399a0_TEST PASS\n");
    return 0;
}
