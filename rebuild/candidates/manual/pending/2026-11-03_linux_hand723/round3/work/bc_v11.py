VARIANTS = [
 ("p1", [("if (IsInitialised) Uninitialise();", "if (ScreenGraphic) Uninitialise();")], None, "EXP dtor bit"),
 ("p2", [("        else if (sprite.RotateAroundZAxis)\n            ApplyCrossedSpriteDisplacementVertexShader();", "        else if (sprite.ZBuffer)\n            ApplyCrossedSpriteDisplacementVertexShader();")], None, "EXP pass6 bit"),
 ("p3", [("        else if (sprite.RotateAroundZAxis)\n            ApplyCrossedSpritePointLightVertexShader(lights);", "        else if (sprite.ZBuffer)\n            ApplyCrossedSpritePointLightVertexShader(lights);")], None, "EXP default bit"),
]
