VARIANTS = [
 ("e1", [("v[5].SetPosition(C3DVector(1.0f, 0.0f, 1.0f));  v[5].SetU1V1(1.0f, 0.0f);  v[5].SetDiffuseColour(0x00ffffff);",
          "v[5].SetDiffuseColour(0x00ffffff);  v[5].SetPosition(C3DVector(1.0f, 0.0f, 1.0f));  v[5].SetU1V1(1.0f, 0.0f);")], None, "v5 colour first"),
 ("e2", [("long rowBase = y * 17;\n        long x;\n        for (x = 0; x < 16; x += 2) {\n            long index = x + rowBase;",
          "for (long x = 0; x < 16; x += 2) {\n            long index = y * 17 + x;")], None, "no rowBase, y*17 + x"),
]
