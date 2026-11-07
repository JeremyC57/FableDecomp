F_OLD = "    AxisX = GetObservedThing()->GetPhysics()->GetFacingVector();"
AXES = [
 ("chain", [("", "", []),
            ("th", "observed thing local", [(F_OLD, "    const CThingGameObject* thing = GetObservedThing();\n    AxisX = thing->GetPhysics()->GetFacingVector();")]),
            ("ph", "physics local", [(F_OLD, "    CThingPhysics* physics = GetObservedThing()->GetPhysics();\n    AxisX = physics->GetFacingVector();")])]),
 ("facing", [("", "", []),
             ("fl", "named facing local", [(">AxisX = ", "C3DVector facing = "), (">GetFacingVector();", "GetFacingVector();\n    AxisX = facing;")]),
             ("fr", "const ref to the facing temp", [(">AxisX = ", "const C3DVector& facing = "), (">GetFacingVector();", "GetFacingVector();\n    AxisX = facing;")]),
             ("fc", "copy ctor temp", [(">GetFacingVector();", "GetFacingVector();\n    AxisX = C3DVector(AxisX);")])]),
 ("axisz", [("", "", []),
            ("zl", "named up local", [("    AxisZ = C3DVector(0.0f, 0.0f, 1.0f);", "    C3DVector up(0.0f, 0.0f, 1.0f);\n    AxisZ = up;")]),
            ("zm", "member stores", [("    AxisZ = C3DVector(0.0f, 0.0f, 1.0f);", "    AxisZ.X = 0.0f;\n    AxisZ.Y = 0.0f;\n    AxisZ.Z = 1.0f;")])]),
 ("cross", [("", "", []),
            ("cr", "cross through refs", [("    AxisY = CrossProduct(AxisX, AxisZ);", "    const C3DVector& x = AxisX;\n    const C3DVector& z = AxisZ;\n    AxisY = CrossProduct(x, z);")])]),
 ("cut", [("", "", []),
          ("cm", "manager local", [("    CameraManager->CutCamera();", "    CGameCameraManager* manager = CameraManager;\n    manager->CutCamera();")])]),
]
