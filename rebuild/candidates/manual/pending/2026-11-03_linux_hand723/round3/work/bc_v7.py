OLD="""                CEngineLocalLightArray* localLights = g_pEngineLightingManager->GetLightsInSphereFast(
                    C3DBoundingSphere(sprite.WorldPos, sprite.BSphereRadius), 5, CEnginePrimitiveThemeChannelDesc(0, 0, 0));
"""
VARIANTS = [
 ("j1", [(OLD, """                CEnginePrimitiveThemeChannelDesc channel = CEnginePrimitiveThemeChannelDesc(0, 0, 0);
                CEngineLocalLightArray* localLights = g_pEngineLightingManager->GetLightsInSphereFast(
                    C3DBoundingSphere(sprite.WorldPos, sprite.BSphereRadius), 5, channel);
""")], None, "copy-init channel"),
 ("j2", [(OLD, """                CEnginePrimitiveThemeChannelDesc channel(0, 0, 0);
                CEngineLocalLightArray* localLights = g_pEngineLightingManager->GetLightsInSphereFast(
                    C3DBoundingSphere(sprite.WorldPos, sprite.BSphereRadius), 5, channel);
""")], None, "named channel"),
 ("j3", [(OLD, """                CEnginePrimitiveThemeChannelDesc channel;
                channel = CEnginePrimitiveThemeChannelDesc(0, 0, 0);
                CEngineLocalLightArray* localLights = g_pEngineLightingManager->GetLightsInSphereFast(
                    C3DBoundingSphere(sprite.WorldPos, sprite.BSphereRadius), 5, channel);
""")], None, "assigned channel"),
 ("j4", [(OLD, """                CEngineLocalLightArray* localLights = g_pEngineLightingManager->GetLightsInSphereFast(
                    C3DBoundingSphere(sprite.WorldPos, sprite.BSphereRadius), 5, CEnginePrimitiveThemeChannelDesc(CEnginePrimitiveThemeChannelDesc(0, 0, 0)));
""")], None, "copied temp"),
]
