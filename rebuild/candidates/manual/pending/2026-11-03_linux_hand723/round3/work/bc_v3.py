OUT="    CRGBFloatColour colour;\n"
BODY="""        colour = CRGBFloatColour(sprite.Colour);
        float alphaScale = premultiplyAlpha ? colour.A : 1.0f;
        colour.R *= alphaScale;
        colour.G *= alphaScale;
        colour.B *= alphaScale;
"""
C6="SetUserVertexShaderConstant(6, colour.R, colour.G, colour.B, colour.A);"
VARIANTS = [
 ("g1", [(OUT,""),(BODY,"""        CRGBFloatColour colour(sprite.Colour);
        float alphaScale = premultiplyAlpha ? colour.A : 1.0f;
        CRGBFloatColour finalColour(colour.R * alphaScale, colour.G * alphaScale, colour.B * alphaScale, colour.A);
"""),(C6,"SetUserVertexShaderConstant(6, finalColour.R, finalColour.G, finalColour.B, finalColour.A);")], None, "colour + finalColour in draw block"),
 ("g2", [(OUT,""),(BODY,"""        CRGBFloatColour colour(sprite.Colour);
        float alphaScale = premultiplyAlpha ? colour.A : 1.0f;
        C3DVector premultiplied(colour.R * alphaScale, colour.G * alphaScale, colour.B * alphaScale);
"""),(C6,"SetUserVertexShaderConstant(6, premultiplied.X, premultiplied.Y, premultiplied.Z, colour.A);")], None, "colour + C3DVector"),
 ("g3", [(OUT,""),(BODY,"""        CRGBFloatColour colour(sprite.Colour);
        float alphaScale = premultiplyAlpha ? colour.A : 1.0f;
        float r = colour.R * alphaScale;
        float g = colour.G * alphaScale;
        float b = colour.B * alphaScale;
"""),(C6,"SetUserVertexShaderConstant(6, r, g, b, colour.A);")], None, "colour + 3 floats"),
 ("g4", [(OUT,""),(BODY,"""        CRGBFloatColour colour(sprite.Colour);
        float alphaScale = premultiplyAlpha ? colour.A : 1.0f;
        colour.R *= alphaScale;
        colour.G *= alphaScale;
        colour.B *= alphaScale;
""")], None, "d3-style colour in draw block (with d8 clamp)"),
]
