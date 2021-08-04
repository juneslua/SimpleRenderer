static void TransformUI(struct nk_context *ctx)
{

    float dragSpeed = 0.5f;

    if (nk_begin(ctx, "Mesh", nk_rect(800, 20, 280, 465), NK_WINDOW_TITLE|NK_WINDOW_MINIMIZABLE|NK_WINDOW_BORDER))
    {
        if (nk_tree_push(ctx, NK_TREE_TAB, "Transform", NK_MAXIMIZED))
        {
            static float xPos = 0, yPos = 0, zPos = 0;
            static float xRot = 0, yRot = 0, zRot = 0;
            static float xSca = 0, ySca = 0, zSca = 0;

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Position", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 3);
            nk_property_float(ctx, "#X ", 0, &xPos, 360, 1.0f, dragSpeed);
            nk_property_float(ctx, "#Y ", 0, &yPos, 360, 1.0f, dragSpeed);
            nk_property_float(ctx, "#Z ", 0, &zPos, 360, 1.0f, dragSpeed);
            
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Rotation", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 3);
            nk_property_float(ctx, "#X ", 0, &xRot, 360, 1.0f, dragSpeed);
            nk_property_float(ctx, "#Y ", 0, &yRot, 360, 1.0f, dragSpeed);
            nk_property_float(ctx, "#Z ", 0, &zRot, 360, 1.0f, dragSpeed);

            nk_layout_row_dynamic(ctx, 25, 1);
            nk_label(ctx, "Scale", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 3);
            nk_property_float(ctx, "#X ", 0, &xSca, 360, 1.0f, dragSpeed);
            nk_property_float(ctx, "#Y ", 0, &ySca, 360, 1.0f, dragSpeed);
            nk_property_float(ctx, "#Z ", 0, &zSca, 360, 1.0f, dragSpeed);

            static int turn = 1;
            static float speed = 10;

            nk_layout_row_dynamic(ctx, 10, 1);
            nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
            nk_layout_row_push(ctx, 0.5f);
            nk_checkbox_label(ctx, "Turn around", &turn);
            nk_layout_row_push(ctx, 0.5f);
            nk_property_float(ctx, "#Speed ", 0, &speed, 500, 0.1f, 0.1f);
            nk_layout_row_end(ctx);

            nk_layout_row_dynamic(ctx, 5, 1);

            nk_tree_pop(ctx);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Mesh", NK_MAXIMIZED))
        {                
            static char path[64];
            static int len;

            nk_layout_row_dynamic(ctx, 2, 1);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 3);
            nk_layout_row_push(ctx, 0.24f);
            nk_label(ctx, "Path", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 0.64f);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, path, &len, 64, nk_filter_default);
            nk_layout_row_push(ctx, 0.12f);
            nk_button_symbol(ctx, NK_SYMBOL_X);
            nk_layout_row_end(ctx);
            nk_tree_pop(ctx);

            nk_layout_row_dynamic(ctx, 2, 1);
        }

        if (nk_tree_push(ctx, NK_TREE_TAB, "Shader", NK_MAXIMIZED))
        {
            static char path[64];
            static int len;
            static struct nk_colorf color = {0.5f, 0.2f, 0.7f, 1.0f};

            nk_layout_row_dynamic(ctx, 2, 1);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 3);
            nk_layout_row_push(ctx, 0.24f);
            nk_label(ctx, "Diffuse", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 0.64f);
            nk_edit_string(ctx, NK_EDIT_SIMPLE, path, &len, 64, nk_filter_default);
            nk_layout_row_push(ctx, 0.12f);
            nk_button_symbol(ctx, NK_SYMBOL_X);
            nk_layout_row_end(ctx);

            nk_layout_row_dynamic(ctx, 2, 1);

            nk_layout_row_begin(ctx, NK_DYNAMIC, 25, 2);
            nk_layout_row_push(ctx, 0.24f);
            nk_label(ctx, "Color", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 0.76f);
            if (nk_combo_begin_color(ctx, nk_rgb_cf(color), nk_vec2(183, 300)))
            {
                nk_layout_row_dynamic(ctx, 2, 1);

                nk_layout_row_dynamic(ctx, 100, 1);
                color = nk_color_picker(ctx, color, NK_RGB);
                
                nk_layout_row_dynamic(ctx, 2, 1);
                
                nk_layout_row_dynamic(ctx, 25, 1);
                color.r = nk_propertyf(ctx, "#R ", 0, color.r, 1.0f, 0.01f, 0.005f);
                color.g = nk_propertyf(ctx, "#G ", 0, color.g, 1.0f, 0.01f, 0.005f);
                color.b = nk_propertyf(ctx, "#B ", 0, color.b, 1.0f, 0.01f, 0.005f);

                nk_layout_row_dynamic(ctx, 2, 1);

                nk_combo_end(ctx);
            }
            nk_layout_row_end(ctx);
            nk_tree_pop(ctx);
        }
    }
    nk_end(ctx);
}