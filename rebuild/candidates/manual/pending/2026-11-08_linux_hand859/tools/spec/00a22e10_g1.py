NORM_OLD = """    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }"""
NORM_MEM = NORM_OLD.replace("        float x = X;\n        float y = Y;\n        float z = Z;\n        float lengthSquared = x * x + y * y + z * z;", "        float lengthSquared = X * X + Y * Y + Z * Z;")
NORM_DOT = NORM_OLD.replace("        float x = X;\n        float y = Y;\n        float z = Z;\n        float lengthSquared = x * x + y * y + z * z;", "        float lengthSquared = Dot(*this);")
AXES = [
 ("body", [("", "", []),
           ("nm", "Normalise reads members directly", [(NORM_OLD, NORM_MEM)]),
           ("nd", "Normalise length through Dot(*this)", [(NORM_OLD, NORM_DOT)])]),
 ("newton", [("", "", []),
             ("w2", "Newton step r * (3 - l*r*r) * 0.5", [("        r = (3.0f - r * r * lengthSquared) * r * 0.5f;", "        r = r * (3.0f - lengthSquared * r * r) * 0.5f;")]),
             ("w3", "Newton step 0.5 * r * (3 - r*r*l)", [("        r = (3.0f - r * r * lengthSquared) * r * 0.5f;", "        r = 0.5f * r * (3.0f - r * r * lengthSquared);")]),
             ("w4", "Newton step in two statements", [("        r = (3.0f - r * r * lengthSquared) * r * 0.5f;", "        float h = 3.0f - r * r * lengthSquared;\n        r = h * r * 0.5f;")])]),
 ("scale", [("", "", []),
            ("sx", "X = X * r stores", [("        X *= r;\n        Y *= r;\n        Z *= r;", "        X = X * r;\n        Y = Y * r;\n        Z = Z * r;")]),
            ("sv", "*this = *this * r", [("        X *= r;\n        Y *= r;\n        Z *= r;", "        *this = *this * r;")])]),
 ("along", [("", "", []),
            ("ad", "Up -= Forward * d directly", [("    C3DVector along = Forward * Up.Dot(Forward);\n    Up -= along;", "    Up -= Forward * Up.Dot(Forward);")]),
            ("ac", "component-wise subtraction", [("    C3DVector along = Forward * Up.Dot(Forward);\n    Up -= along;", "    float d = Up.Dot(Forward);\n    Up.X -= Forward.X * d;\n    Up.Y -= Forward.Y * d;\n    Up.Z -= Forward.Z * d;")]),
            ("an", "named d then along", [("    C3DVector along = Forward * Up.Dot(Forward);\n    Up -= along;", "    float d = Up.Dot(Forward);\n    C3DVector along(Forward.X * d, Forward.Y * d, Forward.Z * d);\n    Up -= along;")])]),
 ("dot", [("", "", []),
          ("df", "Forward.Dot(Up)", [(">Up.Dot(Forward)", "Forward.Dot(Up)")])]),
]
