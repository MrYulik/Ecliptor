"""Boost player emissive + ensure bloom-friendly texture settings."""

import unreal


def main() -> None:
    emis_tex = unreal.load_asset("/Game/Player/lol_Emissive")
    if emis_tex:
        emis_tex.set_editor_property("srgb", True)
        emis_tex.set_editor_property(
            "compression_settings", unreal.TextureCompressionSettings.TC_DEFAULT
        )
        unreal.EditorAssetLibrary.save_loaded_asset(emis_tex)
        unreal.log("Configured lol_Emissive texture (sRGB)")

    mat = unreal.load_asset("/Game/Player/M_Player")
    if not mat:
        raise RuntimeError("M_Player not found")

    expressions = unreal.MaterialEditingLibrary.get_material_expressions(mat)
    boosted = False
    for expr in expressions:
        if isinstance(expr, unreal.MaterialExpressionScalarParameter):
            name = str(expr.get_editor_property("parameter_name"))
            if name == "EmissiveIntensity":
                expr.set_editor_property("default_value", 40.0)
                boosted = True
                unreal.log("Set EmissiveIntensity default = 40")

        # Ensure emissive texture sample uses color
        if isinstance(expr, unreal.MaterialExpressionTextureSampleParameter2D):
            name = str(expr.get_editor_property("parameter_name"))
            if name == "Emissive":
                tex = unreal.load_asset("/Game/Player/lol_Emissive")
                if tex:
                    expr.set_editor_property("texture", tex)

    if not boosted:
        # Add intensity multiply if missing
        group = "Player"
        intensity = unreal.MaterialEditingLibrary.create_material_expression(
            mat,
            unreal.MaterialExpressionScalarParameter.static_class(),
            node_pos_x=-200,
            node_pos_y=1000,
        )
        intensity.set_editor_property("parameter_name", "EmissiveIntensity")
        intensity.set_editor_property("group", group)
        intensity.set_editor_property("default_value", 40.0)

        emis_sample = None
        for expr in unreal.MaterialEditingLibrary.get_material_expressions(mat):
            if isinstance(expr, unreal.MaterialExpressionTextureSampleParameter2D):
                if str(expr.get_editor_property("parameter_name")) == "Emissive":
                    emis_sample = expr
                    break

        if emis_sample:
            # Disconnect old emissive property and rebuild multiply
            unreal.MaterialEditingLibrary.disconnect_material_property(
                mat, unreal.MaterialProperty.MP_EMISSIVE_COLOR
            )
            mul = unreal.MaterialEditingLibrary.create_material_expression(
                mat,
                unreal.MaterialExpressionMultiply.static_class(),
                node_pos_x=40,
                node_pos_y=1000,
            )
            unreal.MaterialEditingLibrary.connect_material_expressions(
                emis_sample, "RGB", mul, "A"
            )
            unreal.MaterialEditingLibrary.connect_material_expressions(
                intensity, "", mul, "B"
            )
            unreal.MaterialEditingLibrary.connect_material_property(
                mul, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR
            )
            unreal.log("Created Emissive * EmissiveIntensity chain")
        else:
            unreal.log_warning("Emissive texture sample not found on M_Player")

    unreal.MaterialEditingLibrary.layout_material_expressions(mat)
    errors = unreal.MaterialEditingLibrary.recompile_material(mat)
    if errors:
        raise RuntimeError("Material compile failed:\n" + "\n".join(errors))

    unreal.EditorAssetLibrary.save_loaded_asset(mat)

    # Ensure an unbound PostProcessVolume exists in the current level for bloom
    world = unreal.EditorLevelLibrary.get_editor_world()
    volumes = unreal.GameplayStatics.get_all_actors_of_class(
        world, unreal.PostProcessVolume
    )
    volume = None
    for v in volumes:
        if v.get_actor_label() == "PP_EmissiveBloom":
            volume = v
            break

    if volume is None:
        volume = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.PostProcessVolume, unreal.Vector(0, 0, 0)
        )
        volume.set_actor_label("PP_EmissiveBloom")

    volume.set_editor_property("unbound", True)
    volume.set_editor_property("priority", 1.0)
    volume.set_editor_property("blend_weight", 1.0)

    settings = volume.get_editor_property("settings")
    settings.set_editor_property("override_bloom_method", True)
    settings.set_editor_property("override_bloom_intensity", True)
    settings.set_editor_property("override_bloom_threshold", True)
    settings.set_editor_property("bloom_method", unreal.BloomMethod.BM_SOG)
    settings.set_editor_property("bloom_intensity", 1.35)
    settings.set_editor_property("bloom_threshold", -1.0)
    # Slight exposure so emissive pops
    settings.set_editor_property("override_auto_exposure_bias", True)
    settings.set_editor_property("auto_exposure_bias", 0.0)
    volume.set_editor_property("settings", settings)

    unreal.EditorLevelLibrary.save_current_level()
    unreal.log("PP_EmissiveBloom configured (unbound bloom)")
    unreal.log("Done: emissive glow + bloom setup")


if __name__ == "__main__":
    main()
